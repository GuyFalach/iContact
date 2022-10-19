#include "Timer.h"

void Timer::timeCount(const unsigned sec)
{
	std::this_thread::sleep_for(std::chrono::seconds(sec));
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	while (start - end < std::chrono::seconds(sec) && !stop)
	{
		end = std::chrono::high_resolution_clock::now();
	}
	if (start - end < std::chrono::seconds(sec))
	{
		this->timesUp = true;
	}
	stop = false;
}

void Timer::startTimer(const unsigned sec, const unsigned state)
{
	if (!timeRunning)
	{
		this->state = state;
		timer = std::thread(&Timer::timeCount, this, sec);
		timer.detach();
		this->timeRunning = true;
	}
}

bool Timer::isFinish()
{
	if (this->timesUp)
	{
		this->timesUp = false;
		this->timeRunning = false;
		return true;
	}
	stop = true;
	this->timesUp = false;
	this->timeRunning = false;
	return false;
}

bool Timer::isSameState(const unsigned state)
{
	return (this->state == state);
}

bool Timer::isRunning()
{
	return this->timeRunning;
}

unsigned Timer::getCurrentState()
{
	return this->state;
}
