
#include "colormap.h"

#include <cmath>
#include <utility>

namespace toolbox {
namespace cmap {

struct crow
{
    float x;
    float v;
};

crow red[] = { {0.000, 0.00},
               {0.125, 0.15},
               {0.250, 0.30},
               {0.375, 0.60},
               {0.500, 1.00},
               {0.625, 0.90},
               {0.750, 0.90},
               {0.875, 0.90},
               {1.000, 1.00} };

crow green[] = { {0.000, 0.00},
                 {0.125, 0.15},
                 {0.250, 0.15},
                 {0.375, 0.20},
                 {0.500, 0.25},
                 {0.625, 0.50},
                 {0.750, 0.75},
                 {0.875, 0.90},
                 {1.000, 1.00} };

crow blue[] = { {0.000, 0.00},
                {0.125, 0.50},
                {0.250, 0.75},
                {0.375, 0.50},
                {0.500, 0.15},
                {0.625, 0.00},
                {0.750, 0.10},
                {0.875, 0.50},
                {1.000, 1.00} };


inline RGB round_rgb(float r, float g, float b)
{
    RGB c;
    c.r = (int)std::roundf(255.0 * r);
    c.g = (int)std::roundf(255.0 * g);
    c.b = (int)std::roundf(255.0 * b);

    return c;
}


inline std::pair<int,int> find_range(const crow rows[], float f)
{
    const int nrows = 9;

    for (int i = 1; i < nrows; i++)
    {
        if (f < rows[i].x)
            return std::pair<int,int>(i, i-1);
    }
    return std::pair<int,int>(nrows - 1, nrows - 2);
}

inline float calc_component(const crow rows[], float f)
{
    auto ri = find_range(rows, f);

    auto & c1 = rows[ri.first];
    auto & c2 = rows[ri.second];

    return c1.v + (c2.v - c1.v) * (( f - c1.x) / ( c2.x - c1.x));
}

RGB cmr(float f)
{
    float r = calc_component(red, f);
    float g = calc_component(green, f);
    float b = calc_component(blue, f);

    return round_rgb(r, g, b);
}

RGB iron(float f)
{
    float r,g,b;

    const float r0 = 0.365079f;
    const float g0 = r0;
    const float g1 = 0.746032;
    const float b0 = g1;

    if (f <= r0)
        r = f / r0;
    else
        r = 1.0;

    if (f <= g0)
        g = 0;
    else if (f < g1)
        g = (f - g0) / (g1 - g0);
    else
        g = 1.0;

    if (f < b0)
        b = 0;
    else
        b = (f - b0) / (1.0 - b0);

    return round_rgb(r,g,b);
}

}}
