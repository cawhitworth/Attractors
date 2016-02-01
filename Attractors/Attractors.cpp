// Attractors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bitmap.h"
#include "lodepng.h"
#include "Colour.h"
#include <cmath>
#include <algorithm>
#include <iostream>

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
    auto w = 640;
    auto h = 512;

    Bitmap bmp(w, h);

    Coord p(0.0, 0.0);

    decimal a = -1.4, b = 1.6, c = 1.0, d = 0.7;

    decimal minX = 0.0, minY = 0.0, maxX = 0.0, maxY = 0.0;

    for (auto i = 0; i < 10000; i++)
    {
        p = iterate(p, a, b, c, d);

        minX = std::min(p.x, minX);
        minY = std::min(p.y, minY);

        maxX = std::max(p.x, maxX);
        maxY = std::max(p.y, maxY);
    }

    p = Coord();

    for (auto i = 0; i < 1000000; i++)
    {
        p = iterate(p, a, b, c, d);

        auto plotX = (p.x - minX) * (w / (maxX - minX));
        auto plotY = (p.y - minY) * (h / (maxY - minY));

        bmp.Plot(plotX, plotY, 0xffffffff);

    }

    std::cout << "(" << minX << "," << minY << "," << ") (" << maxX << "," << maxY << ")" << std::endl;

    auto err = lodepng::encode("output.png", bmp.RawData(), w, h);
    return 0;
}

