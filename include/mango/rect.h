
#ifndef _MANGO_RECT_H

namespace mango
{
	class Rect : public Object
	{
	public:
		Rect(void);
		Rect(int left, int top, int right, int bottom);
		virtual ~Rect(void);

		void set(int left, int top, int right, int bottom);
		void setEx(int left, int top, int width, int height);
		void offset(int dx, int dy);
		bool ptIn(int x, int y);
		bool isEmpty() const;
		bool intersect(const Rect &rect);
		bool intersect(int x, int y, Rect &rect1, int x1, int y1);
		int  subIntersect(Rect rcDst[4], const Rect& src2);

		bool unionRect(const Rect*  rect1);


		Rect& operator =(const Rect &rect);
		bool operator ==(const Rect &rect1) const;

		void inflate(int dx, int dy);
		int getWidth(){int w=right-left;if(w<0) w=0;return w;}
		int getHight(){int h=bottom-top;if(h<0) h=0;return h;}
	public:
		int left;
		int top;
		int right;
		int bottom;
	};
}




#endif