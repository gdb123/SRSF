#include <opencv2/imgproc/imgproc_c.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <math.h>
#include "depth.h"
#include "basic.h"

static float NaN = 0.0/0.0;

using namespace cv;
using namespace std;

void readyDepth(Mat &DEPTHalign, Mat &Z0, Mat &D0, int constZ, int MAXZ, int MINZ)
{
    // Transform the depth in cm and set depth out of range as NaN

	int w = DEPTHalign.cols;
	int h = DEPTHalign.rows;
	int wh = w*h; // Total num of the image points
	float z;
	int d;
	float *dataZ = (float*)Z0.data;
	float *dataD = (float*)D0.data;

    // Apply median filter the blur the depth image
	Mat DEPTH = Mat(h,w,CV_16U);
	medianBlur(DEPTHalign,DEPTH,5);
	medianBlur(DEPTH,DEPTHalign,3);
	medianBlur(DEPTHalign,DEPTH,3);

	int cont = 0;
	ushort* tmp_apun = (ushort*)(DEPTH.data);

	while (cont < wh) // For each image point
    {

		d = tmp_apun[cont];

		if (d > MINZ*constZ && d < MAXZ*constZ ) {

			z = float(d)/constZ;  // cm
			dataZ[cont] = z;
			dataD[cont] = 1./z;
		}

		else
        {

			dataZ[cont] = NaN;
			dataD[cont] = NaN;

		}
		cont++;
	}
}



void buenPunto(Mat Z, CvPoint2D32f punto[], int band[], int Npuntos, int MAXZ, int MINZ, int Wx, int Wy)
{
    // punto[] is point[] 2D image point coordinate
    // set the mask, 0--Nan, 1--Out of range, 2--valid

	float *data = (float*)Z.data;

	int maxX = width[0]-Wx-1;
	int maxY = height[0]-Wy-1;
	int minX = Wx;
	int minY = Wy;

	int cont = 0;

	while (cont < Npuntos)
    {

		int x = punto[cont].x;
		int y = punto[cont].y;

		if (data[cont] > MAXZ || data[cont] < MINZ || x < minX || y < minY || x > maxX || y > maxY ) {

			band[cont] = 2; //Valid is set to 2
		}

		else if ( data[cont] != data[cont]) {

			band[cont] = 0; //NaN is set to 0
		}

		else {

			band[cont] = 1; //Out of range is set to 1

		}
		cont ++;
	}

}


