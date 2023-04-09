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

	int kSize = 3;
	double sig = 1.0;

	if (parameters != 0)
	{
		if (parameters[0] != 0)
		{
			kSize = *((int*)parameters[0]);
			if (kSize <= 0)
				kSize = 1;

			kSize = kSize * 2 + 1;
		}

		if (parameters[1] != 0)
		{
			sig = (double)(*((float*)parameters[1]));
		}
	}

	cv::GaussianBlur(img, img, cv::Size(kSize, kSize), sig);

	return true;
}