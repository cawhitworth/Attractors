#include "stdafx.h"
#include "Bitmap.h"

Bitmap::Bitmap(unsigned width, unsigned height)
{
    m_width = width; m_height = height;

    m_pixels = new unsigned [width * height];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Plot(x, y, 0xff000000);
        }
    }
}

Bitmap::~Bitmap()
{
    delete [] m_pixels;
}

const unsigned char* Bitmap::RawData() const
{
    return reinterpret_cast<unsigned char*>(m_pixels);
}
