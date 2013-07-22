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
	DisplaySettingView::DisplaySettingView(void)
	{

	}

	DisplaySettingView::DisplaySettingView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{

	}

	DisplaySettingView::~DisplaySettingView(void)
	{

	}
	
	int DisplaySettingView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;

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

		rect.setEx(140, 100, 22, 22);
		mDisplayIcon = new ImageView(PLAYING_IDB_ALBUM_IMAGE, TEXT("mAlbumImage"), this, &rect, 0);
		mDisplayIcon->setImageResoure(IDP_ADVANCE_DISPLAY);
		mDisplayIcon->onCreate();

		rect.setEx(165, 100, 22, 22);
		mBrightnessValue = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mBrightnessValue->setTextColor(RGB(255,255,255));
		mBrightnessValue->onCreate();

		rect.setEx(0, 140, 307, 20);		
		mBrightnessBar = new SeekBar(-1, TEXT("mSeekBar"), this, &rect, 0);
		mBrightnessBar->setImageResoure(IDP_SEEKBAR_BKG,IDP_SEEKBAR_SEEK,IDP_SEEKBAR_THUMB);
		mBrightnessBar->setMax(255);
		mBrightnessBar->onCreate();

		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}

	void DisplaySettingView::initView()
	{
		Mstring* mstr;
		int brightness;

		mstr = new Mstring(5);
		gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_READ,&brightness);
		mBrightnessBar->setProgress(brightness);
		mstr->mSprintf("%d",brightness);

		mBrightnessValue->setTextString(mstr->mstr);

		mTitle->setTextResoure(STR_ADVANCE_DISPLAY);	
	}

	void DisplaySettingView::setDisplayValue(int n){
		Mstring* mstr;
		mstr = new Mstring(5);
		mstr->mSprintf("%d",n);
		mBrightnessValue->setTextString(mstr->mstr);
	}
	
	int DisplaySettingView::onDestroy()
	{
		return 0;
	}

	int DisplaySettingView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		return 0;
	}

	int DisplaySettingView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == mBrightnessBar){
			int brightness = mBrightnessBar->getProgress();
			switch(code){
				case NM_SEEKBAR_DOWM:
				case NM_SEEKBAR_MOVE:
				case NM_SEEKBAR_UP:
					gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_WRITE,&brightness);
					setDisplayValue(brightness);
					break;
			}
		}else if(fromView == mBack && NM_CLICK){
			gPlayer.showSettingsView();
		}else if(fromView == mHome && NM_CLICK){
			gPlayer.showPlayingView();
		}
		
		return 0;
	}

	int DisplaySettingView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int DisplaySettingView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				//gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int DisplaySettingView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int DisplaySettingView::onTouchMove(int x, int y, int flag){
		log_i("DisplaySettingView::onTouchMove x=%d,y=%d",x,y);
		return -1;

	}
	int DisplaySettingView::onTouchUp(int x, int y, int flag){
		return -1;
	}
};
