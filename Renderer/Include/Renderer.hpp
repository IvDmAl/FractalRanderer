#pragma once

#include <Fractals/Include/FractalInterface.hpp>
#include <ColoringSchemes/Include/ColoringInterface.hpp>
#include <Eigen/Dense>
#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <sstream>

class Renderer
{
public:
	Renderer(Eigen::Vector2i windowSize, 
		const std::string& name = "Fractal Viewer");

	void SetFractal(std::shared_ptr<FractalInterface> fractal);
	
	void SetColorScheme(std::shared_ptr<ColoringInterface> colorScheme);

	bool Process_frame();
	void SetNumberThreads(int numThreads);

#ifndef TESTER
private:
#endif
	void Draw();
	bool pollEvents();

	void Recalc();
	void Recalc_pixel(double x, double y);
	
	void SmartRecalc(
		Eigen::Vector2d old_center,
		Eigen::Vector2d old_width);
	void move_on_x(Eigen::Vector2d old_center);
	void move_on_y(Eigen::Vector2d old_center);
	void increase(int zoom_val);
	void decrease(int zoom_val);

	void threading_function_call(void (*f)(int, int));

	inline Eigen::Vector2d PixelDiff_to_CoordDiff(
		Eigen::Vector2d pixel_diff);
	inline Eigen::Vector2d CoordDiff_to_PixelDiff(
		Eigen::Vector2d coord_diff);

	inline Eigen::Vector2d PixelToCoord(Eigen::Vector2d pixel);
	inline Eigen::Vector2d CoordToPixel(Eigen::Vector2d coord);
	inline std::complex<double> to_complex(Eigen::Vector2d vector);

	GLFWwindow* window_;

	std::shared_ptr<FractalInterface> fractal_;

	std::shared_ptr<ColoringInterface> colorScheme_;

	Eigen::Vector2i windowSize_;

	double aspectRatio_;

	Eigen::Vector2d center;
	
	Eigen::Vector2d width;

	int numThreads_ = 15;

	std::vector<std::vector<Eigen::Vector3d>> fractalMemory_;
	std::vector<Eigen::Vector2d> pixel_need_recalc;

	std::shared_ptr<Inputs> userInputs_;

	bool need_draw = true;
};