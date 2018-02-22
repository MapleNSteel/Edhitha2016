extern "C"{
#include "gpsData.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libexif/exif-data.h>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <dirent.h>

using namespace cv;
using namespace std;

//One small step for folder traversal.
//One giant step for ADLC.

/*vector<string> getFilesInFolder(string in_folder)
{
    struct dirent *entry;
    DIR *dp;
    vector<string> results;

    dp = opendir(in_folder.c_str());

    if (dp == NULL) {
        fprintf(stderr, "Could not list files of directory %s\n", in_folder.c_str());
        return results;
    }

    while ((entry = readdir(dp))) {
        // Skip . and .. and hidden files
        if (entry->d_name[0] != '.') {
            results.push_back(entry->d_name);
        }
    }

    closedir(dp);
    return results;
}

int main()
{
    string pathDir;
    pathDir = "/home/arjun/Workspace/OpenCV/Images/2015 Aerial Images/Originals/temp";
    float latitudeDegree = 0,latitudeMinute=0, longitudeDegree = 0,longitudeMinute=0, imageDirection = 0;
	vector<string> filenames = getFilesInFolder(pathDir);
    vector<string>::iterator filenames_it;

    ExifData *edSent;

    for (filenames_it = filenames.begin(); filenames_it != filenames.end(); ++filenames_it) {
        string fullpath = pathDir + string("/") + *filenames_it;
        printf("Reading %s.\n", fullpath.c_str());

        // Get latitude, longitude, imageDirection from EXIF data
    	entryPoint(fullpath.c_str(), &latitudeDegree,&latitudeMinute, &longitudeDegree, &longitudeMinute, &imageDirection);
    }
    //float latitude = 0, longitude = 0, imageDirection = 0;

    printf("%f %f %f %f %f  \n", latitudeDegree,latitudeMinute, longitudeDegree,longitudeMinute, imageDirection);
    return 1;
}*/
