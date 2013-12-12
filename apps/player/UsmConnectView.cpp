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
	UsmConnectView::UsmConnectView(void)
	{

	}

	UsmConnectView::UsmConnectView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
	}

	UsmConnectView::~UsmConnectView(void)
	{

	}
	
	int UsmConnectView::onCreate()
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
	void UsmConnectView::initView()
	{
		mUsbState->setTextResoure(STR_USB_CONECT);
	}

	int UsmConnectView::onDestroy()
	{
		return 0;
	}

	int UsmConnectView::onPaint(Canvas& canvas)
	{
		log_i(">>>>>>>>>USM");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_USB_CONECT,120,50,true);
		return 0;
	}

	int UsmConnectView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			mpx=0;
		}
		return 0;
	}

	int UsmConnectView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int UsmConnectView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int UsmConnectView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int UsmConnectView::onTouchMove(int x, int y, int flag){
		return -1;
	}
	int UsmConnectView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
