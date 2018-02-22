/*
 * bae.h
 * Handles processing-calls,and exif extraction.
*/
	#ifndef BAE_H_
	#define BAE_H_

	#include <boost/filesystem.hpp>
#include "PreProcessing/AnomalyDetect.h"

	using namespace cv;

	namespace fs = ::boost::filesystem;

	namespace Edhitha2016{

		using namespace Edhitha2016;

		class Bae {
			bool isRectanglesBound;

			int lowerBinaryThreshold;

			double lowerAreaThreshold,upperAreaThreshold;

			tbb::concurrent_queue< boost::filesystem::path >* addresses;

		public:

			fs::path currentImage;

			AnomalyDetect anomalyDetect;

			Bae();
			Bae(tbb::concurrent_queue< boost::filesystem::path >* addresses ,Size dimensions,int type ,bool isRectanglesBound,double lowerAreaThreshold = 0.00006,
					double upperAreaThreshold = 0.0012,int lowerBinaryThreshold = 50);
			~Bae();
			bool setFrame();
		};

	}
#endif
