#include "../Include/ColoringInterface.hpp"

void ColoringInterface::GetColor(double iterationRatio, 
	Eigen::Vector3d& color)
{
	if (colors.size() < 2)
	{
		return;
	}

	int L = 0, R = colors.size(), mid;
	while (L + 1 < R)
	{
		mid = (R + L) >> 1;
		if (iterationRatio <= colors[L + 1].first)
			R = mid;
		else
			L = mid;
	}

	double blendAmount = (iterationRatio - colors[L].first) /
		(colors[L + 1].first - colors[L].first);

	color = (colors[L + 1].second - colors[L].second) *
		blendAmount + colors[L].second;
}

void ColoringInterface::SetColorPoint(double mandelbrotValue,
	double r, double g, double b)
{
	colors.push_back({mandelbrotValue, Eigen::Vector3d(r, g, b)});
	std::sort(colors.begin(), colors.end(), 
		[](const std::pair<double, Eigen::Vector3d>& lhs,
			const std::pair<double, Eigen::Vector3d>& rhs) {
				return lhs.first < rhs.first;
		});
}