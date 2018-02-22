/*
 * OCR.h
 *
 *  Created on: 6 Nov 2015
 *      Author: arjun
 */

#ifndef BACKEND_ENGINE_OCR_OCR_H_
#define BACKEND_ENGINE_OCR_OCR_H_

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <fstream>
#include <tbb/tbb.h>

using namespace cv;
using namespace tesseract;
using namespace std;
using namespace tbb;


namespace Edhitha2016 {
class TesseractOCR{
	void tesseractOCR(Mat image,string& alphabet,float& confidence);
	TessBaseAPI api;
	public:
	void getCharacterAndOrientation(Mat target,char& alphabet,float& orientation,float& confidence);
};
}/* namespace Edhitha2016 */

#endif /* BACKEND_ENGINE_OCR_OCR_H_ */
