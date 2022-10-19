#include "Pixel.h"

Pixel::Pixel(unsigned int x_value, unsigned int y_value) : x_value(x_value), y_value(y_value)
{
}

/// <summary>
/// Calculate the power of distance between two pixels
/// </summary>
/// <param name="second_pixel">The second pixel to calc</param>
/// <returns>The power of the distance between the two pixels</returns>
const float Pixel::calc_powered_distance(Pixel& second_pixel)
{
	const unsigned int total_x_values = second_pixel.x_value - this->x_value;
	const unsigned int total_y_values = second_pixel.y_value - this->y_value;
	return total_x_values * total_x_values + total_y_values * total_y_values;
}
