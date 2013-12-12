#include "player.h"


namespace mango
{
	
	enum
	{
		BUTTON_SCANNER = 0xf0c0,	
		BUTTON_BROWSER = 0xf0c1,
	};
	SdcardInsertView::SdcardInsertView(void)
	{

	}

	SdcardInsertView::SdcardInsertView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
		mScannerButton = NULL;
		mDirectButton = NULL;
	}

	SdcardInsertView::~SdcardInsertView(void)
	{

	}
	
	int SdcardInsertView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx(10,130,300,50);
		mScannerButton = new TextView(BUTTON_SCANNER, TEXT("mScannerButton"), this, &rect, 0);
		mScannerButton->setTextColor(COLOR_TEXT);
		mScannerButton->setTextSelectColor(COLOR_ORANGE);
		mScannerButton->setTextSize(16);
		mScannerButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mScannerButton->setBackGround(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);

		rect.setEx(10,180,300,50);
		mDirectButton = new TextView(BUTTON_BROWSER, TEXT("mDirectButton"), this, &rect, 0);
		mDirectButton->setTextColor(COLOR_TEXT);
		mDirectButton->setTextSelectColor(COLOR_ORANGE);
		mDirectButton->setTextSize(16);
		mDirectButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mDirectButton->setBackGround(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);

		rect.setEx(286, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);

		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void SdcardInsertView::initView()
	{
		mDirectButton->setTextResoure(STR_SDCARD_BROWSE);
		mScannerButton->setTextResoure(STR_SDCARD_SCANNER);

		mBack->setNormalImageId(IDP_MEDIA_SCAN_STOP_D);
		mBack->setPressedImageId(IDP_MEDIA_SCAN_STOP_D);
	}

	int SdcardInsertView::onDestroy()
	{
		return 0;
	}

	int SdcardInsertView::onPaint(Canvas& canvas)
	{
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_SDCARD_INSERT,120,40,true);
		return 0;
	}

	int SdcardInsertView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			mpx=0;
		}else if(code == VSEEKBAR_TEXTVIEW_UP){
			View *view = (View *)parameter;
			if(view == mScannerButton){
				gmediaprovider.externVolumeScanner("/mnt/external_sd");
				gPlayer.dismissView(this);
			}else if(view == mDirectButton){
				gPlayer.showMediaView(MEDIAVIEW_DISPLAY_TYPE_SDCARD);
			}
		}else if(fromView == mBack && code == NM_CLICK){
			gPlayer.dismissView(this);
		}
		return 0;
	}

	int SdcardInsertView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int SdcardInsertView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int SdcardInsertView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int SdcardInsertView::onTouchMove(int x, int y, int flag){
		return -1;
	}
	int SdcardInsertView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
