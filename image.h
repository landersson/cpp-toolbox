
#pragma once

#include <inttypes.h>
#include <stddef.h>

namespace toolbox
{

enum class PixelFormat
{
    NONE,
    MONO8,
    MONO10,
    MONO12,
    MONO16,
    BayerGB8,
    BayerGB10,
    RGB888,
    RGB32
};


struct ImageData
{
    ImageData(uint8_t *buf=nullptr, int w=0, int h=0, PixelFormat pf=PixelFormat::NONE, int row_stride=-1) :
        data(buf), width(w), height(h), pixfmt(pf)
    {
        stride = (row_stride > 0) ? row_stride : w * 3;
    }
    size_t size() const { return height * stride; }

    uint8_t *data;
    int width;
    int height;
    int stride;
    PixelFormat pixfmt;
};

}
