#include "mango.h"

namespace mango
{
	Pen::Pen(void)
	{
		mStyle = 0; 
		mWidth = 1; 
		mColor = 0;
	}


	Pen::Pen(int style, int width, COLORREF color)
	{
		mStyle = style; 
		mWidth = width; 
		mColor = color;
	}


	Pen::~Pen(void)
	{

	}


	void Pen::set(int style, int width, COLORREF color)
	{
		mStyle = style; 
		mWidth = width; 
		mColor = color;
	}
};

