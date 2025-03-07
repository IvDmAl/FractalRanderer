#include "../Include/includes.hpp"
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <sstream>

Renderer::Renderer(Eigen::Vector2i windowSize, 
	const std::string& name)
	:
	window_(nullptr),
	center(0, 0)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::Renderer");
#endif // DEBUG	

	windowSize_ = windowSize;
	aspectRatio_ = windowSize_.y() / static_cast<double>(windowSize_.x());
	width = Eigen::Vector2d(4, 4 * aspectRatio_);

	fractalMemory_.resize(windowSize_.x(),
		std::vector<Eigen::Vector3d>(windowSize_.y()));

	if (!glfwInit()) {
		std::cout << "GLFW was not initialized properly" << std::endl;
		return;
	}

	window_ = glfwCreateWindow(
		windowSize_.x(), windowSize_.y(), name.c_str(), nullptr, nullptr);

	userInputs_ = std::make_shared<Inputs>(window_);

	if (!window_)
	{
		std::cout << "Window was not created properly" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetWindowSizeLimits(window_,
		windowSize_.x(), windowSize_.y(), windowSize_.x(), windowSize_.y());

	glfwMakeContextCurrent(window_);

	glLoadIdentity();
	glOrtho(0, windowSize_.x(), windowSize_.y(), 0, 0, 1);
	glPointSize(1);

	glfwSetMouseButtonCallback(window_,
		WindowAndInputDataConnection::MouseCallback);
	glfwSetScrollCallback(window_,
		WindowAndInputDataConnection::ScrollCallback);
	glfwSetKeyCallback(window_,
		WindowAndInputDataConnection::KeyboardCallback);
}

bool Renderer::Process_frame() {
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::Process_frame");
#endif // DEBUG


	need_draw |= pollEvents();

	if (true||
		need_draw)
	{
		Draw();
		need_draw = false;
	}

	return !glfwWindowShouldClose(window_);
}

void Renderer::Draw()
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::Draw");
#endif // DEBUG


	glBegin(GL_POINTS);
	for (int x = 0; x < windowSize_.x(); x++)
	{
		for (int y = 0; y < windowSize_.y(); y++)
		{
			glColor3f(
				GLfloat(fractalMemory_[x][y].x()),
				GLfloat(fractalMemory_[x][y].y()),
				GLfloat(fractalMemory_[x][y].z()));

			glVertex2f(GLfloat(x), GLfloat(y));
		}
	}

	glEnd();

	glfwSwapBuffers(window_);
}

bool Renderer::pollEvents() {
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::pollEvents");
#endif // DEBUG


	glfwPollEvents();

	userInputs_->update_vals(windowSize_, center, width);

	bool was_updates = false;
	Eigen::Vector2d old_center = center;
	Eigen::Vector2d old_width = width;

	was_updates |= userInputs_->make_move(windowSize_, width, center);

	was_updates |= userInputs_->make_zoom(width);

	was_updates |= userInputs_->make_keyboards_commands(fractal_,
		windowSize_, width, center);

	if (was_updates)
		SmartRecalc(old_center, old_width);

	userInputs_->Reset_vals();
	return was_updates;
}

void Renderer::SmartRecalc(
	Eigen::Vector2d old_center,
	Eigen::Vector2d old_width)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::SmartRecalc");
#endif // DEBUG

	if (width == old_width)
	{
		Eigen::Vector2<bool> need_move(
			(center - old_center).x() != 0,
		  (center - old_center).y() != 0);

		if (need_move.x() && need_move.y())
		{
			double cur_center_y = center.y();
			center.y() = old_center.y();
			move_on_x(old_center);
			center.y() = cur_center_y;
			old_center.x() = center.x();
			move_on_y(old_center);
		}
		else
		{
			if (need_move.x())
				move_on_x(old_center);
			if (need_move.y())
				move_on_y(old_center);
		}
	}
	else if (center == old_center)
	{
		double ratio_widths = width.x() / old_width.x();
		if (std::abs(ratio_widths - 
			std::round(ratio_widths)) < 0.000001)
		{
			decrease(std::round(ratio_widths));
		}
		else if (
			std::abs((1 / ratio_widths) - 
				std::round(1 / ratio_widths)) < 0.000001)
		{
			increase(std::round(1 / ratio_widths));
		}
		else
		{
			for (int i = 0; i < windowSize_.x(); ++i)
				for (int j = 0; j < windowSize_.y(); ++j)
					pixel_need_recalc.push_back(Eigen::Vector2d(i, j));
			Recalc();
		}
	}
	else
	{
		for (int i = 0; i < windowSize_.x(); ++i)
			for (int j = 0; j < windowSize_.y(); ++j)
				pixel_need_recalc.push_back(Eigen::Vector2d(i, j));
		Recalc();
	}
}

void Renderer::move_on_x(Eigen::Vector2d old_center)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::move_on_x");
#endif // DEBUG
	Eigen::Vector2d pixel_move =
		CoordDiff_to_PixelDiff(old_center - center);

	auto calc = [&](Eigen::Vector2d pos) {
		Eigen::Vector2d old_pos = pos - pixel_move;
		if (//false &&
			0 <= old_pos.x() && old_pos.x() < windowSize_.x() &&
			0 <= old_pos.y() && old_pos.y() < windowSize_.y())
		{
			fractalMemory_[pos.x()][pos.y()] =
				fractalMemory_[old_pos.x()][pos.y()];
		}
		else
		{
			Recalc_pixel(pos.x(), pos.y());
		}
		};

  auto calc_x = [&, this](int y)
		{
			if (pixel_move.x() < 0)
			{
				for (int x = 0; x < windowSize_.x(); ++x)
				{
					calc(Eigen::Vector2d(x, y));
				}
			}
			else
			{
				for (int x = windowSize_.x() - 1; x >= 0; --x)
				{
					calc(Eigen::Vector2d(x, y));
				}
			}
		};

	auto calc_y = [&](int phase, int num_threads) {
		for (int y = phase; y < windowSize_.y(); y += num_threads)
			calc_x(y);
		};

	ThreadingFunctionCall::call(calc_y, "Renderer::move_on_x");
}
void Renderer::move_on_y(Eigen::Vector2d old_center)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::move_on_y");
#endif // DEBUG
	Eigen::Vector2d pixel_move =
		CoordDiff_to_PixelDiff(old_center - center);

	auto calc = [&](Eigen::Vector2d pos) {
		Eigen::Vector2d old_pos = pos - pixel_move;
		if (//false &&
			0 <= old_pos.x() && old_pos.x() < windowSize_.x() &&
			0 <= old_pos.y() && old_pos.y() < windowSize_.y())
		{
			fractalMemory_[pos.x()][pos.y()] =
				fractalMemory_[pos.x()][old_pos.y()];
		}
		else
		{
			Recalc_pixel(pos.x(), pos.y());
		}
		};

	auto calc_y = [&](int x) {
		if (pixel_move.y() < 0)
		{
			for (int y = 0; y < windowSize_.y(); ++y)
				calc(Eigen::Vector2d(x, y));
		}
		else
		{
			for (int y = windowSize_.y() - 1; y >= 0; --y)
				calc(Eigen::Vector2d(x, y));
		}
		};

	auto calc_x = [&, this](int phase, int num_threads)
		{
			for (int x = phase; x < windowSize_.x(); x += num_threads)
			{
				calc_y(x);
			}
		};

	ThreadingFunctionCall::call(calc_x, "Renderer::move_on_y");
}
void Renderer::increase(int zoom_val)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::increase");
#endif // DEBUG

	Eigen::Vector2i center_window = windowSize_ / 2;
	Eigen::Vector2i start = center_window - center_window / zoom_val;
	Eigen::Vector2i end = center_window +
		(windowSize_ - Eigen::Vector2i(1, 1) - center_window) / zoom_val;

	std::vector<std::vector<Eigen::Vector3d>> old_part(
		end.x() - start.x() + 1,
		std::vector<Eigen::Vector3d>(end.y() - start.y() + 1));

	auto copy = [&](int phase, int num_threads) {
		for (int i = start.x() + phase; i <= end.x(); i += num_threads)
		{
			std::memcpy(
				old_part[i - start.x()].begin()._Ptr,
				(fractalMemory_[i].begin() + start.y())._Ptr,
				(end.y() + 1 - start.y()) * sizeof(Eigen::Vector3d)
			);
		}
		};

	auto calc = [&](Eigen::Vector2i pos) {
		Eigen::Vector2i move = pos - center_window;
		if (move.x() % zoom_val == 0 &&
			move.y() % zoom_val == 0)
		{
			Eigen::Vector2i old_pos = center_window + move / zoom_val - start;
			
			std::swap(fractalMemory_[pos.x()][pos.y()],
				old_part[old_pos.x()][old_pos.y()]);
		}
		else
		{
			Recalc_pixel(pos.x(), pos.y());
		}
		};

	auto calc_all = [&](int phase, int num_threads) {
		for (int x = phase; x < windowSize_.x(); x += num_threads)
			for (int y = 0; y < windowSize_.y(); ++y)
				calc(Eigen::Vector2i(x, y));
		};

	ThreadingFunctionCall::call(copy, "Renderer::increase(copy)");
	ThreadingFunctionCall::call(calc_all, "Renderer::increase(call)");
}
void Renderer::decrease(int zoom_val)
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::decrease");
#endif // DEBUG

	Eigen::Vector2i center_window = windowSize_ / 2;
	Eigen::Vector2i start = center_window - center_window / zoom_val;
	Eigen::Vector2i end = center_window +
		(windowSize_ - Eigen::Vector2i(1, 1) - center_window) / zoom_val;

	std::vector<std::vector<Eigen::Vector3d>> new_part(
		end.x() - start.x() + 1,
		std::vector<Eigen::Vector3d>(end.y() - start.y() + 1));

	auto copy = [&](int phase, int num_threads)
		{
			Eigen::Vector2i new_pos;
			for (new_pos.x() = phase; new_pos.x() < new_part.size(); 
				new_pos.x() += num_threads)
				for (new_pos.y() = 0; new_pos.y() < new_part[new_pos.x()].size();
					++new_pos.y())
				{
				  Eigen::Vector2i old_pos = center_window + 
						(new_pos + start - center_window) * zoom_val;

					std::swap(fractalMemory_[old_pos.x()][old_pos.y()],
						new_part[new_pos.x()][new_pos.y()]);
				}
		};

	auto calc = [&](int phase, int num_threads) {
		for (int x = phase; x < start.x(); x += num_threads)
			for (int y = 0; y < windowSize_.y(); ++y)
				Recalc_pixel(x, y);
		for (int x = end.x() + 1 + phase; x < windowSize_.x();
			x += num_threads)
			for (int y = 0; y < windowSize_.y(); ++y)
				Recalc_pixel(x, y);

		for (int x = start.x() + phase; x <= end.x(); x += num_threads)
		{
			for (int y = 0; y < start.y(); ++y)
				Recalc_pixel(x, y);
			
			for (int y = end.y() + 1; y < windowSize_.y(); ++y)
				Recalc_pixel(x, y);

			std::memcpy(
				(fractalMemory_[x].begin() + start.y())._Ptr,
				new_part[x - start.x()].begin()._Ptr,
				(end.y() + 1 - start.y()) * sizeof(Eigen::Vector3d));
		}
		};

	ThreadingFunctionCall::call(copy, "Renderer::decrease(copy)");
	for (int i = 0; i < windowSize_.x(); ++i)
		for (int j = 0; j < windowSize_.y(); ++j)
			fractalMemory_[i][j] = Eigen::Vector3d(1, 1, 1);
	ThreadingFunctionCall::call(calc, "Renderer::decrease(calc)");
}

void Renderer::Recalc()
{
#ifdef DEBUG
	TimeStatistic time_statistic("Renderer::Recalc");
#endif // DEBUG

	auto calc = [this](int phase, int num_threads) {
		for (int i = phase; i < pixel_need_recalc.size(); i += num_threads)
		{
			Recalc_pixel(pixel_need_recalc[i].x(), pixel_need_recalc[i].y());
		}
		};

	ThreadingFunctionCall::call(calc, "Renderer::Recalc");

	pixel_need_recalc.clear();
}

void Renderer::Recalc_pixel(double x, double y)
{
	double result = fractal_->Evaluate(
		std::complex<double>(
			center.x() + width.x() * (x / windowSize_.x() - 0.5),
			center.y() - width.y() * (y / windowSize_.y() - 0.5)));

	colorScheme_->GetColor(result, fractalMemory_[int(x)][int(y)]);
}

Eigen::Vector2d Renderer::PixelDiff_to_CoordDiff(
	Eigen::Vector2d pixel_diff)
{	
	return Eigen::Vector2d(
		pixel_diff.x() * width.x() / windowSize_.x(),
	 -pixel_diff.y() * width.y() / windowSize_.y());
}
Eigen::Vector2d Renderer::CoordDiff_to_PixelDiff(
	Eigen::Vector2d coord_diff)
{	
	return Eigen::Vector2d(
		coord_diff.x() * windowSize_.x() / width.x(),
	 -coord_diff.y() * windowSize_.y() / width.y());
}

Eigen::Vector2d Renderer::PixelToCoord(Eigen::Vector2d pixel)
{
	Eigen::Vector2d windowCenter(
		windowSize_.x() / 2.0,
		windowSize_.y() / 2.0);

	return center + PixelDiff_to_CoordDiff(pixel - windowCenter);
}

Eigen::Vector2d Renderer::CoordToPixel(Eigen::Vector2d coord)
{
	Eigen::Vector2d windowCenter(
		windowSize_.x() / 2.0,
		windowSize_.y() / 2.0);

	return windowCenter + CoordDiff_to_PixelDiff(coord - center);
}

std::complex<double> Renderer::to_complex(Eigen::Vector2d vector)
{
	return std::complex<double>(vector.x(), vector.y());
}

void Renderer::SetFractal(std::shared_ptr<FractalInterface> fractal)
{
	fractal_ = fractal;
	if (colorScheme_ != nullptr)
	{
		for (int i = 0; i < windowSize_.x(); ++i)
			for (int j = 0; j < windowSize_.y(); ++j)
				pixel_need_recalc.push_back(Eigen::Vector2d(i, j));
		Recalc();
	}
}

void Renderer::SetColorScheme(
	std::shared_ptr<ColoringInterface> colorScheme)
{
	colorScheme_ = colorScheme;
	if (fractal_ != nullptr)
	{
		for (int i = 0; i < windowSize_.x(); ++i)
			for (int j = 0; j < windowSize_.y(); ++j)
				pixel_need_recalc.push_back(Eigen::Vector2d(i, j));
		Recalc();
	}
}

void Renderer::SetNumberThreads(int numThreads)
{
	numThreads_ = numThreads;
}
