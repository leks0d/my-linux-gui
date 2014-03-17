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
	USBAudioConnectView::USBAudioConnectView(void)
	{

	}

	USBAudioConnectView::USBAudioConnectView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
	}

	USBAudioConnectView::~USBAudioConnectView(void)
	{

	}
	
	int USBAudioConnectView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx(0,150,320,35);
		mUsbState = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mUsbState->setTextColor(COLOR_WHITE);
		mUsbState->setTextSize(25);
		mUsbState->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mUsbState->setonTouchFresh(false);
		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void USBAudioConnectView::initView()
	{
		//mUsbState->setTextResoure(STR_USB_CONECT);
		
	}
	void USBAudioConnectView::setAudioInfo(const char* recv){
		int bit,rate,ret;
		Mstring *mstr;
		float sampleRate;
		mstr = new Mstring(50);
		ret = sscanf(recv,"usbAudio msg:Rate=%d,Bit=%d",&rate,&bit);
		log_i("recv=%s",recv);
		if(0){
			sampleRate = rate/1000.0;
			mstr->mSprintf("%dBit ",bit);
			mstr->mfloatSprintf("%.1fKHz",sampleRate);
			mUsbState->setTextString(mstr->mstr);
		}else{
			mUsbState->setTextResoure(STR_USB_MODE_DAC);
		}
	}
	
	int USBAudioConnectView::onDestroy()
	{
		return 0;
	}

	int USBAudioConnectView::onPaint(Canvas& canvas)
	{
		log_i(">>>>>>>>>USM");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_USB_CONECT,120,50,true);
		return 0;
	}

	int USBAudioConnectView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			mpx=0;
		}
		return 0;
	}

	int USBAudioConnectView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int USBAudioConnectView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int USBAudioConnectView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int USBAudioConnectView::onTouchMove(int x, int y, int flag){
		return -1;
	}
	int USBAudioConnectView::onTouchUp(int x, int y, int flag){
		return -1;
	}
};
