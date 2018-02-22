/*
 * ColorDetect.h
 *
 *  Created on: 23-Nov-2015
 *      Author: vikram
 */

#ifndef SRC_COLORDETECT_H_
#define SRC_COLORDETECT_H_
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <tbb/tbb.h>

#include<iostream>
#include<string>
#include <set>

using namespace cv;
using namespace std;
using namespace tbb;

namespace Edhitha2016 {
	enum ValidColours {ABS_BLACK,WHITE,BLACK,GRAY,RED,BLUE,GREEN,YELLOW,PURPLE,BROWN,ORANGE,NAC};
	unsigned short colorDetect(Mat src,Mat& shapeImage,Mat& alphabetImage);
	string getColor(unsigned short colorCode);

}
#endif /* SRC_COLORDETECT_H_ */
