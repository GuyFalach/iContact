#pragma once
#include <math.h>

#define X_INDEX 0
#define Y_INDEX 1
#define PI 3.14159265

class ArithmeticAction {
public:
	static const float square_root_calc(const float num);
	static const float* threeEquationsCalc(unsigned int firstPoint[2], unsigned int secondPoint[2], unsigned int thirdPoint[2]);
	static const float calcFaceAngle(float firstCenter[2], float secondCenter[2]);
	static void calculateEyesCenter(float firstCenter[2], float secondCenter[2], float eyesCenterBuffer[2]);
	static const float calcAvg(float array[], int size);
};