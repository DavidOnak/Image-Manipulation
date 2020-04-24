#include "stats.h"
#include <cmath>

/*
*  The stats class is used to calculate accumulated colour values for each pixel in an image.
*  It can be called to return the average value or a score value for a proved rectangle area for twoDTree.c
*/

// constructor for stats, fills vectors using provided pixels from image
stats::stats(PNG & im){
	for (int x = 0; x < im.width(); x++) {
		// make vectors for row here
		vector<long> sumRedCol;
		vector<long> sumGreenCol;
		vector<long> sumBlueCol;
		vector<long> sumsqRedCol;
		vector<long> sumsqGreenCol;
		vector<long> sumsqBlueCol;
		int rsum = 0;
		int bsum = 0;
		int gsum = 0;
		int rsqsum = 0;
		int bsqsum = 0;
		int gsqsum = 0;

		// iterate through a full row
		for (int y = 0; y < im.height(); y++) {
			// do calcutations and fill row vectors
			if (x <= 0) {
				sumRedCol.push_back(im.getPixel(x, y)->r + rsum);
				rsum += im.getPixel(x, y)->r;
				sumBlueCol.push_back(im.getPixel(x, y)->b + bsum);
				bsum += im.getPixel(x, y)->b;
				sumGreenCol.push_back(im.getPixel(x, y)->g + gsum);
				gsum += im.getPixel(x, y)->g;

				sumsqRedCol.push_back(pow(im.getPixel(x, y)->r, 2) + rsqsum);
				rsqsum += pow(im.getPixel(x, y)->r, 2);
				sumsqBlueCol.push_back(pow(im.getPixel(x, y)->b, 2) + bsqsum);
				bsqsum += pow(im.getPixel(x, y)->b, 2);
				sumsqGreenCol.push_back(pow(im.getPixel(x, y)->g, 2) + gsqsum);
				gsqsum += pow(im.getPixel(x, y)->g, 2);

			} else {
				sumRedCol.push_back(sumRed[x-1][y] + im.getPixel(x, y)->r + rsum);
				rsum += im.getPixel(x, y)->r;
				sumBlueCol.push_back(sumBlue[x-1][y] + im.getPixel(x, y)->b + bsum);
				bsum += im.getPixel(x, y)->b;
				sumGreenCol.push_back(sumGreen[x-1][y] + im.getPixel(x, y)->g + gsum);
				gsum += im.getPixel(x, y)->g;

				sumsqRedCol.push_back(sumsqRed[x-1][y] + pow(im.getPixel(x, y)->r, 2) + rsqsum);
				rsqsum += pow(im.getPixel(x, y)->r, 2);
				sumsqBlueCol.push_back(sumsqBlue[x-1][y] + pow(im.getPixel(x, y)->b, 2) + bsqsum);
				bsqsum += pow(im.getPixel(x, y)->b, 2);
				sumsqGreenCol.push_back(sumsqGreen[x-1][y] + pow(im.getPixel(x, y)->g, 2) + gsqsum);
				gsqsum += pow(im.getPixel(x, y)->g, 2);
			}
		}

		// add row vectors to main vectors
		sumRed.push_back(sumRedCol);
		sumGreen.push_back(sumGreenCol);
		sumBlue.push_back(sumBlueCol);
		sumsqRed.push_back(sumsqRedCol);
		sumsqGreen.push_back(sumsqGreenCol);
		sumsqBlue.push_back(sumsqBlueCol);
	}
}

// returns the accumulated sum of a colour value for a given rectangle
long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
	long returnR;
	long returnB;
	long returnG;

	// using ul and lr corner, get the sum of a colour, there are 3 cases for each colour
	switch (channel) {
		case 'r':
			returnR = sumRed[lr.first][lr.second];
			if (ul.first > 0) {
				returnR -= sumRed[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnR -= sumRed[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnR += sumRed[ul.first-1][ul.second-1];
			}
			return returnR;
		case 'g':
			returnG = sumGreen[lr.first][lr.second];
			if (ul.first > 0) {
				returnG -= sumGreen[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnG -= sumGreen[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnG += sumGreen[ul.first-1][ul.second-1];
			}
			return returnG;
		case 'b':
			returnB = sumBlue[lr.first][lr.second];
			if (ul.first > 0) {
				returnB -= sumBlue[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnB -= sumBlue[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnB += sumBlue[ul.first-1][ul.second-1];
			}
			return returnB;
		default:
			printf("Invalid Channel\n");
			return 0;
	}
}

// returns the accumulated sum of a colour value squared for a given rectangle
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
	long returnR;
	long returnB;
	long returnG;

	// using ul and lr corner, get the squared sum of a colour, there are 3 cases for each colour
	switch (channel) {
		case 'r':
			returnR = sumsqRed[lr.first][lr.second];
			if (ul.first > 0) {
				returnR -= sumsqRed[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnR -= sumsqRed[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnR += sumsqRed[ul.first-1][ul.second-1];
			}
			return returnR;
		case 'g':
			returnG = sumsqGreen[lr.first][lr.second];
			if (ul.first > 0) {
				returnG -= sumsqGreen[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnG -= sumsqGreen[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnG += sumsqGreen[ul.first-1][ul.second-1];
			}
			return returnG;
		case 'b':
			returnB = sumsqBlue[lr.first][lr.second];
			if (ul.first > 0) {
				returnB -= sumsqBlue[ul.first-1][lr.second];
			}
			if (ul.second > 0) {
				returnB -= sumsqBlue[lr.first][ul.second-1];
			}
			if ((ul.first > 0) && (ul.second > 0)) {
				returnB += sumsqBlue[ul.first-1][ul.second-1];
			}
			return returnB;
		default:
			printf("Invalid Channel\n");
			return 0;
	}
}

// returns the area of a rectangle given the ul and lr corner
long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	long numCols = lr.first  - ul.first + 1;
	long numRows = lr.second - ul.second + 1;

	return numCols * numRows;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
	long rsum = getSum('r', ul, lr);
	long bsum =  getSum('b', ul, lr);
	long gsum = getSum('g', ul, lr);

	long rsumSq = getSumSq('r', ul, lr);
	long bsumSq =  getSumSq('b', ul, lr);
	long gsumSq = getSumSq('g', ul, lr);

	long redScore = rsumSq - (pow(rsum,2) / rectArea(ul, lr));
	long blueScore = bsumSq - (pow(bsum,2) / rectArea(ul, lr));
	long greenScore = gsumSq - (pow(gsum,2) / rectArea(ul, lr));

	return redScore + blueScore + greenScore;
}

// given a rectangle, return the average color value over the rectangle as a pixel.
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	long area = rectArea(ul, lr);

	long r =  (getSum('r', ul, lr) / area);
	long b =  (getSum('b', ul, lr) / area);
	long g =  (getSum('g', ul, lr) / area);

	return RGBAPixel(r, g, b);
}
