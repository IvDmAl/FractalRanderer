#include "../Include/Mandelbrot.hpp"

#include <mutex>

double Mandelbrot::Evaluate(std::complex<double> c)
{
	static std::mutex mtx;
	mtx.lock();
	FractalInterface::cnt++;
	mtx.unlock();

	std::complex<double> z(0, 0);
	for (double i = 0; i < maxIterations_; i++) {
		z = z*z + c;

		if (std::abs(z) > 2) {
			return i / maxIterations_;
		}
	}
	return 1;
}