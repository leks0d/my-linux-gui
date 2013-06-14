
#include <mango.h>


namespace mango 
{

	Size::Size(void)
	{
		cx = 0;
		cy = 0;
	}
	
	
	Size::Size(int _cx, int _cy)
	{
		cx = _cx;
		cy = _cy;
	}


	Size::~Size(void)
	{

	}


	void Size::set(int cx, int cy)
	{
		this->cx = cx;
		this->cy = cy;
	}


	Size& Size::operator =(const Size &size)
	{
		cx = size.cx;
		cy = size.cy;

		return *this;
	}

}