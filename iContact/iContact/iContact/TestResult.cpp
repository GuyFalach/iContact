#include"TestResult.h"


/// <summary>
/// Draw cross on the middle of the frame
/// </summary>
/// <param name="frame">The frame that the function get and return</param>
void TestResult::markerMiddle(cv::Mat& frame)
{
	int middleRow = frame.rows / 2;
	int middleCols = frame.cols / 2;
	cv::line(frame, cv::Point(middleCols, 0), cv::Point(middleCols, frame.rows), cv::Scalar(255, 0, 0), 2, cv::LINE_8);
	cv::line(frame, cv::Point(0, middleRow), cv::Point(frame.cols, middleRow), cv::Scalar(255, 0, 0), 2, cv::LINE_8); 
}


/// <summary>
/// Draw circle on the frame
/// </summary>
/// <param name="frame">The frame that the function get and return</param>
/// <param name="center">The circle center point</param>
/// <param name="radius">The radius of the circle</param>
void TestResult::markerEyes(cv::Mat& frame, const cv::Point& center, float radius)
{
	cv::circle(frame,center, radius, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
}

/// <summary>
/// This function will convert Frame to Mat
/// </summary>
/// <param name="frame"> The frame</param>
/// <returns>The Mat</returns>
cv::Mat TestResult::frameToMat(Frame& frame)
{
	const unsigned int total = frame.rows * frame.cols;
	unsigned int counter = 0;
	unsigned char* img = new unsigned char[total*3];
	for (unsigned i = 0; i < frame.rows; i++)
	{
		for (unsigned j = 0; j < frame.cols; j++)
		{
			img[counter++] = frame.frame[i][j];
			img[counter++] = frame.frame[i][j];
			img[counter++] = frame.frame[i][j];
		}
	}
	cv::Mat imgInMat(frame.rows, frame.cols, CV_8UC3, img);
	//delete[] img; DO NOT DELETE!!!
	return imgInMat;
}

void TestResult::findEyesTest(float result[2][3], Frame& grayscaleImg)
{
	cv::Mat img = TestResult::frameToMat(grayscaleImg);
	if (result[0][X_AXIS] != 0)
	{
		TestResult::markerEyes(img, cv::Point(int(result[0][X_AXIS]), int(result[0][Y_AXIS])), result[0][RADIUS]);
	}
	if (result[1][X_AXIS] != 0)
	{
		std::cout << int(result[1][X_AXIS]) << " " << int(result[1][Y_AXIS]) << std::endl;
		TestResult::markerEyes(img, cv::Point(int(result[1][X_AXIS]), int(result[1][Y_AXIS])), result[0][RADIUS]);
	}
	cv::imshow("test", img);
	cv::waitKey(10);
}

