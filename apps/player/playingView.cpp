#include "player.h"


namespace mango
{

	PlayingView::PlayingView(void)
	{
	}


	PlayingView::PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{	
		mMSkBitmap = new MSkBitmap();
		isNeedFreshOld = 0;
	}


	PlayingView::~PlayingView(void)
	{

	}


	int PlayingView::onCreate()
	{
		int left = 20;
		Rect rect;
		log_i("PlayingView::onCreate()");
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

		rect.setEx(280, 30, 40, 22);
		mPlayModeButton = new Button(PLAYING_IDB_PLAY_MODE, TEXT("mPlayModeButton"), this, &rect, 0);
				
		rect.setEx(6, 30, 109, 109);
		mAlbumImage = new ImageView(PLAYING_IDB_ALBUM_IMAGE, TEXT("mAlbumImage"), this, &rect, 0);

		rect.setEx(left, 32, 100, 20);
		mAudioInfo = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mAudioInfo"), this, &rect, 0);
		mAudioInfo->setTextColor(COLOR_GRAY);
		mAudioInfo->setTextSize(15);

		rect.setEx(left, 55, 190, 25);
		mMusicName = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mMusicName"), this, &rect, 0);
		mMusicName->setTextColor(RGB(248,136,0));
		mMusicName->setTextSize(23);
		
		rect.setEx(left, 85, 190, 20);
		mArtist = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mArtist"), this, &rect, 0);
		mArtist->setTextColor(RGB(154,154,154));
		mArtist->setTextSize(16);

		rect.setEx(left, 105, 190, 20);
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
		mVolumeText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mVolumeText->onCreate();
		
		rect.setEx(280, 2, 320-285, 19);
		mBatteryIcon = new  ImageView(-1, TEXT("mBatteryIcon"), this, &rect, 0);
		mBatteryIcon->setImageResoure(IDP_BATTERY_0);
		mBatteryIcon->onCreate();
		/*
		rect.setEx(283, 0, 35, 19);		
		mBatteryText = new  TextView(-1, TEXT("mBatteryText"), this, &rect, 0);
		mBatteryText->setTextColor(RGB(255,255,255));
		mBatteryText->setTextSize(13);
		mBatteryText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mBatteryText->onCreate();*/
		
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
		isNeedFresh = 1;
		ViewInit();
		mSeekBarUpdateThread.create(PlayingView::SeekBarRunnig, this);
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

		if(currentinfo == NULL){	
			mMusicName->setTextString("Not find music.");
			mAudioInfo->setTextString(NULL);
			mAlbum->setTextString(NULL);
			mArtist->setTextString(NULL);
			updatePlayMode();
			updatePlayButtonIcon();		
			return;
		}

		mCurrentInfo = *currentinfo;
#if 0
		if( (currentinfo->img_path == NULL) || (strcmp(currentinfo->img_path,"(null)") == 0)){
			mMSkBitmap->release();
		}else{
			SkBitmap skBitmap,*pskBitmap;
			SkCanvas *skCanvas;
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,129,129);
		    pskBitmap->allocPixels();//分配位图所占空间
		    memset((char *)pskBitmap->getPixels(),0,129*129*4);
		    
			bool ret = SkImageDecoder::DecodeFile(currentinfo->img_path,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);			

			if(ret){
				log_i("skBitmap->width()=%d,skBitmap->height()=%d",skBitmap.width(),skBitmap.height());

				skCanvas = new SkCanvas(*pskBitmap);
				SkIRect srcRect;
				SkRect dstRect;
				
				srcRect.set(0,0,skBitmap.width(),skBitmap.height());
				CalculateSize(skBitmap.width(),skBitmap.height(),109,109,dstRect);

				skCanvas->drawBitmapRect(skBitmap,&srcRect,dstRect);

				skCanvas->restore();
				mMSkBitmap->create((int *)pskBitmap->getPixels(),pskBitmap->width(),pskBitmap->height());
			}else{
				log_i("DecodeFile fail path=%s\n",currentinfo->img_path);
				mMSkBitmap->release();
			}
		}
#else
		BitmapFactory::decodeFile(mMSkBitmap,currentinfo->img_path,109,109);
#endif

		mAlbumImage->setMSkBitmap(mMSkBitmap);

		if(mMSkBitmap->isVaild()){
			Rect rect;
			int left = 127;
			
			rect.setEx(left, 32, 100, 20);
			mAudioInfo->setRect(rect);
			
			rect.setEx(left, 55, 190, 25);
			mMusicName->setRect(rect);

			rect.setEx(left, 85, 190, 20);
			mArtist->setRect(rect);

			rect.setEx(left, 105, 190, 20);
			mAlbum->setRect(rect);
		}else{
			Rect rect;
			int left = 20;
			
			rect.setEx(left, 32, 100, 20);
			mAudioInfo->setRect(rect);
			
			rect.setEx(left, 55, 320-left, 25);
			mMusicName->setRect(rect);

			rect.setEx(left, 85, 320-left, 20);
			mArtist->setRect(rect);

			rect.setEx(left, 105, 320-left, 20);
			mAlbum->setRect(rect);
		}
		
		mMusicName->setTextString(currentinfo->name);

		mArtist->setTextString(currentinfo->artist);

		mAlbum->setTextString(currentinfo->album);
		
		mSeekBar->setMax(mPlayinglist->getDuration());
		mSeekBar->setProgress(0);

		mstr->clear();
		mstr->setPlayTime(0);
		mTimeText->setTextString(mstr->mstr);

		mstr->clear();
		mstr->setPlayTime(mPlayinglist->getDuration());
		mDurtionText->setTextString(mstr->mstr);
		
		updatePlayMode();
		updatePlayButtonIcon();
		updateAudioInfo();
		mstr->clear();
		
	}

	void PlayingView::CalculateSize(float srcw,float srch,float dstw,float dsth,SkRect &rect){
		float scaleX,scaleY,scale;
		float w,h,l,t;
		
		scaleX = dstw/srcw;
		//log_i("dstw=%f,srcw=%f,scaleX=%f",dstw,srcw,scaleX);
		
		scaleY = dsth/srch;
		//log_i("dsth=%f,srch=%f,scaleY=%f",dsth,srch,scaleY);
		
		scale = min(scaleX,scaleY);
		//log_i("scaleX=%f,scaleY=%f,scale=%f",scaleX,scaleY,scale);
		
		w = srcw*scale;
		h = srch*scale;

		//log_i("scaleX=%f,scaleY=%f,scale=%f",scaleX,scaleY,scale);

		
		if(w<dstw)
			l = (dstw - w)/2;
		else
			l = 0;
		
		if(h<dsth)
			t = (dsth - h)/2;
		else
			t = 0;
		rect.set(l,t,l+w,t+h);
	}
	void PlayingView::updatePlayMode(){
		int playmode;
		int playModeNormalRes[4] = {IDP_PLAYMODE_0,IDP_PLAYMODE_1,IDP_PLAYMODE_2,IDP_PLAYMODE_3};
		int playModePressRes[4] = {IDP_PLAYMODE_0_S,IDP_PLAYMODE_1_S,IDP_PLAYMODE_2_S,IDP_PLAYMODE_3_S};
		
		playmode = mPlayinglist->getPlayMode();
		log_i("playmode=%d",playmode);
		if(mPlayModeButton != NULL)
			mPlayModeButton->setImageResId(playModeNormalRes[playmode],playModePressRes[playmode]);
	}

	void PlayingView::updatePlayButtonIcon(){
		int isPlay;
		isPlay = mPlayinglist->isPlaying();
		log_i("mPlayinglist->isPlaying()=%d",isPlay);
		if(isPlay)
			mPlayButton->setImageResId(IDP_PLAYING_PAUSE,IDP_PLAYING_PAUSE);
		else
			mPlayButton->setImageResId(IDP_PLAYING_PLAY_ACTIVE,IDP_PLAYING_PLAY_ACTIVE);
	}
	void PlayingView::updateAudioInfo(){
		int info[6];	
		Mstring *mstr;
		float sampleRate;
		
		memset(info,0,sizeof(int)*6);
		mstr = new Mstring(20);
		
		mPlayinglist->getAudioInfo(info);
		
		log_i("getAudioInfo:%d,%d,%d,%d,%d,%d",info[0],info[1],info[2],info[3],info[4],info[5]);
		sampleRate = info[1];
		log_i("getAudioInfo:%.1f",sampleRate);
		sampleRate = sampleRate/1000.0;
		log_i("getAudioInfo:%.1f",sampleRate);
		mstr->mfloatSprintf("%.1fKHz/",sampleRate);
		mstr->mSprintf("%dKbps",info[2]/1000);

		mAudioInfo->setTextString(mstr->mstr);
		mstr->clear();
	}
	
	unsigned int PlayingView::SeekBarRunnig(void *parameter){
			PlayingView *mplayingview = (PlayingView*)parameter;
			while(1){
				Thread::sleep(350);
				mplayingview->postMessage(mplayingview,VM_NOTIFY,NM_SEEK_UPDATE,(unsigned int)mplayingview);
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
		//log_i("PlayingView::onPaint");
		
		rect.setEx(0, 0, 320, 21);
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND, 0, 0, false);
		canvas.fillRect(rect, brush);
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 0, 211, false);

		//log_i("PlayingView::onPaint complete");
		
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
		}else if(fromView == NULL && code == NM_PLAY_COM){
			Playinglist *list = (Playinglist *)parameter;
			log_i("PlayingView::onNotify NM_PLAY_COM");
			list->callbackPlay();
			ViewInit();
		}else if(parameter == mSeekBar){
			switch(code){
				case NM_SEEKBAR_DOWM:
					isNeedFresh = 0;
					break;
				case NM_SEEKBAR_MOVE:				
					break;
				case NM_SEEKBAR_UP:
					mPlayinglist->seekTo(mSeekBar->getProgress());
					isNeedFresh = 1;
					break;
			}
		}else if(code == NM_SEEK_UPDATE){
			//log_i("isNeedFresh=%d,mPlayinglist->isPlaying()=%d",isNeedFresh,mPlayinglist->isPlaying());
			if(isNeedFresh && mPlayinglist->isPlaying()){
				Mstring* mstr;
					
				mstr = new Mstring(10);

				mstr->setPlayTime(mPlayinglist->getCurrent());
					
				mTimeText->setTextString(mstr->mstr);

				mSeekBar->setProgress(mPlayinglist->getCurrent());	
				mstr->clear();
			}
		}else if(code == NM_BATTERY_UPDATE){
			int val = (unsigned int)parameter&0xFF;
			int charge = ((unsigned int)parameter&0xF00)>>8;
			int batteryIcon = IDP_BATTERY_0;
			bool isSpdifIn;
			bool isHeadestIn;
			
			if((mBattery != val || isCharge!=charge) && isNeedFresh){
				/*
				Mstring* mstr;	
				mstr = new Mstring(10);
				mstr->mSprintf("%d%%",(unsigned int)parameter);
				mBatteryText->setTextString(mstr->mstr);
				mstr->clear();
				*/
				if(charge==1 && val!=100){
					batteryIcon=IDP_BATTERY_CHAGER;
				}else{
					if(val<5)
						batteryIcon = IDP_BATTERY_0;
					else if(val<15)
						batteryIcon = IDP_BATTERY_10;
					else if(val<30)
						batteryIcon = IDP_BATTERY_20;
					else if(val<50)
						batteryIcon = IDP_BATTERY_40;
					else if(val<70)
						batteryIcon = IDP_BATTERY_60;
					else if(val<95)
						batteryIcon = IDP_BATTERY_80;
					else
						batteryIcon = IDP_BATTERY_100;
				}
				
				mBatteryIcon->setImageResoure(batteryIcon);
				isCharge = charge;
				mBattery = val;
			}
			if(gPlayer.mSpdifSwitch->isToSwicth()){
				isSpdifIn = gPlayer.isSpdifIn();
				gPlayer.openCodecPower(!isSpdifIn);
				mPlayinglist->setSpdifOut(isSpdifIn);
				gPlayer.mSpdifSwitch->resetSwicth();
			}
			if(gPlayer.mHeadestSwitch->isToSwicth()){
				isHeadestIn = gPlayer.isHeadestIn();
				if(isHeadestIn){
					mPlayinglist->setPauseToStart();
				}else{
					mPlayinglist->setPlayPause();
				}
				updatePlayButtonIcon();
				gPlayer.mHeadestSwitch->resetSwicth();
			}
			if(gPlayer.bootLockCount){
				if(gPlayer.bootLockCount == 1)
					gPlayer.setBootWakeLock(0);
				gPlayer.bootLockCount--;
			}
		}else if(code == FLASH_MOUNT){
			gPlayer.dismissView(gPlayer.mUsmConnectView);
			mPlayinglist->checkPlayintList();
			gmediaprovider.externVolumeScanner("/mnt/sdcard");	
		}else if(code == FLASH_UNMOUNT){
			gPlayer.showUsmConnectView();
			mPlayinglist->stopPlayer();
		}else if(code == SDCARD_MOUNT){
			gPlayer.showSdcardInsertView();
		}else if(code == SDCARD_START_UNMOUNT){
			mPlayinglist->stopForSdcardEject();
		}else if(code == SDCARD_UNMOUNT){
			gPlayer.dismissView(gPlayer.mSdcardInsertView);
			mPlayinglist->checkPlayintList();
			ViewInit();
			gmediaprovider.checkfile();
		}else if(code == MEDIA_SCANNER_START){
			gPlayer.showMediaScannerView();
		}else if(code == MEDIA_SCANNER_END){
			gPlayer.dismissView(gPlayer.mMediaScannerView);
		}else if(code == NM_SPIDF){
			gPlayer.mSpdifSwitch->setPlayerSwitch();
		}else if(code == NM_HEADEST){
			gPlayer.mHeadestSwitch->setPlayerSwitch();
		}else if(code == POWER_STATUS_CHANGE){
			log_i("VM_NOTIFY : POWER_STATUS_CHANGE");
			int powerState = (unsigned int)parameter;
			log_i("powerState = %d",powerState);
			if(powerState == 0){
				isNeedFresh = isNeedFreshOld;
			}else if(powerState == 2){
				isNeedFreshOld = isNeedFresh;
				isNeedFresh = 0;
			}
			log_i("isNeedFresh = %d",isNeedFresh);
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
		log_i("onCommand id = %d",id);
		switch(id)
		{
		case PLAYING_IDB_NEXT:
			mPlayinglist->playNext();
			ViewInit();
			break;
		case PLAYING_IDB_PREV:
			mPlayinglist->playPrev();
			ViewInit();
			break;
		case PLAYING_IDB_PLAY:
			mPlayinglist->playPauseOrStart();
			updatePlayButtonIcon();
			break;
		case PLAYING_IDB_MUSIC:
			gPlayer.showMediaView();
			break;
		case PLAYING_IDB_SETTING:
			isNeedFresh = 0;
			gPlayer.showSettingsView();
			break;
		case PLAYING_SHOW_VOLUME:
			gPlayer.showVolumeView();
			break;
		case PLAYING_UPDATE_BATTERY:
			break;
		case PLAYING_IDB_PLAY_MODE:{
			int mode = mPlayinglist->getPlayMode();
			mode ++;
			if(mode >= 4)
				mode = 0;
			mPlayinglist->setPlayMode(mode);
			updatePlayMode();
			break;
			}
		}
		return -1;
	}

	int Mstring::mSprintf(const char* str,int n)
	{
		char *ptr;
		ptr = mstr + pos;
		pos+=sprintf(ptr,str,n);
		//log_i("mSprintf:mstr=%s",mstr);
		return pos;
	}

	int Mstring::mfloatSprintf(const char* str,float n)
	{
		char *ptr;
		ptr = mstr + pos;
		pos+=sprintf(ptr,str,n);
		//log_i("mSprintf:mstr=%s",mstr);
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
		//log_i("Mstring::setPlayTime n=%d,%s",n,mstr);
	}
};
