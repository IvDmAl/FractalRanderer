#pragma once

#include <vector>
#include <Eigen/Dense>

/*
This class defines a coloring scheme that renders the fractal linearly between user-defined colors. 
Several derived classes exist in the ./ColoringSchemes folder that already contain the inputed colors.
*/

class ColoringInterface
{
public:
	/*
	Given the iterationRatio, returns the RGB values by reference used to color this point. The iterationRatio
	is a value between 0 and 1, where a value closer to 1 means it is closer to being inside the fractal (1 is inside).
	*/
	void GetColor(double iterationRatio, Eigen::Vector3d& color);

	/*
	Adds a color point to the fractal. The mandelbrotValue is a value between 0 and 1, and the rgb values
	are the values to color that point. 
	*/
	void SetColorPoint(double mandelbrotValue, double r, double g, double b);

private:
	// Variables to store specific RGB points
	std::vector<Eigen::Vector3d> colors;
	std::vector<double> mandelbrotValues_;
};