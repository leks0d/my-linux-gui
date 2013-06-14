
#include <mango.h>


namespace mango
{

	bool Canvas::fillRectColor32(Rect& clipRect, Rect& fillRect, COLORREF clr)
	{
		COLORREF *pixel;
		int		 x, y;
		Rect	 rect;

		bool  hasMask = false ;
		int   maskByteStartPos, maskBitPos ;
		BYTE  *maskByte ;

		rect = fillRect;
		if (!rect.intersect(clipRect))
			return false;

		if (mInitRegion && mInitRegion->getMask()) {
			hasMask = true;
		}

		pixel  = (COLORREF *)(mBitmap->getBits());
		pixel  = pixel + mBitmap->getWidth() * rect.top;
		pixel += rect.left; 

		for (y = rect.top; y < rect.bottom; y++) {
			if (hasMask) {
				maskByteStartPos  = (y - mInitRegion->getRect().top) * mInitRegion->getMaskWidthBytes();
				maskByteStartPos += ((rect.left - mInitRegion->getRect().left) >> 3) ;
				maskBitPos	= (rect.left - mInitRegion->getRect().left) & 0x0007 ;	
				maskByte    = (BYTE *)mInitRegion->getMask() + maskByteStartPos;
			}

			for (x = rect.left; x < rect.right; x++) {
				if (hasMask) {
					if ((*maskByte) & (0x80 >> maskBitPos))
						*pixel = clr;

					maskBitPos = (maskBitPos + 1) & 0x07 ;
					if (maskBitPos == 0)
						*maskByte++;

				} else {
					*pixel = clr;
				}

				pixel++;
			}

			pixel += mBitmap->getWidth() - (rect.right - rect.left);
		}

		return true;
	}

	bool Canvas::fillRectColor(Rect& clipRect, Rect& fillRect, COLORREF clr)
	{
		switch (mBitmap->getBitsPixel()) {
		case 32:
			return fillRectColor32(clipRect, fillRect, clr);
		}

		return false;
	}


	bool Canvas::fillRectAlpha(Rect& clipRect, Rect& fillRect, unsigned char alpha)
	{
		COLORREF		*pixel ;
		int				x,  y ;
		Rect			rect ;
		DWORD			perchAlpha = ((DWORD)alpha) << 24 ;

		BOOL  hasMask = false;
		int   maskByteStartPos, maskBitPos;
		BYTE  *maskByte;
		Region* region;

		if (mBitmap && (mBitmap->getBitsPixel() != 32))
			return false ;

		rect = fillRect;
		if (!rect.intersect(clipRect))
			return false;

		if (mInitRegion && mInitRegion->getMask()) {
			hasMask = true;
			region = mInitRegion;
		}

		pixel  = (COLORREF *)(mBitmap->getBits());
		pixel  = pixel + mBitmap->getWidth() * rect.top;
		pixel += rect.left; 

		for (y = rect.top; y < rect.bottom; y++) {
			if (hasMask) {
				maskByteStartPos  = (y - region->getRect().top) * region->getMaskWidthBytes();
				maskByteStartPos += ((rect.left - region->getRect().left) >> 3);
				maskBitPos = (rect.left - region->getRect().left) & 0x0007;	
				maskByte   = (BYTE *)region->getMask() + maskByteStartPos;
			}

			for (x = rect.left; x < rect.right; x++) {
				if (hasMask) {
					if ((*maskByte) & (0x80 >> maskBitPos))
						*pixel = (*pixel & 0x00ffffff) | perchAlpha;

					maskBitPos = (maskBitPos + 1) & 0x07;
					if (maskBitPos == 0)
						*maskByte++;

				} else {
					*pixel = (*pixel & 0x00ffffff) | perchAlpha;
				}

				pixel++;
			}

			pixel += mBitmap->getWidth() - (rect.right - rect.left);
		}

		return true;
	}


	bool Canvas::fillRectAlpha (Rect& fillRect, unsigned char alpha)
	{
		Rect clipRect, fillRectInCanvas;

		if (!getClipRect(clipRect))
			return false;

		fillRectInCanvas = fillRect;
		viewToCanvas(fillRectInCanvas);

		return fillRectAlpha(clipRect, fillRectInCanvas, alpha);
	}


	bool Canvas::fillRect(Rect &rect, Brush &brush)
	{
		Rect clipRect, fillRect;

		if (!getClipRect(clipRect))
			return false;

		fillRect = rect ;
		viewToCanvas(fillRect);

		return fillRectColor(clipRect, fillRect, brush.getColor());
	}

	
	bool Canvas::frameRect(Rect &rect, Brush &brush)
	{
		Rect clipRect, fillRect, rectInCanvas ;

		if (!getClipRect(clipRect))
			return false;

		rectInCanvas = rect ;
		viewToCanvas(rectInCanvas);

		fillRect = rect ;
		fillRect.right  = min (fillRect.left + 1, rect.right); 
		if (fillRectColor(clipRect, fillRect, brush.getColor()))
			return false;

		fillRect		  = rect ;
		fillRect.bottom = min (fillRect.top + 1, rect.bottom); 
		if (fillRectColor(clipRect, fillRect, brush.getColor()))
			return false;

		fillRect		 = rect ;
		fillRect.left  = max (fillRect.right - 1, rect.left); 
		if (fillRectColor(clipRect, fillRect, brush.getColor()))
			return false;

		fillRect		 = rect ;
		fillRect.top   = max (fillRect.bottom - 1, rect.top); 
		if (fillRectColor(clipRect, fillRect, brush.getColor()))
			return false;

		return true;
	}


	bool Canvas::invertRect(Rect &invertRect)
	{
		Rect		rect, clipRect ;
		COLORREF	*pixel ;
		BOOL		bSuccess = false ;
		int			x, y ;

		if (!getClipRect(clipRect))
			return false;

		rect = invertRect;
		viewToCanvas(rect);

		if (!rect.intersect(clipRect))
			return false;

		pixel  = (COLORREF *)(mBitmap->getBits()) ;
		pixel  = pixel + mBitmap->getWidth() * rect.top ;
		pixel += rect.left ; 

		for (y = rect.top ; y < rect.bottom ; y++) {
			for (x = rect.left ; x < rect.right ; x++) {
				*pixel = (~(*pixel)) ;
				pixel++ ;
			}

			pixel += mBitmap->getWidth() - (rect.right - rect.left) ;
		}

		return true;
	}


	bool Canvas::rectangle (int left, int top, int right, int bottom)
	{
		Rect clipRect, fillRect;

		if (!getClipRect(clipRect))
			return false;

		drawLine(clipRect, left,  top,    left,  bottom) ;
		drawLine(clipRect, left,  top,    right, top) ;
		drawLine(clipRect, right, top,	  right, bottom) ;
		drawLine(clipRect, left,  bottom, right, bottom) ;

		if (mBrush) {
			fillRect.left	= min(left + 1, right);
			fillRect.top	= min(top  + 1, bottom);
			fillRect.right	= max(left, right  - 1);
			fillRect.bottom	= max(top,  bottom - 1);
			fillRectColor(clipRect, fillRect, mBrush->getColor());
		}

		return true ;
	}


	bool Canvas::rectangle(Rect &rect)
	{
		return rectangle (rect.left, rect.top, rect.right, rect.bottom);
	}
}

