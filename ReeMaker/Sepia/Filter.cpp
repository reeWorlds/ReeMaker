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

	cv::Mat image = img.clone();

	cv::Mat image_copy = image.clone();

	cv::Mat kern = (cv::Mat_<float>(4, 4) << 0.272, 0.534, 0.131, 0,
											0.349, 0.686, 0.168, 0,
											0.393, 0.769, 0.189, 0,
											0, 0, 0, 1);

	cv::transform(image_copy, image, kern);

	cv::cvtColor(image, image, cv::COLOR_RGB2HSV);
	cv::cvtColor(image, img, cv::COLOR_HSV2RGB);

	return true;
}