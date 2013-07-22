#include "player.h"


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
	SystemInfoView::SystemInfoView(void)
	{

	}

	SystemInfoView::SystemInfoView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mModelNumber = NULL;
		mFirmwareVersion = NULL;
		mStorageSpace = NULL;
	}

	SystemInfoView::~SystemInfoView(void)
	{

	}
	
	int SystemInfoView::onCreate()
	{
		Rect rect;
		int left = 40;
		int top = 60;
		int dinner = 33;
		int size = 17;
		int color = 180;
		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(RGB(255,255,255));
		mTitle->onCreate();

		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();
		
		rect.setEx(left, top, 320-left, 20);
		mModelNumber = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mModelNumber->setTextResoure(STR_SYSTEM_INFO_MODEL);
		mModelNumber->setTextSize(size);
		mModelNumber->setTextColor(RGB(color,color,color));
		mModelNumber->onCreate();
		top+=dinner;
		
		rect.setEx(left, top, 320-left, 20);
		mFirmwareVersion = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mFirmwareVersion->setTextResoure(STR_SYSTEM_INFO_VERSION);
		mFirmwareVersion->setTextSize(size);
		mFirmwareVersion->setTextColor(RGB(color,color,color));
		mFirmwareVersion->onCreate();
		top+=dinner;
		
		rect.setEx(left, top, 320-left, 20);
		mStorageSpace = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mStorageSpace->setTextResoure(STR_SYSTEM_INFO_STORAGE);
		mStorageSpace->setTextSize(size);
		mStorageSpace->setTextColor(RGB(color,color,color));
		mStorageSpace->onCreate();

		initView();
			
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void SystemInfoView::addPoint(int x,int y)
	{

	}

	void SystemInfoView::pointClear(){

	}
	void SystemInfoView::initView()
	{
		Mstring* mstr;
		int brightness;

		//mstr = new Mstring(5);
		//gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_READ,&brightness);	
		//mstr->mSprintf("%d",brightness);

		//mBrightnessValue->setTextString(mstr->mstr);

		mTitle->setTextResoure(STR_ADVANCE_SYSINFO);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mModelNumber->setTextString("DX50");
		mFirmwareVersion->setTextString("V1.00");
		mStorageSpace->setTextString("15.6G");
	}

	int SystemInfoView::onDestroy()
	{
		return 0;
	}

	int SystemInfoView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);	
		
		return 0;
	}

	int SystemInfoView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			//initView();
		}else if(fromView == mBack && NM_CLICK){
			gPlayer.showSettingsView();
		}else if(fromView == mHome && NM_CLICK){
			gPlayer.showPlayingView();
		}
		
		return 0;
	}

	int SystemInfoView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int SystemInfoView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				//gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int SystemInfoView::onTouchDown(int x, int y, int flag)
	{

		return -1;
	}
	int SystemInfoView::onTouchMove(int x, int y, int flag){

		return -1;

	}
	int SystemInfoView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
