#include <>

class MyColoring : public ColoringInterface
{
public:
	/*
	The constructor automatically adds color points to the ColoringInterface
	*/
	MyColoring()
	{
		SetColorPoint(0, 1, 1, 1);
		SetColorPoint(0.3, 1, 1, 0);
		SetColorPoint(0.6, 1, 0, 0);
		SetColorPoint(1, 0, 0, 0);
	}
};