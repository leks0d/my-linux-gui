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
		mCurrentInfo = new mediainfo;
		memset(mCurrentInfo,0,sizeof(mediainfo));
	}

	MusicInfoView::~MusicInfoView(void)
	{

	}
	
	int MusicInfoView::onCreate()
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
		

		rect.setEx(left1, 145, 139-14, 20);
		mSimpleRate = new ValueTextView(SETTING_BACK, TEXT("mSimpleRate"), this, &rect, 0);
		mSimpleRate->setTextResoure(STRSAMPLE_RATE);
		mSimpleRate->onCreate();
		

		rect.setEx(left0, 165, 139-14, 20);
		mSimpleBit = new ValueTextView(SETTING_BACK, TEXT("mSimpleBit"), this, &rect, 0);
		mSimpleBit->setTextResoure(STRSAMPLE_BIT);
		mSimpleBit->onCreate();
		

		rect.setEx(left1, 165, 139-14, 20);
		mSimpleBps = new ValueTextView(STR_MUSIC_BPS, TEXT("mSimpleBps"), this, &rect, 0);
		mSimpleBps->setTextResoure(STR_MUSIC_BPS);
		mSimpleBps->onCreate();
//====================last path ==========		

		rect.setEx(left0, 185, 320-14, 20);
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
		
		log_i("setPlayTime mCurrentInfo.duration");
		
		mstr = new Mstring(10);
		mstr->setPlayTime(mCurrentInfo->duration);
		
		BitmapFactory::decodeFile(mMSkBitmap,mCurrentInfo->img_path,76,76);

		if(mMSkBitmap->isVaild()){
			firstLeft = 98;
		}else{
			firstLeft = 20;
		}
		infoWidth = 320 - firstLeft;
		
		rect.setEx(firstLeft, 36, infoWidth, 20);
		mFilename->setRect(rect);
		rect.setEx(firstLeft, 56, infoWidth, 20);
		mMusicTitle->setRect(rect);
		rect.setEx(firstLeft, 76, infoWidth, 20);
		mAlbum->setRect(rect);
		rect.setEx(firstLeft, 96, infoWidth, 20);
		mArtist->setRect(rect);
		
		mAlbumImage->setMSkBitmap(mMSkBitmap);
		
		mMusicPath->setTextString(mCurrentInfo->path);

		mFilename->setTextString(mCurrentInfo->name);
		mMusicTitle->setTextString(mCurrentInfo->title);
		mAlbum->setTextString(mCurrentInfo->album);
		mArtist->setTextString(mCurrentInfo->artist);
		mDuration->setTextString(mstr->mstr);

		mstr->clear();
	}

	void MusicInfoView::setMusicInfo(mediainfo* info){
		if(info == NULL){
			memset(mCurrentInfo,0,sizeof(mediainfo));
		}else{
			memcpy(mCurrentInfo,info,sizeof(mediainfo));
		}
	}

	int MusicInfoView::onDestroy()
	{
		return 0;
	}
	
	int MusicInfoView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
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
