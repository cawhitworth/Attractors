
#include <map>
#include <functional>
#include "Geometry.h"

typedef struct {
    decimal a, b, c, d;
} Coefficients;

extern std::map< std::string, std::function<Coord(Coord, Coefficients)> > functions; 
