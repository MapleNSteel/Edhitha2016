/*
 * OCR.cpp
 *
 * Created on: 6 Nov 2015
 *      Author: arjun
 */
#include "TesseractOCR.h"

namespace Edhitha2016 {
void TesseractOCR::getCharacterAndOrientation(Mat target,char& alphabet,float& orientation,float& confidence){

	Point center = Point(target.cols/2, target.rows/2 );
	float maxConfidence=0;
	float maxAngle=0;
	string maxAlphabet;

	double confidenceList[62];
	for(unsigned short i=0;i<62;i++)
		confidenceList[i]=0.0;

	unsigned int occurenceList[62];
		for(unsigned short i=0;i<62;i++)
			occurenceList[i]=0;

	string tempAlphabet;
	float tempConfidence;
	for(int angle = 180; angle>=-180;angle-=5){

		double scale = 1;
		/// Get the rotation matrix with the specifications above
		Mat rot_mat = getRotationMatrix2D( center, angle, scale );
		Mat dst;
		/// Rotate the warped target
		warpAffine( target, dst, rot_mat, target.size() );
		tesseractOCR(dst,tempAlphabet,tempConfidence);
//		cout<<tempAlphabet<<","<<tempConfidence<<endl;
		if(!isalnum(tempAlphabet[0]))
			continue;
		else{
			if(isalpha(tempAlphabet[0]))
				if(tempAlphabet[0]>='a' && tempAlphabet[0]<='z'){
					confidenceList[tempAlphabet[0]-'a']+=((double)tempConfidence);
					occurenceList[tempAlphabet[0]-'a']++;
				}
				if(tempAlphabet[0]>='A' && tempAlphabet[0]<='Z'){
					confidenceList[26+tempAlphabet[0]-'A']+=((double)tempConfidence);
					occurenceList[26+tempAlphabet[0]-'A']++;
				}
			if(isdigit(tempAlphabet[0])){
				confidenceList[52+tempAlphabet[0]-'0']+=((double)tempConfidence);
				occurenceList[52+tempAlphabet[0]-'0']++;
			}
		}
		if(tempConfidence>maxConfidence){
			maxConfidence=tempConfidence;
			maxAngle=angle;
			maxAlphabet=tempAlphabet;
		}
	}
	unsigned short tempMax=0;
	for(unsigned short i=0;i<62;i++){
		if(occurenceList[i]!=0){
			/*if(i>=0 && i<=25)
				cout<<(char)('a'+i)<<"-";
			if(i>=26 && i<=51)
				cout<<(char)('A'+i-26)<<"-";
			if(i>=52 && i<=61)
				cout<<(char)('0'+i-52)<<"-";

			cout<<((double)confidenceList[i])<<",";
*/
			if(confidenceList[i]>confidenceList[tempMax])
				tempMax=i;
		}
	}

	alphabet=maxAlphabet[0];
	orientation=-maxAngle;
	confidence = maxConfidence;
}

void TesseractOCR::tesseractOCR(Mat image,string& alphabet,float& confidence)
{
	if (api.Init("/usr/local/share/", "eng", tesseract::OEM_DEFAULT)) {
	        fprintf(stderr, "Could not initialize tesseract.\n");
	        exit(1);
	}
	api.SetVariable("tessedit_char_whitelist","ABCDEFGHIJKLMNOPQRSTUVXYZabcedfghijklmnopqrstuvxyz0123456789");
	api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	api.SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), image.step1());
	if(api.Recognize(0)!=0)
		return;

	ResultIterator* ri = api.GetIterator();
	if(ri->GetUTF8Text(tesseract::RIL_SYMBOL)!=NULL)
     alphabet= ri->GetUTF8Text(tesseract::RIL_SYMBOL);
     confidence=ri->Confidence(RIL_SYMBOL);

    delete ri;
}
} /* namespace Edhitha2016 */
