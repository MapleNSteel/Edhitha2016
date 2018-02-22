/*
 * colorDetect.cpp
 *
 *  Created on: 23-Nov-2015
 *      Author: vikram
 */
/*
 *
 */
#include "ColorDetect.h"

unsigned short color_check(Vec3b color)
{
	if(color.val[1]==0 && color.val[2]==0)
			return Edhitha2016::ABS_BLACK;
		if(color.val[1]<=36 && color.val[2]>=242)
			return Edhitha2016::WHITE;
		if(color.val[2]<78)
			return Edhitha2016::BLACK;
		if(color.val[1]<=89 && (color.val[2]>=78 &&color.val[2]<=127))
			return Edhitha2016::GRAY;
		if((color.val[0]<7.5 && color.val[0]>=0) || (color.val[0]>=178.5))
			return Edhitha2016::RED;
		if(color.val[0]>=7.5 && color.val[0]<22.5)
			return Edhitha2016::ORANGE;
		if(color.val[0]>=22.5 && color.val[0]<27)
			return Edhitha2016::YELLOW;
		if(color.val[0]>=27 && color.val[0]<87.5)
			return Edhitha2016::GREEN;
		if(color.val[0]>=87.5 && color.val[0]<135)
			return Edhitha2016::BLUE;
		if(color.val[0]>=135 && color.val[0]<180)
			return Edhitha2016::PURPLE;
		if(color.val[0]>=15 && color.val[0]<=16 && color.val[1]>=254 && color.val[2]<=76.5)
			return Edhitha2016::BROWN;
		return Edhitha2016::NAC;
}
namespace Edhitha2016 {
string getColor(unsigned short colorCode){
	switch(colorCode){
		case ABS_BLACK: return "Absolute Black";
						break;
		case WHITE: return "White";
					break;
		case BLACK: return "Black";
					break;
		case GRAY:  return "Gray";
				   	break;
		case RED:	return "Red";
					break;
		case BLUE:	return "Blue";
					break;
		case GREEN: return "Green";
					break;
		case YELLOW:return "Yellow";
					break;
		case PURPLE:return "Purple";
					break;
		case BROWN: return "Brown";
					break;
		case ORANGE:return "Orange";
					break;
		}
	return "Not a valid colour";
}
unsigned short colorDetect(Mat src,Mat& shapeImage,Mat& alphabetImage)//pass encoded shape and alpha data.
{
	unsigned short shapeIndex,alphabetIndex;

	Mat samples(src.rows * src.cols, 3, CV_32F);

	Mat new_images[3];
	new_images[0] = Mat::zeros(src.size(),src.type());
	new_images[1] = Mat::zeros(src.size(),src.type());
	new_images[2] = Mat::zeros(src.size(),src.type());

	parallel_for(0,src.rows,[&](int y){
		parallel_for(0,src.cols,[&](int x){
			parallel_for(0,3,[&](int z){
				samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y,x)[z];
				});
			});
	});

	  const int clusterCount = 3;
	  Mat labels;
	  const int attempts = 5;
	  Mat centers;
	  kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 100, 1), attempts, KMEANS_PP_CENTERS, centers );
	   //Clustering is done
	  //Now copying into new image
	  int occurences[3];
	  occurences[0]=occurences[1]=occurences[2]=0;

	  parallel_for(0,src.rows,[&](int y){
	  		parallel_for(0,src.cols,[&](int x){
			  int cluster_idx = labels.at<int>(y + x*src.rows,0);
			  occurences[cluster_idx]++;

		      new_images[cluster_idx].at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx,0);
			  new_images[cluster_idx].at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx,1);
			  new_images[cluster_idx].at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx,2);
	  	  });
	  });
	  //kmeans ends here
	  //Color iterator starts here
	  //Extracting color after detecting a change (only two colours will exist in the image now)

	  Mat color[3];
	  color[0] = Mat(1,1,CV_8UC3);
	  color[1] = Mat(1,1,CV_8UC3);
	  color[2] = Mat(1,1,CV_8UC3);

	  color[0].at<Vec3b>(0,0)[0] = centers.at<float>(0,0);
	  color[0].at<Vec3b>(0,0)[1] = centers.at<float>(0,1);
	  color[0].at<Vec3b>(0,0)[2] = centers.at<float>(0,2);

	  color[1].at<Vec3b>(0,0)[0] = centers.at<float>(1,0);
	  color[1].at<Vec3b>(0,0)[1] = centers.at<float>(1,1);
	  color[1].at<Vec3b>(0,0)[2] = centers.at<float>(1,2);

	  color[2].at<Vec3b>(0,0)[0] = centers.at<float>(2,0);
	  color[2].at<Vec3b>(0,0)[1] = centers.at<float>(2,1);
	  color[2].at<Vec3b>(0,0)[2] = centers.at<float>(2,2);

	cvtColor(color[0],color[0],COLOR_BGR2HSV);
	cvtColor(color[1],color[1],COLOR_BGR2HSV);
	cvtColor(color[2],color[2],COLOR_BGR2HSV);

	unsigned short ans[3];
	ans[0]=(color_check(color[0].at<Vec3b>(0,0)));
	ans[1]=(color_check(color[1].at<Vec3b>(0,0)));
	ans[2]=(color_check(color[2].at<Vec3b>(0,0)));

	unsigned short position=0;
	unsigned short indices[2];

	for(unsigned short i=0;i<3;i++){
		if(ans[i]==ABS_BLACK){
			continue;
		}
		else
			indices[position++]=i;
	}

	if(occurences[indices[0]]<occurences[indices[1]]){

		shapeIndex = indices[1];
		alphabetIndex = indices[0];
	}
	else{

		shapeIndex = indices[0];
		alphabetIndex = indices[1];
	}

	shapeImage = new_images[shapeIndex];
	alphabetImage = new_images[alphabetIndex];

	//Do lab.

	return ans[shapeIndex]+(ans[alphabetIndex]*12);
	}
}
