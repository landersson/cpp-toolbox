
#pragma once
#include "image.h"

namespace toolbox {
int
compress_jpeg(unsigned char** buffer,
              unsigned width,
              unsigned height,
              unsigned components,
              int quality = 80);
toolbox::ImageData
decompress_jpeg(uint8_t* buffer, size_t len);

}
