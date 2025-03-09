#include "../Include/Mandelbrot.hpp"

#include <mutex>

double Mandelbrot::Evaluate(std::complex<double> c)
{
	std::complex<double> z(0, 0);
	for (double i = 0; i < maxIterations_; i++) {
		z = z*z + c;

		double&& re = z.real();
		double&& im = z.imag();
		if (re * re + im * im > 4) {
			return i / maxIterations_;
		}
	}
	return 1;
}