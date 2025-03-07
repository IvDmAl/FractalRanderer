#include <iostream>
#include <ColoringSchemes/Include/Classic.hpp>
#include <Fractals/Include/Mandelbrot.hpp>
#include <Renderer/Include/Includes.hpp>
#include <Eigen/Dense>

int main()
{
	// User instructions
	std::cout << "This example renders the Mandelbrot Set." << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\t- 'Left Click + Mouse' to pan" << std::endl;
	std::cout << "\t- 'Mouse Scroll Wheel' to zoom" << std::endl;
	std::cout << "\t- 'r' to reset back to the Mandelbrot Set" << std::endl;
	std::cout << 
		"\t- 'q' and 'w' to increase/decrease the amount of iterations" << std::endl;

	// Set the window's width and height if they are passed as command line arguments
	Eigen::Vector2i windowSize(1280, 720);
	// TODO: Add argc and argv command line arg functionality

	// Create a fractal renderer window with a passed width and height
	Renderer renderer(windowSize, "Mandelbrot Viewer");

	// Set the render's fractal to the mandelbrot set
	renderer.SetFractal(std::make_shared<Mandelbrot>());

	// Set the coloring scheme to blue
	renderer.SetColorScheme(std::make_shared<Classic>());

	// Continuously render the fractal
	while (renderer.Process_frame()) {
#ifdef DEBUG
		TimeStatistic::print_statistic();
		ThreadingFunctionCall::print();
		std::system("cls");
		Logs::get_instance().print();
#endif // DEBUG
	}

	return 0;
}
