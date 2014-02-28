#include "player.h"


namespace mango
{
	
	enum
	{
		ADAPTER_PLAYING = 0xf0c0,	
		ADAPTER_FILE = 0xf0c1,
		ADAPTER_ALBUM = 0xf0c2,
		ADAPTER_ARTIST = 0xf0c3,
		ADAPTER_MUSIC = 0xf0c4,
	};
	ShutDownView::ShutDownView(void)
	{

	}

	ShutDownView::ShutDownView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
	}

	ShutDownView::~ShutDownView(void)
	{

	}
	
	int ShutDownView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;

		setFocus(this);
		invalidateRect();
		return 0;
	}
	void ShutDownView::addPoint(int x,int y)
	{
		if(count >= mMax){
			int* xtemp;
			int* ytemp;
			
			if(mMax == 0){
				mMax=32;
			}else
				mMax*=2;
			
			xtemp = new int[mMax];
			ytemp = new int[mMax];

			memcpy(xtemp,mpx,count*sizeof(int));
			memcpy(ytemp,mpy,count*sizeof(int));

			delete mpx;
			delete mpy;

			mpx = xtemp;
			mpy = ytemp;
			
		}

		mpx[count] = x;
		mpy[count] = y;
		count++;
		log_i("PointDrawView::addPoint count=%d",count);
	}

	void ShutDownView::pointClear(){
		if(count>0){
			log_i("PointDrawView::pointClear");
			count = mMax = 0;
			delete mpx;
			delete mpy;
			mpx = mpy = NULL;
		}
	}
	void ShutDownView::initView()
	{
		Mstring* mstr;
		int brightness;

		mstr = new Mstring(5);
		gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_READ,&brightness);	
		mstr->mSprintf("%d",brightness);

		mBrightnessValue->setTextString(mstr->mstr);
	}

	int ShutDownView::onDestroy()
	{
		return 0;
	}

	int ShutDownView::onPaint(Canvas& canvas)
	{
		if(gPlayer.mBoardType == 1)
			canvas.drawImageResource(IDP_SHUTDOWM_LOGO_DX90,0,0,false);
		else
			canvas.drawImageResource(IDP_SHUTDOWM_LOGO,0,0,false);
		return 0;
	}

	int ShutDownView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			mpx=0;
		}
		return 0;
	}

	int ShutDownView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int ShutDownView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int ShutDownView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int ShutDownView::onTouchMove(int x, int y, int flag){
		return -1;

	}
	int ShutDownView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
