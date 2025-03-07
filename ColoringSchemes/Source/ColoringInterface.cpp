#include "../Include/ColoringInterface.hpp"

void ColoringInterface::GetColor(double iterationRatio, 
	Eigen::Vector3d& color)
{
	if (mandelbrotValues_.size() < 2)
	{
		return;
	}

	int p = 0;
	for (p; p < mandelbrotValues_.size() - 1; p++)
	{
		if (iterationRatio <= mandelbrotValues_[p + 1])
			break;
	}

	double blendAmount = (iterationRatio - mandelbrotValues_[p]) /
		(mandelbrotValues_[p + 1] - mandelbrotValues_[p]);

	color = (colors[p + 1] - colors[p]) * blendAmount + colors[p];
}

void ColoringInterface::SetColorPoint(double mandelbrotValue, 
	double r, double g, double b)
{
	colors.push_back(Eigen::Vector3d(r, g, b));
	mandelbrotValues_.push_back(mandelbrotValue);
}