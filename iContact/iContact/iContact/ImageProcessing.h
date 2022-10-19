#pragma once
#include "arithmeticAction.h"
#include"Camera.h"
#include "TestResult.h"
#include "Mouse.h"
#include "Timer.h"
#include <chrono>

#define NON_EDGE 0
#define STRONG_EDGE 3000
#define MIN_RADIUS 7
#define MAX_RADIUS 12
#define KERNEL_SIZE 3
#define X_INDEX 0
#define Y_INDEX 1

enum ERRORS
{
    CIRCLES_NOT_FOUND,
    ONE_CIRCLE_MISSING,
    SUCCESS
};


class ImageProcessing
{
public:
    static Frame grayscaleConvertor(Frame& frame);
    static Frame maxPooling(Frame image);
    static Frame subtractFrames(Frame frame1, Frame frame2);
    static bool movementDetection(Frame differenceInfo);
    static Frame& edgeDetection(unsigned char** grayscaleImg, const unsigned int colNum, const unsigned int rowNum, const unsigned int minGradient = 80);
    static const int twoEyesFinded(Frame greyscaleImg, float result[2][3], float radius = 0, float minX = 0, float minY = 0, const unsigned int mulColRange = 5, const unsigned int mulRowRange = 2);
    static unsigned blink(float result[2][3]);
    static unsigned timeWink(float result[2][3], Timer& time);
    static void event(float result[2][3], const float angle, Mouse& mouse, Timer& time);
    static const float calcRadius(unsigned char** blackWhiteImg, const unsigned int colNum, const unsigned int rowNum, const unsigned int minRadius = MIN_RADIUS, const unsigned int maxRadius = MAX_RADIUS);
private:
    static const float* getCircleCenterPlace(Frame grayscaleImg, const float radius, const unsigned int mulColRange, const unsigned int mulRowRange, unsigned int minX = 0, unsigned int minY = 0);
    static Frame emphasisOneColor(Frame grayscaleImg, const unsigned char color);
    static unsigned char** sobelOperator(unsigned char** grayscele_img, const unsigned int columnNum, const unsigned int rowNum);
    static const Frame& cutImgInfo(unsigned char** blackWhiteImg, const float eyeRadius, const float eyeCenter[2], bool getTwoEyes = false);
    static unsigned char** edgeFillter(unsigned char** pixels_gradient, const unsigned int columnNum, const unsigned int rowNum, const unsigned int minGradient);
    static Frame blurFillter(Frame image);
    template <typename T>
    static unsigned char sumFillterArea(Frame image, const unsigned int y, const unsigned int x, T** kernel, const unsigned int kernelCol, const unsigned int kernelRow);
    template <typename T>
    static Frame applyFillter(Frame image, T** kernel, const unsigned int kernelCol, const unsigned int kernelRow);
    static unsigned char maxPixels(Frame image, const unsigned int y, const unsigned int x, const unsigned edgePooling);
};


/// <summary>
/// summarizing the kernel area with the image area
/// </summary>
/// <typeparam name="T"> the type of the kernel(should be int/float/unsigned char)</typeparam>
/// <param name="image"> the frame</param>
/// <param name="y"> the y of the point to start the summarizing</param>
/// <param name="x">the x of the point to start the summarizing</param>
/// <param name="kernel"> the kernel</param>
/// <param name="kernelCol"> the number of cols in the kernel</param>
/// <param name="kernelRow">the number of rows in the kernel</param>
/// <returns>summarizing to one pixel</returns>
template<typename T>
static unsigned char ImageProcessing::sumFillterArea(Frame image, const unsigned int y, const unsigned int x, T** kernel, const unsigned int kernelCol, const unsigned int kernelRow)
{
    float sum = 0;
    for (unsigned i = 0; i < kernelRow; i++)
    {
        for (unsigned j = 0; j < kernelCol; j++)
        {
            sum += kernel[i][j] * image.frame[i + x][j + y];
        }
    }
    unsigned char tempSum = sum;
    return tempSum;
}

/// <summary>
/// apply the fillter on image
/// </summary>
/// <typeparam name="T"> the type of the kernel(should be int/float/unsigned char)</typeparam>
/// <param name="image"> the frame</param>
/// <param name="kernel"> the kernel</param>
/// <param name="kernelCol"> the number of cols in the kernel</param>
/// <param name="kernelRow">the number of rows in the kernel</param>
/// <returns>the new image</returns>
template<typename T>
static Frame ImageProcessing::applyFillter(Frame image, T** kernel, const unsigned int kernelCol, const unsigned int kernelRow)
{
    int reduceRow = (int)kernelRow / 2;
    int reduceCol = (int)kernelCol / 2;
    Frame output;
    output.numColors = 1;
    output.cols = image.cols - (reduceCol * 2);
    output.rows = image.rows - (reduceRow * 2);
    output.frame = new unsigned char* [output.rows];
    for (int i = 0; i < output.rows; i++)
    {
        output.frame[i] = new unsigned char[output.cols];

        for (int j = 0; j < output.cols; j++)
        {
            unsigned char sum = sumFillterArea<T>(image, j, i,kernel, kernelCol, kernelRow);
            output.frame[i][j] = sum;
        }
    }
    return output;
}
