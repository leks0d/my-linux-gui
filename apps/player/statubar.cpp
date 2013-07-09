#include "mango.h"


namespace mango
{
	StatuBar::StatuBar(){

	}

	StatuBar::StatuBar(int id, const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{

	}

	
	int StatuBar::onPaint(Canvas& canvas)
	{
		int imageId = 0;
		Rect rect;
		Brush brush(RGB(0, 0, 0));

		rect.setEx(0, 0, mRect.right - mRect.left, mRect.bottom - mRect.top); 
		canvas.fillRect(rect, brush);
		return 0;
	}
	int StatuBar::onCreate(){
		Rect rect;
		rect.setEx(150, 0, 20, 22);
		
		mVolumeButton = new  Button(12, NULL, this, &rect, 0);
		mVolumeButton->setNormalImageId(0x775);
		mVolumeButton->onCreate();
	}

	int StatuBar::onDestroy()
	{	
		if(mVolumeButton)
			mVolumeButton->onDestroy();
		safeFree(mVolumeButton);		
	}

	int StatuBar::onNotify(View* fromView, int code, void* parameter){return 0;}
	int StatuBar::onKeyDown(int keyCode, int flag){return 0;}
	int StatuBar::onKeyUp(int keyCode, int flag){return 0;}
	int StatuBar::onCommand(int id, int code, View* fromView){return 0;}
};


