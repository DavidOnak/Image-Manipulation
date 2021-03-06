#include "HSLAPixel.h"

namespace imageUtil {
	HSLAPixel::HSLAPixel() {
		s = 0.0;
		l = 1.0;
		a = 1.0;
	}
	
	HSLAPixel::HSLAPixel(double hue, double saturation, double luminance) {
		h = hue;
		s = saturation;
		l = luminance;
	}
	
	HSLAPixel::HSLAPixel(double hue, double saturation, double luminance, double alpha) {
		h = hue;
		s = saturation;
		l = luminance;
		a = alpha;
	}	
}