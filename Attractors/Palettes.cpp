
#include "Colour.h"
#include "Palettes.h"

std::vector<Gradient> Palettes = {
    { "Hot", {
        GradientPoint { 0.0, Colour(50, 0, 0) },
        GradientPoint { 0.1 , Colour(255, 0, 0) },
        GradientPoint { 0.4 , Colour(255, 255, 0) },
        GradientPoint { 1.0, Colour(255, 255, 255) }
    } },
    { "Smoke", {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(150, 150, 150) },
        GradientPoint { 0.2, Colour(100, 100, 100) },
        GradientPoint { 0.4, Colour(25, 25, 25) },
        GradientPoint { 1.0, Colour(0,0,0) }
    } },
    { "BlueSmoke", {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(150, 150, 170) },
        GradientPoint { 0.2, Colour(100, 100, 120) },
        GradientPoint { 0.4, Colour(25, 25, 45) },
        GradientPoint { 1.0, Colour(0,0,0) }
    } },
    { "WhiteOrange", {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(255, 200, 100) },
        GradientPoint { 0.2, Colour(255, 127, 0) },
        GradientPoint { 0.4, Colour(127, 64, 0) },
        GradientPoint { 1.0, Colour(0,0,0) }
    } },
    { "WhiteCyan", {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(100, 200, 255) },
        GradientPoint { 0.2, Colour(0, 127, 255) },
        GradientPoint { 0.4, Colour(0, 64, 127) },
        GradientPoint { 1.0, Colour(0,0,0) }
    }},
    { "WhitePurple", {
        GradientPoint { 0.0, Colour(255, 255, 255) },
        GradientPoint { 0.1, Colour(200, 100, 255) },
        GradientPoint { 0.2, Colour(127, 0, 255) },
        GradientPoint { 0.4, Colour(64, 0, 127) },
        GradientPoint { 1.0, Colour(0,0,0) }
    } },
    { "PurpleBlue", {
        GradientPoint { 0.0, Colour(0, 0, 25) },
        GradientPoint { 0.1, Colour(0, 0, 125) },
        GradientPoint { 0.2, Colour(50, 0, 175) },
        GradientPoint { 0.4, Colour(100, 0, 255) },
        GradientPoint { 1.0, Colour(255, 0, 255) },
    } },
    { "PetrolOlive", {
            GradientPoint { 0.0, Colour(3, 14, 14)},
            GradientPoint { 0.1, Colour(34, 145, 140)},
            GradientPoint { 0.2, Colour(85, 145, 48)},
            GradientPoint { 0.5, Colour(38, 65, 22)},
            GradientPoint { 1.0, Colour(0,0,0)},
    } },
    { "PastelPink", {
        GradientPoint { 0.0, Colour(200, 180, 180) },
        GradientPoint { 0.01, Colour(255, 230, 230) },
        GradientPoint { 0.1, Colour(230, 200, 200) },
        GradientPoint { 1.0, Colour(180, 150, 150) }
    } }
};

