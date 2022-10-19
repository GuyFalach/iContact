#include <math.h>
#include <iostream>
#include "Pixel.h"
#include"TestResult.h"
#include<opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include<iostream>
#include <chrono>
#include "activation.h"
#include "Mouse.h"

#define ERROR -1
using namespace std::chrono;
using namespace cv;


int main()
{
	unsigned char** result = nullptr;
	cv::Mat frame;
	unsigned int movementCounter = 0;
	try
	{
		bool markerTheEye = false;
		Camera cam;
		Mouse mouse;

		Frame newEdgeFame;
		frame = cam.giveFrame();
		float eyesInfo[2][3] = { 0 };
		ActivationFace::initialize(frame, newEdgeFame, cam,  eyesInfo);
		bool actionEnded{ false };
		bool moveStatus = false;
		bool clickFrame = false;
		const float radius = eyesInfo[0][RADIUS];
		int failCounter = 0;
		unsigned clickFrameCounter = 0;
		Frame lastEdgeFrame = newEdgeFame.getCopy();
		cv::waitKey(20);
		while (true)
		{
			//get a new frame and convert it to grayscale
			frame = cam.giveFrame();
			Frame Copyframe = Camera::matToFrame(frame);
			Frame gray = ImageProcessing::grayscaleConvertor(Copyframe);
			
			//replace the two frame that are been saved (the two latest one)
			Camera::freeFrame(lastEdgeFrame);

			//show the detected edgest of the frame
			Frame result = ImageProcessing::edgeDetection(gray.frame, gray.cols, gray.rows, 100);
			Mat my_image = TestResult::frameToMat(result);
			cv::imshow("edge.jpg", my_image);

			lastEdgeFrame = newEdgeFame.getCopy();
			newEdgeFame = result.getCopy();

			if (ActivationFace::faceMouseConnector(gray, newEdgeFame, lastEdgeFrame, mouse, eyesInfo, actionEnded, failCounter, movementCounter, moveStatus, clickFrame) == false)
			{
				std::cout << "reintialize begins!\n";
				cv::waitKey(100);
				ActivationFace::initialize(frame, newEdgeFame, cam, eyesInfo);
			}
			else
			{
				if (clickFrame == true)
				{
					clickFrameCounter++;
					if (clickFrameCounter > 25)
					{
						clickFrame = false;
						clickFrameCounter = 0;
					}
				}
			}
			if (movementCounter > 100)
			{
				std::cout << "Good bye!!\n";
				Camera::freeFrame(result);
				Camera::freeFrame(gray);
				frame.empty();
				return 0;
			}
			//free all the created frames
			Camera::freeFrame(result);
			Camera::freeFrame(gray);

			if (frame.empty()) break;

			cv::waitKey(20);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}