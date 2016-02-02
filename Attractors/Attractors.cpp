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

typedef struct {
    decimal a, b, c, d;
} Coefficients;

// Clifford attractor
Coord iterate(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = std::sin(coeffs.a * p.y) + coeffs.c * std::cos(coeffs.a * p.x);
    out.y = std::sin(coeffs.b * p.x) + coeffs.d * std::cos(coeffs.b * p.y);

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
    Coord p {};

    auto xScale { (w - 1) / bounds.width() };
    auto yScale { (h - 1) / bounds.height() };

    maxExposure = 0;

    auto reset = iterations / 10;
    
    if (log) std::cout << "Exposing";

    for (auto i = 0U; i < iterations ; i++)
    {
        if (log) { if (i % reset == 0) std::cout << "."; }
        p = iterate_function(p);

        auto plotX = static_cast<unsigned int>(fmax(0, floor((p.x - bounds.bl.x) * xScale)));
        auto plotY = static_cast<unsigned int>(fmax(0, floor((p.y - bounds.bl.y) * yScale)));

        auto c = bmp.Point(plotX, plotY) + 1;

        if (c > maxExposure) maxExposure = c;

        bmp.Plot(plotX, plotY, c);
    }
    
    if (log) std::cout << std::endl;

    return bmp;
}

Bitmap develop(const Bitmap& bitmap, unsigned maxExposure, decimal gamma, Gradient grad)
{
    auto bmp { bitmap.Copy() };

    auto x = 0, y = 0;

    for(auto& px : bmp)
    {
        auto pct = static_cast<decimal>(px) / static_cast<decimal>(maxExposure);

        pct = pow(pct, 1.0 / gamma);

        pct = std::min(1.0, pct);

        px = grad.ColourAt(pct);
    }

    return bmp;
}

std::function<Coord(Coord)> find_interesting_coeffs(Rect& bounds)
{
    std::uniform_real_distribution<decimal> distribution { -2.0, 2.0 };
    std::default_random_engine re {};

    re.seed(static_cast<unsigned int>(time(nullptr)));

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

        auto fn = std::bind(iterate, std::placeholders::_1, coeffs);
        bounds = find_bounds(fn);

        if (bounds.bl.x == 0 || bounds.bl.y == 0 ||
            bounds.tr.x == 0 || bounds.tr.y == 0) { found = false; }
        else {

            auto maxExposure{ 0U };
            auto exposed = expose(640, 512, 10000, bounds, fn, maxExposure);

            if (maxExposure > 10) found = false;
        }

    } while (!found);

    std::cout << "Coeffs (" << coeffs.a << "," << coeffs.b << "," << coeffs.c << "," <<coeffs.d << ")" << std::endl;

    auto fn = std::bind(iterate, std::placeholders::_1, coeffs);
    return fn;
}

int main()
{
    using namespace std::placeholders;

    auto w { 640U }, h { 512U }, iterations { 100U * 1000 * 1000 };

    Rect bounds;
    auto function = find_interesting_coeffs(bounds);

    auto maxExposure{ 0U };
    auto exposed = expose(w, h, iterations, bounds, function, maxExposure, true);

    auto grad = Gradient {};

    grad.points = {
        GradientPoint { 0.0, Colour(0,0,0) },
        GradientPoint { 0.3 , Colour(255, 0, 0) },
        GradientPoint { 0.6 , Colour(255, 255, 0) },
        GradientPoint { 1.0, Colour(255, 255, 255) }
    };

    auto bmp = develop(exposed, maxExposure * 0.8, 1.5, grad);

    std::cout << "Bounds (" << bounds.bl.x << "," << bounds.bl.y << ") (" << bounds.tr.x << "," << bounds.tr.y << ")" << std::endl;
    std::cout << "Exposure comp: " << maxExposure << std::endl;

    auto err = lodepng::encode("output.png", bmp.RawData(), w, h);
    return 0;
}

