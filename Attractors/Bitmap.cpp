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

Bitmap::Bitmap(Bitmap&& bitmap)
    : m_width(bitmap.m_width)
    , m_height(bitmap.m_height)
    , m_pixels(bitmap.m_pixels)
{
    bitmap.m_pixels = nullptr;
    bitmap.m_width = bitmap.m_height = 0;
}

Bitmap& Bitmap::operator=(Bitmap&& bitmap)
{
    m_height = bitmap.m_height;
    m_width = bitmap.m_width;
    m_pixels = bitmap.m_pixels;
    bitmap.m_pixels = nullptr;
    bitmap.m_width = bitmap.m_height = 0;
    return *this;
}

Bitmap::~Bitmap()
{
    if (m_pixels != nullptr)
        delete [] m_pixels;
}

const unsigned char* Bitmap::RawData() const
{
    return reinterpret_cast<unsigned char*>(m_pixels);
}
