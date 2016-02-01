#pragma once

class Bitmap
{
public:
    Bitmap(unsigned width, unsigned height);
    ~Bitmap();


    inline void Plot(unsigned x, unsigned y, unsigned color)
    {
        m_pixels[x + y * m_width] = color;
    }

    inline const unsigned Point(unsigned x, unsigned y) const
    {
        return m_pixels[x + y * m_width];
    }

    const unsigned char* RawData() const;

private:
    unsigned* m_pixels;
    unsigned m_width;
    unsigned m_height;
};

