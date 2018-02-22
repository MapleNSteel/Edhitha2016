#include "gpsData.h"
//Remove spaces on the right of the string
void trim_spaces(char *buf)
{
    char *s = buf-1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0;  //nul terminate the string on the first of the final spaces
}

//Show the tag name and contents if the tag exists
void show_tag(ExifData *d, ExifIfd ifd, ExifTag tag)
{
     //See if this tag exists
    ExifEntry *entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS],tag);
    if (entry) {
        char buf[1024];

        //Get the contents of the tag in human-readable form
        exif_entry_get_value(entry, buf, sizeof(buf));

        //Don't bother printing it if it's entirely blank
        trim_spaces(buf);
        if (*buf) {
            printf("%s: %s\n", exif_tag_get_name_in_ifd(tag,EXIF_IFD_GPS), buf);
        }
    }
}

void getLatitudeString (ExifData *d,long double* latitude)
    {
		char latitudeChar[12],latitudeOrientation[12];
		ExifEntry *entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE);
		exif_entry_get_value(entry, latitudeChar, 12);
		entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE_REF);
		exif_entry_get_value(entry, latitudeOrientation, 12);

		//Seperating the Degree, Minute, Second from String format.
		char* latDeg="";
		char* latMin="";
		latDeg = strtok(latitudeChar, ",");
		latMin = strtok(NULL, ",");

		*latitude = atof(latDeg) + atof(latMin)/60.0;
		if(latitudeOrientation[0]=='S')
			*latitude = -*latitude;
    }

void getLongitudeString (ExifData *d,long double* longitude)
    {
		char longitudeChar[12],longitudeOrientation[12];

		ExifEntry *entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE);
		exif_entry_get_value(entry, longitudeChar, 12);
		entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE_REF);
		exif_entry_get_value(entry, longitudeOrientation, 12);

		//Seperating the Degree, Minute, Second from String format.
		char *longDeg="", *longMin="";
		longDeg = strtok(longitudeChar, ",");
		longMin = strtok(NULL, ",");

		*longitude = atof(longDeg) + atof(longMin)/60.0;
		if(longitudeOrientation[0]=='W')
			*longitude = -*longitude;
    }

void getImageDirection(ExifData *d,long double* imageDirection)
{
	char directionChar[20];
	ExifEntry *entry = exif_content_get_entry(d->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_IMG_DIRECTION);
	exif_entry_get_value(entry, directionChar, 12);
	*imageDirection=atof(directionChar);
}

void entryPoint(const char* address, long double* latitude, long double* longitude, long double* imageDirection)
{
    ExifData *ed = exif_data_new_from_file(address);
     /*Show all the tags that might contain information about the
     * photographer*/

    show_tag(ed, EXIF_IFD_0, EXIF_TAG_DATE_TIME_ORIGINAL);

    getLatitudeString(ed,latitude);
    getLongitudeString(ed,longitude);
    getImageDirection(ed,imageDirection);

    //EUCLIDEAN DISTANCE
    //  cv::Point a(0, 0);
    //  cv::Point b(2248, 0);
    //  double res = cv::norm(a-b);//Euclidian distance

     //Free the EXIF data
    //exif_data_unref(ed);
}
