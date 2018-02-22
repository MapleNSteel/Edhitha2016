
/*
 * AnomalyDetect.cpp
 *
 *  Created on: 15 Aug 2015
 *      Author: arjun
 */
#define CUDA
#ifdef CUDA
#include "AnomalyDetect.h"

namespace Edhitha2016{

int minArea;
int maxArea;

vector<vector<Point> > contours;

Mat __saliencyFrame;

Target currentTarget;

short targetCount;
unsigned int clamp(unsigned int number, unsigned int low, unsigned int high){
	if(number<low){
		return low;
	}
	else if(number>high){
		return high;
	}
	else
		return number;
}
AnomalyDetect::AnomalyDetect(){
	lowerBinaryThreshold=0;

	lowerAreaThreshold=0,upperAreaThreshold=0;
	isRectanglesBound=false;
}
AnomalyDetect::AnomalyDetect(Size dimension,int type,bool isRectanglesBound,
		double lowerAreaThreshold, double upperAreaThreshold, int lowerBinaryThreshold){


	this->__frame = Mat(dimension,type);

	this->lowerAreaThreshold = lowerAreaThreshold;
	this->upperAreaThreshold = upperAreaThreshold;

	this->lowerBinaryThreshold = lowerBinaryThreshold;

	minArea = (int)(lowerAreaThreshold*dimension.height*dimension.width);
	maxArea = (int)(upperAreaThreshold*dimension.height*dimension.width);

	this->isRectanglesBound =isRectanglesBound;

	__saliencyFrame = Mat(dimension,CV_8UC3);

	targetCount=0;

}
inline std::string intTostring(int number){

	std::stringstream ss;
	ss << number;
	return ss.str();
}

AnomalyDetect::~AnomalyDetect(){

	__frame.release();
	__saliencyFrame.release();

	printf("Destroyed CUDA\n");

}

Rect AnomalyDetect::enlargeROI(Mat frm, Rect boundingBox, int padding)
{
	 Rect returnRect = Rect(boundingBox.x - padding, boundingBox.y - padding, boundingBox.width + (padding * 2), boundingBox.height + (padding * 2));
	    if (returnRect.x < 0)returnRect.x = 0;
	    if (returnRect.y < 0)returnRect.y = 0;
	    if (returnRect.x+returnRect.width >= frm.cols)returnRect.width = frm.cols-returnRect.x;
	    if (returnRect.y+returnRect.height >= frm.rows)returnRect.height = frm.rows-returnRect.y;
	    return returnRect;
}
void AnomalyDetect::detectTarget() {
	Mat dst, grey;
	cv::cvtColor(__frame, dst, CV_BGR2HSV);
	Scalar avg_color = mean(dst);

	Mat newimg(3000, 4496, CV_8UC3, avg_color);
	Mat final;

	cv::subtract(dst, newimg, final);
	cv::cvtColor(final, final, CV_HSV2BGR);
	cv::cvtColor(final, final, CV_BGR2GRAY);

	vector<vector<Point> > contours;
	vector<Rect> bboxes;
	Ptr<MSER> mser = MSER::create(5, 1000, 3000, 0.05, 0.2, 200, 1.01, 0.002, 5);
	mser->detectRegions(final, contours, bboxes);
	Mat crop2, crop, mask;

	if (bboxes.size()== 0)
		return;


	for(unsigned int i=0;i<bboxes.size();i++)
			  {
			   bboxes[i]=enlargeROI(__frame,bboxes[i],5);
			   if(i>0)
			   {
				   if((abs(bboxes[i].width-bboxes[i-1].width)<10)&&(abs(bboxes[i].height-bboxes[i-1].height)<10))
				   continue;
			   }

		crop2 = __frame(bboxes[i]).clone();
		Mat img1;
		cv::medianBlur(crop2,crop,5);
		cv::cvtColor(crop, img1, CV_BGR2GRAY);
		Canny(img1, img1, 100, 150);
		dilate(img1, img1, Mat(), Point(-1,-1));
	    erode(img1,img1,Mat(),Point(-1,-1));
		vector<Vec4i> lines;
		Mat img2;
		img1.copyTo(img2);
		vector<vector<Point> >contours1;
		findContours(img1,contours1,lines, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE,Point(0,0));
		Mat mask = Mat::zeros(img1.rows, img1.cols, CV_8UC1);
		vector<double> areas(contours1.size());
		for (unsigned int i = 0; i < contours1.size(); i++)
			areas[i] = contourArea(Mat(contours1[i]));
		double max;
		Point maxPosition;
		cv::minMaxLoc(Mat(areas), 0, &max, 0, &maxPosition);

		drawContours(mask, contours1, maxPosition.y, Scalar(1), CV_FILLED);
		Mat crop1(crop.rows, crop.cols, CV_8UC3);
		crop1.setTo(Scalar(0, 0, 0));
		crop.copyTo(crop1, mask);
		cv::normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);

		crop1.copyTo(currentTarget.targetImage);
		target.push(currentTarget);
	                    }
	colourFilterAndOCR();
}
void AnomalyDetect::colourFilterAndOCR(){
	Mat shapeImage,alphabetImage,alphabetImageMask,shapeImageMask,tempAlphabetImage;

	Mat alphabetMorphologyElement=getStructuringElement(MORPH_OPEN,Size(3,3));
	unsigned int limit=target.size();
	for(unsigned int i = 0 ; i < limit ; i++){
		shapeImage=Mat::zeros(target.front().targetImage.size(),CV_8UC3);
		alphabetImage=Mat::zeros(target.front().targetImage.size(),CV_8UC3);
		tempAlphabetImage=Mat::zeros(target.front().targetImage.size(),CV_8UC3);

		short colourConfig = colorDetect(target.front().targetImage,shapeImage,alphabetImage);//Where BrE meets MuE.
		char alphabet;
		float confidence,orientation;
		//10/june blr to dulles, 28/june to san diego; 20/july san fran to blr
		//10/june blr to dulles, 23/June NY to LA || 24/June Dulles to LA, 29/June LA to BLR
		bool weirdColour = colourConfig%12 == NAC || colourConfig/12 == NAC || colourConfig%12 == ABS_BLACK
				|| colourConfig/12 == ABS_BLACK;
		bool equalColour = colourConfig/12==colourConfig%12;//false; //Debatable

		if(!( weirdColour || equalColour )){

			inRange(shapeImage, Scalar(1, 1, 1), Scalar(255, 255, 255), shapeImageMask);

			inRange(alphabetImage, Scalar(1, 1, 1), Scalar(255, 255, 255), alphabetImageMask);
			morphologyEx(alphabetImageMask,alphabetImageMask,MORPH_OPEN,alphabetMorphologyElement);

			int alphabetArea = cv::countNonZero(alphabetImageMask);
			int shapeArea = cv::countNonZero(shapeImageMask)+alphabetArea;

			vector<Vec4i> hierarchy;
			vector<vector<Point> > alphabetContour;

			findContours(alphabetImageMask, alphabetContour, hierarchy, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);

			Moments shapeMoment = moments(shapeImageMask,1);
			Moments alphabetMoment = moments(alphabetImageMask,1);

			Vec3f shapeCentre,alphabetCentre;

			shapeCentre = Vec3f(shapeMoment.m10/shapeMoment.m00,shapeMoment.m01/shapeMoment.m00);
			alphabetCentre = Vec3f(alphabetMoment.m10/alphabetMoment.m00,alphabetMoment.m01/alphabetMoment.m00);

			float ratio = ((float)alphabetArea/shapeArea);//Problematic
			if(!(ratio>=0.05 &&ratio<=0.30)){
				goto erase;
			}
			if((alphabetArea<1 || shapeArea<1) || (alphabetArea==shapeArea) || cv::norm(shapeCentre,alphabetCentre) > 10.0){
				erase:target.pop();
				continue;
			}

			alphabetImage.copyTo(tempAlphabetImage,alphabetImageMask);
			tempAlphabetImage.copyTo(alphabetImage);

			tesseractOCR.getCharacterAndOrientation(alphabetImage,alphabet,orientation,confidence);

//			cout<<"Alphabet:"<<alphabet<<endl;
//			cout<<"Alphabet Confidence:"<<confidence<<endl;

			if(confidence<80){
				target.pop();
				continue;
			}
			target.front().shapeCode=backEndProcessing(shapeImage);

			RotatedRect alphabetBoundingBox = minAreaRect(alphabetContour[0]);

			//No such thing as too much information.
			target.front().alphabetHeight = alphabetBoundingBox.size.height;
			target.front().alphabetWidth = alphabetBoundingBox.size.width;

			//				if()
			cout<<"Shape:"<<returnShape(target.front().shapeCode)<<endl;

			cout<<"Shape Area:"<<shapeArea<<endl;
			cout<<"Alphabet Area:"<<alphabetArea<<endl;
			target.front().ratio =(100.0*((float)alphabetArea/shapeArea));
		}
		else{
			target.pop();
			continue;
		}
		target.front().targetColourConfig=colourConfig;
		target.front().targetAlphabet=alphabet;
		cout<<"Alphabet:"<<target.front().targetAlphabet<<endl;
		cout<<"Target Orientation:"<<target.front().targetOrientation<<endl;

		long double x,y;
		x = target.front().localTargetPosition.x*cos(target.front().targetOrientation* 3.141592653589793 / 180.0) + target.front().localTargetPosition.y*sin(target.front().targetOrientation* 3.141592653589793 / 180.0);
		y = -target.front().localTargetPosition.x*sin(target.front().targetOrientation* 3.141592653589793 / 180.0) + target.front().localTargetPosition.y*cos(target.front().targetOrientation* 3.141592653589793 / 180.0);

		x*=0.018676471;
		y*=0.018676471;

		target.front().targetOrientation+=orientation;
		if(target.front().targetOrientation>=360)
			target.front().targetOrientation-=360;

		cout<<setprecision(11)<<"Longitude:"<<target.front().longitude<<endl;
		cout<<setprecision(11)<<"Latitude:"<<target.front().latitude<<endl;

		double latitude,longitude;

		const GeodesicExact& geodesicExact = GeodesicExact::WGS84();
		geodesicExact.Direct(target.front().latitude,target.front().longitude,atan2(x,y),sqrt(x*x+y*y),latitude,longitude);
		target.front().latitude=latitude;
		target.front().longitude=longitude;

		cout<<setprecision(11)<<"Target Longitude:"<<target.front().longitude<<endl;
		cout<<setprecision(11)<<"Target Latitude:"<<target.front().latitude<<endl;

		target.front().targetConfidence=confidence;

		target.push(target.front());
		target.pop();
	}
}

Mat AnomalyDetect::getFrame(){
	return __frame;
}
void AnomalyDetect::setFrame(Mat* frame){

	frame->copyTo(this->__frame);
}
void AnomalyDetect::setFrame(string address){

	cout<<endl<<"Address:"<<address<<endl;

	do{
		this->__frame = imread(address);
	}while(__frame.empty());

	currentTarget.fileAddress=address;
	entryPoint(address.c_str(),&currentTarget.latitude,&currentTarget.longitude, &currentTarget.targetOrientation);
}
queue<Target>& AnomalyDetect::getTargets(){
	return target;
}
}
#endif
