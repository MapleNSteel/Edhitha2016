/*
 * Baelando.h
 *
 *  Created on: 21 Oct 2015
 *      Author: arjun
 *      Handles Image-acquisition.
 */

#ifndef DEBUG_BACKEND_BAELANDO_H_
#define DEBUG_BACKEND_BAELANDO_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

#include <tbb/concurrent_queue.h>

#include "Engine/Bae.h"
#include "Engine/PreProcessing/ColourFilter/ColorDetect.h"
#include "Engine/PreProcessing/ShapeDetect/ShapeDetect.h"

namespace fs = ::boost::filesystem;

namespace Edhitha2016 {

	using namespace Edhitha2016;

	class Baelando {

		fs::path rootDirectory;
		unsigned int targetsWritten;

		public:
		Bae bae;

		Baelando();
		Baelando(fs::path rootDirectory,tbb::concurrent_queue< boost::filesystem::path >* addresses,Size dimensions, int type ,bool isRectanglesBound,double lowerAreaThreshold,
						double upperAreaThreshold,int lowerBinaryThreshold);
		~Baelando();

		Baelando& operator= (Baelando& other);

		static string getImageRootDirectory(char* dir);
		static void getAllImages(const fs::path& root,tbb::concurrent_queue< boost::filesystem::path >* addresses);

		void writeImages(Mat saliencymap,Mat thresholdedFrame);
		void writeTargetImages(queue<Target> target);
	};

} /* namespace Edhitha2016 */

#endif /* DEBUG_BACKEND_BAELANDO_H_ */

