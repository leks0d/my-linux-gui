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
	EqSettingsView::EqSettingsView(void)
	{

	}

	EqSettingsView::EqSettingsView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mCurrentValue = new int[8];
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
		mTitle->setTextColor(COLOR_TEXT);
		mTitle->onCreate();

		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();

		for(i=0;i<6;i++){
			rect.setEx(52*i+5, 28, 52, 20);
			mEqMode[i] = new TextView(i, TEXT("mTitle"), this, &rect, 0);
			mEqMode[i]->setTextColor(COLOR_GRAY);
			mEqMode[i]->setTextSelectColor(COLOR_ORANGE);
			mEqMode[i]->setBackGround(IDP_EQ_MODE_BGD,IDP_EQ_MODE_BGD_S);
			mEqMode[i]->setTextSize(14);
			mEqMode[i]->setTextLayoutType(TEXT_LAYOUT_CENTER);
			mEqMode[i]->setTextResoure(eqMode[i]);
			mEqMode[i]->setTextDisSectBgd(IDP_EQ_MODE_BGD_DS);
		}

		for(i=0;i<8;i++){
			rect.setEx(diver*i+freLeft, 53, diver, 13);
			mEqFrequency[i] = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
			mEqFrequency[i]->setTextColor(COLOR_GRAY);
			mEqFrequency[i]->setTextSize(13);
			mEqFrequency[i]->setTextLayoutType(TEXT_LAYOUT_CENTER);
			mEqFrequency[i]->setTextResoure(eqFrequcy[i]);
		}

		for(i=0;i<8;i++){
			rect.setEx(diver*i+freLeft+6, 66, diver, 132);
			mVerticalSeekBar[i] = new VerticalSeekBar(i, TEXT("mTitle"), this, &rect, 0);
			mVerticalSeekBar[i]->setMax(MAX_EQ);
			mVerticalSeekBar[i]->setImageResoure(IDP_EQ_SEEKBAR,IDP_EQ_SEEKBAR,IDP_EQ_SEEKBAR_THUMB);
		}

		for(i=0;i<8;i++){
			rect.setEx(diver*i+freLeft, 190, diver, 13);
			mEqValue[i] = new TextView(SETTING_TITLE, TEXT("mEqValue"), this, &rect, 0);
			mEqValue[i]->setTextColor(COLOR_GRAY);
			mEqValue[i]->setTextSize(13);
			mEqValue[i]->setTextLayoutType(TEXT_LAYOUT_CENTER);
		}

		rect.setEx(0, 213, 115, 27);		
		mControlButton = new TextView(SETTING_EQ_OPENORCLOSE, TEXT("mControlButton"), this, &rect, 0);
		mControlButton->setTextColor(COLOR_GRAY);
		mControlButton->setTextSelectColor(COLOR_ORANGE);
		mControlButton->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mControlButton->setTextResoure(STR_EQ_OPEN);
		mControlButton->onCreate();

		rect.setEx(205, 213, 115, 27);	
		mResetButton = new TextView(SETTING_EQ_RESET, TEXT("mResetButton"), this, &rect, 0);
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
		TextView *text;		
		
		gSettingProvider.query(SETTING_EQMODE_ID,&mCurrentMode);
		gSettingProvider.query(SETTING_EQSTATE_ID,&mEqOpen);
		gSettingProvider.EqQuery(mCurrentMode,mCurrentValue);
		
		mTitle->setTextResoure(STR_SETTING_EQ);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);

		resetEqValue();
	}

	void EqSettingsView::resetEqValue(){
		Mstring* mstr;
		int i;

		mstr = new Mstring(5);

		for(i=0;i<8;i++){
			mstr->clear();
			mstr->mSprintf("%d",mCurrentValue[i]);
			mEqValue[i]->setTextString(mstr->mstr);
			mVerticalSeekBar[i]->setProgress(valueToProgress(mCurrentValue[i]));
		}
		mstr->clear();
		
		for(i=0;i<6;i++){
			if(mCurrentMode == i)
				mEqMode[i]->setTextSelect(1);
			else
				mEqMode[i]->setTextSelect(0);
			
			mEqMode[i]->setTextEnable(mEqOpen);
		}
		mResetButton->setTextEnable(mEqOpen);
		for(i=0;i<8;i++){
			mVerticalSeekBar[i]->setEnable(mEqOpen);
		}
		if(mEqOpen)
			mControlButton->setTextResoure(STR_EQ_COLSE);
		else
			mControlButton->setTextResoure(STR_EQ_OPEN);
		
	}

	void EqSettingsView::setCurrentEqMode(int n){
		if(mCurrentMode!=n){
			mCurrentMode = n;
			gSettingProvider.update(SETTING_EQMODE_ID,mCurrentMode);
			gSettingProvider.EqQuery(mCurrentMode,mCurrentValue);
		}
	}
	void EqSettingsView::setCurrentEqValue(int id,int val){
		mCurrentValue[id] = val;
		gSettingProvider.EqUpdate(mCurrentMode,mCurrentValue);
	}
	void EqSettingsView::setEqModeToReset(){
		memcpy(mCurrentValue,EqValue[mCurrentMode],8*sizeof(int));
		gSettingProvider.EqUpdate(mCurrentMode,mCurrentValue);
	}

	
	int EqSettingsView::onDestroy()
	{
		return 0;
	}

	int EqSettingsView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,true);	
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 0, 211, true);
		return 0;
	}

	int EqSettingsView::onNotify(View* fromView, int code, void* parameter)
	{
		View *view = (View *)parameter;
		if(fromView == NULL && code == NM_DISPLAY){
			//initView();
		}else if(fromView == mBack && code == NM_CLICK){
			gPlayer.showSettingsView();
		}else if(fromView == mHome && code == NM_CLICK){
			gPlayer.showPlayingView();
		}else if(code == VSEEKBAR_TOUCH_CHANGE){
			VerticalSeekBar *seekbar = (VerticalSeekBar *)fromView;
			mCurrentValue[fromView->mId] = progressToValue(seekbar->getProgress());
			resetEqValue();
		}else if(code == VSEEKBAR_TOUCH_UP){			
			VerticalSeekBar *seekbar = (VerticalSeekBar *)fromView;
			setCurrentEqValue(fromView->mId,progressToValue(seekbar->getProgress()));
			resetEqValue();
			mPlayinglist->setEqBandLevel(fromView->mId,mCurrentValue[fromView->mId]);
		}else if(code == VSEEKBAR_TEXTVIEW_UP&&view->mId<6){
			setCurrentEqMode(view->mId);
			resetEqValue();
			mPlayinglist->setEq(mCurrentValue);
		}else if(code == VSEEKBAR_TEXTVIEW_UP&&view->mId==SETTING_EQ_RESET){
			setEqModeToReset();
			resetEqValue();
			mPlayinglist->setEq(mCurrentValue);
		}else if(code == VSEEKBAR_TEXTVIEW_UP&&view->mId==SETTING_EQ_OPENORCLOSE){
			mEqOpen=!mEqOpen;
			gSettingProvider.update(SETTING_EQSTATE_ID,mEqOpen);
			resetEqValue();
			mPlayinglist->AudioEqualizerEnable(mEqOpen);
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
