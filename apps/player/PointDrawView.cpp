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
	PointDrawView::PointDrawView(void)
	{

	}

	PointDrawView::PointDrawView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
	}

	PointDrawView::~PointDrawView(void)
	{

	}
	
	int PointDrawView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;

		setFocus(this);
		invalidateRect();
		return 0;
	}
	void PointDrawView::addPoint(int x,int y)
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

	void PointDrawView::pointClear(){
		if(count>0){
			log_i("PointDrawView::pointClear");
			count = mMax = 0;
			delete mpx;
			delete mpy;
			mpx = mpy = NULL;
		}
	}
	void PointDrawView::initView()
	{
		Mstring* mstr;
		int brightness;

		mstr = new Mstring(5);
		gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_READ,&brightness);	
		mstr->mSprintf("%d",brightness);

		mBrightnessValue->setTextString(mstr->mstr);
	}

	int PointDrawView::onDestroy()
	{
		return 0;
	}

	int PointDrawView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		//canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		//canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		Rect rect;
		Brush brush(RGB(0, 0, 0));
		Brush pbrush(RGB(255, 255, 255));
		int i;
		
		//if(isNeedPaint){
			isNeedPaint = 0;
			rect.setEx(0, 0, 320, 240);
			canvas.fillRect(rect, brush);
			
			rect.setEx(300, 0, 1, 240);
			canvas.fillRect(rect, pbrush);
		//}
		
		for(i=0;i<count;i++){
			rect.setEx(mpx[i], mpy[i], 1, 1);
			canvas.fillRect(rect, pbrush);
		}
		
		return 0;
	}

	int PointDrawView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			//initView();
			mpx=0;
		}
		
		return 0;
	}

	int PointDrawView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int PointDrawView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int PointDrawView::onTouchDown(int x, int y, int flag)
	{
		pointClear();
		addPoint(x,y);
		isNeedPaint = 1;
		invalidateRect();
		return -1;
	}
	int PointDrawView::onTouchMove(int x, int y, int flag){
		log_i("DisplaySettingView::onTouchMove x=%d,y=%d",x,y);
		addPoint(x,y);
		invalidateRect();
		return -1;

	}
	int PointDrawView::onTouchUp(int x, int y, int flag){
		addPoint(x,y);
		invalidateRect();
		return -1;
		}
};
