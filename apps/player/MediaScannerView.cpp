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
	MediaScannerView::MediaScannerView(void)
	{

	}

	MediaScannerView::MediaScannerView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
	}

	MediaScannerView::~MediaScannerView(void)
	{

	}
	
	int MediaScannerView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx(0,150,320,25);
		mUsbState = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mUsbState->setTextColor(COLOR_WHITE);
		mUsbState->setTextSize(25);
		mUsbState->setTextLayoutType(TEXT_LAYOUT_CENTER);
		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void MediaScannerView::initView()
	{
		mUsbState->setTextResoure(STR_USB_CONECT);
	}

	int MediaScannerView::onDestroy()
	{
		return 0;
	}

	int MediaScannerView::onPaint(Canvas& canvas)
	{
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_USB_CONECT,115,50,false);
		return 0;
	}

	int MediaScannerView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			mpx=0;
		}
		return 0;
	}

	int MediaScannerView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int MediaScannerView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int MediaScannerView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int MediaScannerView::onTouchMove(int x, int y, int flag){
		return -1;
	}
	int MediaScannerView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
