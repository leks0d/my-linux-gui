
#ifndef _MANGO_RECT_H

namespace mango
{
	class Point : public Object
	{
	public:
		Point(void);
		Point(int x, int y);
		virtual ~Point(void);

		void set(int x, int y);
		Point& operator =(const Point &pt);
		void offset(int dx, int dy);

	public:
		int x;
		int y;
	};
}




#endif