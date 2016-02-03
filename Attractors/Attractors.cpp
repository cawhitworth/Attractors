// Attractors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <ctime>

#include "Geometry.h"
#include "Bitmap.h"
#include "lodepng.h"
#include "Colour.h"
#include "Gradients.h"
#include <sstream>

std::default_random_engine re {};

typedef struct {
    decimal a, b, c, d;
} Coefficients;

Coord clifford_attractor(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = std::sin(coeffs.a * p.y) + coeffs.c * std::cos(coeffs.a * p.x);
    out.y = std::sin(coeffs.b * p.x) + coeffs.d * std::cos(coeffs.b * p.y);

    return out;
}

Coord peter_de_jong_attractor(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = std::sin(coeffs.a * p.y) - std::cos(coeffs.b * p.x);
    out.y = std::sin(coeffs.c * p.x) - std::cos(coeffs.d * p.y);

    return out;
}

Coord experiment(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = coeffs.c * std::sin(coeffs.a * (p.x + p.y)) - coeffs.d * std::cos(coeffs.b * (p.y - p.x));
    out.y = coeffs.d * std::cos(coeffs.a * (p.x - p.y)) + coeffs.c * std::sin(coeffs.b * (p.y + p.x));

    return out;
}

Rect find_bounds(std::function<Coord(Coord)> iter, unsigned iterations = 10000)
{
    Rect bounds;
    Coord p {};

    for (auto i = 0U; i < iterations; i++)
    {
        p = iter(p);

        bounds.bl.x = std::min(p.x, bounds.bl.x);
        bounds.bl.y = std::min(p.y, bounds.bl.y);

        bounds.tr.x = std::max(p.x, bounds.tr.x);
        bounds.tr.y = std::max(p.y, bounds.tr.y);
    }

    return bounds;
}

Bitmap expose(unsigned w, unsigned h, unsigned iterations, Rect bounds, std::function<Coord(Coord)> iterate_function, unsigned &maxExposure, bool log = false)
{
    Bitmap bmp {w,h,0};
    Coord p;

    auto xScale = (w - 1) / bounds.width();
    auto yScale = (h - 1) / bounds.height();

    maxExposure = 0;

    auto reset = iterations / 10;
    
    if (log) std::cout << "Exposing";

    for (auto i = 0U; i < iterations ; i++)
    {
        if (log) { if (i % reset == 0) std::cout << "."; }

        p = iterate_function(p);

        auto plotX = static_cast<unsigned int>(floor((p.x - bounds.bl.x) * xScale));
        auto plotY = static_cast<unsigned int>(floor((p.y - bounds.bl.y) * yScale));

        plotX = std::max(0U, std::min(plotX, w-1));
        plotY = std::max(0U, std::min(plotY, h-1));

        auto c = bmp.Point(plotX, plotY) + 1;

        if (c > maxExposure) maxExposure = c;

        bmp.Plot(plotX, plotY, c);
    }
    
    if (log) std::cout << std::endl;

    return bmp;
}

Bitmap develop(const Bitmap& bitmap, unsigned maxExposure, decimal gamma, Gradient grad)
{
    auto bmp = bitmap.Copy();

    auto x = 0, y = 0;

    for(auto& px : bmp)
    {
        auto pct = static_cast<decimal>(px) / static_cast<decimal>(maxExposure);

        pct = pow(pct, 1.0 / gamma);

        pct = std::max(0.0, std::min(1.0, pct));

        px = grad.ColourAt(pct);
    }

    return bmp;
}


std::function<Coord(Coord)> find_interesting_coeffs(std::function<Coord(Coord, Coefficients)> iterate, Rect& bounds)
{
    using namespace std::placeholders;
    std::uniform_real_distribution<decimal> distribution { -2.0, 2.0 };

    auto randomCoefficient(bind(distribution, re));
    Coord p {};

    Coefficients coeffs;

    bool found;
    do {
        found = true;
        coeffs.a = static_cast<decimal>(randomCoefficient());
        coeffs.b = static_cast<decimal>(randomCoefficient());
        coeffs.c = static_cast<decimal>(randomCoefficient());
        coeffs.d = static_cast<decimal>(randomCoefficient());

        auto fn = std::bind(iterate, _1, coeffs);
        bounds = find_bounds(fn);

        if (bounds.bl.x == 0 || bounds.bl.y == 0 ||
            bounds.tr.x == 0 || bounds.tr.y == 0) {
            found = false; 
            continue;
        }

        auto maxExposure = 0U;
        auto exposed = expose(640, 512, 10000, bounds, fn, maxExposure);

        if (maxExposure > 10) found = false;

    } while (!found);

    std::cout << "Coeffs (" << coeffs.a << "," << coeffs.b << "," << coeffs.c << "," <<coeffs.d << ")" << std::endl;

    auto fn = std::bind(iterate, _1, coeffs);
    return fn;
}

int main()
{
    using namespace std::placeholders;

    re.seed(static_cast<unsigned int>(time(nullptr)));

    auto w = 2560U, h = 1440U;

    auto img = 0;

    auto iters = { 500 * 1000 };

    std::vector<Gradient> palettes = { Hot, Smoke, BlueSmoke, WhiteOrange, WhiteCyan, WhitePurple, PurpleBlue, PastelPink , PetrolOlive };
    std::uniform_int_distribution<unsigned> palette_distribution(0, palettes.size()-1);

    std::vector< std::function<Coord(Coord, Coefficients)> > functions = { clifford_attractor, peter_de_jong_attractor, experiment };
    std::uniform_int_distribution<unsigned> function_distribution(0, functions.size()-1);

    while (true) {
        Rect bounds;
        auto function = find_interesting_coeffs(functions[function_distribution(re)], bounds);

        std::cout << "Bounds (" << bounds.bl.x << "," << bounds.bl.y << ") (" << bounds.tr.x << "," << bounds.tr.y << ")" << std::endl;

        for (auto i : iters)
        {

            auto maxExposure = 0U;
            auto exposed = expose(w, h, i * 1000, bounds, function, maxExposure, true);

            auto palette = palettes[palette_distribution(re)];
            auto bmp = develop(exposed, static_cast<unsigned>(maxExposure * 0.8), 1.5, palette);

            std::cout << "Iters " << i << " * 1000, ";
            std::cout << "Exposure comp: " << maxExposure << std::endl;

            std::stringstream ss;
            ss << "output_" << img++ << ".png";

            auto err = lodepng::encode(ss.str(), bmp.RawData(), w, h);
        }
    }

    return 0;
}

