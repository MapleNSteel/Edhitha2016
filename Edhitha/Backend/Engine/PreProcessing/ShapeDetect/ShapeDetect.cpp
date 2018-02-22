#include "ShapeDetect.h"
using namespace Edhitha2016;

//LINE 106: ENTER THE IMAGE PATH OR JUST SEND THE MAT IMAGE?
namespace Edhitha2016
{
string returnShape(short int shape){
	switch(shape){
	case SQUARE: 		return "SQUARE";
	break;
	case RECTANGLE:		return "RECTANGLE";
	break;
	case TRAPEZOID:		return "TRAPEZOID";
	break;
	case TRIANGLE:  	return "TRIANGLE";
	break;
	case PENTAGON:		return "PENTAGON";
	break;
	case HEXAGON:		return "HEXAGON";
	break;
	case HEPTAGON: 		return "HEPTAGON";
	break;
	case OCTAGON:		return "OCTAGON";
	break;
	case CIRCLE:		return "CIRCLE";
	break;
	case SEMICIRCLE:	return "SEMI-CIRCLE";
	break;
	case QUARTERCIRCLE:	return "QUARTER-CIRCLE";
	break;
	case STAR:			return "STAR";
	break;
	case CROSS:			return "CROSS";
	break;
	default:	    return "NOT A SHAPE";
	break;
	}
}


double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void thinningIteration(Mat& im, int iter)
{
	Mat marker = Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows-1; i++)
	{
		for (int j = 1; j < im.cols-1; j++)
		{
			uchar p2 = im.at<uchar>(i-1, j);
			uchar p3 = im.at<uchar>(i-1, j+1);
			uchar p4 = im.at<uchar>(i, j+1);
			uchar p5 = im.at<uchar>(i+1, j+1);
			uchar p6 = im.at<uchar>(i+1, j);
			uchar p7 = im.at<uchar>(i+1, j-1);
			uchar p8 = im.at<uchar>(i, j-1);
			uchar p9 = im.at<uchar>(i-1, j-1);

			int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
					(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
					(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
					(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i,j) = 1;
		}
	}
	im &= ~marker;
}


void thinning(Mat& im)
{
	im /= 255;

	Mat prev = Mat::zeros(im.size(), CV_8UC1);
	Mat diff;
	do {
		thinningIteration(im, 0);
		thinningIteration(im, 1);
		absdiff(im, prev, diff);
		im.copyTo(prev);
	}
	while (countNonZero(diff) > 0);

	im *= 255;
}

unsigned short backEndProcessing(Mat src)
{
	//Padding.
	Mat padded;
	int padding = 20;
	padded.create(src.rows + 2*padding, src.cols + 2*padding, src.type());
	padded.setTo(Scalar::all(0));
	src.copyTo(padded(Rect(padding, padding, src.cols, src.rows)));

	padded.copyTo(src);

	int count = 2;
	resize(src, src,cv::Point(500,500), 0, 0, count);
	count --;
	//imshow("Source Image", src);
	Mat dst, cdst, cdst2, cdst3;
	Canny(src, dst, 50, 200, 3);
	GaussianBlur(dst,dst,Size(7,7),10.0);
	cvtColor(dst, cdst2, COLOR_GRAY2BGR);
	bool errorYes;

	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 5 );
	for( size_t i = 0; i < lines.size(); i++ ){
		Vec4i l = lines[i];
		line( cdst2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,100), 1, true);
	}

	//imwrite("/home/ankan/houghOutput.jpg", cdst2);
	//src = imread("/home/ankan/houghOutput.jpg");

	src = cdst2;

	//	  	  if(src.empty())
	//	  	 	 {
	//	  	 		 cout << "ERROR 2: file cannot be opened." << endl;
	//	  	 		 return -1;
	//	  	 	 }

	Mat bw;

	cvtColor(src, bw, COLOR_BGR2GRAY );
	threshold(bw, bw, 9, 255, THRESH_BINARY);

	thinning(bw);
	Mat image = Mat::zeros(600,600, CV_8UC3);

	vector <vector<Point> >  contours;
	findContours(bw.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	double lineSegmentLengths[20];
	vector<Point> approx;
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional to the contour perimeter. //0.0125
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.0135, true);
	}

	int var = 0;  double res;
	//printf("FIRST OPTION:	");
	unsigned int rnd = 0;

	if(approx.size() == 0)
		return NOTASHAPE;
	for( rnd = 0; rnd < approx.size()-1 ; rnd++)
	{
		line(image, approx[rnd], approx[rnd+1], (255,255,255), 3);
		res = norm(approx[rnd]-approx[rnd+1]); //Euclidian distance
		lineSegmentLengths[var]=res;
		var++;
	}
	line(image, approx[rnd], approx[0], (255,255,255), 3);
	res = norm(approx[rnd]-approx[0]);
	lineSegmentLengths[var]=res;
	//printf("%f", lineSegmentLengths[var]);
	sort(lineSegmentLengths, lineSegmentLengths+(var+1));
	//		for(int dumb=0; dumb<=var; dumb++)
	//		{
	//			printf("%f \n", lineSegmentLengths[dumb]);
	//		}

	if((lineSegmentLengths[var-2] > (1.5*lineSegmentLengths[var-3])) && (lineSegmentLengths[var-4] < (1.3*lineSegmentLengths[var-3])))
	{
		return 3;
	}

	if((lineSegmentLengths[1]>=(0.85*lineSegmentLengths[9])) && (var>10))
	{
		return 11;
	}

	if(lineSegmentLengths[var]>(1.7*lineSegmentLengths[var-2]))
	{
		return 9;
	}

	if((lineSegmentLengths[1]>(0.85*lineSegmentLengths[2])) && (lineSegmentLengths[2] > (0.85 * lineSegmentLengths[3])) && (lineSegmentLengths[var-1]>(0.85*lineSegmentLengths[var])) && (lineSegmentLengths[var-2]>(0.85*lineSegmentLengths[var-1])) && (lineSegmentLengths[var-3]>(0.85*lineSegmentLengths[var-2])) && (var>10))
	{
		return 12;
	}

	//imwrite("/home/ankan/reconstructedImage.jpg", image);
	//src = imread("/home/ankan/reconstructedImage.jpg");

	src=image;
	cvtColor(image, image, COLOR_BGR2GRAY);
	//		if(src.empty())
	//		{
	//			cout << "ERROR 3: file cannot be opened." << endl;
	//			return -1;
	//		}

	Mat img1 = src;
	//FIRST PASS OF ALGORITHM STARTS HERE.

	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	//Mat bw;
	Canny(gray, bw, 0, 100, 5);

	//vector<vector<Point> > contours;
	findContours(bw, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	bool check = true;
	//vector<Point> approx;
	src.copyTo(dst);

	for (unsigned int i = 0; i <= (contours.size() - 1); i++)
	{

		// Approximate contour with accuracy proportional to the contour perimeter. //0.0125
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.0124, true);
		//For non-convex shapes.

		if (fabs(!isContourConvex(approx)))
		{
			check = false;
			//vector< vector <Point> > hullP(contours.size());
			vector< vector <int> > hullI(contours.size());
			vector< vector <Vec4i> > convexityOutput;

			for (unsigned int i = 0; i < contours.size(); i++)
			{
				//convexHull( (contours[i]), hullP[i], false );
				convexHull( (contours[i]), hullI[i], false );

				//convexityDefects( contours[i], hullI[i], convexityOutput[i]);
			}
		}

		if (approx.size() == 3)
		{
			return 3;    // Triangles
		}
		else if (approx.size() > 3 && approx.size() < 13)
		{

			// Number of vertices of polygonal curve
			int vtc = approx.size();
			//printf("\nVertices detected: %d \n", vtc);

			// Get the cosines of all corners (just in case of irregularities).

			vector<double> cos;
			for (int j = 2; j < vtc+1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

			// Sort ascending the cosine values
			sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos;

			try{
				maxcos = cos.at(var-1);
			} catch(const out_of_range& err)
			{
				errorYes = true;
				continue;
			}

			if(errorYes == true)
			{
				maxcos = cos.at(var-2);
			}
//			printf("%f  %f", mincos, maxcos);

			// Using the degrees obtained above and the number of vertices to determine the shape of the contour
			if ((var == 3) && (mincos >= -0.07) && (maxcos <= 0.07))
			{
				// Detect rectangle or square
				Rect r = boundingRect(contours[i]);

				double ratio = abs(1 - (double)r.width / r.height);

				if(ratio <= 0.2)
					return 0;

				else if (ratio >= 0.2)
					return 1;
			}

			else if((var == 3) && ((mincos < -0.07) || (maxcos > 0.07)) && (lineSegmentLengths[2]<(1.1*lineSegmentLengths[1])))
				return 2;

			// Actual Cos = -0.30
			else if (var == 3)
			{
				if(lineSegmentLengths[2]>(1.1*lineSegmentLengths[1]))
					return 1;

				else if(lineSegmentLengths[2]<(1.1*lineSegmentLengths[1]))
					return 0;
			}
			else if (((var == 4) || (vtc == 5)) && mincos >= -0.40 && maxcos <= -0.20)
				return 4;
			// Actual Cos = -0.50

			else if (((var == 5) || (vtc == 6)) && mincos >= -0.60 && maxcos <= -0.35)
				return 5;
			// Actual Cos = -0.61
			else if (((var == 6) || (vtc == 7)) && mincos >= -0.70 && maxcos <= -0.50)
				return 6;
			//Actual Cos = -0.707
			else if (vtc == 8 && mincos >= -0.75 && maxcos <= -0.65)
				return 7;
			else if(((var == 8) || (var == 9) || (vtc == 9 || vtc == 10 || vtc == 11 || vtc == 12)) && (check == true))
				return 8;
			//else if(vtc == 10 && check == true)
			//setLabel(dst, "CIRCLE", contours[i]);
			//else if(vtc == 11)
			//setLabel(dst, "CIRCLE", contours[i]);
			else if((vtc == 10) && (check == false))
				return 11;
			else if((vtc == 12) && (check == false))
				return 12;

			else
			{
				// TEMPLATE MATCHING STARTS.
				//Loading all the shapes into memory.
				Mat squaTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/squaTemp.jpg");
				Mat rectTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/recTemp.jpg");
				Mat cirTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/cirTemp.jpg");
				Mat semiCirTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/semiCTemp.jpg");
				Mat quaCirTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/quaCTemp.jpg");
				Mat triaTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/triaTemp.jpg");
				Mat trapTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/trapTemp.jpg");
				Mat pentTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/pentTemp.jpg");
				Mat hexTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/hexTemp.jpg");
				Mat hepTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/heptTemp.jpg");
				Mat octTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/octTemp.jpg");
				Mat starTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/starTemp.jpg");
				Mat crossTemp = imread("/home/arjun/Workspace/OpenCV/Edhitha/Resources/Shape Templates/crossTemp.jpg");

				//converting the colours to BGR.
				cvtColor(img1, img1, COLOR_BGR2GRAY);
				cvtColor(squaTemp, squaTemp, COLOR_BGR2GRAY);
				cvtColor(rectTemp, rectTemp, COLOR_BGR2GRAY);
				cvtColor(cirTemp, cirTemp, COLOR_BGR2GRAY);
				cvtColor(semiCirTemp, semiCirTemp, COLOR_BGR2GRAY);
				cvtColor(quaCirTemp, quaCirTemp, COLOR_BGR2GRAY);
				cvtColor(triaTemp, triaTemp, COLOR_BGR2GRAY);
				cvtColor(trapTemp, trapTemp, COLOR_BGR2GRAY);
				cvtColor(pentTemp, pentTemp, COLOR_BGR2GRAY);
				cvtColor(hexTemp, hexTemp, COLOR_BGR2GRAY);
				cvtColor(hepTemp, hepTemp, COLOR_BGR2GRAY);
				cvtColor(octTemp, octTemp, COLOR_BGR2GRAY);
				cvtColor(starTemp, starTemp, COLOR_BGR2GRAY);
				cvtColor(crossTemp, crossTemp, COLOR_BGR2GRAY);

				//obtaining the threshold outlines.
				//writing intermediate templates to designated locations.
				//SPECIFY FOLDER.

				threshold(img1, img1, 20, 255, 0);
				threshold(squaTemp, squaTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template1.jpg", squaTemp);
				threshold(rectTemp, rectTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template2.jpg", rectTemp);
				threshold(cirTemp, cirTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template3.jpg", cirTemp);
				threshold(semiCirTemp, semiCirTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template4.jpg", semiCirTemp);
				threshold(quaCirTemp, quaCirTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template5.jpg", quaCirTemp);
				threshold(triaTemp, triaTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template6.jpg", triaTemp);
				threshold(trapTemp, trapTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template7.jpg", trapTemp);
				threshold(pentTemp, pentTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template8.jpg", pentTemp);
				threshold(hexTemp, hexTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template9.jpg", hexTemp);
				threshold(hepTemp, hepTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template10.jpg", hepTemp);
				threshold(octTemp, octTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template11.jpg", octTemp);
				threshold(starTemp, starTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template12.jpg", starTemp);
				threshold(crossTemp, crossTemp, 20, 255, 0);
				//imwrite("/home/ankan/Pix/templates/template13.jpg", crossTemp);

				//obtaining the vector contours.
				vector<vector<Point> > contours0;
				findContours(img1, contours0, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours2;
				findContours(squaTemp, contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours3;
				findContours(rectTemp, contours3, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours4;
				findContours(cirTemp, contours4, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours5;
				findContours(semiCirTemp, contours5, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours6;
				findContours(triaTemp, contours6, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours7;
				findContours(pentTemp, contours7, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours8;
				findContours(hexTemp, contours8, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours9;
				findContours(hepTemp, contours9, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours10;
				findContours(octTemp, contours10, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours11;
				findContours(starTemp, contours11, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours12;
				findContours(crossTemp, contours12, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours13;
				findContours(quaCirTemp, contours13, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				vector<vector<Point> > contours14;
				findContours(trapTemp, contours14, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

				double ansArray[13];

				//Comparing templates.
				ansArray[0] = matchShapes(contours0[0],contours2[0],3,0.0);
				//printf("Square: %f \n", ansArray[0]);
				ansArray[1] = matchShapes(contours0[0],contours3[0],3,0.0);
				//printf("Rect: %f \n", ansArray[1]);
				ansArray[2] = matchShapes(contours0[0],contours4[0],3,0.0);
				//printf("Cir: %f \n", ansArray[2]);
				ansArray[3] = matchShapes(contours0[0],contours5[0],3,0.0);
				//printf("SemiCir: %f \n", ansArray[3]);
				ansArray[4] = matchShapes(contours0[0],contours6[0],3,0.0);
				//printf("Tria: %f \n", ansArray[4]);
				ansArray[5] = matchShapes(contours0[0],contours7[0],3,0.0);
				//printf("Penta: %f \n", ansArray[5]);
				ansArray[6] = matchShapes(contours0[0],contours8[0],3,0.0);
				//printf("Hexa: %f \n", ansArray[6]);
				ansArray[7] = matchShapes(contours0[0],contours9[0],3,0.0);
				//printf("Hepta: %f \n", ansArray[7]);
				ansArray[8] = matchShapes(contours0[0],contours10[0],3,0.0);
				//printf("Octa: %f \n", ansArray[8]);
				ansArray[9] = matchShapes(contours0[0],contours11[0],3,0.0);
				//printf("Star: %f \n", ansArray[9]);
				ansArray[10] = matchShapes(contours0[0],contours12[0],3,0.0);
				//printf("Cross: %f \n", ansArray[10]);
				ansArray[11] = matchShapes(contours0[0],contours13[0],3,0.0);
				//printf("QuarterCircle: %f \n", ansArray[11]);
				ansArray[12] = matchShapes(contours0[0],contours14[0],3,0.0);
				//printf("Trap: %f \n", ansArray[12]);

				double temp; char tempC;
				char shapesArrayCode[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'};
				for (int bubOne = 0; bubOne < 13; ++bubOne)
				{
					for (int bubTwo = 0; bubTwo < (12 - bubOne); ++bubTwo)
					{
						if (ansArray[bubTwo] > ansArray[bubTwo + 1])
						{
							//Sort the Correlation Factors.
							temp = ansArray[bubTwo];
							ansArray[bubTwo] = ansArray[bubTwo + 1];
							ansArray[bubTwo + 1] = temp;

							//Sort the shapes list.
							tempC = shapesArrayCode[bubTwo];
							shapesArrayCode[bubTwo] = shapesArrayCode[bubTwo + 1];
							shapesArrayCode[bubTwo + 1] = tempC;
						}
					}
				}

				if(ansArray[0] <= 0.002)
				{
					if(shapesArrayCode[0] == 'a')
						return 0;
					//returnShape(Edhitha2016::SQUARE);

					else if(shapesArrayCode[0] == 'b')
						return 1;
					//returnShape(Edhitha2016::RECTANGLE);

					else if(shapesArrayCode[0] == 'c')
						return 8;
					//returnShape(Edhitha2016::CIRCLE);

					else if(shapesArrayCode[0] == 'd')
						return 9;
					//returnShape(Edhitha2016::SEMICIRCLE);

					else if(shapesArrayCode[0] == 'e')
						return 3;
					//returnShape(Edhitha2016::TRIANGLE);

					else if(shapesArrayCode[0] == 'f')
						return 4;
					//returnShape(Edhitha2016::PENTAGON);

					else if(shapesArrayCode[0] == 'g')
						return 5;
					//returnShape(Edhitha2016::HEXAGON);

					else if(shapesArrayCode[0] == 'h')
						return 6;
					//returnShape(Edhitha2016::HEPTAGON);

					else if(shapesArrayCode[0] == 'i')
						return 7;
					//returnShape(Edhitha2016::OCTAGON);

					else if(shapesArrayCode[0] == 'j')
						return 11;
					//returnShape(Edhitha2016::STAR);

					else if(shapesArrayCode[0] == 'k')
						return 12;
					//returnShape(Edhitha2016::CROSS);

					else if(shapesArrayCode[0] == 'l')
						return 10;
					//returnShape(Edhitha2016::QUARTERCIRCLE);

					else if(shapesArrayCode[0] == 'm')
						return 2;
					//returnShape(Edhitha2016::TRAPEZOID);

					else
						return 13;
					//returnShape(Edhitha2016::NOTASHAPE);
				}
			}
		}

	}
	return 50;
}
}
