#include "player.h"

#define VOLUME_REFRESH 100
#define VOLUME_TIME 2000/VOLUME_REFRESH

namespace mango
{
	
	enum{
		SETTING_BACK = 10,
		SETTING_HOME,
		SETTING_TITLE,
		PLAYING_IDB_PLAY,
		PLAYING_IDB_MUSIC,
		PLAYING_IDB_SETTING,
		PLAYING_IDB_VOLUME,
		PLAYING_IDB_ALBUM_IMAGE,
		PLAYING_IDB_MUSIC_NAME
	};
	enum
	{
		ADAPTER_PLAYING = 0xf0c0,	
		ADAPTER_FILE = 0xf0c1,
		ADAPTER_ALBUM = 0xf0c2,
		ADAPTER_ARTIST = 0xf0c3,
		ADAPTER_MUSIC = 0xf0c4,
	};
	VolumeView::VolumeView(void)
	{

	}

	VolumeView::VolumeView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
		closeCount = VOLUME_TIME;
	}

	VolumeView::~VolumeView(void)
	{

	}
	
	int VolumeView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx(136,95,60,15);
		mVolumeStr = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mVolumeStr->setTextColor(COLOR_WHITE);
		mVolumeStr->setTextResoure(STR_VOLUME);
		mVolumeStr->setTextSize(15);
		mVolumeStr->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mVolumeStr->onCreate();
		
		rect.setEx(136,105,60,20);
		mVolume = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mVolume->setTextColor(COLOR_WHITE);
		mVolume->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mVolume->setTextSize(32);
		mVolume->onCreate();
		
		initView();	

		
		
		setFocus(this);
		invalidateRect();
		isShow = 1;
		return 0;
	}

	void VolumeView::setVolume(int vol){
		Mstring* mstr;
		int volume;
		
		mstr = new Mstring(5);
		volume = vol;
		mstr->mSprintf("%d",volume);
		
		mVolume->setTextString(mstr->mstr);	
	}
	
	unsigned int VolumeView::VolumeRunning(void* p){
		VolumeView* iVolume = (VolumeView*)p;
		int vol,closeCount;
		closeCount = VOLUME_TIME;
		while (1) {
			Thread::sleep(100);
			
			log_i("VolumeView::VolumeRunning isKeyDown=%d,mDownKeyCode=%d",iVolume->isKeyDown,iVolume->mDownKeyCode);

			if(iVolume->isKeyDown){
				
				vol = gPlayer.getVolume();
				
				switch(iVolume->mDownKeyCode){
					case KEYCODE_VOLUMEUP:					
						vol++;
						break;
					case KEYCODE_VOLUMEDOWN:
						vol--;
						break;
				}
				
				if(vol>=0&&vol<=255){
					gPlayer.setVolume(vol);
					iVolume->setVolume(vol);
				}

				closeCount = VOLUME_TIME;
			}
			else{
				closeCount--;
				if(closeCount<=0)
					break;
			}			
		}
		iVolume->isShow = 0;
		gPlayer.showPlayingView();
	}
	
	void VolumeView::addPoint(int x,int y)
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
	}

	void VolumeView::pointClear(){
		if(count>0){
			count = mMax = 0;
			delete mpx;
			delete mpy;
			mpx = mpy = NULL;
		}
	}
	void VolumeView::initView()
	{
		Mstring* mstr;
		int brightness;
		int volume;
		
		mstr = new Mstring(5);
		volume = gPlayer.getVolume();	
		mstr->mSprintf("%d",volume);
		
		mVolume->setTextString(mstr->mstr);

		mVolumeThread.create(VolumeRunning,this);
	}

	int VolumeView::onDestroy()
	{
		return 0;
	}

	int VolumeView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_VULUME_ICON,(320-204)/2,(240-204)/2,true);
		Rect rect;
		Brush brush(RGB(0, 0, 0));
		
		return 0;
	}

	int VolumeView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}
		
		return 0;
	}

	int VolumeView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_VOLUMEUP:
			case KEYCODE_VOLUMEDOWN:
				isKeyDown = 1;
				mDownKeyCode = keyCode;
				break;
		}

		return 0;
	}

	int VolumeView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_VOLUMEUP:
			case KEYCODE_VOLUMEDOWN:
				isKeyDown = 0;
				mDownKeyCode = keyCode;
				break;
				
		}
		return 0;
	}

	int VolumeView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int VolumeView::onTouchMove(int x, int y, int flag){

		return -1;

	}
	int VolumeView::onTouchUp(int x, int y, int flag){

		return -1;
		}
};
