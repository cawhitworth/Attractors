#pragma once

#include <iostream>

class Bitmap
{
public:
    Bitmap(unsigned width, unsigned height);
    ~Bitmap();

    Bitmap(const Bitmap&) = delete;
    Bitmap(Bitmap&&);

    Bitmap& operator=(const Bitmap&) = delete;
    Bitmap& operator=(Bitmap &&);

    inline void Plot(unsigned x, unsigned y, unsigned color) const
    {
        m_pixels[x + y * m_width] = color;
    }

    inline unsigned Point(unsigned x, unsigned y) const
    {
        return m_pixels[x + y * m_width];
    }

    const unsigned char* RawData() const;

private:
    unsigned* m_pixels;
    unsigned m_width;
    unsigned m_height;
};

