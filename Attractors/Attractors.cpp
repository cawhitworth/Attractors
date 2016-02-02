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

#define decimal double

typedef _Coord<decimal> Coord;
typedef _Rect<decimal> Rect;

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

Rect find_bounds(std::function<Coord(Coord, Coefficients)> iter, Coefficients coeffs)
{
    Rect bounds;
    Coord p {};

    for (auto i = 0; i < 10000; i++)
    {
        p = iter(p, coeffs);

        bounds.bl.x = std::min(p.x, bounds.bl.x);
        bounds.bl.y = std::min(p.y, bounds.bl.y);

        bounds.tr.x = std::max(p.x, bounds.tr.x);
        bounds.tr.y = std::max(p.y, bounds.tr.y);
    }

    return bounds;
}

Bitmap Expose(unsigned w, unsigned h, unsigned iterations, Rect bounds, std::function<Coord(Coord)> iterate_function)
{
    Bitmap bmp {w,h};
    Coord p {};

    auto xScale = (w-1) / bounds.width();
    auto yScale = (h-1) / bounds.height();

    auto maxExposure = 0U;

    for (auto i = 0U; i < iterations ; i++)
    {
        p = iterate_function(p);

        auto plotX = static_cast<unsigned int>(fmax(0, floor((p.x - bounds.bl.x) * xScale)));
        auto plotY = static_cast<unsigned int>(fmax(0, floor((p.y - bounds.bl.y) * yScale)));

        auto c = bmp.Point(plotX, plotY) + 1;

        if (c > maxExposure) maxExposure = c;
        bmp.Plot(plotX, plotY, c);
    }

    return bmp;
}

int main()
{
    using namespace std::placeholders;
    std::uniform_real_distribution<decimal> distribution { -2.0, 2.0 };
    std::default_random_engine re {};

    re.seed(static_cast<unsigned int>(time(nullptr)));

    auto randomCoefficient(bind(distribution, re));

    auto w { 640U }, h { 512U }, iterations { 10U * 1000 * 1000 };


    Coord p {};

    Coefficients coeffs;
    Rect bounds;

    do {
        coeffs.a = static_cast<decimal>(randomCoefficient());
        coeffs.b = static_cast<decimal>(randomCoefficient());
        coeffs.c = static_cast<decimal>(randomCoefficient());
        coeffs.d = static_cast<decimal>(randomCoefficient());

        bounds = find_bounds( iterate, coeffs);
    } while (bounds.bl.x == 0 || bounds.bl.y == 0 ||
             bounds.tr.x == 0 || bounds.tr.y == 0);
    
    std::cout << "(" << coeffs.a << "," << coeffs.b << "," << coeffs.c << "," <<coeffs.d << ")" << std::endl;
    std::cout << "(" << bounds.bl.x << "," << bounds.bl.y << ") (" << bounds.tr.x << "," << bounds.tr.y << ")" << std::endl;

    auto fn = bind(iterate, _1, coeffs);
    auto bmp = Expose(w, h, iterations, bounds, fn);

    auto err = lodepng::encode("output.png", bmp.RawData(), w, h);
    return 0;
}

