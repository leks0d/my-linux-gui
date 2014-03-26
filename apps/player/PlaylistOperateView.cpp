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
	PlaylistOperateView::PlaylistOperateView(void)
	{

	}

	PlaylistOperateView::PlaylistOperateView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mOperateMuiscListAdapter = NULL;
		mPlayListAdapter = NULL;
		mType = 0;
	}

	PlaylistOperateView::~PlaylistOperateView(void)
	{

	}
	
	int PlaylistOperateView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;

		rect.setEx(0,21,320,219);
		mListView = new MediaListView(TEXT("operate List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->setListViewBackground(IDP_PLAYING_LIST_BACK);
		mListView->onCreate();

		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("operatemBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();

		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("operatemTitle"), this, &rect, 0);
		mTitle->setTextColor(COLOR_TEXT);
		mTitle->onCreate();

		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("operatemBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();

		initView();
		setFocus(this);
		invalidateRect();
		return 0;
	}

	void PlaylistOperateView::initView()
	{
		Mstring* mstr;
		int brightness;

		mTitle->setTextString(mPlayListItem.name);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);

		initOperateMuiscList();
	}
	void PlaylistOperateView::initPlayList(){
		mListView->deleteAllItems();

		if(mPlayListAdapter == NULL){
			mPlayListAdapter = new PlayListAdapter(mListView); 
		}
		
		mPlayListAdapter->refresh();
		
		mTitle->setTextResoure(STR_LAYLIST_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1200);
	}

	void PlaylistOperateView::initOperateMuiscList(){
		int img[]={IDP_PLAYMODE_0,IDP_PLAYMODE_1,IDP_PLAYMODE_2,IDP_PLAYMODE_3};
		int imgsec[]={IDP_PLAYMODE_0_S,IDP_PLAYMODE_1_S,IDP_PLAYMODE_2_S,IDP_PLAYMODE_3_S};
		int text[]={STR_OPERATE_PLAY,STR_OPERATE_DELETE};
		int i,count = 2;

		log_i("mType=0x%x",mType);
		if(mType == OPERATE_TYPE_MUSIC_PLAYLIST)
			text[3] = STR_MUSIC_PLAYLIST_REMOVE;
		
		if(mOperateMuiscListAdapter == NULL){
			log_i("initOperateMuiscList mOperateMuiscListAdapter new");
			mListView->deleteAllItems();
			mOperateMuiscListAdapter = new OperateMuiscListAdapter(mListView,ADAPTER_PLAYING);
			mOperateMuiscListAdapter->setData(img,imgsec,text,count);
		}else{
			log_i("initOperateMuiscList mOperateMuiscListAdapter refresh");
			mOperateMuiscListAdapter->setData(img,imgsec,text,count);
		}
		setMainState(0x1000);		
	}
	
	int PlaylistOperateView::onDestroy()
	{
		return 0;
	}

	int PlaylistOperateView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,true);
		return 0;
	}

	int PlaylistOperateView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == mBack && NM_CLICK){
			gPlayer.dismissView(this);
		}else if(fromView == mHome && NM_CLICK){
			gPlayer.showPlayingView();
		}else if(fromView == mListView && NM_CLICK){
			LISTVIEW_RECORD* record;
			int type,index;
			
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;
			type = (int)(record->m_lvItem.paramType);
			index = (int)(record->m_lvItem.iItem);
			
			switch(getMainState()){
				case 0x1000:{
						switch(index){
							case 0:{
									ArrayMediaInfo array;
									PlayList::queryPlaylistMem(array,mPlayListItem.id);
									if(array.getCount()>0){
										mPlayinglist->clearAll();
										mPlayinglist->addArrayItem(array);
										mPlayinglist->playMediaInfo(array.getMediaInfo(0));
									}
									break;
								}
							case 1:
								PlayList::delPlaylist(mPlayListItem.id);
								break;
						}

						gPlayer.dismissView(this);
						break;
					}
				case 0x1200:{
						break;
					}
			}
			
		}
		
		return 0;
	}

	int PlaylistOperateView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int PlaylistOperateView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}
		return 0;
	}

	int PlaylistOperateView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int PlaylistOperateView::onTouchMove(int x, int y, int flag){
		log_i("DisplaySettingView::onTouchMove x=%d,y=%d",x,y);
		return -1;

	}
	int PlaylistOperateView::onTouchUp(int x, int y, int flag){
		return -1;
	}

};
