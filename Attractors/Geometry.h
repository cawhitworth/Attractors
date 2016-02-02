#pragma once 

#include "Decimal.h"

template<typename T>
struct _Coord
{
    T x;
    T y;

    _Coord(T _x, T _y) : x(_x), y(_y) {}
    _Coord() : x(0.0), y(0.0) {}
};

template<typename T>
struct _Rect
{
    _Coord<T> bl;
    _Coord<T> tr;

    T width() const { return tr.x - bl.x; }
    T height() const { return tr.y - bl.y; }
};


typedef _Coord<decimal> Coord;
typedef _Rect<decimal> Rect;