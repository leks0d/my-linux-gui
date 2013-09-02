
#include "mango.h"

namespace mango
{
	StaticView::StaticView(void)
	{
		mTitlePaintStartX = 0;
		mTitlePaintExtent.set(0, 0);
	}


	StaticView::StaticView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		:View(title, parent, rect, style, show)
	{
		updateTextPaintExtent(title);
	}


	StaticView::~StaticView(void)
	{

	}


	int StaticView::setTitle(const TCHAR* title)
	{
		updateTextPaintExtent(title);

		return View::setTitle(title);
	}

	int StaticView::setTextString(const char* string){
		WCHAR  wcBuf[81];
		int    i;
		if(string == NULL){
			setTitle(NULL);
			return 0;
		}
		memset(wcBuf,0,sizeof(wcBuf));
		if(Charset::isTextUtf8(string)){
			i = Charset::multiByteToWideChar(CP_UTF8, string, strlen(string), wcBuf, 80);
		}else{
			i = Charset::multiByteToWideChar(CP_ACP, string, strlen(string), wcBuf, 80);
		}
		setTitle(wcBuf);
		return 0;
	}

	int StaticView::setTextResoure(int res){
		WCHAR	wStrBuf[256 + 1];
		if (gSessionLocal.mResource.loadString(res, wStrBuf, 256 + 1) > 0){
			setTitle(wStrBuf);
		}
	}
	int StaticView::setTextSize(int size){
		mTextSize = size;
		invalidateRect();
	}
	int StaticView::setTextColor(int color){
		mColor = color;
		invalidateRect();
	}
	int StaticView::setTextBkRes(int res){
		mBkRes = res;
		invalidateRect();
	}

	int StaticView::onPaint(Canvas& canvas)
	{
		Rect rect;
		TCHAR *title;
		getClientRect(rect);

		title = getTitle();
		if (title != NULL)
		{
			canvas.setTextColor(mColor);
			canvas.setTextSize(mTextSize);
			canvas.drawImageResource(mBkRes,0,0);
			if (mTitlePaintExtent.cx < rect.right - rect.left)
				canvas.drawText(title, -1, rect, DT_SINGLELINE | DT_NOPREFIX);
			else
				canvas.drawText(title, -1, mTitlePaintStartX,  (rect.bottom - rect.top - mTitlePaintExtent.cy) / 2);
		}

		return 0;
	}


	int StaticView::onTouchDown(int x, int y, int flag)
	{
		Rect rect;

		getClientRect(rect);
		if (mTitlePaintExtent.cx < rect.right - rect.left)
			return 0;

		setCapture(this);
		mTouchPrevPosition.set(x, y);

		return 0;
	}


	int StaticView::onTouchMove(int x, int y, int flag)
	{
		if (getCapture () != this)
			return 0 ;

		cartoonDrag(x - mTouchPrevPosition.x);
		mTouchPrevPosition.set(x, y);
	
		return 0 ;
	}


	int StaticView::onTouchUp(int x, int y, int flag)
	{
		if (getCapture() != this)
			return 0 ;

		releaseCapture () ;
		return 0 ;
	}


	int StaticView::getTitlePaintStartX()
	{
		return mTitlePaintStartX;
	}


	int StaticView::setTitlePaintStartX(int x)
	{
		Rect  rect;
		getClientRect(rect);

		if (x >= 0)
			mTitlePaintStartX = 0;
		else if (mTitlePaintExtent.cx + x < rect.right - rect.left)
			mTitlePaintStartX = rect.right - rect.left - mTitlePaintExtent.cx;
		else
			mTitlePaintStartX = 0;

		return mTitlePaintStartX;
	}



	void StaticView::updateTextPaintExtent(const TCHAR* title)
	{
		Canvas* canvas;

		if (title)
		{
			
			canvas = getCanvas();
			canvas->setTextColor(mColor);
			canvas->setTextSize(mTextSize);
			canvas->getTextExtentPoint(title, -1, mTitlePaintExtent);
			releaseCanvas();
		}
		else
		{
			mTitlePaintExtent.set(0, 0);
		}
		mTitlePaintStartX = 0;
	}


	int StaticView::cartoonDrag(int distance)
	{
		int	 dx = 0;
		Rect rect;
		getClientRect(rect);

		if (distance < 0 && mTitlePaintExtent.cx + mTitlePaintStartX + distance < (rect.right - rect.left))
			dx = (rect.right - rect.left) - mTitlePaintStartX - mTitlePaintExtent.cx;
		else if (distance > 0 && distance + mTitlePaintStartX > 0)
			dx = 0 - mTitlePaintStartX;
		else
			dx = distance;

		if (dx)
		{
			mTitlePaintStartX += dx;
			cartoonDisplay();
		}
		return 0;
	}


	void StaticView::cartoonDisplay(void)
	{
		Canvas* canvas;

		canvas = getCanvas();

		if ((mStyle & VS_TRANSPARENT) && getParent())
		{
			Region  region(mRect);

			canvas->renew(getParent());
			canvas->select(&region);
			getParent()->onPaint(*canvas);
			canvas->renew(this);
			canvas->select((Region*)0);
		}
	
		paint(*canvas);

		canvas->swapScreenFrontBuffer();
		releaseCanvas();
	}


	void StaticView::paint(Canvas& canvas)
	{
		Rect rect;
		TCHAR *title;
		getClientRect(rect);

		title = getTitle();
		if (title)
		{
			canvas.setTextColor(mColor);
			canvas.setTextSize(mTextSize);
			canvas.drawImageResource(mBkRes,0,0);
			if (mTitlePaintExtent.cx < rect.right - rect.left)
				canvas.drawText(title, -1, rect, DT_SINGLELINE | DT_NOPREFIX);
			else
				canvas.drawText(title, -1, mTitlePaintStartX,  (rect.bottom - rect.top - mTitlePaintExtent.cy) / 2);
		}
	}


};

