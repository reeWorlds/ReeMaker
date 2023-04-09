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
	cv::Mat img2;
	int ts1,ts2;

	if (parameters != 0)
	{
		if (parameters[0] != 0)
		{
			ts1 = *((int*)parameters[0]);
		}
		if (parameters[0] != 0)
		{
			ts2 = *((int*)parameters[1]);
		}
	}

	cv::Canny(img, img2, ts1, ts2);
	cv::cvtColor(img2, img, cv::COLOR_GRAY2RGB);

	return true;
}