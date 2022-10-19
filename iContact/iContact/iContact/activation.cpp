#include "activation.h"

bool ActivationFace::calcMiddlePoint(Frame grey, const int radius, float middlePointBuffer[2])
{
	
	return true;
}


float ActivationFace::claculateAngle(float previousPlace[2][3], float currentPlace[2][3])
{
	float middle1[2] = { 0 };
	float middle2[2] = { 0 };
	float previosCircleOne[2] = { previousPlace[0][X_AXIS], previousPlace[0][Y_AXIS] };
	float previosCicleTwo[2] = { previousPlace[1][X_AXIS], previousPlace[1][Y_AXIS] };
	float currentCircleOne[2] = { currentPlace[0][X_AXIS], currentPlace[0][Y_AXIS] };
	float currentCicleTwo[2] = { currentPlace[1][X_AXIS], currentPlace[1][Y_AXIS] };
	ArithmeticAction::calculateEyesCenter(previosCircleOne, previosCicleTwo, middle1);
	ArithmeticAction::calculateEyesCenter(currentCircleOne, currentCicleTwo, middle2);
	return ArithmeticAction::calcFaceAngle(middle1, middle2);
}


bool ActivationFace::faceMouseConnector(Frame& newFrameGray, Frame& newFrameEdge, Frame& lastFrame, Mouse& mouse, float originalEyesInfo[2][3], bool& actionEnded, int& failCounter, unsigned int& noMovementCounter, bool& moveState, bool& clickFrame)
{
	float resultBuffer[2][3] = { 0 };
	const float radius{ originalEyesInfo[0][RADIUS] };
	if (ImageProcessing::movementDetection(ImageProcessing::subtractFrames(newFrameEdge, lastFrame)))
	{
		noMovementCounter = 0;
		const int eyesFindingStatus = ImageProcessing::twoEyesFinded(newFrameGray, resultBuffer, radius, originalEyesInfo[0][X_AXIS] - 8 * radius, originalEyesInfo[0][Y_AXIS] - 4 * radius, 12, 7);
		//reinitializing test
		if (eyesFindingStatus == CIRCLES_NOT_FOUND)
		{
			std::cout << "Not good!\n";
			failCounter++;
			if (failCounter > 50)
			{
				mouse.stop();
				moveState = false;
				clickFrame = false;
				return false;
			}
			TestResult::findEyesTest(resultBuffer, newFrameGray);
			return true;
		}
		failCounter = 0;

		//mouse movement manneger
		if (eyesFindingStatus == SUCCESS)
		{
			if (clickFrame == false)
			{
				actionEnded = true;
				float angle = ActivationFace::claculateAngle(originalEyesInfo, resultBuffer);
				std::cout << angle << "\n";
				if (moveState == true)
				{
					mouse.stop();
				}
				moveState = true;
				mouse.moveMouse(angle);
				originalEyesInfo[0][X_AXIS] = resultBuffer[0][X_AXIS];
				originalEyesInfo[0][Y_AXIS] = resultBuffer[0][Y_AXIS];
				originalEyesInfo[1][X_AXIS] = resultBuffer[1][X_AXIS];
				originalEyesInfo[1][Y_AXIS] = resultBuffer[1][Y_AXIS];
			}
			TestResult::findEyesTest(resultBuffer, newFrameGray);
		}
		else if (actionEnded == true && eyesFindingStatus == ONE_CIRCLE_MISSING)
		{
			if (moveState == true)
			{
				moveState = false;
				mouse.stop();
				return true;
			}
			if (std::abs(radius - resultBuffer[0][RADIUS]) < 2)
			{
				if (std::abs(originalEyesInfo[0][X_AXIS] - resultBuffer[0][X_AXIS]) > 5 * radius && std::abs(originalEyesInfo[0][Y_AXIS] - resultBuffer[0][Y_AXIS]) > 2 * radius)
				{
					mouse.RightClick();
					actionEnded = false;
				}
				else if (std::abs(originalEyesInfo[1][X_AXIS] - resultBuffer[1][X_AXIS]) > 5 * radius && std::abs(originalEyesInfo[1][Y_AXIS] - resultBuffer[1][Y_AXIS]) > 2 * radius)
				{
					mouse.LeftClick();
					actionEnded = false;
				}
				clickFrame = true;
			}
			else
			{
				for (unsigned int i = 0; i < 10; i++)
				{
					TestResult::findEyesTest(resultBuffer, newFrameGray);
					cv::waitKey(10);
				}
			}
			TestResult::findEyesTest(resultBuffer, newFrameGray);
			cv::waitKey(10);
			memset(resultBuffer, 0, sizeof(resultBuffer));
		}
		else
		{
			noMovementCounter++;
		}
		return true;
	}
}


const void ActivationFace::initialize(cv::Mat& inputFrame, Frame& outputFrame, Camera& cam, float result[2][3])
{
	const int MAX_COUNT = 10;
	Frame copiedInput = Camera::matToFrame(inputFrame);
	Frame grayedInput = ImageProcessing::grayscaleConvertor(copiedInput);
	float lastCenter[2] = { 0 };
	float foundRadiuses[10] = { 0 };
	float radiusAvg{ 0 };
	unsigned counter = 0;
	unsigned oneEyeCounter = 0;
	const float maxY = grayedInput.rows - grayedInput.rows / 3;
	for (int i = 0; i < 5; i++)
	{
		grayedInput = ActivationFace::getGrayscleImg(cam, inputFrame);
		int errorMsg = ImageProcessing::twoEyesFinded(grayedInput, result);
		while (errorMsg != SUCCESS)
		{
			float minY = result[0][Y_AXIS] + 1.2 * result[0][RADIUS];
			float foundRadius = result[0][RADIUS];
			memset(result, 0, sizeof(result));
			while (errorMsg == ONE_CIRCLE_MISSING)
			{
				memset(result, 0, sizeof(result));
				Camera::freeFrame(grayedInput);
				grayedInput = ActivationFace::getGrayscleImg(cam, inputFrame);
				errorMsg = ImageProcessing::twoEyesFinded(grayedInput, result, foundRadius, 0, minY);
				minY = result[0][Y_AXIS] - 1.2 * result[0][RADIUS];
				TestResult::findEyesTest(result, grayedInput);
				if (oneEyeCounter > 10)
				{
					oneEyeCounter = 0;
					foundRadius = 0;
				}
				else
				{
					oneEyeCounter++;
				}
				cv::waitKey(10);
			}
			if (errorMsg != SUCCESS)
			{
				cv::waitKey(10);
				Camera::freeFrame(grayedInput);
				grayedInput = ActivationFace::getGrayscleImg(cam, inputFrame);
				memset(result, 0, sizeof(result));
				errorMsg = ImageProcessing::twoEyesFinded(grayedInput, result, radiusAvg);
				TestResult::findEyesTest(result, grayedInput);
				Frame edges = ImageProcessing::edgeDetection(grayedInput.frame, grayedInput.cols, grayedInput.rows);
				cv::Mat my_image = TestResult::frameToMat(edges);
				Camera::freeFrame(edges);
				cv::imshow("edge.jpg", my_image);
				counter++;
				if (counter == MAX_COUNT)
				{
					counter = 0;
					i = 0;
					radiusAvg = 0;
				}
			}
		}
		foundRadiuses[i] = result[0][0];

		//A validation test
		if (i != 0)
		{
			if (std::abs(radiusAvg - result[0][RADIUS]) < 2 && (std::abs(lastCenter[X_AXIS - 1] - result[0][X_AXIS]) < 2 && std::abs(lastCenter[Y_AXIS - 1] - result[0][Y_AXIS]) < 2))
			{
				if (i > 1)
				{
					radiusAvg = ArithmeticAction::calcAvg(foundRadiuses, i);
				}
			}
			else if (i >= 2 && (std::abs(radiusAvg - result[0][RADIUS]) > 5 || (std::abs(lastCenter[X_AXIS - 1] - result[0][X_AXIS]) > radiusAvg || std::abs(lastCenter[Y_AXIS - 1] - result[0][Y_AXIS]) > radiusAvg)))
			{
				i--;
			}
			else
			{
				i = -1;
				radiusAvg = 0;
			}
		}
		else
		{
			radiusAvg = result[0][RADIUS];
		}
		lastCenter[X_AXIS - 1] = result[0][X_AXIS];
		lastCenter[Y_AXIS - 1] = result[0][Y_AXIS];

	}
	outputFrame.frame = grayedInput.frame;
	outputFrame.cols = grayedInput.cols;
	outputFrame.rows = grayedInput.rows;

	for (unsigned int i = 0; i < 10; i++)
	{
		std::cout << "Eyes was found!\n";
		TestResult::findEyesTest(result, outputFrame);
		cv::waitKey(100);
	}

}



Frame& ActivationFace::getGrayscleImg(Camera& cam, cv::Mat& matOutout)
{
	matOutout = cam.giveFrame();
	Frame copiedOutput = Camera::matToFrame(matOutout);
	Frame grayedOutput = ImageProcessing::grayscaleConvertor(copiedOutput);
	Camera::freeFrame(copiedOutput);
	return grayedOutput;
}