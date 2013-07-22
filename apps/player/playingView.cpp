#include "player.h"

namespace mango
{
	enum
	{
		PLAYING_IDB_NEXT = 10,
		PLAYING_IDB_PREV,
		PLAYING_IDB_PLAY,
		PLAYING_IDB_MUSIC,
		PLAYING_IDB_SETTING,
		PLAYING_IDB_VOLUME,
		PLAYING_IDB_ALBUM_IMAGE,
		PLAYING_IDB_MUSIC_NAME
	};

	PlayingView::PlayingView(void)
	{
	}


	PlayingView::PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{
	}


	PlayingView::~PlayingView(void)
	{

	}


	int PlayingView::onCreate()
	{

		Rect rect;
		rect.setEx(80, 180, 32, 23);
		mPrevButton = new Button(PLAYING_IDB_PREV, TEXT("mPrevButton"), this, &rect, 0);
		mPrevButton->setNormalImageId(IDP_PLAYING_PREV_INACTIVE);
		mPrevButton->setPressedImageId(IDP_PLAYING_PREV_ACTIVE);
		mPrevButton->onCreate();
		
		rect.setEx(210, 180, 32, 23);
		mNextButton = new Button(PLAYING_IDB_NEXT, TEXT("mNextButton"), this, &rect, 0);
		mNextButton->setNormalImageId(IDP_PLAYING_NEXT_INACTIVE);
		mNextButton->setPressedImageId(IDP_PLAYING_NEXT_ACTIVE);
		mNextButton->onCreate();
		
		rect.setEx(145, 180, 32, 23);
		mPlayButton = new Button(PLAYING_IDB_PLAY, TEXT("mPlayButton"), this, &rect, 0);
		mPlayButton->setNormalImageId(IDP_PLAYING_PLAY_ACTIVE);
		mPlayButton->setPressedImageId(IDP_PLAYING_PLAY_ACTIVE);
		mPlayButton->onCreate();
				
		rect.setEx(6, 30, 110, 110);
		mAlbumImage = new ImageView(PLAYING_IDB_ALBUM_IMAGE, TEXT("mAlbumImage"), this, &rect, 0);

		
		rect.setEx(127, 55, 190, 25);
		mMusicName = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mMusicName"), this, &rect, 0);
		mMusicName->setTextColor(RGB(248,136,0));
		mMusicName->setTextSize(23);
		
		rect.setEx(127, 85, 190, 15);
		mArtist = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mArtist"), this, &rect, 0);
		mArtist->setTextColor(RGB(154,154,154));
		mArtist->setTextSize(16);

		rect.setEx(127, 100, 190, 15);
		mAlbum = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mAlbum"), this, &rect, 0);
		mAlbum->setTextColor(RGB(154,154,154));
		mAlbum->setTextSize(16);
		
		rect.setEx(130, 0, 20, 21);
		mVolumeButton = new  Button(13, TEXT("mVolumeButton"), this, &rect, 0);
		mVolumeButton->setNormalImageId(DPI_VOLUME_ICON);
		mVolumeButton->onCreate();
		
		rect.setEx(150, 2, 20, 16);
		mVolumeText = new  TextView(-1, TEXT("mVolumeText"), this, &rect, 0);
		mVolumeText->setTextSize(13);
		mVolumeText->setTextColor(RGB(255,255,255));
		mVolumeText->onCreate();
		
		rect.setEx(255, 0, 28, 19);
		mBatteryIcon = new  ImageView(-1, TEXT("mBatteryIcon"), this, &rect, 0);
		mBatteryIcon->setImageResoure(IDP_BATTERY_ICON);
		mBatteryIcon->onCreate();
		
		rect.setEx(283, 2, 35, 16);		
		mBatteryText = new  TextView(-1, TEXT("mBatteryText"), this, &rect, 0);
		mBatteryText->setTextColor(RGB(255,255,255));
		mBatteryText->setTextSize(13);
		mBatteryText->onCreate();
		
		rect.setEx(0, 140, 307, 20);		
		mSeekBar = new  SeekBar(-1, TEXT("mSeekBar"), this, &rect, 0);
		mSeekBar->setImageResoure(IDP_SEEKBAR_BKG,IDP_SEEKBAR_SEEK,IDP_SEEKBAR_THUMB);
		mSeekBar->onCreate();
		
		rect.setEx(15, 166, 60, 15);		
		mTimeText = new  TextView(-1, TEXT("playtimetext"), this, &rect, 0);
		mTimeText->setTextColor(RGB(255,255,255));
		mTimeText->setTextSize(13);
		mTimeText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mTimeText->onCreate();
		
		rect.setEx(270, 166, 50, 15);		
		mDurtionText = new  TextView(-1, TEXT("mDurtionText"), this, &rect, 0);
		mDurtionText->setTextColor(RGB(255,255,255));
		mDurtionText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mDurtionText->setTextSize(13);
		mDurtionText->onCreate();

		rect.setEx(0, 213, 115, 27);		
		mMyMusicText = new  TextView(PLAYING_IDB_MUSIC, TEXT("mMyMusicText"), this, &rect, 0);
		mMyMusicText->setTextColor(RGB(209,209,209));
		mMyMusicText->setTextSize(16);
		mMyMusicText->setTextResoure(MUSIC_MY_MUSIC);
		mMyMusicText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mMyMusicText->onCreate();

		rect.setEx(205, 213, 115, 27);		
		mSettingText = new  TextView(PLAYING_IDB_SETTING, TEXT("mSettingText"), this, &rect, 0);
		mSettingText->setTextColor(RGB(209,209,209));
		mSettingText->setTextSize(16);
		mSettingText->setTextResoure(MUSIC_MUSIC_FUN);
		mSettingText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mSettingText->onCreate();
		
		ViewInit();
		
		return -1;
	}

	void PlayingView::ViewInit(void){
		log_i("PlayingView::ViewInit");	
		
		Mstring* mstr;
		mediainfo* currentinfo;
		currentinfo = mPlayinglist->getPlayingItem();
		
		mstr = new Mstring(10);
		mstr->mSprintf("%d",gPlayer.getVolume());
		mVolumeText->setTextString(mstr->mstr);
		
		mstr->clear();
		mstr->mSprintf("%d%%",35);
		mBatteryText->setTextString(mstr->mstr);

		if(currentinfo == NULL){
			mAlbumImage->setImageResoure(IDP_DEFAULT_ALBUM_ICON);
			mMusicName->setTextString("Not find music.");
			return;
		}
		mCurrentInfo = *currentinfo;
		
		mMusicName->setTextString(mCurrentInfo.name);
		
		mArtist->setTextString(mCurrentInfo.artist);

		mAlbum->setTextString(mCurrentInfo.album);
		
		mSeekBar->setMax(mPlayinglist->getDuration());

		mstr->clear();
		mstr->setPlayTime(mPlayinglist->getCurrent());
		mTimeText->setTextString(mstr->mstr);

		mstr->clear();
		mstr->setPlayTime(mPlayinglist->getDuration());
		mDurtionText->setTextString(mstr->mstr);
		
		mstr->clear();
		mDurtionText->getTextString(mstr->mstr);

		mSeekBarUpdateThread.create(PlayingView::SeekBarRunnig, this);
		isNeedFresh = 1;

	}
	
	unsigned int PlayingView::SeekBarRunnig(void *parameter){
			PlayingView *mplayingview = (PlayingView*)parameter;
			while(1){
				Thread::sleep(350);
				if(mplayingview->isNeedFresh && mPlayinglist->isPlaying()){
					Mstring* mstr;
					mstr = new Mstring(10);
					mstr->clear();
					mstr->setPlayTime(mPlayinglist->getCurrent());
					
					mplayingview->mTimeText->setTextString(mstr->mstr);
					/*
					mstr->clear();
					mstr->setPlayTime(mPlayinglist->getDuration());
					mplayingview->mDurtionText->setTextString(mstr->mstr);
					*/
					mplayingview->mSeekBar->setProgress(mPlayinglist->getCurrent());
				}
			}
			return 0;
	}
	
	int PlayingView::onDestroy()
	{
		if (mPrevButton)
			mPrevButton->onDestroy();
		
		if (mNextButton)
			mNextButton->onDestroy();

		safeFree(mPrevButton);
		safeFree(mNextButton);

		return -1;
	}


	int PlayingView::onPaint(Canvas& canvas)
	{
		Rect rect;
		Brush brush(RGB(25, 25, 25));
		log_i("PlayingView::onPaint");
		
		rect.setEx(0, 0, 320, 21);
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND, 0, 0, false);
		canvas.fillRect(rect, brush);
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 0, 211, false);

		log_i("PlayingView::onPaint complete");
		
		return 0;
	}


	int PlayingView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			log_i("PlayingView::onNotify NM_DISPLAY");
			ViewInit();
			isNeedFresh = 1;
		}else if(fromView == NULL && code == NM_DISMISS){
			log_i("PlayingView::onNotify NM_DISMISS");
			isNeedFresh = 0;
		}
		
		return 0;
	}


	int PlayingView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode)	{
		case KEYCODE_BACK:
			break;
		}

		return 0;
	}


	int PlayingView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode)	{
		case KEYCODE_BACK:
			break;
		}

		return -1;
	}


	int PlayingView::onCommand(int id, int code, View* fromView)
	{
		int volume;

		switch(id)
		{
		case PLAYING_IDB_NEXT:
			volume = gPlayer.getVolume();
			volume += 4;
			if (volume > 255)
				volume = 255;
				gPlayer.setVolume(volume);
			break;

		case PLAYING_IDB_PREV:
			volume = gPlayer.getVolume();
			volume -= 4;
			if (volume < 0)
				volume = 0;
				gPlayer.setVolume(volume);
			break;
		case PLAYING_IDB_MUSIC:
			gPlayer.showMediaView();
			break;
		case PLAYING_IDB_SETTING:
			isNeedFresh = 0;
			gPlayer.showSettingsView();
			break;			
		}
		return -1;
	}

	int Mstring::mSprintf(const char* str,int n)
	{
		char *ptr;
		ptr = mstr + pos;
		pos+=sprintf(ptr,str,n);
		return pos;
	}

	void Mstring::setPlayTime(int n){
		char *ptr;
		int sec,min,time;
		time = n/1000;
		sec=time%60;
		min=time/60;
		ptr = mstr + pos;
		pos+=sprintf(ptr,"%d:%d",min,sec);
		log_i("Mstring::setPlayTime n=%d,%s",n,mstr);
	}
};
