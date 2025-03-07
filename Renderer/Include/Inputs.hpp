#pragma once

#include <Eigen/Dense>
#include <set>
#include <Fractals/Include/FractalInterface.hpp>

struct Inputs
{
	Inputs(GLFWwindow* window);

	void update_vals(const Eigen::Vector2i& windowSize,
		const Eigen::Vector2d& center,
		const Eigen::Vector2d& width);

	void Reset_vals();

	bool make_move(const Eigen::Vector2i& windowSize,
		const Eigen::Vector2d& width,
		Eigen::Vector2d& center);

	bool make_zoom(Eigen::Vector2d& width);

	bool make_keyboards_commands(
		std::shared_ptr<FractalInterface>& fractal,
		const Eigen::Vector2i& windowSize,
		Eigen::Vector2d& width,
		Eigen::Vector2d& center);

	// The x and y position of the mouse
	Eigen::Vector2d mousePos;

	// The left and right click of the mouse
	bool leftClick = false;
	bool rightClick = false;

	// The scrollwheel of the mouse
	double scrollwheel = 0;

	std::set<int> pressedChar;
	GLFWwindow* window;
};