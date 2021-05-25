#include <cmath>
#include <string>
#include "Decimal.h"
#include "Geometry.h"
#include "Functions.h"


Coord clifford_attractor(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = std::sin(coeffs.a * p.y) + coeffs.c * std::cos(coeffs.a * p.x);
    out.y = std::sin(coeffs.b * p.x) + coeffs.d * std::cos(coeffs.b * p.y);

    return out;
}

Coord peter_de_jong_attractor(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = std::sin(coeffs.a * p.y) - std::cos(coeffs.b * p.x);
    out.y = std::sin(coeffs.c * p.x) - std::cos(coeffs.d * p.y);

    return out;
}

Coord experiment(Coord p, Coefficients coeffs)
{
    Coord out;
    out.x = coeffs.c * std::sin(coeffs.a * (p.x + p.y)) - coeffs.d * std::cos(coeffs.b * (p.y - p.x));
    out.y = coeffs.d * std::cos(coeffs.a * (p.x - p.y)) + coeffs.c * std::sin(coeffs.b * (p.y + p.x));

    return out;
}

Coord experiment2(Coord p, Coefficients coeffs)
{
    Coord out;
    decimal ab = coeffs.a + coeffs.b;
    decimal ba = coeffs.b - coeffs.a;
    decimal cd = coeffs.c + coeffs.d;
    decimal dc = coeffs.d - coeffs.c;
    out.x = coeffs.c * std::sin(ab * p.y) + coeffs.d * std::cos(ba * p.x);
    out.y = coeffs.a * std::sin(cd * p.x) - coeffs.b * std::cos(dc * p.y);

    return out;
}

std::map< std::string, std::function<Coord(Coord, Coefficients)> > functions = {
        { "Clifford", clifford_attractor},
        { "DeJong", peter_de_jong_attractor},
        { "Experiment", experiment },
        { "Experiment2", experiment2 }
};
