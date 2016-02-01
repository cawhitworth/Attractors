#pragma once

#define R(col) ((unsigned char)( (col) & 0xff ))
#define G(col) ((unsigned char)( ( (col) >> 8 ) & 0xff ))
#define B(col) ((unsigned char)( ( (col) >> 16 ) & 0xff ))
#define A(col) ((unsigned char)( ( (col) >> 24 ) & 0xff ))

#define SetA(col, val) { col = (col & 0x00ffffff) | ( (val) << 24) ; }

#define ColourA(r,g,b,a) \
    ((unsigned)( \
        ( (r) & 0xff ) |\
        ( ( (g) & 0xff ) << 8) |\
        ( ( (b) & 0xff ) << 16) |\
        ( ( (a) & 0xff ) << 24)\
    ))

#define Colour(r,g,b) \
    ((unsigned)( \
        ( (r) & 0xff ) |\
        ( ( (g) & 0xff ) << 8) |\
        ( ( (b) & 0xff ) << 16) |\
        ( 0xff << 24)\
    ))

inline unsigned Distance(unsigned c1, unsigned c2)
{
    int r = R(c1) - R(c2);
    int g = G(c1) - G(c2);
    int b = B(c1) - B(c2);

    auto d = ((r*r) + (g*g) + (b*b));

    return (unsigned) d;
}