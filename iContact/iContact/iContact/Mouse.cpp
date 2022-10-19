#include "Mouse.h"

/// <summary>
/// This function will move the mouse to place on the screen
/// </summary>
/// <param name="x">the value of x</param>
/// <param name="y">the value of y</param>
void Mouse::MouseMove(int x, int y)
{
    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx = x * (65535.0f / fScreenWidth);
    double fy = y * (65535.0f / fScreenHeight);
    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = fx;
    Input.mi.dy = fy;
    ::SendInput(1, &Input, sizeof(INPUT));
}

/// <summary>
/// Move the mouse by adding to x and y value 
/// </summary>
/// <param name="moveX"> in how much the mouse on the x value </param>
/// <param name="moveY"> in how much the mouse on the y value </param>
void Mouse::moveTowordAngle(const int moveX, const int moveY)
{
    moving = true;
	while (!this->stopFlag)
	{
		POINT p;
		GetCursorPos(&p);
		MouseMove(p.x+moveX, p.y-moveY);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}

    moving = false;
	this->stopFlag = false;
}

void Mouse::scrollAngle(const int angle)
{
    scrolling = true;
    while (!this->stopFlag)
    {
        if (30 <= angle <= 150)
        {
            this->scrollUp();
        }
        else if (-30 >= angle >= -150)
        {
            this->scrollDown();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    scrolling = false;
    this->stopFlag = false;
}

void Mouse::scrollUp()
{
    mouse_event(MOUSEEVENTF_WHEEL, MOUSEEVENTF_ABSOLUTE, MOUSEEVENTF_ABSOLUTE, 1000, NULL);
}

void Mouse::scrollDown()
{
    mouse_event(MOUSEEVENTF_WHEEL, MOUSEEVENTF_ABSOLUTE, MOUSEEVENTF_ABSOLUTE, -1000, NULL);
}

/// <summary>
/// Stop the moving of the mouse
/// </summary>
void Mouse::stop()
{
    if (moving || scrolling)
    {
        this->stopFlag = true;
    }
    Sleep(50);
}


/// <summary>
/// Move the mouse in specific angle
/// </summary>
/// <param name="angle"> the angle which the mouse move by </param>
void Mouse::moveMouse(const int angle)
{
    const unsigned MOVING_SPEED = 5;
    const int xAdd = round(cos(angle * PI / BASE) * MOVING_SPEED);
    const int yAdd = round(sin(angle * PI / BASE) * MOVING_SPEED);
    this->stop();
    activity = std::thread(&Mouse::moveTowordAngle, this, xAdd, yAdd);

    activity.detach();
}

void Mouse::scroll(const int angle)
{
    this->stop();
    activity = std::thread(&Mouse::scrollAngle, this, angle);

    activity.detach();
}


void Mouse::LeftClick()
{
    INPUT Input = { 0 };
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void Mouse::RightClick()
{
    INPUT Input = { 0 };
    // right down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // right up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

// get in scroll mode
void Mouse::ScrollClick()
{   
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

bool Mouse::ifMoving()
{
    return moving;
}

bool Mouse::ifscrolling()
{
    return this->scrolling;
}
