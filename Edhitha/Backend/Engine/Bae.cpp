/*
 * bae.cpp
 *
 *  Created on: 17 Aug 2015
 *      Author: arjun
 */
#include <iostream>
#include "Bae.h"

#include "PreProcessing/OCR/TesseractOCR.h"

using namespace Edhitha2016;

namespace Edhitha2016{

	Bae::Bae():anomalyDetect(){
		addresses = NULL;
		isRectanglesBound=false;
		lowerBinaryThreshold=0;

		lowerAreaThreshold=0,upperAreaThreshold=0;
	}

	Bae::Bae(tbb::concurrent_queue< boost::filesystem::path >* addresses ,Size dimensions ,int type ,bool isRectanglesBound,double lowerAreaThreshold,
			double upperAreaThreshold, int lowerBinaryThreshold)
	:anomalyDetect(dimensions,type,isRectanglesBound,lowerAreaThreshold
					,upperAreaThreshold,lowerBinaryThreshold){
		// TODO Auto-generated constructor stub
		this->addresses = addresses;
		this->lowerAreaThreshold = lowerAreaThreshold;
		this->upperAreaThreshold = upperAreaThreshold;

		this->lowerBinaryThreshold = lowerBinaryThreshold;

		this->isRectanglesBound =isRectanglesBound;
	}

	Bae::~Bae() {
		// TODO Auto-generated destructor stub
		printf("Destroyed bae\n");
	}
	bool Bae::setFrame(){

		if(this->addresses->empty())
			return false;

		fs::path temp;
		addresses->try_pop(temp);

		this->anomalyDetect.setFrame(temp.string());
		this->currentImage = temp;

		return true;
	}
}
