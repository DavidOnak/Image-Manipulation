#include <iostream>
#include <cmath>
#include <cstdlib>

#include "imageUtil/hslaPNG.h"
#include "imageUtil/HSLAPixel.h"
#include "ImageEffects.h"

using namespace imageUtil;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
hslaPNG grayscale(hslaPNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel *pixel = image.getPixel(x, y);

      // `pixel` is a pointer to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly.  No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel->s = 0;
    }
  }

  return image;
}

/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 200 pixels away, the luminance will always 0.
 *
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
hslaPNG createSpotlight(hslaPNG image, int centerX, int centerY) {
	int height = image.height();
	int width = image.width();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int pixelDistance = (int) sqrt(pow(centerX - x, 2.0) + pow(centerY - y, 2.0));
			double percent = (100 - (0.5 * pixelDistance)) / 100;

			if (percent < 0) {
				percent = 0;
			}

			HSLAPixel *pixel = image.getPixel(x, y);
			pixel->l = pixel->l * percent;
		}
	}

  return image;
}


/**
 * Returns a image transformed to UBC colors.
 *
 * The hue of every pixel is set to the a hue value of either yellow or
 * blue, based on if the pixel's hue value is closer to yellow than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The UBCify'd image.
**/
hslaPNG ubcify(hslaPNG image) {
	int height = image.height();
	int width = image.width();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			HSLAPixel *pixel = image.getPixel(x, y);

			int blueDist = abs(240 - pixel->h);
			int yellowDist = abs(60 - pixel->h);

			if (blueDist <= yellowDist) {
				pixel->h = 210; // make pixel blue
			} else {
				pixel->h = 40; // make pixel yellow
			}
		}
	}

  return image;
}


/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs.
* @param secondImage The second of the two PNGs.
*
* @return The watermarked image.
*/
hslaPNG watermark(hslaPNG firstImage, hslaPNG secondImage) {
	int height = secondImage.height();
	int width = secondImage.width();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			HSLAPixel *pixel2 = secondImage.getPixel(x, y);

			if (pixel2->l == 1.0) {
				HSLAPixel *pixel1 = firstImage.getPixel(x, y);
				pixel1->l += 0.2;
				
				// incase luminance gets too high set to 1
				if (pixel1->l > 1.0) {
					pixel1->l == 1.0;
				}
			}
		}
	}

  return firstImage;
}

