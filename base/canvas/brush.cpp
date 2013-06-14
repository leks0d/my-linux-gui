#include "mango.h"

namespace mango
{

	Brush::Brush(void)
	{

	}
	
	Brush::Brush(COLORREF clr)
	{
		mStyle = BS_SOLID;
		mColor = clr;
		mHatch = 0;
	}

	Brush::~Brush(void)
	{

	}

	void Brush::set(int style, COLORREF clr, int hatch)
	{
		mStyle = style;
		mColor = clr;
		mHatch = hatch;
	}
}
