#pragma once

#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include <exception>
#include <iostream>
#include "Camera.h"

enum circleInfoIndex {
	RADIUS,
	X_AXIS,
	Y_AXIS
};

class TestResult
{
public:
	static void markerMiddle(cv::Mat& frame);
	static void markerEyes(cv::Mat& frame, const cv::Point& center, float radius);
	static cv::Mat frameToMat(Frame& frame);
	static void findEyesTest(float result[2][3], Frame& grayscaleImg);
};