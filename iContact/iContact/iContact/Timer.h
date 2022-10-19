#pragma once

#include<iostream>
#include<thread>
#include<chrono>

class Timer
{
	unsigned state = 0;
	unsigned previousState = 0;

	bool timeRunning = false;
	bool timesUp = false;
	bool stop = false;
	std::thread timer;

	void timeCount(const unsigned sec);
	
public:

	void startTimer(const unsigned sec, const unsigned state);
	bool isFinish();
	bool isSameState(const unsigned state);
	bool isRunning();
	unsigned getCurrentState();
};