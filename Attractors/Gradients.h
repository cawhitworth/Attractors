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

    Gradient(std::vector<GradientPoint> points) : points(points) {}
} Gradient;

Gradient Hot{ {
        GradientPoint { 0.0, Colour(0,0,0) },
        GradientPoint { 0.3 , Colour(255, 0, 0) },
        GradientPoint { 0.6 , Colour(255, 255, 0) },
        GradientPoint { 1.0, Colour(255, 255, 255) }
    }
};

Gradient WhiteOrange{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.5, Colour(255, 127, 0) },
        GradientPoint { 0.9, Colour(127, 64, 0) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};
