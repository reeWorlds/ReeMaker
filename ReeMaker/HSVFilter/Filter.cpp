#include "Filter.h"

#include <opencv2\opencv.hpp>

// NOTE: only uchar pixel types supported
// Parameters:
// 0: kernel size (int)
// 1: sigma (float)
bool apply(unsigned char* imageData, int imageWidth, int imageHeight, int channelsCount, void** parameters)
{

	int type = CV_8UC1;
	switch (channelsCount)
	{
	case 1:
		type = CV_8UC1;
		break;
	case 2:
		type = CV_8UC2;
		break;
	case 3:
		type = CV_8UC3;
		break;
	case 4:
		type = CV_8UC4;
		break;
	default:
		type = CV_8UC1;
		break;
	}

	cv::Mat img(imageHeight, imageWidth, type, imageData, cv::Mat::AUTO_STEP);
	cv::Mat hsv;

	cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);

	int h;
	int s;
	int v;

	if (parameters != 0)
	{
		if (parameters[0] != 0)
		{
			h = *((int*)parameters[0]);
		}

		if (parameters[1] != 0)
		{
			s = *((int*)parameters[1]);
		}

		if (parameters[1] != 0)
		{
			v = *((int*)parameters[2]);
		}
	}

	int i, j;

	for (i = 0; i < hsv.rows; i++)
	{

		for (j = 0; j < hsv.cols; j++)
		{
			cv::Vec3b a = hsv.at<cv::Vec3b>(i, j);
			int k = a[0] + h;
			if (k>180){ k -= 180; }
			if (k<0){ k += 180; }
			a[0] = k;
			k = a[1] + s;
			if (k>255){ k = 255; }
			if (k<0){ k = 0; }
			a[1] = k;
			k = a[2] + v;
			if (k>255){ k = 255; }
			if (k<0){ k = 0; }
			a[2] = k;
			hsv.at<cv::Vec3b>(i, j) = a;
		}
	}

	cv::cvtColor(hsv, img, cv::COLOR_HSV2RGB);
	

	return true;
}