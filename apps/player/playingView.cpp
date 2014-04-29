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
		isUsmCon = 0;
		isMediaScanning = 0;
		isSdcardShare = 0;
	}


	PlayingView::~PlayingView(void)
	{

	}


	int PlayingView::onCreate()
	{
		int left = 127;
		int volumeX = 143 ;
		Rect rect;
		log_i("PlayingView::onCreate()");
		//return 0;
		rect.setEx(10, 2, 52, 16);
		mEqState = new TextView(-1, TEXT("mEqStateText"), this, &rect, 0);
		mEqState->setTextSize(14);
		mEqState->setTextColor(COLOR_TEXT);
		mEqState->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mEqState->onCreate();
		
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
		
		rect.setEx(145, 178, 32, 30);
		mPlayButton = new Button(PLAYING_IDB_PLAY, TEXT("mPlayButton"), this, &rect, 0);
		mPlayButton->setNormalImageId(IDP_PLAYING_PLAY_ACTIVE);
		mPlayButton->setPressedImageId(IDP_PLAYING_PLAY_ACTIVE);
		mPlayButton->onCreate();

		rect.setEx(290, 28, 40, 22);
		mPlayModeButton = new Button(PLAYING_IDB_PLAY_MODE, TEXT("mPlayModeButton"), this, &rect, 0);
		
		rect.setEx(250, 28, 40, 22);
		mAddToPlaylistButton = new Button(PLAYING_IDB_ADD_TO_PLAYLIST, TEXT("mAddToPlaylistButton"), this, &rect, 0);
		
		rect.setEx(6, 30, 109, 109);
		mAlbumImage = new ImageView(PLAYING_IDB_ALBUM_IMAGE, TEXT("mAlbumImage"), this, &rect, 0);
		if(gPlayer.mProductType==1)
			mAlbumImage->setImageResoure(IDP_MUSICINFO_ICON_DX90);
		else
			mAlbumImage->setImageResoure(IDP_MUSICINFO_ICON);

		mAlbumImage->setBitmapAlps(true);
		
		rect.setEx(left, 32, 120, 20);
		mAudioInfo = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mAudioInfo"), this, &rect, 0);
		mAudioInfo->setTextColor(COLOR_GRAY);
		mAudioInfo->setTextSize(15);

		rect.setEx(left, 55, 190, 25);
		mMusicName = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mMusicName"), this, &rect, 0,SW_HIDE);
		mMusicName->setTextColor(RGB(248,136,0));
		mMusicName->setTextSize(23);

		rect.setEx(left, 55, 188, 25); 
		mMusicNameStatic = new StaticView(TEXT("mMusicName"), this, &rect, 0);
		mMusicNameStatic->setTextColor(COLOR_PLAY_ORANGE);
		mMusicNameStatic->setTextSize(23);
		mMusicNameStatic->setTextBkRes(IDP_PLAYING_MUSICNAME_BK);
		
		rect.setEx(left, 90, 190, 25);
		mArtistNameStatic = new StaticView(TEXT("mArtist"), this, &rect, 0);
		mArtistNameStatic->setTextColor(RGB(154,154,154));
		mArtistNameStatic->setTextSize(16);
		mArtistNameStatic->setTextBkRes(IDP_PLAYING_MUSICNAME_BK);

		rect.setEx(left, 115, 190, 30);
		mAlbumNameStatic = new StaticView(TEXT("mArtist"), this, &rect, 0);
		mAlbumNameStatic->setTextColor(RGB(154,154,154));
		mAlbumNameStatic->setTextSize(16);
		mAlbumNameStatic->setTextBkRes(IDP_PLAYING_MUSICNAME_BK);
		/*
		rect.setEx(left, 85, 190, 20);
		mArtist = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mArtist"), this, &rect, 0);
		mArtist->setTextColor(RGB(154,154,154));
		mArtist->setTextSize(16);

		rect.setEx(left, 105, 190, 20);
		mAlbum = new TextView(PLAYING_IDB_MUSIC_NAME, TEXT("mAlbum"), this, &rect, 0);
		mAlbum->setTextColor(RGB(154,154,154));
		mAlbum->setTextSize(16);*/
		
		rect.setEx(volumeX, 0, 20, 21);
		mVolumeButton = new  Button(13, TEXT("mVolumeButton"), this, &rect, 0);
		mVolumeButton->setNormalImageId(DPI_VOLUME_ICON);
		mVolumeButton->setPressedImageId(DPI_VOLUME_ICON);
		mVolumeButton->onCreate();
		
		rect.setEx(volumeX+20, 2, 20, 16);
		mVolumeText = new TextView(-1, TEXT("mVolumeText"), this, &rect, 0);
		mVolumeText->setTextSize(13);
		mVolumeText->setTextColor(COLOR_TEXT);
		mVolumeText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mVolumeText->onCreate();
		
		rect.setEx(290, 4, 320-285, 19);
		mBatteryIcon = new  ImageView(-1, TEXT("mBatteryIcon"), this, &rect, 0);
		mBatteryIcon->setImageResoure(IDP_BATTERY_0);
		mBatteryIcon->setBitmapAlps(true);
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
		mTimeText->setTextColor(COLOR_TEXT);
		mTimeText->setTextSize(13);
		mTimeText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mTimeText->onCreate();
		
		rect.setEx(270, 166, 50, 15);		
		mDurtionText = new  TextView(-1, TEXT("mDurtionText"), this, &rect, 0);
		mDurtionText->setTextColor(COLOR_TEXT);
		mDurtionText->setTextLayoutType(TEXT_LAYOUT_LEFT);
		mDurtionText->setTextSize(13);
		mDurtionText->onCreate();

		rect.setEx(0, 213, 115, 27);		
		mMyMusicText = new  TextView(PLAYING_IDB_MUSIC, TEXT("mMyMusicText"), this, &rect, 0);
		mMyMusicText->setTextColor(COLOR_TEXT);
		mMyMusicText->setTextSelectColor(COLOR_ORANGE);
		mMyMusicText->setTextSize(16);
		mMyMusicText->setTextResoure(MUSIC_MY_MUSIC);
		mMyMusicText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mMyMusicText->onCreate();

		rect.setEx(205, 213, 115, 27);		
		mSettingText = new  TextView(PLAYING_IDB_SETTING, TEXT("mSettingText"), this, &rect, 0);
		mSettingText->setTextColor(COLOR_TEXT);
		mSettingText->setTextSelectColor(COLOR_ORANGE);
		mSettingText->setTextSize(16);
		mSettingText->setTextResoure(MUSIC_MUSIC_FUN);
		mSettingText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mSettingText->onCreate();
		isNeedFresh = 1;
		ViewInit();
		log_i("mSeekBarUpdateThread.create.");
		mSeekBarUpdateThread.create(PlayingView::SeekBarRunnig, this);
		log_i("PlayingView::onCreate leave.");
		return 0;
	}

	void PlayingView::ViewInit(void){
		Mstring* mstr;
		mediainfo* currentinfo;
		int duration = 0;
		int current = 0;
		
		currentinfo = mPlayinglist->getPlayingItem();

		mstr = new Mstring(20);
	
		mstr->mSprintf("%d",gPlayer.getVolume());
		mVolumeText->setTextString(mstr->mstr);
		
		updatePlayMode();
		updatePlayButtonIcon();
		updateAudioInfo();
		updateEqState();
	
		if(currentinfo == NULL){
			log_i("tag");
			mMusicName->setTextResoure(STR_NO_MUSIC);
			mMusicNameStatic->setTextResoure(STR_NO_MUSIC);
			mAudioInfo->setTextString(NULL);
			mAlbumNameStatic->setTextString(NULL);
			mArtistNameStatic->setTextString(NULL);
			mAddToPlaylistButton->setNormalImageId(-1);
			mAlbumImage->setMSkBitmap(NULL);
			return;
		}
		mAddToPlaylistButton->setNormalImageId(IDP_PLAYING_MUSIC_ADD_PLAYLIST);
#if 0
		if( (currentinfo->img_path == NULL) || (strcmp(currentinfo->img_path,"(null)") == 0)){
			mMSkBitmap->release();
		}else{
			SkBitmap skBitmap,*pskBitmap;
			SkCanvas *skCanvas;
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,129,129);
		    pskBitmap->allocPixels();//����λͼ��ռ�ռ�
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
		//log_i("tag currentinfo->img_path=0x%x:%s",currentinfo->img_path,currentinfo->img_path);
		mMSkBitmap->createFile(currentinfo->img_path);
		if(!mMSkBitmap->isVaild()){
			if(currentinfo->img_path!=NULL && currentinfo->img_path[0] == '/'){
				char sdcardPath[1024]={"/mnt/external_sd/.audio_data/album_img/"};
				strcat(sdcardPath,currentinfo->img_path + sizeof("/mnt/sdcard/.album_img"));
				mMSkBitmap->createFile(sdcardPath);
			}
		}
		//BitmapFactory::decodeFile(mMSkBitmap,"/mnt/sdcard/cover.jpg",109,109);
#endif

		//log_i("tag");
		mAlbumImage->setMSkBitmap(mMSkBitmap);
		//log_i("tag 0x%s",currentinfo->title);
		mMusicName->setTextString(currentinfo->title);
		//log_i("tag 0x%x",currentinfo->title);
		mMusicNameStatic->setTextString(currentinfo->title);
		//log_i("tag");
		mArtistNameStatic->setTextString(currentinfo->artist);
		//log_i("tag");
		mAlbumNameStatic->setTextString(currentinfo->album);
		//log_i("tag");
		
		duration = mPlayinglist->getDuration();
		if(duration<=1 && currentinfo->duration!=0){
			duration = currentinfo->duration;
			log_i("invail duration,use currentinfo->duration=%d",duration);
		}
		
		current = mPlayinglist->getCurrent();
		if(current<0||current>duration){
			current = 0;
			log_i("error current,clear current");
		}

		if(currentinfo->isCue){
			duration = currentinfo->duration;
			current = current - currentinfo->cueStart;
			if(current<0){
				current = 0;
			}
		}else if(currentinfo->add_time>0 && !mPlayinglist->isPlaying() && !mPlayinglist->isPause()){
			current = currentinfo->add_time;
			if( duration<=1 || (duration>1 && current>duration)){
				current = 0;
			}
		}
		
		mSeekBar->setMax(duration);
		mSeekBar->setProgress(current);
		
		mstr->clear();
		mstr->setPlayTime(duration);
		mDurtionText->setTextString(mstr->mstr);
		
		mstr->clear();
		mstr->setPlayTime(current);
		mTimeText->setTextString(mstr->mstr);
	
		delete mstr;
		
		log_i("PlayingView::ViewInit end");	
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
	void PlayingView::updateEqState(){
		int eqMode[]={STR_EQ_CUSTOM,STR_EQ_POP,STR_EQ_ROCK,STR_EQ_CLASSICAL,STR_EQ_DANCE,STR_EQ_BASS};
		int mode,state;

		gSettingProvider.query(SETTING_EQSTATE_ID,&state);
		gSettingProvider.query(SETTING_EQMODE_ID,&mode);

		log_i("SETTING_EQSTATE_ID state = %d",state);
		if(state){
			mEqState->setTextResoure(eqMode[mode]);
		}else{
			mEqState->setTextResoure(0);
		}
	}
	
	void PlayingView::updateAudioInfo(){
		int info[6];
		Mstring *mstr;
		float sampleRate;
		
		memset(info,0,sizeof(int)*6);
		mstr = new Mstring(30);

		mPlayinglist->getAudioInfo(info);
		
		sampleRate = info[1];
		sampleRate = sampleRate/1000.0;
		
		mstr->mSprintf("%dBit ",info[0]);
		mstr->mfloatSprintf("%.1fKHz",sampleRate);
		
		
		mAudioInfo->setTextString(mstr->mstr);

		delete mstr;
		mstr = NULL;
		log_i("PlayingView::updateAudioInfo end");
	}
	
	unsigned int PlayingView::SeekBarRunnig(void *parameter){
			PlayingView *mplayingview = (PlayingView*)parameter;
			while(1){
				Thread::sleep(350);
				mplayingview->postMessage(mplayingview,VM_NOTIFY,NM_SEEK_UPDATE,(unsigned int)mplayingview);
			}
			return 0;
	}
	unsigned int PlayingView::shutdownRunnig(void *parameter){
		gPlayer.shutDown();
		Thread::sleep(1000);
		log_i("reboot(RB_POWER_OFF)");
		reboot(RB_POWER_OFF);
	}
	unsigned int PlayingView::updateSDcardRunnig(void *parameter){
		int p = (int)parameter;
		
		if(p == 1){
			if(Environment::isSDcardChange())
				gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,SDCARD_FILE_CHECK,0);
		}
		
		Environment::updateSDcard();
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
		Brush brush(ARGB(100,25, 25, 25));
		//log_i("PlayingView::onPaint");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,true);
		canvas.drawImageResource(IDP_BACKGROUND_BOTTOM, 1, 211, true);

		//log_i("PlayingView::onPaint complete");
		
		return 0;
	}

	int PlayingView::onNotify(View* fromView, int code, void* parameter)
	{
		//log_i("code = %d,NM_POWER_OFF=%d",code,NM_POWER_OFF);
		if(fromView == NULL && code == NM_DISPLAY){
			log_i("PlayingView::onNotify NM_DISPLAY");
			ViewInit();
			if(gPowerManager->getPowerState() == 0)
				isNeedFresh = 1;
			log_i("PlayingView::onNotify NM_DISPLAY leave");
		}else if(fromView == NULL && code == NM_DISMISS){
			log_i("PlayingView::onNotify NM_DISMISS");
			isNeedFresh = 0;
		}else if(fromView == NULL && code == NM_PLAY_COM){
			Playinglist *list = (Playinglist *)parameter;
			list->callbackPlay();
			ViewInit();
		}else if(parameter == mSeekBar){
			switch(code){
				case NM_SEEKBAR_DOWM:
					isNeedFresh = 0;
					break;
				case NM_SEEKBAR_MOVE:
					if(mSeekBar!=NULL){
						Mstring* mstr;
						int progress = mSeekBar->getProgress();
						mediainfo *info = mPlayinglist->getPlayingItem();
						
						if(progress<0)
							progress = 0;
						if(progress>mSeekBar->getMax())
							progress=mSeekBar->getMax();
						
						mstr = new Mstring(20);
						mstr->clear();
						mstr->setPlayTime(progress);
						mTimeText->setTextString(mstr->mstr);
						delete mstr;
					}
					break;
				case NM_SEEKBAR_UP:{
						mediainfo *info;
						int cur;
						isNeedFresh = 1;
						cur = mPlayinglist->getCurrent();
						if(cur<2000)
							break;
						else{
							info = mPlayinglist->getPlayingItem();
							if(info->isCue){
								int seek;
								seek = mSeekBar->getProgress() + info->cueStart;
								if(seek<info->cueStart+info->duration)
									mPlayinglist->seekTo(seek);
								else
									log_i("inval seek posistion seek=%d,cueStart+duration=%d",seek,info->cueStart+info->duration);
							}else{
								mPlayinglist->seekTo(mSeekBar->getProgress());
							}
						}
					}
					
					break;
			}
		}else if(code == NM_SEEK_UPDATE){
/******************* ��δ��������ж�cue����һ��*****************/
			if(mPlayinglist!=NULL&&mPlayinglist->isPlaying()){
				mediainfo *info;
				int cur;
				
				info = mPlayinglist->getPlayingItem();
				
				//log_i("iscue=%d",info->isCue);
				
				if(info->isCue){
					cur = mPlayinglist->getCurrent();
					
					//log_i("cur=%d,cueStart+duration=%d",cur,info->cueStart + info->duration);
					
					
					if(cur > info->cueStart + info->duration){
						mPlayinglist->callbackPlay();
						ViewInit();
						return 0;
					}
				}
			}
/*********************************END****************************************/
			//log_i("isNeedFresh=%d",isNeedFresh);

			if(isNeedFresh){
			
				if(mPlayinglist->isPlaying()){
					
					Mstring* mstr;
					mediainfo *info;

					info = mPlayinglist->getPlayingItem();
					
					if(mSeekBar!=NULL){
						
						int max = mSeekBar->getMax();
						int cur = mPlayinglist->getCurrent();

						if(info->isCue){
							cur = cur - info->cueStart;
							if(cur<0)
								cur = 0;
						}
						
						if(cur>=0&&cur<=max){
							
							mstr = new Mstring(20);
							
							mstr->clear();
							mstr->setPlayTime(cur);
							mTimeText->setTextString(mstr->mstr);

							mSeekBar->setProgress(cur);
							
							delete mstr;
						}else{
							log_i("inval Progress isCue=%d,cur =%d,max=%d",info->isCue,cur,max);
						}
					}
				}
				
				//log_i("NM_SEEK_UPDATE end");
			}
		}
		else if(code == NM_BATTERY_UPDATE){
			int val = (unsigned int)parameter&0xFFFF;
			int charge = ((unsigned int)parameter&0xF0000)>>16;
			int batteryIcon = IDP_BATTERY_0;
			bool isSpdifIn;
			bool isHeadestIn;

			//log_i("voltage_now val=%d",val);
					
			if((mBattery != val || isCharge!=charge) && isNeedFresh){
				if(charge==1 && val!=100){
					batteryIcon=IDP_BATTERY_CHAGER;
				}else{
					if(val<3660)
						batteryIcon = IDP_BATTERY_0;
					else if(val<3730)
						batteryIcon = IDP_BATTERY_10;
					else if(val<3780)
						batteryIcon = IDP_BATTERY_20;
					else if(val<3830)
						batteryIcon = IDP_BATTERY_40;
					else if(val<3950)
						batteryIcon = IDP_BATTERY_60;
					else if(val<4080)
						batteryIcon = IDP_BATTERY_80;
					else
						batteryIcon = IDP_BATTERY_100;
				}
				
				mBatteryIcon->setImageResoure(batteryIcon);
				isCharge = charge;
				mBattery = val;
			}
			if(gPlayer.mSpdifSwitch->isToSwicth()){
				if(gPlayer.mCodecType == 0){
					isSpdifIn = gPlayer.isSpdifIn();
					mPlayinglist->setSpdifOut(isSpdifIn);
				}
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
			if(gPlayer.bootLockCount>0){
				if(gPlayer.bootLockCount == 1)
					gPlayer.setBootWakeLock(0);
				gPlayer.bootLockCount--;
			}
			mKeyCount.TriggerKey();
		}
		else if(code == FLASH_MOUNT){
			//Environment::logcat();
			//Environment::openadb();
			Environment::checkWallpaper();
			
			mPlayinglist->checkPlayintList(FLASH_PATH);
			
			if(isUsmCon == 1){
				gmediaprovider.externVolumeScanner("/mnt/sdcard");
			}else if(!FileAttr::FileExist(SDCARD_BLOCK_PATH)&&Environment::isSDcardChange()){
				gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,SDCARD_UNMOUNT,0);
			}else{
				ViewInit();
			}
			isUsmCon = 0;
			gPlayer.dismissView(gPlayer.mUsmConnectView);
		}else if(code == FLASH_UNMOUNT){
			isUsmCon = 1;
			gPlayer.showUsmConnectView();
			mPlayinglist->stopPlayer();
		}else if(code == SDCARD_MOUNT){
			int firstMount = 0;
			
			if(Environment::sdcardNeedScanner()){
				gPlayer.showSdcardInsertView();
			}else{		//not exist boot file,mean to boot up with existing SD and first mount.
				firstMount = 1;
			}

			if(isSdcardShare && firstMount)
				mPlayinglist->checkPlayintList(SDCARD_PATH);

			if(isSdcardShare)
				gmediaprovider.externFileCheck();
			
			mUpdateSDcardThread.create(updateSDcardRunnig,(void*)firstMount);
			isSdcardShare = 0;
		}else if(code == SDCARD_START_UNMOUNT){
			mPlayinglist->stopForSdcardEject();
			
			if(gmediaprovider.mediaCanStop())
				gmediaprovider.stopMediaScanner();
		}else if(code == SDCARD_UNMOUNT){
			isSdcardShare = 0;
			gPlayer.dismissView(gPlayer.mSdcardInsertView);
			mPlayinglist->checkPlayintList(SDCARD_PATH);
			gmediaprovider.deleteMusicOnDir(SDCARD_PATH);
			Environment::updateSDcard();
			ViewInit();
		}else if(code == MEDIA_SCANNER_START){
			isMediaScanning = 1;
			gPlayer.showMediaScannerView();
		}else if(code == MEDIA_SCANNER_END){
			isMediaScanning = 0;
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
		}else if(code == NM_POWER_OFF){
			log_i("----------NM_POWER_OFF");
			gPlayer.showShutDownView();
			mShutdowmThread.create(PlayingView::shutdownRunnig, this);
		}else if(code == SDCARD_FILE_CHECK){
			gmediaprovider.externFileCheck();
		}else if(code == SDCARD_SHARE_UNMOUNT){
			isSdcardShare = 1;
		}else if(code == USBHIFI_AUDIO_MSG){
			mPlayinglist->stopPlayer();
			Environment::openMute();
			gPlayer.openWm8740Mute();
			gPlayer.closeWm8740Mute();
			gPlayer.showUSBAudioConnectView();
			//if(gPlayer.mCodecType==1)	
			//	gPlayer.setHardwareVolume(200);
		}else if(code == USBHIFI_AUDIO_STOP){
			system("stop usbd");
			gPlayer.dismissView(gPlayer.mUSBAudioConnectView);
			system("start usbd");
			//if(gPlayer.mCodecType==1)	
			//	gPlayer.setHardwareVolume(255);
			
			log_i("----------USBHIFI_AUDIO_STOP");
		}
		return 0;
	}


	int PlayingView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
			case KEYCODE_PREV:
			case KEYCODE_NEXT:
				mKeyCount.initKeyPress(keyCode);
				break;
		}

		return 0;
	}


	int PlayingView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode)	{
			case KEYCODE_BACK:
				break;
			case KEYCODE_PREV:
			case KEYCODE_NEXT:
				if(!mKeyCount.isKeyPress(keyCode)){
					if(keyCode == KEYCODE_NEXT){
						gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_NEXT,NULL);
					}else if(keyCode == KEYCODE_PREV){
						gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_PREV,NULL);
					}
				}
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
			mPlayinglist->playNext(mPlayinglist->isPlaying());
			ViewInit();
			break;
		case PLAYING_IDB_PREV:
			if(mPlayinglist->isPlaying()){
				int cur,des;

				mediainfo* info;
				info =mPlayinglist->getPlayingItem();
				
				if(info){
					if(info->isCue){
						cur = mPlayinglist->getCurrent() - info->cueStart;
						des = info->cueStart;
					}else{
						cur = mPlayinglist->getCurrent();
						des = 0;
					}
				}
					
				if(cur>5000)
					mPlayinglist->seekTo(des);
				else
					mPlayinglist->playPrev();
			}else{
				mPlayinglist->playPrev(false);
			}
			ViewInit();
			break;
		case PLAYING_IDB_PLAY:{
			int ret = 0;
			ret = mPlayinglist->playPauseOrStart();
			updatePlayButtonIcon();
			if(ret == 1)
				updateAudioInfo();
			break;
		}
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
		case PLAYING_IDB_ADD_TO_PLAYLIST:{
				mediainfo info,*pinfo;
				pinfo = mPlayinglist->getPlayingItem();
				if(pinfo){
					info = *pinfo;
					gPlayer.showMusicOperateView(info,0,1);
				}
				break;
			}
		}
		log_i("onCommand id = %d end",id);
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
	int Mstring::mStringSprintf(const char* str,char *s){
		char *ptr;
		ptr = mstr + pos;
		pos+=sprintf(ptr,str,s);
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
		
		if(min>=10)
			ptr+=sprintf(ptr,"%d:",min);
		else
			ptr+=sprintf(ptr,"0%d:",min);

		if(sec>=10)
			ptr+=sprintf(ptr,"%d",sec);
		else
			ptr+=sprintf(ptr,"0%d",sec);
		pos = ptr - mstr;
		//log_i("Mstring::setPlayTime n=%d,%s",n,mstr);
	}
};
