#pragma once

#include <iostream>
//#include "ColoringSchemes/include/ColoringSchemes/Classic.h"
//#include <ColoringSchemes/Rainbow.h>
//#include <ColoringSchemes/MyСoloring.h>
//#include "Fractals/include/Fractals/Mandelbrot.h"
//#include "Fractals/BurningShip.h"
//#include "Fractals/CosFractal.h"
#include "Renderer/include/Renderer.hpp"
//#include <chrono>
#include <Eigen/Dense>

using namespace std;

int main()
{
	// User instructions
	cout << "This example renders the Mandelbrot Set." << endl;
	cout << "Commands:" << endl;
	cout << "\t- 'Left Click + Mouse' to pan" << endl;
	cout << "\t- 'Mouse Scroll Wheel' to zoom" << endl;
	cout << "\t- 'r' to reset back to the Mandelbrot Set" << endl;
	cout << "\t- 'q' and 'w' to increase/decrease the amount of iterations" << endl;

	// Set the window's width and height if they are passed as command line arguments
	Eigen::Vector2i windowSize(1280, 720);
	// TODO: Add argc and argv command line arg functionality

	// Create a fractal renderer window with a passed width and height
	//Renderer renderer(windowSize, "Mandelbrot Viewer");

	// Set the render's fractal to the mandelbrot set
	//renderer.SetFractal(std::make_shared<Mandelbrot>());

	// Set the coloring scheme to blue
	//renderer.SetColorScheme(std::make_shared<Classic>());

	// Continuously render the fractal
	//while (renderer.Process_frame()) {}

	return 0;
}
