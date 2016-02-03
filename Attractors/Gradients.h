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

Gradient Hot { {
        GradientPoint { 0.0, Colour(50, 0, 0) },
        GradientPoint { 0.1 , Colour(255, 0, 0) },
        GradientPoint { 0.4 , Colour(255, 255, 0) },
        GradientPoint { 1.0, Colour(255, 255, 255) }
    }
};

Gradient Smoke{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(150, 150, 150) },
        GradientPoint { 0.2, Colour(100, 100, 100) },
        GradientPoint { 0.4, Colour(25, 25, 25) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};

Gradient BlueSmoke{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(150, 150, 170) },
        GradientPoint { 0.2, Colour(100, 100, 120) },
        GradientPoint { 0.4, Colour(25, 25, 45) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};

Gradient WhiteOrange{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(255, 200, 100) },
        GradientPoint { 0.2, Colour(255, 127, 0) },
        GradientPoint { 0.4, Colour(127, 64, 0) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};

Gradient WhiteCyan{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(100, 200, 255) },
        GradientPoint { 0.2, Colour(0, 127, 255) },
        GradientPoint { 0.4, Colour(0, 64, 127) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};

Gradient WhitePurple{ {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(200, 100, 255) },
        GradientPoint { 0.2, Colour(127, 0, 255) },
        GradientPoint { 0.4, Colour(64, 0, 127) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }
};

Gradient PurpleBlue{
    {
        GradientPoint { 0.0, Colour(0, 0, 25) },
        GradientPoint { 0.1, Colour(0, 0, 125) },
        GradientPoint { 0.2, Colour(50, 0, 175) },
        GradientPoint { 0.4, Colour(100, 0, 255) },
        GradientPoint { 1.0, Colour(255, 0, 255) },
    } 
};

Gradient PetrolOlive{
        {
            GradientPoint { 0.0, Colour(3, 14, 14)},
            GradientPoint { 0.1, Colour(34, 145, 140)},
            GradientPoint { 0.2, Colour(85, 145, 48)},
            GradientPoint { 0.5, Colour(38, 65, 22)},
            GradientPoint { 1.0, Colour(0,0,0)},
        }
};

Gradient PastelPink{ {
        GradientPoint { 0.0, Colour(200, 180, 180) },
        GradientPoint { 0.01, Colour(255, 230, 230) },
        GradientPoint { 0.1, Colour(230, 200, 200) },
        GradientPoint { 1.0, Colour(180, 150, 150) }
    }
};

