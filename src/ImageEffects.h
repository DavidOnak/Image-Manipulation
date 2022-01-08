#ifndef Image_Effect_H
#define Image_Effect_H

#include "imageUtil/hslaPNG.h"
using namespace imageUtil;

hslaPNG grayscale(hslaPNG image);
hslaPNG createSpotlight(hslaPNG image, int centerX, int centerY);
hslaPNG ubcify(hslaPNG image);
hslaPNG watermark(hslaPNG firstImage, hslaPNG secondImage);

#endif
