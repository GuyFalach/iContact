#pragma once

#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include <exception>
#include <iostream>


struct Frame
{
	unsigned char** frame;
	unsigned rows;
	unsigned cols;
	unsigned numColors = 1;
	Frame getCopy();
}; 



class Camera
{
public:
	Camera();
	cv::Mat giveFrame();
	static Frame matToFrame(cv::Mat& mat);
	static void freeFrame(Frame& frame);
	void closeCamera();
	void openCamera();
	~Camera();

private:
	cv::VideoCapture cap;
};


