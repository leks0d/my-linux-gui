
#include <mango.h>


namespace mango
{
	Canvas::Canvas(void)
	{
		mCharExtra = 0;
	}


	Canvas::~Canvas(void)
	{

	}

	void Canvas::viewToCanvas(Point& pt)
	{
		pt.x += mViewPos.x; 
		pt.y += mViewPos.y;
	}

	void Canvas::viewToCanvas(int& x, int& y)
	{
		x += mViewPos.x; 
		y += mViewPos.y;
	}

	void Canvas::viewToCanvas(Rect& rect)
	{
		rect.left   += mViewPos.x; 
		rect.right  += mViewPos.x; 
		rect.top	+= mViewPos.y;
		rect.bottom += mViewPos.y;
	}

	//获得DC 的剪切矩形
	bool Canvas::getClipRect (Rect& clipRect)
	{
		bool success = false ;

		do
		{
			//位图区
			clipRect.setEx(0, 0, mBitmap->getWidth(), mBitmap->getHeight());

			//开始区域裁剪
			if (mInitRegion){
				if (!clipRect.intersect(mInitRegion->getRect()))
					break;
			}

			//用户设置区域裁剪
			if (mRegion) {
				if (!clipRect.intersect(mRegion->getRect()))
					break ;
			}

			//窗口裁剪
			//if (pDcObj->m_pWnd)
			//{
			//	rect = rcClip ;
			//	if (!IntersectRect (&rcClip, &rect, &(pDcObj->m_pWnd->m_Rect)))
			//		break ;
			//}

			success = true;

		}while (0);

		return success;
	}


	Bitmap* Canvas::select(Bitmap* bitmap)
	{
		Bitmap* b = mBitmap;
		mBitmap = bitmap;
		return b; 
	}


	Pen* Canvas::select(Pen* pen)
	{
		Pen* p = mPen;
		mPen = pen;
		return p; 
	}


	Brush*  Canvas::select(Brush* brush)
	{
		Brush* b = mBrush;
		mBrush = brush;
		return b; 
	}


	Font* Canvas::select(Font* font)
	{
		Font* f = mFont;
		mFont = font;
		return f; 
	}


	void Canvas::renew(class View* view)
	{
		mInitRegion = NULL;
		mRegion = NULL;

		mBitmap = &gSessionLocal.mStockGraphic.mBitmap;
		mPen = &gSessionLocal.mStockGraphic.mPen;
		mBrush = &gSessionLocal.mStockGraphic.mBrush;
		mFont = &gSessionLocal.mStockGraphic.mFont;

		mPenPos.set(0, 0);  //当前笔的位置
		
		if (view)
			mViewPos.set(view->mRect.left, view->mRect.top);  //当前View在画布中的位置
		else
			mViewPos.set(0, 0);

		mCharExtra = 0;

		mTextColor = RGB(0, 0, 0);
		mBkColor = ARGB(255,127, 127, 127);

		mRop2 = 0;		
		mBkMode = TRANSPARENT; //TRANSPARENT;		
		mAlpha = 0;		//0 -- 全透明, 255 -- 不透
	}


	bool Canvas::drawImageResource(int id, int x, int y, bool alpha)
	{
		bool success = false;
		RESOURCEIMAGE_HEADER imageHeader;

		Rect	rcBmp, rcClip ;
		void*	resourceBuffer = NULL;

		do
		{
			if (id <= 0)
				break ;

			resourceBuffer = malloc(RESOURCE_MAX_BYTES);
			if (resourceBuffer == NULL)
				break;

			if (gSessionLocal.mResource.loatBitmap(id, &imageHeader, resourceBuffer, RESOURCE_MAX_BYTES) <= 0)
				break;


			viewToCanvas(x, y);
			getClipRect(rcClip);

			
			rcBmp.setEx(x, y, imageHeader.m_iWidth, imageHeader.m_iHeight);
			if (!rcClip.intersect(rcBmp))
				break ;

			bitBlt_32(mBitmap->getBits(), mBitmap->getWidth(), rcClip.left,  rcClip.top,  rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, \
				resourceBuffer, imageHeader.m_iWidth, rcClip.left - x, rcClip.top -y, \
				alpha ? SRCALPHADSTALPHA : SRCCOPY, 255, NULL) ;

			success = true;

		} while (0) ;

		safeFree(resourceBuffer);
		return success;
	}

	bool Canvas::drawImageResource(int id, int x, int y,int width, bool alpha){
		bool success = false;
		RESOURCEIMAGE_HEADER imageHeader;

		Rect	rcBmp, rcClip ;
		void*	resourceBuffer = NULL;

		do
		{
			if (id <= 0)
				break ;

			resourceBuffer = malloc(RESOURCE_MAX_BYTES);
			if (resourceBuffer == NULL)
				break;

			if (gSessionLocal.mResource.loatBitmap(id, &imageHeader, resourceBuffer, RESOURCE_MAX_BYTES) <= 0)
				break;

			if(width<imageHeader.m_iWidth)
				imageHeader.m_iWidth = width;
			
			viewToCanvas(x, y);
			getClipRect(rcClip);

			rcBmp.setEx(x, y, imageHeader.m_iWidth, imageHeader.m_iHeight);
			if (!rcClip.intersect(rcBmp))
				break ;

			bitBlt_32(mBitmap->getBits(), mBitmap->getWidth(), rcClip.left,  rcClip.top,  rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, \
				resourceBuffer, imageHeader.m_iWidth, rcClip.left - x, rcClip.top -y, \
				alpha ? SRCALPHADSTALPHA : SRCCOPY, 255, NULL) ;

			success = true;

		} while (0) ;

		safeFree(resourceBuffer);
		return success;

	}


	void Canvas::swapScreenFrontBuffer()
	{
		gSessionLocal.swapScreenFrontBuffer();
	}

}












