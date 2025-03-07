#pragma once

#include <vector>
#include <Eigen/Dense>

class ColoringInterface
{
public:
	void GetColor(double iterationRatio, Eigen::Vector3d& color);
	void SetColorPoint(double mandelbrotValue, 
		double r, double g, double b);

private:
	std::vector<Eigen::Vector3d> colors;
	std::vector<double> mandelbrotValues_;
};