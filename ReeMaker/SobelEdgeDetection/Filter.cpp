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
	int dx, dy;

	if (parameters != 0)
	{
		if (parameters[0] != 0)
		{
			dx = *((int*)parameters[0]);
		}
		if (parameters[0] != 0)
		{
			dy = *((int*)parameters[1]);
		}
	}
	if (dx == 0 && dy == 0)
	{
		dx++;
	}
	cv::Sobel(img, img2, img2.type(), dx, dy, 3, 1, 0, cv::BORDER_REFLECT_101);

	cv::cvtColor(img2, img2, cv::COLOR_RGB2HSV);
	cv::cvtColor(img2, img, cv::COLOR_HSV2RGB);

	return true;
}