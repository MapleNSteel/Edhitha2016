#ifdef __GPSDATA_H
extern "C"{
#endif
#define GPSDATA_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <libexif/exif-data.h>
	#include <dirent.h>

	void trim_spaces(char *buf);
	void show_tag(ExifData *d, ExifIfd ifd, ExifTag tag);
	void getLatitudeString (ExifData *d, long double* latitude);
	void getLongitudeString (ExifData *d,long double* longitude);
	void getImageDirection (ExifData *d,long double* imageDirection);
	void entryPoint(const char* address, long double* latitude, long double* longitude, long double* imageDirection);

	#ifdef __GPS_DATA_H
}
#endif
