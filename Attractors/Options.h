#pragma once
#include <string>
#include "Decimal.h"

typedef struct
{
    bool random_everything = true;

    bool expose = true;
    bool develop = true;
    bool random_coefficients = true;

    unsigned width = 1920U, height = 1080;
    unsigned iterations = 100 * 1000;

    std::string palette = "";
    std::string attractor = "";

    std::string exposed_filename;
    std::string output_filename = "output.png";

    decimal gamma = 1.5;
    decimal exposure = 0.8;

    void Check()
    {
        if (random_everything)
        {
            palette = "";
            attractor = "";
            random_coefficients = true;
        }
    }
} StartupOptions;

StartupOptions get_startup_options(int argc, char** argv);