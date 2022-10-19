#pragma once
#include "Camera.h"
#include "arithmeticAction.h"
#include "ImageProcessing.h"

#define ERROR -1

class ActivationFace
{
public:
	static const void initialize(cv::Mat& inputFrame, Frame& outputFrame, Camera & cam, float eyesInfo[2][3]);
	static float claculateAngle(float previousPlace[2][3], float currentPlace[2][3]);
	static bool faceMouseConnector(Frame& newFrameGray, Frame& newFrameEdge, Frame& lastFrame, Mouse& mouse, float originalEyesInfo[2][3], bool& actionEnded, int& failCounter, unsigned int& noMovementCounter, bool& moveState, bool& clickFrame);
private:
	static bool calcMiddlePoint(Frame grey, const int radius, float middlePointBuffer[2]);
	static Frame& getGrayscleImg(Camera& cam, cv::Mat& matOutout);
};