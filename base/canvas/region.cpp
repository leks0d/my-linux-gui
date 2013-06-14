
#include "mango.h"

namespace mango
{
	Region::Region(void)
	{

	}


	Region::Region(Rect &rect)
	{
		int x;
		x = rect.left;

		init(rect.left, rect.top, rect.right, rect.bottom);
	}


	Region::Region(int left, int top, int right, int bottom)
	{
		init(left, top, right, bottom);
	}


	Region::~Region(void)
	{

	}

	void Region::set(int left, int top, int right, int bottom)
	{
		init(left, top, right, bottom);
	}


	bool Region::reset(Rect &rect)
	{
		int	maskWidthBytes ;
		int	totalBytes ;

		if (rect.isEmpty())
			return false;

		maskWidthBytes = (((rect.right - rect.left + 7) >> 3) + 3) & (~0x3);
		totalBytes     = (rect.bottom - rect.top) * maskWidthBytes;

		if (totalBytes > mMaskBytes)
			return false;

		mRect = rect;
		mMaskWidthBytes = maskWidthBytes;

		validRect(rect);

		return true;
	}


	bool Region::ptIn(int x, int y)
	{
		int iPos ;
		int iBit ;

		if (!mRect.ptIn(x, y))
			return false;

		iPos  = mMaskWidthBytes * (y - mRect.top);
		iPos += ((x - mRect.left) >> 3);
		iBit  = ((x - mRect.left) % 8);
		if (mMask[iPos] & (0x0080 >> iBit))
			return true ;
		else
			return false ;
	}


	bool Region::offset(int dx, int dy)
	{
		mRect.offset(dx, dy);
		return true;
	}


	bool Region::validRect(Rect &rect)
	{
		int   y ;
		int   xByte, cxBytes ;
		int   iByte ;
		BYTE *pbyMask ;
		BYTE  byLeftMask, byRightMask ;
		Rect  rectInter = mRect;

		if (!rectInter.intersect(rect))
			return true ;

		cxBytes = ((rectInter.right - 1 - mRect.left) >> 3) - ((rectInter.left  - mRect.left) >> 3);

		cxBytes--;
		byLeftMask  = 0xff >> ((rectInter.left  - mRect.left) & 0x07);
		byRightMask = 0xff << (7 - ((rectInter.right - 1 - mRect.left) & 0x07));
		if (cxBytes < 0)
			byLeftMask |= byRightMask ;

		for (y = rectInter.top ; y < rectInter.bottom ; y++)
		{
			iByte   = (y - mRect.top) * mMaskWidthBytes ;
			iByte  += ((rectInter.left - mRect.left) >> 3) ;

			pbyMask = mMask + iByte ;

			*pbyMask |= byLeftMask ;
			pbyMask++ ;

			if (cxBytes < 0) //可防止超界操作
				continue ;

			for (xByte = 0 ; xByte < cxBytes ; xByte++)
				*pbyMask++ = 0xff ;

			*pbyMask |= byRightMask ;
		}

		return true;	
	}

	bool Region::invalidRect(Rect &rect)
	{
		int   y;
		int   xByte, cxBytes;
		int   iByte;
		BYTE *pbyMask;
		BYTE  byLeftMask, byRightMask;
		Rect  rectInter = mRect;

		if (!rectInter.intersect(rect))
			return true;

		cxBytes = ((rectInter.right - 1 - mRect.left) >> 3) - ((rectInter.left - mRect.left) >> 3);
		cxBytes--;
		byLeftMask  = 0xff << (8 - ((rectInter.left  - mRect.left) % 8)) ;
		byRightMask = 0xff >> (1 + ((rectInter.right - 1 - mRect.left) % 8)) ;

		if (cxBytes < 0)
			byLeftMask |= byRightMask ;

		for (y = rectInter.top; y < rectInter.bottom; y++)
		{
			iByte   = (y - mRect.top) * mMaskWidthBytes;
			iByte  += ((rectInter.left - mRect.left) >> 3);

			pbyMask   = mMask + iByte;
			*pbyMask &= byLeftMask;
			pbyMask++ ;

			if (cxBytes < 0) //可防止超界操作
				continue ;

			for (xByte = 0 ; xByte < cxBytes ; xByte++)
				*pbyMask++ = 0 ;

			*pbyMask &= byRightMask ;
		}
		return true;
	}


	void Region::init(int left, int top, int right, int bottom)
	{
		Rect		rect ;

		rect.set(left, top, right, bottom);
		if (rect.isEmpty())
			rect.set(0, 0, 0, 0) ;

		//int			iMaskBytes ;
		//iMaskBytes  = (((right - left) + 3) / 4) * (bottom - top);
		//pRgnObj->m_pbyMask = (PBYTE)lcMalloc (iMaskBytes) ;
		//if (pRgnObj->m_pbyMask == NULL)
		//{
		//	lcFree (pRgnObj) ; 
		//	return NULL ;
		//}

		mRect = rect;
		mMaskWidthBytes = (((((right - left) + 7) >> 3) + 3) / 4);
		mMask = NULL; 
		mMaskBytes = 0; 
	}
}

