#include "../Include/FractalInterface.hpp"

void FractalInterface::SetMaxIterations(int maxIterations)
{
	maxIterations_ = maxIterations;
}
int FractalInterface::GetMaxIterations()
{
	return maxIterations_;
}