#include"Camera.h"

Camera::Camera()
{
	openCamera();
}

cv::Mat Camera::giveFrame()
{
	cv::Mat frame;
	cap >> frame;
	return frame;
}

Frame Camera::matToFrame(cv::Mat& mat)
{
	Frame newFrame;
	newFrame.rows = mat.rows;
	newFrame.cols = mat.cols;
	newFrame.numColors = 3;
	newFrame.frame = new uchar * [mat.rows];
	const unsigned newCols = mat.cols * sizeof(uint8_t) * 3;
	for (int i = 0; i < newFrame.rows; ++i)
	{
		newFrame.frame[i] = new uchar[newCols];
	}

	for (int i = 0; i < newFrame.rows; ++i)
	{
		std::memcpy(newFrame.frame[i], mat.ptr(i), newCols);
	}
	return newFrame;
}

void Camera::freeFrame(Frame& frame)
{
	if (frame.frame != nullptr)
	{
		for (int i = 0; i < frame.rows; ++i)
		{
			delete[] frame.frame[i];
		}

		delete[] frame.frame;
		frame.frame = nullptr;
	}
}

void Camera::closeCamera()
{
	this->cap.release();
}

void Camera::openCamera()
{
	if (!this->cap.open(0))
	{
		throw std::exception("the camera don't opened");
	}	
}

Camera::~Camera()
{
	closeCamera();
}

Frame Frame::getCopy()
{
	unsigned char** copiedFrame = new unsigned char* [cols];
	for (unsigned int i = 0; i < rows; i++)
	{
		copiedFrame[i] = new unsigned char[cols];
		for (unsigned int j = 0; j < cols; j++)
		{
			copiedFrame[i][j] = frame[i][j];
		}
	}
	return Frame{ copiedFrame, rows, cols };
}
