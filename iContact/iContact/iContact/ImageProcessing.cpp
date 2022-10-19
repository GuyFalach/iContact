#include"ImageProcessing.h"
#include<vector>

/// <summary>
/// Convert normal frame to grayscale frame
/// </summary>
/// <param name="frame"> The normal frame</param>
/// <returns>The grayscale frame</returns>
Frame ImageProcessing::grayscaleConvertor(Frame& frame)
{
    Frame grayscaleFrame;
    grayscaleFrame.cols = frame.cols;
    grayscaleFrame.rows = frame.rows;
    grayscaleFrame.numColors = 1;

    const float redToGray = 0.2126;
    const float greenToGray = 0.7152;
    const float blueToGray = 0.0722;
    unsigned colorsCols = frame.cols * frame.numColors;

    grayscaleFrame.frame = new unsigned char* [frame.rows];

    for (int i = 0; i < grayscaleFrame.rows; i++)
    {
        unsigned counterCols = 0;
        grayscaleFrame.frame[i] = new unsigned char[grayscaleFrame.cols];

        for (int j = 0; j < colorsCols; j += 3)
        {
            unsigned char gray = redToGray * frame.frame[i][j] + greenToGray * frame.frame[i][j + 1] + blueToGray * frame.frame[i][j + 2];
            grayscaleFrame.frame[i][counterCols] = gray;
            counterCols++;
        }
    }
    Camera::freeFrame(frame);
    return grayscaleFrame;
}


/// <summary>
/// Findes the circle and radius of the two eyes
/// </summary>
/// <param name="edges">The frame edges</param>
/// <param name="result">An empty buffer that the results will be enters to</param>
/// <param name="radius">The radius of the eyes</param>
const int ImageProcessing::twoEyesFinded(Frame greyscaleImg, float result[2][3], float radius, float minX, float minY, const unsigned int mulColRange , const unsigned int mulRowRange)
{
    Frame edges = ImageProcessing::edgeDetection(greyscaleImg.frame, greyscaleImg.cols, greyscaleImg.rows);
    if (radius == 0)
    {
        radius = calcRadius(edges.frame, edges.cols, edges.rows);
        if (radius == 0)
        {
            return CIRCLES_NOT_FOUND;
        }
    }
    const float* firstCircle = getCircleCenterPlace(edges, radius, mulColRange, mulRowRange, minX, minY);
    if (firstCircle == nullptr)
    {
        return CIRCLES_NOT_FOUND;
    }
    result[0][RADIUS] = radius;
    result[0][X_AXIS] = firstCircle[X_AXIS];
    result[0][Y_AXIS] = firstCircle[Y_AXIS];

    minX = firstCircle[X_AXIS] + 6 * radius;
    minY = firstCircle[Y_AXIS] - 1.2 * radius;
    
    const float* secondCircle = getCircleCenterPlace(edges, radius, mulColRange, mulRowRange, minX, minY);
    if (secondCircle == nullptr || std::abs(secondCircle[Y_AXIS] - result[0][Y_AXIS]) > radius || std::abs(secondCircle[X_AXIS] - result[0][X_AXIS]) < 5 * radius)
    {
        return ONE_CIRCLE_MISSING;
    }
    result[1][X_AXIS] = secondCircle[X_AXIS];
    result[1][Y_AXIS] = secondCircle[Y_AXIS];
    result[0][RADIUS] = (float)radius;

    TestResult::findEyesTest(result, greyscaleImg);

    return SUCCESS;
}


/// <summary>
/// The function finds circle with the given radius
/// </summary>
/// <param name="grayscaleImg">The grayscale img that the circle will be found on</param>
/// <param name="radius">The wanted radius of the circle</param>
/// <returns>The center of the circle that was found</returns>
const float* ImageProcessing::getCircleCenterPlace(Frame blackWhiteFrame, const float radius, const unsigned int mulColRange, const unsigned int mulRowRange, unsigned int minX, unsigned int minY)
{
    const float* circleInfo = nullptr;
    unsigned char** blackWhiteImg = blackWhiteFrame.frame;
    unsigned int colRange[2] = { blackWhiteFrame.cols / 3 + 1, blackWhiteFrame.cols - blackWhiteFrame.cols / 3 };
    unsigned int rowRange[2] = { blackWhiteFrame.rows / 3 + 1, blackWhiteFrame.rows - blackWhiteFrame.rows / 3 };
    unsigned int circleCols[3] = { 0 };
    unsigned int circleRows[3] = { 0 };
    if (minX != 0)
    {
        colRange[0] = minX;
        colRange[1] = minX + mulColRange * radius;
    }
    if (minY != 0)
    {
        rowRange[0] = minY;
        rowRange[1] = minY + mulRowRange * radius;
    }
    for (unsigned int i = rowRange[0]; i < rowRange[1]; i++)
    {
        for (unsigned int j = colRange[0]; j < colRange[1]; j++)
        {
            if (blackWhiteImg[i][j] != NON_EDGE && blackWhiteImg[i][j] <= STRONG_EDGE)
            {
                circleCols[0] = j;
                circleRows[0] = i;
                for (unsigned int k = j + 1; k < colRange[1]; k++)
                {
                    if (blackWhiteImg[i][k] != NON_EDGE && blackWhiteImg[i][j] <= STRONG_EDGE)
                    {
                        circleCols[1] = k;
                        circleRows[1] = i;
                        for (unsigned int m = circleCols[0] + 1; m < colRange[1]; m++)
                        {
                            if (blackWhiteImg[i + 4][m] != NON_EDGE && blackWhiteImg[i][j] <= STRONG_EDGE)
                            {
                                circleCols[2] = m;
                                circleRows[2] = i + 4;
                                unsigned int point1[2] = { circleCols[0], circleRows[0] };
                                unsigned int point2[2] = { circleCols[1], circleRows[1] };
                                unsigned int point3[2] = { circleCols[2], circleRows[2] };
                                const float* circleInfo{ ArithmeticAction::threeEquationsCalc(point1, point2, point3) };
                                if (std::abs(radius - circleInfo[RADIUS]) < 2)
                                {
                                    return circleInfo;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}


/// <summary>
/// Clac the radius of the circle in the image
/// </summary>
/// <param name="blackWhiteImg">An image with where the edge are detected and marked by white</param>
/// <param name="colNum">The size of each row in the image</param>
/// <param name="rowNum">The amount of rows in the image</param>
/// <returns>The radius of the circle</returns>
const float ImageProcessing::calcRadius(unsigned char** blackWhiteImg, const unsigned int colNum, const unsigned int rowNum, const unsigned int minRadius, const unsigned int maxRadius)
{
    const unsigned int colRange[2] = { colNum / 3 + 1, colNum - colNum / 3 };
    const unsigned int rowRange[2] = { 50, rowNum - rowNum / 3 };
    unsigned int circleCols[5] = { 0 };
    unsigned int circleRows[5] = { 0 };
    float radius{ 0 };
    unsigned int circleCounter{ 0 };
    for (unsigned int i = rowRange[0]; i < rowRange[1]; i++)
    {
        for (unsigned int j = colRange[0]; j < colRange[1] ; j++)
        {
            if (blackWhiteImg[i][j] != NON_EDGE && blackWhiteImg[i][j] < STRONG_EDGE)
            {
                circleCols[0] = j;
                circleRows[0] = i;
                for (unsigned int k = j + 1; k < j + 35; k++)
                {
                    if (blackWhiteImg[i][k] != NON_EDGE && blackWhiteImg[i][k] < STRONG_EDGE)
                    {
                        circleCols[1] = k;
                        circleRows[1] = i;
                        for (unsigned int m = circleCols[0] + 1; m < circleCols[0] + 40; m++)
                        {
                            if (blackWhiteImg[i + 2][m] != NON_EDGE && blackWhiteImg[i + 2][m] < STRONG_EDGE)
                            {
                                circleCols[2] = m;
                                circleRows[2] = i + 2;
                                for (unsigned int n = m + 1; n < m + 40; n++)
                                {
                                    if (blackWhiteImg[i + 2][n] != NON_EDGE && blackWhiteImg[i + 2][n] < STRONG_EDGE)
                                    {
                                        circleCols[3] = n;
                                        circleRows[3] = i + 2;

                                        for (unsigned int o = n + 1; o < n + 40; o++)
                                        {
                                            if (blackWhiteImg[i + 4][o] != NON_EDGE && blackWhiteImg[i + 4][o] < STRONG_EDGE)
                                            {
                                                circleCols[4] = o;
                                                circleRows[4] = i + 4;
                                                unsigned int point1[2] = { circleCols[0], circleRows[0] };
                                                unsigned int point2[2] = { circleCols[1], circleRows[1] };
                                                unsigned int point3[2] = { circleCols[2], circleRows[2] };
                                                unsigned int point4[2] = { circleCols[3], circleRows[3] };
                                                unsigned int point5[2] = { circleCols[4], circleRows[4] };
                                                radius = ArithmeticAction::threeEquationsCalc(point1, point2, point3)[0];
                                                if (radius <= MAX_RADIUS && radius >= MIN_RADIUS)
                                                {
                                                    float radius2 = ArithmeticAction::threeEquationsCalc(point1, point2, point4)[0];
                                                    float radius3 = ArithmeticAction::threeEquationsCalc(point3, point4, point5)[0];
                                                    if (std::abs(radius - radius2) < 2 && std::abs(radius3 - radius2) < 2 && std::abs(radius3 - radius) < 2)
                                                    {
                                                        return radius;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0.0;
}


/// <summary>
/// Detect the edges of the frame.
/// </summary>
/// <param name="grayscaleImg">A frame in grayscale type.</param>
/// <param name="colNum">The number of cell in each row.</param>
/// <param name="rowNum">The number of row in the frame.</param>
/// <returns>Black and white image which show where the frame edges are.</returns>
Frame& ImageProcessing::edgeDetection(unsigned char** grayscaleImg, const unsigned int colNum, const unsigned int rowNum, const unsigned int minGradient)
{
    Frame grayScale{};
    Frame result{};
    grayScale.frame = grayscaleImg;
    grayScale.cols = colNum;
    grayScale.rows = rowNum;
    const Frame bluredGrayScale = blurFillter(grayScale);
    result.frame = edgeFillter(sobelOperator(bluredGrayScale.frame, bluredGrayScale.cols, bluredGrayScale.rows), bluredGrayScale.cols, bluredGrayScale.rows, minGradient);
    result.cols = bluredGrayScale.cols;
    result.rows = bluredGrayScale.rows;
    return result;
}




/// <summary>
/// Cutted all the uneccery data from the black and white img
/// </summary>
/// <param name="blackWhiteImg">The black and white img</param>
/// <param name="eyeRadius">The radius of the eye</param>
/// <param name="eyeCenter">The index of the the</param>
/// <returns>Cutted eye without one eye</returns>
const Frame& ImageProcessing::cutImgInfo(unsigned char** blackWhiteImg, const float eyeRadius, const float eyeCenter[2], bool getTwoEyes)
{
    Frame result;
    unsigned int cuttedXCounter{ 0 };
    unsigned int cuttedYCounter{ 0 };
    unsigned int minYIndex = eyeCenter[Y_INDEX] - eyeRadius - 5;
    unsigned int maxYIndex = eyeCenter[Y_INDEX] + eyeRadius + 5;
    unsigned int minXIndex = eyeCenter[X_INDEX] + eyeRadius + 10;
    unsigned int maxXIndex = eyeCenter[X_INDEX] + 3 * eyeRadius + 50;
    if (getTwoEyes)
    {
        minXIndex -= 2 * eyeRadius - 10;
    }
    unsigned char** cuttedImg = new unsigned char* [maxYIndex - minYIndex];
    for (unsigned int i = minYIndex; i <= maxYIndex; i++)
    {
        cuttedImg[cuttedYCounter] = new unsigned char[maxXIndex - minXIndex];
        cuttedXCounter = 0;
        for (unsigned int j = minXIndex; j <= maxXIndex; j++)
        {
            cuttedImg[cuttedYCounter][cuttedXCounter] = blackWhiteImg[i][j];
            cuttedXCounter++;
        }
        cuttedYCounter++;
    }
    result.frame = cuttedImg;
    result.rows = maxYIndex - minYIndex;
    result.cols = maxXIndex - minXIndex;
    return result;
}


/// <summary>
/// This function will apply blur fillter on the image
/// </summary>
/// <param name="image">the image</param>
/// <returns>the image blured</returns>
Frame ImageProcessing::blurFillter(Frame image)
{
    const float BLUR_KERNEL[KERNEL_SIZE][KERNEL_SIZE] = { {0.0625, 0.125, 0.0625},
                                                        {0.125, 0.25, 0.125},
                                                        {0.0625, 0.125, 0.0625} };
    float** kernel = new float* [KERNEL_SIZE];
    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        kernel[i] = new float[KERNEL_SIZE];
        for (int j = 0; j < KERNEL_SIZE; j++)
        {
            kernel[i][j] = BLUR_KERNEL[i][j];
        }
    }
    Frame output = ImageProcessing::applyFillter<float>(image, kernel, KERNEL_SIZE, KERNEL_SIZE);
    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        delete[] kernel[i];
    }
    delete[] kernel;
    return output;
}


/// <summary>
/// Will do max pooling on the frame(didn't check if work as should, DO NOT use)
/// </summary>
/// <param name="image">the frame</param>
/// <returns>the pooled frame</returns>
Frame ImageProcessing::maxPooling(Frame image)
{
    const unsigned poolBy = 4;
    Frame output;
    output.rows = image.rows / poolBy;
    output.cols = image.cols / poolBy;
    output.frame = new unsigned char* [output.rows];
    unsigned rowCounter = 0;
    for (int i = 0; i < image.rows; i+= poolBy)
    {
        output.frame[rowCounter] = new unsigned char[output.cols];
        unsigned colCounter = 0;
        for (int j = 0; j < image.cols; j += poolBy)
        {
            output.frame[rowCounter][colCounter] = maxPixels(image, j, i, poolBy);
            colCounter++;
        }
        rowCounter++;
    }
    return output;
}


/// <summary>
/// Subtract the two frame pixel by pixel(no nagative pixel value) 
/// </summary>
/// <param name="frame1">the first frame</param>
/// <param name="frame2">the second frame</param>
/// <returns>the subtract of the two frames</returns>
Frame ImageProcessing::subtractFrames(Frame frame1, Frame frame2)
{
    Frame output;
    output.rows = frame1.rows;
    output.cols = frame1.cols;
    output.frame = new unsigned char* [output.rows];
    for (int i = 0; i < output.rows; i++)
    {
        output.frame[i] = new unsigned char[output.cols];
        for (int j = 0; j < output.cols; j++)
        {
            int temp = frame1.frame[i][j] - frame2.frame[i][j];
            if (temp > 20)
            {
                output.frame[i][j] = 255;
            }
            else
            {
                output.frame[i][j] = 0;
            }
        }
    }
    return output;
}


bool ImageProcessing::movementDetection(Frame differenceInfo)
{
    for (unsigned int i = 0; i < differenceInfo.rows; i++)
    {
        for (unsigned int j = 0; j < differenceInfo.cols; j++)
        {
            if (differenceInfo.frame[i][j] == 255)
            {
                return true;
            }
        }
    }
    return false;
}


unsigned ImageProcessing::blink(float result[2][3])
{
    if (result[0] != nullptr && result[1] == nullptr)
    {
        return 1;
    }
    else if (result[0] == nullptr && result[1] != nullptr)
    {
        return 2;
    }
    else if (result[0] != nullptr && result[1] != nullptr)
    {
        return 3;
    }
    return 0;
}

unsigned ImageProcessing::timeWink(float result[2][3], Timer& time)
{
    int eyesEvent = blink(result);
    if (!time.isRunning())
    {
        if (!time.isSameState(eyesEvent))
        {
            if (time.isFinish())
            {
                return time.getCurrentState();
            }
            else
            {
                time.startTimer(1, eyesEvent);
            }
        }
    }
    else
    {
        if (!time.isSameState(eyesEvent))
        {
            time.startTimer(1, eyesEvent);
        }
    }
    return 0;
}

void ImageProcessing::event(float result[2][3], const float angle, Mouse& mouse, Timer& time)
{
    int eyesEvent = timeWink(result, time);
    if (eyesEvent == 0)
    {
        mouse.moveMouse(angle);
    }
    else if (eyesEvent == 1 || eyesEvent == 2)
    {
        if (mouse.ifMoving())
        {
            mouse.stop();
        }
        else
        {
            if (eyesEvent == 1)
            {
                mouse.RightClick();
            }
            else if (eyesEvent == 2)
            {
                mouse.LeftClick();
            }
        }
    }
}


Frame ImageProcessing::emphasisOneColor(Frame grayscaleImg, const unsigned char color)
{
    unsigned counter = 0;
    Frame newFrame;
    newFrame.cols = grayscaleImg.cols;
    newFrame.rows = grayscaleImg.rows;
    newFrame.frame = new unsigned char* [newFrame.rows];
    for (int i = 0; i < newFrame.rows; i++)
    {
        newFrame.frame[i] = new unsigned char [newFrame.cols];

        for (int j = 0; j < newFrame.cols; j++)
        {
            if (grayscaleImg.frame[i][j] == color)
            {
                newFrame.frame[i][j] = 255;
                counter++;
            }
            else
            {
                newFrame.frame[i][j] = 0;
            }
            
        }
    }
    std::cout << counter << "\n";
    return newFrame;
}

/// <summary>
/// For every pixel it says the likelihood of it being an edge. 
/// </summary>
/// <param name="grayscele_img">The grayscale img.</param>
/// <param name="columnNum">The number of cell in the row.</param>
/// <param name="rowNum">The number of rows in the img.</param>
/// <returns>The likelihood of each pixel to be an edge.</returns>
unsigned char** ImageProcessing::sobelOperator(unsigned char ** grayscele_img, const unsigned int columnNum, const unsigned int rowNum)
{
    const int X_KERNEL[KERNEL_SIZE][KERNEL_SIZE] = { {-1, 0, 1},
                                           {-2, 0, 2},
                                           {-1, 0, 1} };
    const int Y_KERNEL[KERNEL_SIZE][KERNEL_SIZE] = { {-1, -2, -1},
                                               {0,0,0},
                                               {1, 2, 1} };
    unsigned char** totalGradients = new unsigned char* [rowNum];
    const unsigned int regularCol{ columnNum - 1 };
    const unsigned int regularRol{ rowNum - 1 };
    int x_gradient{ 0 };
    int y_gradient{ 0 };
    totalGradients[0] = new unsigned char[columnNum]();

    for (unsigned int i = 1; i < regularRol; i++)
    {
        totalGradients[i] = new unsigned char[columnNum]();
        for (unsigned int j = 1; j < regularCol; j++)
        {
            x_gradient = 0;
            y_gradient = 0;
            for (unsigned int k = 0; k < KERNEL_SIZE; k++)
            {
                for (unsigned int m = 0; m < KERNEL_SIZE; m++)
                {
                    x_gradient += X_KERNEL[k][m] * grayscele_img[i + k - 1][j + m - 1];
                    y_gradient += Y_KERNEL[k][m] * grayscele_img[i + k - 1][j + m - 1];
                }
            }
            totalGradients[i][j] = ArithmeticAction::square_root_calc(x_gradient * x_gradient + y_gradient * y_gradient);
        }

        x_gradient = 0;
        y_gradient = 0;
        totalGradients[i][columnNum - 1] = 0;
    }
    totalGradients[regularRol] = new unsigned char[columnNum];
    return totalGradients;
}


/// <summary>
/// convert the gradient to black of white image.
/// </summary>
/// <param name="pixels_gradient">The gradient of each pixel.</param>
/// <param name="columnNum">The numer of cell in each row.</param>
/// <param name="rowNum">The number of row in the array.</param>
/// <returns>Black and white image when white = edge.</returns>
unsigned char** ImageProcessing::edgeFillter(unsigned char** pixels_gradient, const unsigned int columnNum, const unsigned int rowNum, const unsigned int minGradient)
{
    unsigned char** result = new unsigned char* [rowNum]();
    result[0] = new unsigned char[columnNum]();
    for (unsigned int i = 1; i < rowNum - 1; i++)
    {
        result[i] = new unsigned char[columnNum]();
        for (unsigned int j = 1; j < columnNum - 1; j++)
        {
            if (pixels_gradient[i][j] <= minGradient)
            {
                result[i][j] = 0;
            }
            else
            {
                result[i][j] = pixels_gradient[i][j];
            }
        }
        delete pixels_gradient[i];
    }
    result[rowNum - 1] = new unsigned char [columnNum]();
    delete pixels_gradient;
    return result;
}


/// <summary>
/// Return the biggest value of pixal in the image in the pooling square area
/// </summary>
/// <param name="image">the frame</param>
/// <param name="y">the y of the point to start the checking</param>
/// <param name="x">the x of the point to start the checking</param>
/// <param name="edgePooling">the size of one edge of the pooling square</param>
/// <returns>the value of the biggest pixal</returns>
unsigned char ImageProcessing::maxPixels(Frame image, const unsigned int y, const unsigned int x, const unsigned edgePooling)
{
    unsigned char max = image.frame[x][y];
    const int xrange = x + edgePooling;
    const int yrange = y + edgePooling;
    for (unsigned i = x; i < xrange; i++)
    {
        for (unsigned j = y; j < yrange; j++)
        {
            if (max < image.frame[x][y])
            {
                max = image.frame[x][y];
            }
        }
    }
    return max;
}
