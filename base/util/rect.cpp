
#include <mango.h>


namespace mango 
{
	Rect::Rect(void)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}


	Rect::Rect(int left0, int top0, int right0, int bottom0)
	{
		left = left0;
		top = top0;
		right = right0;
		bottom = bottom0;
	}


	Rect::~Rect(void)
	{

	}


	void Rect::set(int left0, int top0, int right0, int bottom0)
	{
		left = left0;
		top = top0;
		right = right0;
		bottom = bottom0;
	}


	void Rect::setEx(int left0, int top0, int width0, int height0)
	{
		left = left0;
		top = top0;
		right = left0 + width0;
		bottom = top0 + height0;
	}


	void Rect::offset(int dx, int dy)
	{
		left += dx;
		top += dy;
		right += dx;
		bottom += dy;
	}


	bool Rect::ptIn(int x, int y)
	{
		if ((x < left) || (x >= right))
			return false;
		else if ((y < top) || (y >= bottom))
			return false;
		else
			return true;
	}


	bool Rect::isEmpty() const
	{
		if (right <= left)
			return true;
		else if (bottom <= top)
			return true;
		else
			return false;
	}


	bool Rect::intersect(const Rect &rect)
	{
		left   = max(left, rect.left);
		right  = min(right, rect.right);
		top    = max(top, rect.top) ;
		bottom = min(bottom, rect.bottom) ;

		if(isEmpty())
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
			return false;
		}
		else
			return true;
	}


	bool Rect::intersect(int x, int y, Rect &rect1, int x1, int y1)
	{
		Rect rect;
		bool intersectant;

		rect = *this;

		rect.offset(0 - x, 0 - y);
		rect1.offset(0 - x1, 0 - y1);

		intersectant = rect.intersect(rect1);
		if (intersectant)
		{
			*this = rect;
			rect1 = rect;
		}

		offset(x, y);
		rect1.offset(x1, y1);

		return intersectant;
	}


	//两矩阵区相差
	int Rect::subIntersect(Rect rcDst[4], const Rect& src2)
	{
		Rect interRect ;

		interRect = *this;
		if (!interRect.intersect(src2))
			return 0;

		rcDst[0].left   = left   ;
		rcDst[0].top    = top    ;
		rcDst[0].right  = interRect.right  ;
		rcDst[0].bottom = interRect.top    ;

		rcDst[1].left   = left   ;
		rcDst[1].top    = interRect.top    ;
		rcDst[1].right  = interRect.left   ;
		rcDst[1].bottom = bottom ;

		rcDst[2].left   = interRect.left   ;
		rcDst[2].top    = interRect.bottom ;
		rcDst[2].right  = right  ;
		rcDst[2].bottom = bottom ;

		rcDst[3].left   = interRect.right  ;
		rcDst[3].top    = top    ;
		rcDst[3].right  = right  ;
		rcDst[3].bottom = interRect.bottom ;

		return 4 ;
	}


	bool Rect::subMargin(const Rect& margin)
	{
		left   += margin.left;
		top	   += margin.top;
		right  -= margin.right;
		bottom -= margin.bottom;

		return isEmpty();
	}

	//合并矩形
	bool Rect::unionRect(const Rect*  rect1)
	{

		bool empty, empty1;

		if (rect1 == NULL)
			return false;

		empty = isEmpty();
		empty1 = rect1->isEmpty();

		if (empty && empty1)
		{
			return false ;
		}
		else if (empty)
		{
			*this = *rect1 ;
		}
		else if (empty1)
		{
		}
		else
		{
			left	= min(left, rect1->left);
			top	    = min(top,  rect1->top);
			right   = max(right, rect1->right);
			bottom  = max(bottom, rect1->bottom);
			
			return true;
		}

		return true;
	}


	Rect& Rect::operator =(const Rect &rect)
	{
		left = rect.left;
		top = rect.top;
		right = rect.right;
		bottom = rect.bottom;

		return *this;
	}


	bool Rect::operator ==(const Rect &rect1) const 
	{
		if ((left != rect1.left) || (top != rect1.top) || (right != rect1.right) || (bottom != rect1.bottom))
			return false;

		return true;
    }  


	void Rect::inflate(int dx, int dy)
	{
		left -= dx;
		top -= dy;
		right += dx;
		bottom += dy;
	}


}