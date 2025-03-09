#include "../Include/includes.hpp"

std::unordered_map<GLFWwindow*, Inputs*> WindowAndInputDataConnection::connections_;

void WindowAndInputDataConnection::connect_window_and_input_data(
	GLFWwindow* window, Inputs* input_data)
{
	connections_[window] = input_data;
}

void WindowAndInputDataConnection::disconnect_window(GLFWwindow* window)
{
	connections_.erase(window);
}

void WindowAndInputDataConnection::MouseCallback(GLFWwindow* window,
	int button, int action, int mods)
{
	if (connections_.find(window) == connections_.end())
		return;

	Inputs* data = connections_.at(window);

	//If the left click has been pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			data->leftClick = true;
		}
		else if (action == GLFW_RELEASE)
		{
			data->leftClick = false;
		}
	}

	//If the right click has been pressed
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			data->rightClick = true;
		}
		else if (action == GLFW_RELEASE)
		{
			data->rightClick = false;
		}
	}
}

void WindowAndInputDataConnection::ScrollCallback(GLFWwindow* window,
	double xoffset, double yoffset)
{
	if (connections_.find(window) == connections_.end())
		return;
	
	connections_[window]->scrollwheel = yoffset;
}

void WindowAndInputDataConnection::KeyboardCallback(GLFWwindow* window,
	int key, int scancode, int action, int mods)
{
	if (connections_.find(window) == connections_.end())
		return;

	Inputs* data = connections_[window];

	if (action == GLFW_PRESS)
	{
		data->pressedChar.insert(key);
	}
	if (action == GLFW_RELEASE)
	{
		data->pressedChar.erase(key);
	}
}