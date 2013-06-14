

#include <mango.h>


namespace mango
{
	bool Canvas::drawLine32(Rect& clipRect, int xStart, int yStart, int xEnd, int yEnd)
	{

		int		x,   y ;
		int		cx, cy ;
		int		  bitmapWidth ;
		COLORREF *pixel, *bitmapBits ;
		COLORREF  clrPen ;

		bool  hasMask = false;
		Rect maskRect;

		int   maskByteStartPos, iMaskBit ;
		BYTE  *pbyMask ;

		if (mInitRegion && mInitRegion->getMask()) {
			hasMask = true;
			maskRect = mInitRegion->getRect();
		}

		cx = xEnd - xStart ;
		cy = yEnd - yStart ;

		bitmapBits   = (COLORREF *)(mBitmap->getBits());
		bitmapWidth	 = mBitmap->getWidth();
		clrPen   = mPen->getColor();

		if (cy == 0) { //横线
			if (yStart < clipRect.top || yStart >= clipRect.bottom)
				return true;

			if (xEnd < xStart) { //交互坐标
				x		= xStart;
				xStart	= xEnd;
				xEnd	= x;
			}

			xStart = max(xStart, clipRect.left);
			xEnd   = min(xEnd,   clipRect.right);

			pixel  = bitmapBits + bitmapWidth * yStart ;
			pixel += xStart ; 

			if (hasMask) {
				maskByteStartPos  = (yStart - maskRect.top) * mInitRegion->getMaskWidthBytes();
				maskByteStartPos += ((xStart - maskRect.left) >> 3);
				iMaskBit	= (xStart - maskRect.left) & 0x0007;	
				pbyMask     = (BYTE *)mInitRegion->getMask() + maskByteStartPos;
			}

			for (x = xStart ; x < xEnd ; x++)
			{
				if (hasMask)
				{
					if ((*pbyMask) & (0x80 >> iMaskBit))
						*pixel = clrPen ;

					iMaskBit = (iMaskBit + 1) & 0x07 ;
					if (iMaskBit == 0)
						*pbyMask++ ;

					pixel++ ;
				}
				else
					*pixel++ = clrPen ;
			}
		}
		else if (cx == 0)
		{
			//竖线
			if (xStart < clipRect.left || xStart >= clipRect.right)
				return TRUE ;

			if (yEnd < yStart)
			{
				//交互坐标
				y		= yStart ;
				yStart	= yEnd ;
				yEnd	= y ;
			}

			yStart = max (yStart, clipRect.top) ;
			yEnd   = min (yEnd,   clipRect.bottom) ;

			pixel  = bitmapBits + bitmapWidth * yStart ;
			pixel += xStart ; 


			if (hasMask)
			{
				maskByteStartPos   = (yStart - maskRect.top) * mInitRegion->getMaskWidthBytes();
				maskByteStartPos  += ((xStart - maskRect.left) >> 3) ;
				iMaskBit	= (xStart - maskRect.left) & 0x0007 ;	
				pbyMask     = (BYTE *)mInitRegion->getMask() + maskByteStartPos ;
			}

			for (y = yStart ; y < yEnd ; y++)
			{
				if (hasMask)
				{
					if ((*pbyMask) & (0x80 >> iMaskBit))
						*pixel = clrPen ;

					pbyMask += mInitRegion->getMaskWidthBytes();
				}
				else
					*pixel = clrPen ;

				pixel += bitmapWidth ; 
			}
		}
		else if (abs (cx) >= abs (cy))
		{
			//斜线
			if (xEnd < xStart)
			{
				//交互坐标
				x		= xStart ;
				xStart	= xEnd ;
				xEnd	= x ;

				y		= yStart ;
				yStart	= yEnd ;
				yEnd	= y ;

				cx	= 0 - cx ;
				cy	= 0 - cy ;
			}

			for (x = xStart ; x < xEnd ; x++)
			{
				if (x < clipRect.left || x >= clipRect.right)
					continue ;

				y = (x - xStart) * cy / cx + yStart ;

				if (y < clipRect.top || y >= clipRect.bottom)
					continue ;

				pixel  = bitmapBits + bitmapWidth * y ;
				pixel += x ;

				if (hasMask)
				{
					maskByteStartPos   = (y - maskRect.top) * mInitRegion->getMaskWidthBytes();
					maskByteStartPos  += ((x - maskRect.left) >> 3) ;
					iMaskBit	= (x - maskRect.left) & 0x0007 ;	
					pbyMask     = (BYTE *)mInitRegion->getMask() + maskByteStartPos ;

					if ((*pbyMask) & (0x80 >> iMaskBit))
						*pixel = clrPen ;
				}
				else
					*pixel = clrPen ;
			}
		}
		else 
		{
			//斜线
			if (yEnd < yStart)
			{
				//交互坐标
				x		= xStart ;
				xStart	= xEnd ;
				xEnd	= x ;

				y		= yStart ;
				yStart	= yEnd ;
				yEnd	= y ;

				cx	= 0 - cx ;
				cy	= 0 - cy ;
			}

			for (y = yStart ; y < yEnd ; y++)
			{
				if (y < clipRect.top || y >= clipRect.bottom)
					continue ;


				x = (y - yStart) * cx / cy + xStart ;
				if (x < clipRect.left || x >= clipRect.right)
					continue ;

				pixel  = bitmapBits + bitmapWidth * y ;
				pixel += x ;

				if (hasMask)
				{
					maskByteStartPos   = (y - maskRect.top) * mInitRegion->getMaskWidthBytes();
					maskByteStartPos  += ((x - maskRect.left) >> 3) ;
					iMaskBit	= (x - maskRect.left) & 0x0007 ;	
					pbyMask     = (BYTE *)mInitRegion->getMask() + maskByteStartPos ;

					if ((*pbyMask) & (0x80 >> iMaskBit))
						*pixel = clrPen ;
				}
				else
					*pixel = clrPen ;
			}
		}

		return true ;
	}


	bool Canvas::drawLine(Rect& clipRect, int xStart, int yStart, int xEnd, int yEnd)
	{
		bool succeed = false;

		switch (mBitmap->getBitsPixel())
		{
		case 32:
			succeed = drawLine32(clipRect, xStart, yStart, xEnd, yEnd) ;
			break ;

		case 16:
			break ;
		}

		return succeed ;
	}


	bool Canvas::moveTo(int x, int y, Point* point)
	{
		viewToCanvas(x, y);

		if (point)
			*point = mPenPos;

		mPenPos.x = x;
		mPenPos.y = y;

		return true;
	}


	bool Canvas::lineTo(int x,  int y)
	{
		Rect clipRect ;

		if (!getClipRect(clipRect))
			return false;

		viewToCanvas(x, y);


		drawLine(clipRect, mPenPos.x, mPenPos.y, x, y);

		mPenPos.x = x ;
		mPenPos.y = y ;

		return true;
	}


	bool Canvas::polyline(const Point *point, int count)
	{
		Rect	clipRect;
		Point	start, end;

		if (point == NULL)
			return false;

		if (!getClipRect(clipRect))
			return false;

		for (; count > 1; count--)	{
			start = *point;
			end   = *(point + 1);

			viewToCanvas(start);
			viewToCanvas(end);

			drawLine(clipRect, start.x, start.y, end.x, end.y);
			point++;
		}

		return true;
	}


	bool Canvas::polylineTo(const Point *point, int count)
	{
		Rect	clipRect;
		Point	start, end;

		if (point == NULL)
			return false;

		if (!getClipRect (clipRect))
			return false;

		start = mPenPos;
		for (; count > 0; count--) {
			end = *point;
			viewToCanvas(end);
			drawLine (clipRect, start.x, start.y, end.x, end.y);

			point++;
			mPenPos = end;
			start   = end;
		}

		return true;
	}

}