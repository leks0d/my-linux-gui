#include "player.h"


namespace mango
{
	
	enum
	{
		BUTTON_SCANNER = 0xf0c0,	
		BUTTON_BROWSER = 0xf0c1,
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
		mScannerButton = NULL;
		mDirectButton = NULL;
	}

	MediaScannerView::~MediaScannerView(void)
	{

	}
	
	int MediaScannerView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx(40,140,280,40);
		mScannerButton = new TextView(BUTTON_SCANNER, TEXT("mScannerButton"), this, &rect, 0);
		mScannerButton->setTextColor(COLOR_GRAY);
		mScannerButton->setTextSize(16);
		mScannerButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		
		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void MediaScannerView::initView()
	{
		mScannerButton->setTextResoure(STR_MEDIA_SCANNING);
	}

	int MediaScannerView::onDestroy()
	{
		return 0;
	}

	int MediaScannerView::onPaint(Canvas& canvas)
	{
		log_i("MediaScannerView::onPaint");
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
