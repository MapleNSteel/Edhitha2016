/*
x * Baelando.cpp
 *
 *  Created on: 21 Oct 2015
 *      Author: arjun
*/
#include "Baelando.h"

using namespace Edhitha2016;

namespace Edhitha2016 {

	Baelando::Baelando():bae(){
		targetsWritten=0;
	}
	Baelando::Baelando(fs::path rootDirectory,tbb::concurrent_queue< boost::filesystem::path >* addresses,Size dimensions, int type ,bool isRectanglesBound,double lowerAreaThreshold ,
			double upperAreaThreshold,int lowerBinaryThreshold):bae(addresses,dimensions,type,
					isRectanglesBound,lowerAreaThreshold,upperAreaThreshold,lowerBinaryThreshold){
		this->rootDirectory=rootDirectory;
		targetsWritten=0;
	}

	Baelando::~Baelando() {
		printf("Destroyed baelando\n");
	}
	Baelando& Baelando::operator= (Baelando& other){
		Baelando temp(other);
		*this = other;

		return *this;
	}
	string Baelando::getImageRootDirectory(char* dir){

		string address,directory;

		directory = dir;
		directory.append(".conf");

		fstream conf;
		conf.open(directory.c_str());

		if(conf.is_open()){
			getline(conf,address);
		}
		else{
			conf.open(directory.c_str(),ios::out);

			cout<<"Conf file not found. Please enter image source address :";
			cin>>address;

			conf<<address;
		}

		conf.close();
		//create folder change listeners.
		return address;
	}
	void Baelando::getAllImages(const fs::path& root,tbb::concurrent_queue< boost::filesystem::path >* addresses)
	{
	    if(!fs::exists(root) || !fs::is_directory(root))
	    	return;

	    fs::directory_iterator it(root);
	    fs::directory_iterator endit;
	    while(it != endit)
	    {
	        if(fs::is_regular_file(*it) && it->path().extension() == ".JPG")
	        	addresses->push(fs::path(it->path().c_str()));
	        ++it;
	    }
	}
	void Baelando::writeTargetImages(queue<Target> target){
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);

		string outputDirectory = this->rootDirectory.parent_path().parent_path().string();
		outputDirectory.append("/Outputs/Targets/");

		string resultFileDirectory = outputDirectory;
		resultFileDirectory.append("Results.txt");

		ofstream myfile;
		myfile.open(resultFileDirectory.c_str());

		Target tempTarget;
		for(unsigned int i=0;i<!target.empty();i++){
			tempTarget=target.front();
			target.pop();
			std::ostringstream ss;
			ss<<outputDirectory+tempTarget.targetAlphabet;
			ss<<"-";
			ss<<i+targetsWritten;
			ss<<".PNG";

			myfile<<"\n"<<ss.str();
			myfile<<i+targetsWritten<<"\nFile Address:"<<tempTarget.fileAddress<<"\nConfidence:"<<tempTarget.targetConfidence<<"\nRatio:"<<tempTarget.ratio;
			myfile<<"\nAlphabet Colour:"<<getColor(tempTarget.targetColourConfig/12)<<"\nShape Colour:"<<getColor(tempTarget.targetColourConfig%12);
			myfile<<std::setprecision(12)<<"\nOrientation:"<<tempTarget.targetOrientation<<"\nLatitude,Longitude:"<<tempTarget.latitude<<","<<tempTarget.longitude<<"\n";
			myfile<<"Shape:"<<returnShape(tempTarget.shapeCode)<<"\n";

			imwrite(ss.str(), tempTarget.targetImage,compression_params);
			targetsWritten++;
		}
	}
}
