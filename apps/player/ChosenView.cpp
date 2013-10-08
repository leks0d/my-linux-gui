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
	ChosenView::ChosenView(void)
	{

	}

	ChosenView::ChosenView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mType = 0;
	}

	ChosenView::~ChosenView(void)
	{

	}
	
	int ChosenView::onCreate()
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
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->onCreate();

		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();

		rect.setEx(0, 100, 320, 22);
		mBrightnessValue = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mBrightnessValue->setTextColor(RGB(255,255,255));
		mBrightnessValue->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mBrightnessValue->onCreate();
		
		rect.setEx(0, 213, 115, 27);		
		mMyMusicText = new  TextView(PLAYING_IDB_MUSIC, TEXT("mMyMusicText"), this, &rect, 0);
		mMyMusicText->setTextColor(RGB(209,209,209));
		mMyMusicText->setTextSelectColor(COLOR_ORANGE);
		mMyMusicText->setTextSize(16);
		mMyMusicText->setTextResoure(STR_SYSTEM_ASK_YES);
		mMyMusicText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mMyMusicText->onCreate();

		rect.setEx(205, 213, 115, 27);		
		mSettingText = new  TextView(PLAYING_IDB_SETTING, TEXT("mSettingText"), this, &rect, 0);
		mSettingText->setTextColor(RGB(209,209,209));
		mSettingText->setTextSelectColor(COLOR_ORANGE);
		mSettingText->setTextSize(16);
		mSettingText->setTextResoure(STR_SYSTEM_ASK_NO);
		mSettingText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mSettingText->onCreate();

		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}

	void ChosenView::initView()
	{
		int noticeres,titleres;

		switch(mType){
			case CHOSEN_POWEROFF:
				titleres = STR_SYSTEM_POWEROFF;
				noticeres = STR_SYSTEM_POWEROFF_MSG;
				break;
			case CHOSEN_RECOVERY:
				titleres = STR_SYSTEM_RESTET;
				noticeres = STR_SYSTEM_RESTET_MESSAGE;
				break;
			case CHOSEN_SYSTEMUPDATE:
				titleres = STR_SYSTEM_UPDATE;
				noticeres = STR_SYSTEM_UPDATE_MSG;
				break;
		}
		mBrightnessValue->setTextResoure(noticeres);
		mTitle->setTextResoure(titleres);
		gPowerManager->setPowerState(POWER_STATE_ON);
		log_i("set power state POWER_STATE_ON");
	}

	unsigned int ChosenView::PoweroffRunnig(void *parameter){
		ChosenView *chosen = (ChosenView *)parameter;
		long start,dur,sleep;

		start = Time::getMillisecond();
		gPlayer.shutDown();
		dur = Time::getMillisecond()-start;

		sleep = 1000*2 - dur;

		if(sleep>0)
			mango::Thread::sleep(sleep);

		log_i("sleep=%ld",sleep);
		
		chosen->dispatchEvent();
	}
	void ChosenView::dispatchEvent(){
		switch(mType){
			case CHOSEN_POWEROFF:
				reboot(RB_POWER_OFF);
				break;
			case CHOSEN_RECOVERY:
				Environment::recovery();
				break;
			case CHOSEN_SYSTEMUPDATE:
				Environment::install();
				break;
		}
	}

	int ChosenView::onDestroy()
	{
		return 0;
	}

	int ChosenView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 0, 211, false);
		return 0;
	}

	int ChosenView::onNotify(View* fromView, int code, void* parameter)
	{
		log_i("onNotify code=%d,fromView=0x%x,mBack=0x%x",code,fromView,mBack);
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == mBack){
			gPlayer.dismissView(this);
		}else if(fromView == mHome){
			gPlayer.showPlayingView();
		}else if(parameter == mMyMusicText){
			gPlayer.showShutDownView();
			mPoweroffThread.create(ChosenView::PoweroffRunnig,(void * )this);
		}else if(parameter == mSettingText){
			gPlayer.dismissView(this);
		}
		return 0;
	}

	int ChosenView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int ChosenView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				//gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int ChosenView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int ChosenView::onTouchMove(int x, int y, int flag){
		log_i("DisplaySettingView::onTouchMove x=%d,y=%d",x,y);
		return -1;

	}
	int ChosenView::onTouchUp(int x, int y, int flag){
		return -1;
	}
};
