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
	~Renderer();

	void SetFractal(std::shared_ptr<FractalInterface> fractal);
	
	void SetColorScheme(std::shared_ptr<ColoringInterface> colorScheme);

	bool Process_frame();
#ifndef TESTER
private:
#endif
	void Draw();
	bool pollEvents();

	void Recalc();
	void Recalc_pixel(double x, double y);
	
	void SmartRecalc(
		Eigen::Vector2d& old_center,
		Eigen::Vector2d& old_width);
	void move(Eigen::Vector2d& old_center);
	void move_on_x(Eigen::Vector2d old_center);
	void move_on_y(Eigen::Vector2d old_center);
	void change_scale(Eigen::Vector2d& old_width);
	void increase(int zoom_val);
	void decrease(int zoom_val);

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

	Eigen::Vector3d** fractalMemory_;

	std::shared_ptr<Inputs> userInputs_;

	bool need_draw = true;
};