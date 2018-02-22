#ifndef BACKEND_ENGINE_PREPROCESSING_SHAPEDETECT_SHAPEDETECT_H_
#define BACKEND_ENGINE_PREPROCESSING_SHAPEDETECT_SHAPEDETECT_H_
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <stdexcept>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace Edhitha2016{

	enum ValidShapes {SQUARE, RECTANGLE, TRAPEZOID, TRIANGLE, PENTAGON, HEXAGON, HEPTAGON, OCTAGON, CIRCLE, SEMICIRCLE, QUARTERCIRCLE, STAR, CROSS, NOTASHAPE};
	string returnShape(short int shape);
	void thinning(Mat& im);
	void thinningIteration(Mat& im, int iter);
	double angle(Point pt1, Point pt2, Point pt0);
	unsigned short backEndProcessing(Mat src);
}

#endif /* BACKEND_ENGINE_PREPROCESSING_SHAPEDETECT_SHAPEDETECT_H_ */
