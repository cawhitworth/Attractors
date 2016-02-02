#include "stdafx.h"
#include "Bitmap.h"
#include "Colour.h"

Bitmap::Bitmap(unsigned width, unsigned height) : Bitmap(width, height, Colour(0,0,0))
{
}

Bitmap::Bitmap(unsigned width, unsigned height, unsigned colour)
    : m_width(width)
    , m_height(height)
{
    m_pixels.resize(width * height);
    for (auto x = 0U; x < width; x++)
    {
        for (auto y = 0U; y < height; y++)
        {
            Plot(x, y, colour);
        }
    }
}

Bitmap Bitmap::Copy() const
{
    Bitmap bmp{ m_width, m_height };
    std::copy(begin(), end(), bmp.begin());
    return bmp;
}

const unsigned char* Bitmap::RawData() const
{
    return reinterpret_cast<unsigned const char*>(&m_pixels[0]);
}
