#pragma once

#include <iostream>
#include <vector>

class Bitmap
{
public:
    Bitmap() : Bitmap(0, 0){}
    Bitmap(unsigned width, unsigned height);
    Bitmap(unsigned width, unsigned height, std::vector<unsigned char> data);
    Bitmap(unsigned width, unsigned height, unsigned colour);

    Bitmap(const Bitmap&) = delete;
    Bitmap(Bitmap&&) = default;

    Bitmap& operator=(const Bitmap&) = delete;
    Bitmap& operator=(Bitmap &&) = default;

    Bitmap Copy() const;

    inline void Plot(unsigned x, unsigned y, unsigned color)
    {
        m_pixels[x + y * m_width] = color;
    }

    inline unsigned Point(unsigned x, unsigned y) const
    {
        return m_pixels[x + y * m_width];
    }

    const unsigned char* RawData() const;

    std::vector<unsigned>::const_iterator begin() const { return m_pixels.begin(); }
    std::vector<unsigned>::const_iterator end() const { return m_pixels.end(); }

    std::vector<unsigned>::iterator begin()  { return m_pixels.begin(); }
    std::vector<unsigned>::iterator end() { return m_pixels.end(); }

    unsigned Height() const { return m_height; }
    unsigned Width() const { return m_width; }

private:
    std::vector<unsigned> m_pixels;
    unsigned m_width;
    unsigned m_height;
};

