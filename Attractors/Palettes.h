#pragma once

#include <vector>
#include <algorithm>

#include "Decimal.h"
#include "Colour.h"

typedef struct GradientPoint {
    decimal p;
    unsigned c;

    GradientPoint(decimal p, unsigned c) : p(p), c(c) {}
} GradientPoint;

typedef struct Gradient
{
    std::string name;
    std::vector<GradientPoint> points;

    unsigned ColourAt(decimal p)
    {

        std::sort(points.begin(), points.end(), [](const GradientPoint& a, const GradientPoint& b) { return a.p < b.p; });

        auto baseColor = points[0].c;
        auto basePoint = points[0].p;

        for (auto i = 0U; i < points.size() - 1; i++)
        {
            if (points[i].p <= p && p <= points[i+1].p)
            {
                auto d = (p - points[i].p) / (points[i+1].p - points[i].p);
                return Lerp(points[i].c, points[i + 1].c, d);
            }
        }
        return points[points.size() - 1].c;
    }

    Gradient(std::string name, std::vector<GradientPoint> points) : name(name), points(points) {}
    Gradient() : name("") {}
} Gradient;

extern std::vector<Gradient> Palettes;
