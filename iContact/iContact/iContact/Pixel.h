#pragma once

class Pixel
{
public:
	Pixel(unsigned int x_value, unsigned int y_value);
	const float calc_powered_distance(Pixel& second_pixel);
private:
	unsigned int x_value;
	unsigned int y_value;
};