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

int main()
{
    std::uniform_real_distribution<decimal> distribution { -2.0, 2.0 };
    std::default_random_engine re {};

    re.seed(time(nullptr));

    auto randomCoefficient(bind(distribution, re));

    unsigned int w { 640 }, h { 512 }, iterations { 10 * 1000 * 1000 };

    Bitmap bmp {w,h};

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

    p = Coord {};

    decimal xScale = (w-1) / bounds.width();
    decimal yScale = (h-1) / bounds.height();

    for (auto i = 0; i < iterations ; i++)
    {
        p = iterate(p, coeffs);

        auto plotX = fmax(0, floor((p.x - bounds.bl.x) * xScale));
        auto plotY = fmax(0, floor((p.y - bounds.bl.y) * yScale));

        auto c = bmp.Point(plotX, plotY);
        unsigned char r = R(c), g = G(c), b = B(c);
        if (r < 0xfe) r += 2;
        if (g < 0xff) g++;
        if (b < 0xff) b++;

        bmp.Plot(plotX, plotY, Colour(r,g,b));

    }

    auto err = lodepng::encode("output.png", bmp.RawData(), w, h);
    return 0;
}

