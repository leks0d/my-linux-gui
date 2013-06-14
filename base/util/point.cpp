
#include <mango.h>


namespace mango 
{
	Point::Point(void)
	{
		x = 0;
		y = 0;
	}


	Point::Point(int x0, int y0)
	{
		x = x0;
		y = y0;
	}


	Point::~Point(void)
	{

	}


	void Point::set(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	void Point::offset(int dx, int dy)
	{
		x += dx;
		y += dy;
	}


	


	Point& Point::operator =(const Point &pt)
	{
		x = pt.x;
		y = pt.y;

		return *this;
	}


}