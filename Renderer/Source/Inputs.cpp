#include "../Include/includes.hpp"

Inputs::Inputs(GLFWwindow* window)
	: window(window)
{
	WindowAndInputDataConnection::connect_window_and_input_data(
		window, this);
}

void Inputs::update_vals(Eigen::Vector2i windowSize,
		Eigen::Vector2d center,
		Eigen::Vector2d width) 
{
	Eigen::Vector2d mouseRaw;
	glfwGetCursorPos(window, &mouseRaw.x(), &mouseRaw.y());

	mousePos.x() = center.x() + (mouseRaw.x() - windowSize.x() / 2) *
		width.x() / windowSize.x();
	mousePos.y() = center.y() + (mouseRaw.y() - windowSize.y() / 2) *
		width.y() / windowSize.y();
}

void Inputs::Reset_vals()
{
	scrollwheel = 0;
}

bool Inputs::make_move(Eigen::Vector2i windowSize,
	Eigen::Vector2d width,
	Eigen::Vector2d& center) 
{	
	bool was_changes = false;
	if (pressedChar.find(GLFW_KEY_UP) != pressedChar.end() ||
		pressedChar.find(GLFW_KEY_DOWN) != pressedChar.end() ||
		pressedChar.find(GLFW_KEY_LEFT) != pressedChar.end() ||
		pressedChar.find(GLFW_KEY_RIGHT) != pressedChar.end())
	{
		Eigen::Vector2d delta(0, 0);
		if (pressedChar.find(GLFW_KEY_UP) != pressedChar.end())
		{
			delta.y() -= 0.1;
		}
		if (pressedChar.find(GLFW_KEY_DOWN) != pressedChar.end())
		{
			delta.y() += 0.1;
		}
		if (pressedChar.find(GLFW_KEY_RIGHT) != pressedChar.end())
		{
			delta.x() -= 0.1;
		}
		if (pressedChar.find(GLFW_KEY_LEFT) != pressedChar.end())
		{
			delta.x() += 0.1;
		}
		
		center += delta.asDiagonal() * width;
		was_changes = true;
	}
	else
	{
		static Eigen::Vector2d oldMousePos(0, 0);
		Eigen::Vector2d mousePos;

		glfwGetCursorPos(window, &mousePos.x(), &mousePos.y());

		if (leftClick)
		{
			// Update the center of the fractal from the mouse's x diff and y diff
			Eigen::Vector2d diffPos = mousePos - oldMousePos;
			Eigen::Vector2d changeInPosition(
				diffPos.x() / windowSize.x(),
				-diffPos.y() / windowSize.y());

			center -= changeInPosition.asDiagonal() * width;
			was_changes = true;
		}
		std::swap(oldMousePos, mousePos);
	}

	return was_changes;
}

bool Inputs::make_zoom(Eigen::Vector2d& width)
{
	if (scrollwheel == 0) return false;

	if (pressedChar.find(GLFW_KEY_Z) == pressedChar.end())
	{
		double zoomAmount = std::max(0.00001, 1 - (scrollwheel * 0.1));
		width *= zoomAmount;
	}
	else
	{
		if (scrollwheel > 0)
			width /= scrollwheel + 1;
		else
			width *= -scrollwheel + 1;
	}
	return true;
}

bool Inputs::make_keyboards_commands(
	std::shared_ptr<FractalInterface>& fractal,
	Eigen::Vector2i windowSize,
	Eigen::Vector2d& width,
	Eigen::Vector2d& center) 
{
	bool was_changes = false;

	static double iterationMultiplier = 1;
	if (pressedChar.find(GLFW_KEY_Q) != pressedChar.end())
	{
		iterationMultiplier *= 1.25;

		fractal->SetMaxIterations(
			iterationMultiplier * 15 * pow(log10(windowSize.x() / width.x()),
				1.8));

		was_changes = true;
	}
	if (pressedChar.find(GLFW_KEY_K) != pressedChar.end())
	{
		iterationMultiplier /= 1.25;

		fractal->SetMaxIterations(
			iterationMultiplier * 15 * pow(log10(windowSize.x() / width.x()),
				1.8));
		
		was_changes = true;
	}


	if (pressedChar.find(GLFW_KEY_R) != pressedChar.end())
	{
		center = Eigen::Vector2d(0, 0);
		width = Eigen::Vector2d(4, 4 * windowSize.y() / windowSize.x());

		iterationMultiplier = 1;
		was_changes = true;
	}
	return was_changes;
}