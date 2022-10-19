#include "arithmeticAction.h"

/// <summary>
/// A fast calculate of square root
/// </summary> 
/// <param name="num">The num that will be square root</param>
/// <returns>The square root of the given number</returns>
const float ArithmeticAction::square_root_calc(const float num)
{
	static union { int i; float f; } u{ 0 };
	u.i = 0x2035AD0C + (*(int*)&num >> 1);
	return num / u.f + u.f * 0.25f;
}


/// <summary>
/// Calculete the radius of the circle throw three point in the circle
/// </summary>
/// <param name="firstPoint">The first point</param>
/// <param name="secondPoint">The second point</param>
/// <param name="thirdPoint">The third point </param>
/// <returns>The radius of the circle and its center</returns>
const float* ArithmeticAction::threeEquationsCalc(unsigned int firstPoint[2], unsigned int secondPoint[2], unsigned int thirdPoint[2])
{
    static float result[3] = { 0 };
    double xCount[3] = { 0 };
    double yCount[3] = { 0 };
    long numCount[3] = { 0 };
    unsigned long radius{ 0 };
    double xDistamce{ 0 };
    double yCenter{ 0 };
    double xCenter{ 0 };

    // convert three eqation into two
    numCount[0] = (firstPoint[0] * firstPoint[0] + firstPoint[1] * firstPoint[1]) - (secondPoint[0] * secondPoint[0] + secondPoint[1] * secondPoint[1]);
    numCount[1] = (firstPoint[0] * firstPoint[0] + firstPoint[1] * firstPoint[1]) - (thirdPoint[0] * thirdPoint[0] + thirdPoint[1] * thirdPoint[1]);
    xCount[0] = (firstPoint[0] * -2) - (secondPoint[0] * -2);
    yCount[0] = (firstPoint[1] * -2) - (secondPoint[1] * -2);
    xCount[1] = (firstPoint[0] * -2) - (thirdPoint[0] * -2);
    yCount[1] = (firstPoint[1] * -2) - (thirdPoint[1] * -2);

    // convert two eqation into one
    xDistamce = (float)xCount[0] / xCount[1];
    xCount[1] *= xDistamce;
    yCount[1] *= xDistamce;
    numCount[1] *= xDistamce;
    yCount[2] = yCount[1] - yCount[0];
    numCount[2] = numCount[1] - numCount[0];

    // solve the one eqation
    yCenter = (float)numCount[2] / yCount[2] * -1;
    xCenter = (numCount[0] + yCount[0] * yCenter) / xCount[0] * -1;
    radius = (firstPoint[0] - xCenter) * (firstPoint[0] - xCenter) + (firstPoint[1] - yCenter) * (firstPoint[1] - yCenter);
    result[0] = square_root_calc(radius);
    result[1] = xCenter;
    result[2] = yCenter;
    return result;
}


/// <summary>
/// Claculate the angle between the two eyes center
/// </summary>
/// <param name="firstCenter">The center of the eyes in the old image</param>
/// <param name="secondCenter">The center of the eyes in the new image</param>
/// <returns>The angele of the eyes in between the two images</returns>
const float ArithmeticAction::calcFaceAngle(float firstCenter[2], float secondCenter[2])
{
    float circlesGradient{ 0 };
    
    if (firstCenter[X_INDEX] == secondCenter[X_INDEX])
    {
        if (firstCenter[Y_INDEX] > secondCenter[Y_INDEX])
        {
            return 90;
        }
        else if (firstCenter[Y_INDEX] < secondCenter[Y_INDEX])
        {
            return -90;
        }
        else
        {
            return 0;
        }
    }
    if (firstCenter[Y_INDEX] == secondCenter[Y_INDEX])
    {
        if (firstCenter[X_INDEX] > secondCenter[X_INDEX])
        {
            return -180;
        }
        else if (firstCenter[X_INDEX] < secondCenter[X_INDEX])
        {
            return 180;
        }
        else
        {
            return 0;
        }
    }
    circlesGradient = -1 * (firstCenter[Y_INDEX] - secondCenter[Y_INDEX]) / (firstCenter[X_INDEX] - secondCenter[X_INDEX]);
    return atan(circlesGradient) * 180 / PI;
}

/// <summary>
/// The function gets the two eyes cesnter and calculter the centers of the two eyes
/// </summary>
/// <param name="firstCenter">The eye center's place</param>
/// <param name="secondCenter">The second eye center's place</param>
/// <returns>The place of the centers of the two eyes</returns>
void ArithmeticAction::calculateEyesCenter(float firstCenter[2], float secondCenter[2], float eyesCenterBuffer[2])
{
    static float eyesCenter[2] = { 0 };
    eyesCenterBuffer[X_INDEX] = (firstCenter[X_INDEX] + secondCenter[X_INDEX]) / 2.0;
    eyesCenterBuffer[Y_INDEX] = (firstCenter[Y_INDEX] + secondCenter[Y_INDEX]) / 2.0;
}

const float ArithmeticAction::calcAvg(float inputBuffer[], int size)
{
    float sum{ 0 };
    for (unsigned int i = 0; i < size; i++)
    {
        sum += inputBuffer[i];
    }
    return (sum / size);
}

