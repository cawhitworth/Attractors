// Attractors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bitmap.h"
#include "lodepng.h"
#include "Colour.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <ctime>

#define decimal double

template<typename T>
struct _Coord
{
    T x;
    T y;

    _Coord(T _x, T _y) : x(_x), y(_y) {}
    _Coord() : x(0.0), y(0.0) {}
};

typedef _Coord<decimal> Coord;

// Clifford attractor
Coord iterate(Coord p, decimal a, decimal b, decimal c, decimal d)
{
    Coord out;
    out.x = std::sin(a * p.y) + c * std::cos(a * p.x);
    out.y = std::sin(b * p.x) + d * std::cos(b * p.y);

    return out;
}


int main()
{
    std::uniform_real_distribution<decimal> distribution { -2.0, 2.0 };
    std::default_random_engine re {};

    re.seed(time(nullptr));

    auto randomCoefficient{ bind(distribution, re) };

    unsigned int w { 1920 }, h { 1080 }, iterations { 100 * 1000 * 1000 };

    Bitmap bmp {w,h};

    Coord p {};

    decimal A, B, C, D;
    decimal minX,  minY, maxX, maxY;

    do {
        A = static_cast<decimal>(randomCoefficient());
        B = static_cast<decimal>(randomCoefficient());
        C = static_cast<decimal>(randomCoefficient());
        D = static_cast<decimal>(randomCoefficient());

        minX = minY = maxX = maxY = 0.0;

        p = Coord {};

        for (auto i = 0; i < 10000; i++)
        {
            p = iterate(p, A, B, C, D);

            minX = std::min(p.x, minX);
            minY = std::min(p.y, minY);

            maxX = std::max(p.x, maxX);
            maxY = std::max(p.y, maxY);
        }
    } while (minX == 0.0 || minY == 0.0 || maxX == 0.0 || maxY == 0.0);
    
    std::cout << "(" << A << "," << B << "," << C << "," <<D << ")" << std::endl;
    std::cout << "(" << minX << "," << minY << ") (" << maxX << "," << maxY << ")" << std::endl;

    p = Coord {};

    decimal xScale = (w-1) / (maxX - minX);
    decimal yScale = (h-1) / (maxY - minY);

    for (auto i = 0; i < iterations ; i++)
    {
        p = iterate(p, A, B, C, D);

        auto plotX = (p.x - minX) * xScale;
        auto plotY = (p.y - minY) * yScale;

        auto c = bmp.Point(plotX, plotY);
        auto r = R(c), g = G(c), b = B(c);
        if (r < 0xfe) r += 2;
        if (g < 0xff) g++;
        if (b < 0xff) b++;

        bmp.Plot(plotX, plotY, Colour(r,g,b));

    }


    auto err = lodepng::encode("output.png", bmp.RawData(), w, h);
    return 0;
}

