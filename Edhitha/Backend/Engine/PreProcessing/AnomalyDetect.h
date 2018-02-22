/*
 * AnomalyDetect.h
 *
 *  Created on: 23 Sep 2015
 *      Author: arjun
 *      Handles Processing.
 */
#define CUDA
#ifdef CUDA
#ifndef CUDAANOMALYDETECT_H_
#define CUDAANOMALYDETECT_H_

#include "OCR/TesseractOCR.h"
#include "ColourFilter/ColorDetect.h"
#include "ShapeDetect/ShapeDetect.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <math.h>
#include <queue>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

extern "C"{
#include "../MetaData/gpsData.h"
}

#include <GeographicLib/GeodesicExact.hpp>
#include <GeographicLib/Constants.hpp>

using namespace cv;
using namespace std;
using namespace GeographicLib;

namespace Edhitha2016{

using namespace Edhitha2016;

struct Target{

	string fileAddress;

	Mat targetImage;
	unsigned short targetColourConfig;
	string targetAlphabet;
	long double targetOrientation;
	float targetConfidence;

	unsigned short shapeCode;

	//Constraint Parameters
	float shapeHeight,shapeWidth;
	float alphabetHeight,alphabetWidth;

	//Empirical Parameters
	float ratio;

	Point localTargetPosition;
	long double latitude,longitude;
};

class AnomalyDetect{

	Rect enlargeROI(Mat frm, Rect boundingBox, int padding);
	void colourFilterAndOCR();

	int lowerBinaryThreshold;

	double lowerAreaThreshold,upperAreaThreshold;

	bool isRectanglesBound;
	vector<Rect> boundingRectangles;

	Mat __frame;

	queue<Target> target;

	TesseractOCR tesseractOCR;

public:
	AnomalyDetect();
	AnomalyDetect(Size dimensions,int type,bool isRectanglesBound,
			double lowerAreaThreshold = 0.000078986,double upperAreaThreshold = 0.001263784,
			int lowerBinaryThreshold = 50);//53.54330709 = px m^-1 || 16.32 px ft^-1

	~AnomalyDetect();

	Mat getFrame();
	void setFrame(Mat* frame);
	void setFrame(string address);

	void detectTarget();

	queue<Target>& getTargets();
};
}
#endif /* CUDAANOMALYDETECT_H_ */
#endif
