#pragma once

#include "FractalInterface.hpp"

class Mandelbrot : public FractalInterface
{
public:
	double Evaluate(std::complex<double> c);
};