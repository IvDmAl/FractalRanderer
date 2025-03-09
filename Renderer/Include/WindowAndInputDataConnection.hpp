#pragma once

#include "Inputs.hpp"
#include <functional>
#include <unordered_map>

class WindowAndInputDataConnection
{
public:
	static void connect_window_and_input_data(GLFWwindow* window, 
		Inputs* input_data);

	static void disconnect_window(GLFWwindow* window);

	static void MouseCallback(GLFWwindow* window, 
		int button, int action, int mods);

	static void ScrollCallback(GLFWwindow* window, 
		double xoffset, double yoffset);

	static void KeyboardCallback(GLFWwindow* window, 
		int key, int scancode, int action, int mods);

private:
	static std::unordered_map<GLFWwindow*, Inputs*> connections_;
};