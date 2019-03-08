#include <inttypes.h>

#pragma once

namespace toolbox
{

struct RGB
{
    RGB(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : r(red), g(green), b(blue)
    { }

    void operator+=(const RGB & other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
    }

    RGB operator*(float s)
    {
        RGB scaled = *this;
        scaled.r *= s;
        scaled.g *= s;
        scaled.b *= s;
        return scaled;
    }

    uint8_t r,g,b;

};

}
