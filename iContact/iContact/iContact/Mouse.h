#pragma once

#include<iostream>
#include<thread>
#include <mutex>
#include<Windows.h>
#include <chrono>
#include <math.h>

#define PI 3.14159265
#define BASE 180

class Mouse
{
	std::thread activity;
	bool stopFlag = false;
	bool moving = false;
	bool scrolling = false;

	void MouseMove(int x, int y);
	void moveTowordAngle(int moveX, int moveY);
	void scrollAngle(const int angle);

	void scrollUp();
	void scrollDown();

public:

	void stop();

	void moveMouse(const int angle);
	void scroll(const int angle);

	void LeftClick();
	void RightClick();

	void ScrollClick();

	bool ifMoving();
	bool ifscrolling();
};