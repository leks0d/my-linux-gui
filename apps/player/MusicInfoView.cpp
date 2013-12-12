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
	MusicInfoView::MusicInfoView(void)
	{

	}

	MusicInfoView::MusicInfoView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mMSkBitmap = new MSkBitmap();
		memset(&mCurrentInfo,0,sizeof(mediainfo));
		log_i("---------");
	}

	MusicInfoView::~MusicInfoView(void)
	{

	}
	
	int MusicInfoView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		rect.setEx(0, 0, 41, 22);
		log_i("----");
		//return 0;
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(COLOR_TEXT);
		mTitle->setTextResoure(STR_SETTING_MUSICINFO);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->onCreate();

		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();

		rect.setEx(16, 36, 76, 76);
		mAlbumImage = new ImageView(PLAYING_IDB_ALBUM_IMAGE, TEXT("mAlbumImage"), this, &rect, 0);

//===============music info=======================
		
		rect.setEx(firstLeft, 36, 225, 20);
		mFilename = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mFilename->setTextResoure(STR_FILE_NAME);
		mFilename->onCreate();

		rect.setEx(firstLeft, 56, 225, 20);
		mMusicTitle = new ValueTextView(SETTING_BACK, TEXT("mMusicTitle"), this, &rect, 0);
		mMusicTitle->setTextResoure(STR_MUSIC_TITLE);
		mMusicTitle->onCreate();
		
		rect.setEx(firstLeft, 76, 225, 20);
		mAlbum = new ValueTextView(SETTING_BACK, TEXT("mAlbum"), this, &rect, 0);
		mAlbum->setTextResoure(STR_MUSIC_ALBUM);
		mAlbum->onCreate();	

		rect.setEx(firstLeft, 96, 225, 20);
		mArtist = new ValueTextView(SETTING_BACK, TEXT("mArtist"), this, &rect, 0);
		mArtist->setTextResoure(STR_MUSIC_ARTIST);
		mArtist->onCreate();
//==============middle 4    music info=======================		
		int left0,left1;
		left0 = 16; left1 = left0 + 135;
		
		rect.setEx(left0, 145, 139-14, 20);
		mDuration = new ValueTextView(SETTING_BACK, TEXT("mDuration"), this, &rect, 0);
		mDuration->setTextResoure(STR_MUSIC_DURATION);
		mDuration->onCreate();
		

		rect.setEx(left1, 145, 318-left1, 20);
		mSimpleRate = new ValueTextView(SETTING_BACK, TEXT("mSimpleRate"), this, &rect, 0);
		mSimpleRate->setTextResoure(STRSAMPLE_RATE);
		mSimpleRate->onCreate();
		

		rect.setEx(left0, 165, 139-14, 20);
		mSimpleBit = new ValueTextView(SETTING_BACK, TEXT("mSimpleBit"), this, &rect, 0);
		mSimpleBit->setTextResoure(STRSAMPLE_BIT);
		mSimpleBit->onCreate();
		

		rect.setEx(left1, 165, 318-left1, 20);
		mSimpleBps = new ValueTextView(STR_MUSIC_BPS, TEXT("mSimpleBps"), this, &rect, 0);
		mSimpleBps->setTextResoure(STR_MUSIC_BPS);
		mSimpleBps->onCreate();
//====================last path ==========		

		rect.setEx(left0, 185, 318-14, 20);
		mMusicPath = new ValueTextView(SETTING_BACK, TEXT("mMusicPath"), this, &rect, 0);
		mMusicPath->setTextResoure(STR_MUSIC_PATH);
		mMusicPath->onCreate();
		
		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}
	
	void MusicInfoView::initView()
	{
		Mstring* mstr;
		mediainfo *info;
		Rect rect;
		int firstLeft,infoWidth;

		//return;
		mstr = new Mstring(10);
		mstr->setPlayTime(mCurrentInfo.duration);
		
		BitmapFactory::decodeFile(mMSkBitmap,mCurrentInfo.img_path,76,76);

		if(mMSkBitmap->isVaild()){
			firstLeft = 98;
		}else{
			firstLeft = 20;
		}
		infoWidth = 318 - firstLeft;
		
		rect.setEx(firstLeft, 36, infoWidth, 20);
		mFilename->setRect(rect);
		rect.setEx(firstLeft, 56, infoWidth, 20);
		mMusicTitle->setRect(rect);
		rect.setEx(firstLeft, 76, infoWidth, 20);
		mAlbum->setRect(rect);
		rect.setEx(firstLeft, 96, infoWidth, 20);
		mArtist->setRect(rect);
		
		mAlbumImage->setMSkBitmap(mMSkBitmap);
		
		mMusicPath->setTextString(mCurrentInfo.path);
		log_i("mCurrentInfo->path=%s",mCurrentInfo.path);
		mFilename->setTextString(mCurrentInfo.name);
		mMusicTitle->setTextString(mCurrentInfo.title);
		mAlbum->setTextString(mCurrentInfo.album);
		mArtist->setTextString(mCurrentInfo.artist);
		mDuration->setTextString(mstr->mstr);

		delete mstr;
		mstr = NULL;
		
		updateAudioInfo();
	}
	void MusicInfoView::updateAudioInfo(){
		int info[6];	
		Mstring *mstr;
		float sampleRate;

		if((mPlayinglist->getPlayingItem() != NULL) && 
			(mCurrentInfo.id == mPlayinglist->getPlayingItem()->id)	){
			
			log_i("show playing music info.");
			memset(info,0,sizeof(int)*6);
			mstr = new Mstring(30);

			mPlayinglist->getAudioInfo(info);
			
			sampleRate = info[1];
			sampleRate = sampleRate/1000.0;
			
			mstr->mfloatSprintf("%.1fKHz",sampleRate);
			mSimpleRate->setTextString(mstr->mstr);
			
			mstr->clear();
			mstr->mSprintf("%dKbps",info[2]/1000);
			mSimpleBps->setTextString(mstr->mstr);

			mstr->clear();
			mstr->mSprintf("%dBit",info[0]);
			mSimpleBit->setTextString(mstr->mstr);
			
			delete mstr;
			mstr = NULL;
		}
		else{
			mSimpleRate->setTextString(" ");
			mSimpleBit->setTextString(" ");
			mSimpleBps->setTextString(" ");
		}
		log_i("PlayingView::updateAudioInfo end");
	}

	void MusicInfoView::setMusicInfo(mediainfo* src){
		safefreeMediainfo(&mCurrentInfo);
		
		if(src == NULL){
			//memset(&mCurrentInfo,0,sizeof(mediainfo));
			return;
		}else{
			;
		}
		
			//log_i("enter src=0x%x",src);
			memset(&mCurrentInfo,0,sizeof(mediainfo));
			mCurrentInfo.id = src->id;
			mCurrentInfo.track = src->track;
			mCurrentInfo.add_time = src->add_time;
			mCurrentInfo.duration = src->duration;
			mCurrentInfo.isInPlayList = src->isInPlayList;
			mCurrentInfo.inPlay = src->inPlay;
			mCurrentInfo.isPlayed = src->isPlayed;
			mCurrentInfo.times = src->times;
			//log_i("src->path=%s",src->path);
			
			if(src->path == NULL)
				return;
			mCurrentInfo.path = new char[strlen(src->path)+1];
			strcpy(mCurrentInfo.path,src->path);
			
			//log_i("tag src->name_key=0x%x",src->name_key);
			if(src->name != NULL){
				mCurrentInfo.name = new char[strlen(src->name)+1];
				strcpy(mCurrentInfo.name,src->name);
			}
			
			//log_i("tag ");
			if(src->name_key != NULL){
				mCurrentInfo.name_key = new char[strlen(src->name_key)+1];
				strcpy(mCurrentInfo.name_key,src->name_key);
			}
			
			//log_i("tag");
			if(src->title != NULL){
				mCurrentInfo.title = new char[strlen(src->title)+1];
				strcpy(mCurrentInfo.title,src->title);
			}else{
				//log_i("mplaylist[des].title=0x%x",mplaylist[des].title);
			}
			
			//log_i("tag");
			if(src->title_key != NULL){
				mCurrentInfo.title_key = new char[strlen(src->title_key)+1];
				strcpy(mCurrentInfo.title_key,src->title_key);
			}
			
			//log_i("tag");
			if(src->artist != NULL){
				mCurrentInfo.artist = new char[strlen(src->artist)+1];
				strcpy(mCurrentInfo.artist,src->artist);
			}
			
			//log_i("tag");
			if(src->artist_key != NULL){
				mCurrentInfo.artist_key = new char[strlen(src->artist_key)+1];
				strcpy(mCurrentInfo.artist_key,src->artist_key);
			}
			
			//log_i("tag");
			if(src->album != NULL){
				mCurrentInfo.album = new char[strlen(src->album)+1];
				strcpy(mCurrentInfo.album,src->album);
			}
			
			//log_i("tag");
			if(src->album_key != NULL){
				mCurrentInfo.album_key = new char[strlen(src->album_key)+1];
				strcpy(mCurrentInfo.album_key,src->album_key);
			}
			
			//log_i("tag");
			if(src->img_path != NULL){
				mCurrentInfo.img_path = new char[strlen(src->img_path)+1];
				strcpy(mCurrentInfo.img_path,src->img_path);
			}
			//log_i("leave");
	}

	int MusicInfoView::onDestroy()
	{
		return 0;
	}
	
	int MusicInfoView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		log_i("MediaView::onPaint complete");
		return 0;
	}


	int MusicInfoView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == mBack && NM_CLICK){
			gPlayer.dismissView(this);
		}else if(fromView == mHome && NM_CLICK){
			gPlayer.showPlayingView();
		}
		return 0;
	}


	int MusicInfoView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}


	int MusicInfoView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

};
