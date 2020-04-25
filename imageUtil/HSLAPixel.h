#ifndef HSLAPixel_H
#define HSLAPixel_H

namespace imageUtil {
	class HSLAPixel {
		public:
		double h;
		double s;
		double l;
		double a;

		HSLAPixel();
		HSLAPixel(double, double, double);
		HSLAPixel(double, double, double, double);	 
	};
}

#endif