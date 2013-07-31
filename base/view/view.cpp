#include "mango.h"

namespace mango
{
	View::View(void)
	{
		View(NULL, NULL, 0, SW_HIDE);
	}



	View::View(const TCHAR* title, View* parent, Rect* rect, int style, int show)
	{
		mMessageQueue = &gMessageQueue;
		mTitle = NULL;

		mParent = parent;
		mStyle  = style;
		mShowState = show;

		if (rect)
			mRect = *rect;
		else if (mParent)
			mRect = mParent->mRect;
		else 
			gSession.getScreenRect(mRect);

		if (mParent)
			mRect.offset(mParent->mRect.left, mParent->mRect.top);

		setTitle(title);

		gSession.mViewZAxis.add(this);
	}



	View::~View(void)
	{
		gSession.mViewZAxis.del(this);
	}


	int View::onCreate()
	{
		invalidateRect(&mRect);
		return 0;
	}


	int View::onDestroy()
	{
		return -1;
	}


	int View::onActivate()
	{
		return -1;
	}


	int View::onInactivate()
	{
		return -1;
	}


	int View::onPaint(Canvas& canvas)
	{
		Rect rect(0, 0, 200, 200);
		Brush brush(RGB(255, 0, 0));

		canvas.fillRect(rect, brush);
		canvas.drawText("View", 3, 100, 100);

		return -1;
	}


	int View::setTitle(const TCHAR* newTitle)
	{

		int				titleLen, newTitleLen;
		LPTSTR			lpNewTitle;
		
		if (!newTitle) {
			safeFree(mTitle);
			return -1;
		}

		

		newTitleLen = String::lstrlen(newTitle);

		if (mTitle)
			titleLen = String::lstrlen(mTitle);
		else
			titleLen = 0 ;

		if (titleLen < newTitleLen)
		{
			safeFree(mTitle);
			mTitle = (LPTSTR)malloc((1 + newTitleLen) * sizeof (TCHAR));
			if (mTitle == NULL)
				return -1 ;

			String::copy (mTitle, newTitle);
		}
		else if (mTitle)
			String::copy (mTitle, newTitle);

		invalidateRect() ;

		name = new char[30];
		Charset::wideCharToMultiByte(CP_UTF8, newTitle, String::lstrlen(newTitle), name, MAX_PATH * 3);
		name[29] = '\0';

		log_i("View::setTitle name=%s",name);
		
		return newTitleLen;
	}


	TCHAR* View::getTitle()
	{
		return mTitle;
	}

	void View::getRect(Rect &rect)
	{
		rect = mRect;
	}


	int View::onKeyDown(int keyCode, int flag)
	{
		return 0;

	}


	int View::onKeyUp(int keyCode, int flag)
	{
		return 0;
	}


	int View::onTouchDown(int x, int y, int flag)
	{
		return 0;
	}

	int View::onTouchMove(int x, int y, int flag)
	{
		return 0;
	}

	int View::onTouchUp(int x, int y, int flag)
	{
		return 0;
	}


	int View::onNotify(View* fromView, int code, void* parameter)
	{
		return 0;
	}

	
	int View::onCommand(int id, int code, View* fromView)
	{
		return 0;
	}


	void View::invalidateRect(const Rect* rect)
	{
		gSession.mViewZAxis.invalidateViewRect(this, rect);
	}


	void  View::getClientRect(Rect& rect)
	{
		rect.setEx(0, 0, mRect.right - mRect.left, mRect.bottom - mRect.top);
	}



	View* View::setCapture(View* view)
	{
		if (view == NULL)
			view = this;

		return gSession.mViewZAxis.setCapture(view);
	}


	View* View::releaseCapture()
	{
		return gSession.mViewZAxis.setCapture(NULL);
	}


	View* View::getCapture()
	{
		return  gSession.mViewZAxis.getCapture();
	}


	View* View::setFocus(View* view)
	{
		if (view == NULL)
			view = this;

		return gSession.mViewZAxis.setFocus(view);
	}


	View* View::getFocus(void)
	{
		return  gSession.mViewZAxis.getFocus();
	}


	
	View*  View::getParent()
	{
		return mParent;
	}


	Canvas* View::getCanvas()
	{
		gSessionLocal.mCanvasMutex.lock();
		gSessionLocal.mStockGraphic.mCanvas.renew(this);
		return &gSessionLocal.mStockGraphic.mCanvas;
	}


	void View::releaseCanvas()
	{
		gSessionLocal.mCanvasMutex.unlock();
		return;
	}



	int View::sendMessage(class View* view, unsigned int id, unsigned int parameter1, unsigned int paramter2)
	{
		return mMessageQueue->send(view, id, parameter1, paramter2);
	}


	int View::postMessage(class View* view, unsigned int id, unsigned int parameter1, unsigned int paramter2)
	{
		return mMessageQueue->post(view, id, parameter1, paramter2);
	}

	int UseEventInterface::onKeyDispatch(int keyCode,int action, int flag){
		return 0;
	}
	int UseEventInterface::onTouchDispatch(int x,int y, int action){
		return 0;
	}
}



