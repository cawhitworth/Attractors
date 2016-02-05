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
#include "Palettes.h"
#include <map>
#include "Utils.h"
#include "Options.h"
#include "Functions.h"

std::default_random_engine re {};

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
        if (log) { if (i % reset == 0) std::cout << "." << std::flush; }

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

int main(int argc, char* argv[])
{
    auto options = get_startup_options(argc, argv);
    using namespace std::placeholders;

    re.seed(static_cast<unsigned int>(time(nullptr)));

    auto w = options.width, h = options.height;
    auto iters = options.iterations * 1000;

    std::cout << "Image (" << w << ", " << h << "), " << iters << " iterations" << std::endl;

    std::function< Coord(Coord, Coefficients) > function;

    if (functions.find(options.attractor) == functions.end())
    {
        std::uniform_int_distribution<unsigned> function_distribution(0, functions.size()-1);
        auto fn_index = function_distribution(re);
        std::cout << "Using attractor: " << keys(functions)[fn_index] << std::endl;
        function = values(functions)[fn_index];
    }
    else
    {
        function = functions[options.attractor];
    }

    Bitmap exposed;
    auto maxExposure = 0U;

    if (options.expose) {
        Rect bounds;
        auto bound_function = find_interesting_coeffs(function, bounds);

        std::cout << "Bounds (" << bounds.bl.x << "," << bounds.bl.y << ") (" << bounds.tr.x << "," << bounds.tr.y << ")" << std::endl;

        exposed = expose(w, h, iters, bounds, bound_function, maxExposure, true);
        if (options.exposed_filename != "")
        {
            lodepng::encode(options.exposed_filename, exposed.RawData(), w, h);
        }
    }
    else
    {
        std::vector<unsigned char> data;
        auto err = lodepng::decode(data, w, h, options.exposed_filename);
        exposed = Bitmap(w, h, data);
        for(const auto p : exposed)
        {
            if (p > maxExposure) { maxExposure = p; }
        }
    }

    if (options.develop) {

        Gradient palette;
        if (options.palette != "")
        {
            for (auto p : Palettes)
            {
                if (p.name == options.palette)
                {
                    palette = p;
                }
            }
        }
        else
        {
            std::uniform_int_distribution<unsigned> palette_distribution(0, Palettes.size()-1);
            palette = Palettes[palette_distribution(re)];
        }

        std::cout << "Developing with palette " << palette.name << ", exposure = " << options.exposure << ", gamma = " << options.gamma << std::endl;

        auto bmp = develop(exposed, static_cast<unsigned>(maxExposure * options.exposure), options.gamma, palette);

        auto err = lodepng::encode(options.output_filename, bmp.RawData(), w, h);
    }

    return 0;
}

