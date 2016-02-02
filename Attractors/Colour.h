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

template<typename T>
inline unsigned Lerp(unsigned c1, unsigned c2, T amount)
{
    return Colour(
        static_cast<unsigned>(R(c1) + (R(c2) - R(c1)) * amount),
        static_cast<unsigned>(G(c1) + (G(c2) - G(c1)) * amount),
        static_cast<unsigned>(B(c1) + (B(c2) - B(c1)) * amount)
        );
}

inline unsigned Distance(unsigned c1, unsigned c2)
{
    int r = R(c1) - R(c2);
    int g = G(c1) - G(c2);
    int b = B(c1) - B(c2);

    auto d = ((r*r) + (g*g) + (b*b));

    return (unsigned) d;
}