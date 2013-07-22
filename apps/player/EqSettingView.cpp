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
	EqSettingsView::EqSettingsView(void)
	{

	}

	EqSettingsView::EqSettingsView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mEqMode = new TextView[6];
		mEqFrequency = new TextView[8];
		mVerticalSeekBar = new VerticalSeekBar[8];
	}

	EqSettingsView::~EqSettingsView(void)
	{

	}
	
	int EqSettingsView::onCreate()
	{
		Rect rect;
		int left = 40;
		int top = 60;
		int dinner = 33;
		int size = 17;
		int color = 180;
		int i,count = 8;
		int eqMode[]={STR_EQ_CUSTOM,STR_EQ_POP,STR_EQ_ROCK,STR_EQ_CLASSICAL,STR_EQ_DANCE,STR_EQ_BASS};
		int eqFrequcy[]={STR_EQ_30,STR_EQ_180,STR_EQ_330,STR_EQ_600,STR_EQ_1K,STR_EQ_4K,STR_EQ_12K,STR_EQ_16K};
		int freLeft = 19,diver = 34;
		
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

		for(i=0;i<6;i++){
			rect.setEx(52*i+5, 28, 52, 20);
			mEqMode = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
			mEqMode->setTextColor(COLOR_GRAY);
			mEqMode->setTextSelectColor(COLOR_ORANGE);
			mEqMode->setBackGround(IDP_EQ_MODE_BGD,IDP_EQ_MODE_BGD);
			mEqMode->setTextSize(14);
			mEqMode->setTextLayoutType(TEXT_LAYOUT_CENTER);
			mEqMode->setTextResoure(eqMode[i]);
		}

		for(i=0;i<8;i++){
			rect.setEx(diver*i+freLeft, 53, diver, 13);
			mEqFrequency = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
			mEqFrequency->setTextColor(COLOR_GRAY);
			mEqFrequency->setTextSize(13);
			mEqFrequency->setTextLayoutType(TEXT_LAYOUT_CENTER);
			mEqFrequency->setTextResoure(eqFrequcy[i]);
		}
		
		for(i=0;i<8;i++){
			rect.setEx(diver*i+freLeft+6, 66, 34, 132);
			mVerticalSeekBar = new VerticalSeekBar(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
			mVerticalSeekBar->setMax(13);
			mVerticalSeekBar->setProgress(6);
			mVerticalSeekBar->setImageResoure(IDP_EQ_SEEKBAR,IDP_EQ_SEEKBAR,IDP_EQ_SEEKBAR_THUMB);
			mVerticalSeekBar->onCreate();
		}

		rect.setEx(0, 213, 115, 27);		
		mControlButton = new TextView(SETTING_TITLE, TEXT("mControlButton"), this, &rect, 0);
		mControlButton->setTextColor(COLOR_GRAY);
		mControlButton->setTextSelectColor(COLOR_ORANGE);
		mControlButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mControlButton->setTextResoure(STR_EQ_OPEN);
		mControlButton->onCreate();

		rect.setEx(205, 213, 115, 27);	
		mResetButton = new TextView(SETTING_TITLE, TEXT("mResetButton"), this, &rect, 0);
		mResetButton->setTextColor(COLOR_GRAY);
		mResetButton->setTextSelectColor(COLOR_ORANGE);
		mResetButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mResetButton->setTextResoure(STR_EQ_RESET);
		mResetButton->onCreate();	
		
		initView();
			
		setFocus(this);
		invalidateRect();
		return 0;
	}
	void EqSettingsView::addPoint(int x,int y)
	{

	}

	void EqSettingsView::pointClear(){

	}
	void EqSettingsView::initView()
	{
		Mstring* mstr;
		int brightness;

		//mstr = new Mstring(5);
		//gPlayer.ioctrlBrightness(IOCTRL_BRIGTNESS_READ,&brightness);	
		//mstr->mSprintf("%d",brightness);

		//mBrightnessValue->setTextString(mstr->mstr);

		mTitle->setTextResoure(STR_SETTING_EQ);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);

	}

	int EqSettingsView::onDestroy()
	{
		return 0;
	}

	int EqSettingsView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);	
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 0, 211, false);
		return 0;
	}

	int EqSettingsView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			//initView();
		}else if(fromView == mBack && code == NM_CLICK){
			gPlayer.showSettingsView();
		}else if(fromView == mHome && code == NM_CLICK){
			gPlayer.showPlayingView();
		}
		
		return 0;
	}

	int EqSettingsView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int EqSettingsView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int EqSettingsView::onTouchDown(int x, int y, int flag)
	{

		return -1;
	}
	int EqSettingsView::onTouchMove(int x, int y, int flag){

		return -1;

	}
	int EqSettingsView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
