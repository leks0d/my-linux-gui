#include "player.h"


namespace mango
{
#define LIST_MUSIC_ICON_LEFT 20

	enum
	{
		ADAPTER_PLAYING = 0xf0c0,	
		ADAPTER_FILE = 0xf0c1,
		ADAPTER_ALBUM = 0xf0c2,
		ADAPTER_ARTIST = 0xf0c3,
		ADAPTER_MUSIC = 0xf0c4,
	};

	static void strlwr(char *string){
		int wid = 'a' - 'A';
		while( (*string) != '\0'){
			if(*string <= 'Z' && *string >= 'A')
				*string += wid;
			string++;
		}
			
	}
	static char * getfilename(char *path){
		int i,len = 0;
		if(path == 0)
			return 0;
		len = strlen(path);
		for(i = len-1 ; i > 0 ; i--){
			if(*(path+i) == '/' )
				return path+i+1;
		}
	}
	static char * getparent(char *path,char *parent){
		int i,len = 0;
		if(path == 0)
			return 0;
		len = strlen(path);
		memset(parent,0,len);
		for(i = len-1 ; i > 0 ; i--){
			if(*(path+i) == '/' ){
				memcpy(parent,path,i);
				break;
			}
		}
		//*(parent+i) = '\0';
	}
	static char * getfiletype(char *file){
		int i,len = 0;
		char type[10];
		if(file == 0)
			return 0;
		len = strlen(file);
		i = len-1;
		while(*(file+i) != '.'){
			//log_i("*(file+i)=%c\n",*(file+i))
			i--;
			if(i==0)
				break;
		}
		if(len-i>5||i == 0)
			return 0;
		strcpy(type,file+i+1);
		strlwr(type);
		return type;
	}	
	MediaListView::MediaListView(void)
	{

	}


	MediaListView::MediaListView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: ListView(title, parent, rect, style, show)
	{

	}


	MediaListView::~MediaListView(void)
	{

	}


	int MediaListView::compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2)
	{
		if (((DWORD)(reocrd1->m_lvItem.lParam) & FILE_ATTRIBUTE_DIRECTORY) != ((DWORD)(reocrd2->m_lvItem.lParam) & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (((DWORD)(reocrd1->m_lvItem.lParam) & FILE_ATTRIBUTE_DIRECTORY))
				return -1;
			else
				return 1;

		} else	{
			return String::lstrcmpi(reocrd1->m_lvItem.pszText, reocrd2->m_lvItem.pszText) ;
		}
	}

	
	MediaView::MediaView(void)
	{

	}


	MediaView::MediaView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mMusicAdapter = NULL;
		mPlayingListAdapter = NULL;
		mAlbumAdapter = NULL;
		mArtistAlbumAdapter = NULL;
		mArtistAdapter = NULL;
		mMainListAdapter = NULL;
		mRootDirectListAdapter = NULL;
		mAlbumMusicAdapter = NULL;
		mArtistMusicAdapter = NULL;
		mPlayListAdapter = NULL;
		mPlayListMemAdapter = NULL;
		mGenreAdapter = NULL;
		mGenreMusicAdapter = NULL;
		
		mNeedPlayPath = NULL;
		mOrderMenuShow = 0;
		mCurrentPlaylistId = -1;
	}


	MediaView::~MediaView(void)
	{

	}
	

	int MediaView::onCreate()
	{	
		Rect rect;
		int menuBkX = 5,orderTextX = menuBkX+3,orderDiv = 78,orderTextY = 21;

		rect.setEx(0,21,318,219);
		mListView = new MediaListView(TEXT("Media List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->setListViewBackground(IDP_PLAYING_LIST_BACK);
		mListView->onCreate();
		
		rect.setEx(menuBkX,21,315,43);
		mOrderMenuBk = new ImageView(50, TEXT("Media List"), this, &rect, LVS_LIST,SW_HIDE);
		mOrderMenuBk->setImageResoure(IDP_MUSIC_ORDERBY);
		mOrderMenuBk->setBitmapStart(0,-2);
		mOrderMenuBk->setBitmapAlps(true);
		mOrderMenuBk->onCreate();

		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(RGB(255,255,255));
		mTitle->setIconRes(IDP_ORDER_SEC);
		mTitle->onCreate();

		rect.setEx(orderTextX, orderTextY, orderDiv, 40);
		mOrderByTitle = new TextView(MEDIA_ORDER_TILE, TEXT("mOrderByTitle"), this, &rect,0,SW_HIDE);
		mOrderByTitle->setTextColor(RGB(255,255,255));
		mOrderByTitle->setTextSelectColor(RGB(255,148,0));
		mOrderByTitle->setTextResoure(STR_ORDERBY_TITLE);
		mOrderByTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mOrderByTitle->onCreate();
		orderTextX+=78;
		rect.setEx(orderTextX, orderTextY, orderDiv, 40);
		mOrderByAlbum = new TextView(MEDIA_ORDER_ALBUM, TEXT("mOrderByAlbum"), this, &rect,0,SW_HIDE);
		mOrderByAlbum->setTextColor(RGB(255,255,255));
		mOrderByAlbum->setTextSelectColor(RGB(255,148,0));
		mOrderByAlbum->setTextResoure(STR_ORDERBY_ALBUM);
		mOrderByAlbum->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mOrderByAlbum->onCreate();
		orderTextX+=78;
		rect.setEx(orderTextX, orderTextY, orderDiv, 40);
		mOrderByArtist = new TextView(MEDIA_ORDER_ARTIST, TEXT("mOrderByArtist"), this, &rect,0,SW_HIDE);
		mOrderByArtist->setTextColor(RGB(255,255,255));
		mOrderByArtist->setTextSelectColor(RGB(255,148,0));
		mOrderByArtist->setTextResoure(STR_ORDERBY_ARTIST);
		mOrderByArtist->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mOrderByArtist->onCreate();
		orderTextX+=78;
		rect.setEx(orderTextX, orderTextY, orderDiv, 40);
		mOrderByTime = new TextView(MEDIA_ORDER_TIME, TEXT("mOrderByTime"), this, &rect,0,SW_HIDE);
		mOrderByTime->setTextColor(RGB(255,255,255));
		mOrderByTime->setTextSelectColor(RGB(255,148,0));
		mOrderByTime->setTextResoure(STR_ORDERBY_TIME);
		mOrderByTime->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mOrderByTime->onCreate();
		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_HOME, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();
		
#ifdef WIN32
		String::copy(mCurrentPath, TEXT("E:"));
#else
		String::copy(mCurrentPath, TEXT("/mnt"));
#endif
		//renewFillViewList();
		initMainList();
		setFocus(this);

		invalidateRect();
		log_i("leave\n");
		return 0;
	}
	void MediaView::showOrderByMenu(){
		int state = SW_NORMAL;
		if(state!= mOrderMenuBk->getShowState()){
			mOrderMenuBk->setShowState(state);
			mOrderByTitle->setShowState(state);
			mOrderByArtist->setShowState(state);
			mOrderByAlbum->setShowState(state);
			mOrderByTime->setShowState(state);
		}
	}
	void MediaView::dismissOrderByMenu(){
		int state = SW_HIDE;
		if(state != mOrderMenuBk->getShowState()){
			mOrderMenuBk->setShowState(state);
			mOrderByTitle->setShowState(state);
			mOrderByArtist->setShowState(state);
			mOrderByAlbum->setShowState(state);
			mOrderByTime->setShowState(state);
		}
	}	
	bool MediaView::menuIsShow(){
		return mOrderMenuBk->getShowState()==1?true:false;
	}
	int MediaView::onDestroy()
	{
		if (mListView)
			mListView->onDestroy();
		
		safeDelete(mListView);
		return 0;
	}
	int MediaView::onPaint(Canvas& canvas)
	{
		//log_i("MediaView::onPaint");
		
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND,0,0,false);		
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		return 0;
	}

	int MediaView::onCommand(int id, int code, View* fromView)
	{
			int volume;
	
			switch(id)
			{
				case SETTING_BACK:
					backEvent();
					break;
				case SETTING_HOME:
					gPlayer.showPlayingView();
					break;
				case SETTING_TITLE:{
						switch(getMainState()){
							case 0x1300:
								if(menuIsShow())
									dismissOrderByMenu();
								else
									showOrderByMenu();
								break;
						}			
						break;
					}
				case MEDIA_ORDER_TILE:
				case MEDIA_ORDER_ALBUM:
				case MEDIA_ORDER_ARTIST:
				case MEDIA_ORDER_TIME:
					dismissOrderByMenu();
					mListView->deleteAllItems();
					mMusicAdapter->setMusicOrderby(id);
					break;
			}
			return -1;
	}


	void MediaView::insertFileToListView(UINT mask, TCHAR* name, int iconId, void* param,int itext,int paramtype)
	{
		ListViewItem  lvItem;

		lvItem.mask     = mask;//LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvItem.iItem    = 0;
		lvItem.iSubItem = 0;
		lvItem.pszText  = name;
		lvItem.iImage   = iconId;
		lvItem.iText   = itext;
		lvItem.lParam   = (void *)param;
		lvItem.paramType = paramtype;

		mListView->insertItem(&lvItem);
	}


	void MediaView::fillDirectoryFile(TCHAR*  path)
	{
		int		count = 0;
		TCHAR	fileName[MAX_PATH];
		int     fileAttribute;
		File    file;
		char    cName[255];
		
		if (!file.findOpen(mCurrentPath)) {
			log_e ("findOpen") ;
			return ;
		}

		while(file.findNext(fileName, &fileAttribute,cName))
		{
			//log_i("cName = %s",cName);
			if (fileAttribute & FILE_ATTRIBUTE_HIDDEN)
				continue;
			if(cName[0] == '.')
					continue;
			if (fileAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{
			
				if (String::lstrcmp (fileName, TEXT (".")) == 0)
					continue;

				if (String::lstrcmp (fileName, TEXT ("..")) == 0)
					continue;

				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_FOLDER_ICON, (void*)FILE_ATTRIBUTE_DIRECTORY);
				count++;
			}
			else if (ismusic(cName) //|| isCueFile(cName)
				) //PathMatchSpecEx (FindFileData.cFileName, pctl->lpofn->lpstrFilter, PMSF_MULTIPLE))
			{
			//	mListView->insertString(0, fileName);
				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, getMusicIcon(cName), (void*)FILE_ATTRIBUTE_NORMAL);
				count++;
			}

		}

		file.findClose();
	}


	void MediaView::renewFillViewList()
	{
		if(isRootDirectory()){
			initRootDirect();
			return;
		}
		mListView->deleteAllItems();
		fillDirectoryFile(mCurrentPath);
		mListView->sort();
		mListView->invalidateRect();
		
		mTitle->setTextResoure(STR_FILE_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();	
		setMainState(0x1210);
	}
	void MediaView::initRootDirect(){
		int img[]={IDP_DFLASH_ICON,IDP_DSDCARD_ICON,IDB_DUSBOTG_ICON};
		int imgsec[]={IDP_DFLASH_ICON,IDP_DSDCARD_ICON,IDB_DUSBOTG_ICON};
		int text[]={STR_D_FLASH,STR_D_SDCAR,STR_D_USBOTG};
		int i,count = 3;
		
		if(mRootDirectListAdapter == NULL){
			mListView->deleteAllItems();
			mRootDirectListAdapter = new RootDirectListAdapter(mListView,ADAPTER_PLAYING);
			mRootDirectListAdapter->setData(img,imgsec,text,count);
		}else{
			log_i("mRootDirectListAdapter->refresh()");
			mRootDirectListAdapter->refresh();
		}
		setMainState(0x1200);
	}
	void MediaView::initMainList(){
		
		int img[]={IDP_PLAYING_LIST,IDP_FILE_LIST,IDP_PLAYLIST_LIST,
			IDP_ALL_MUSIC,IDP_ALBUM_LIST,IDP_ARTIST_LIST,IDP_GENRE_LIST};
		int til[]={STR_PLAYING_LIST,STR_FILE_LIST,STR_LAYLIST_LIST,
			STR_MUSIC_LIST,STR_ALBUM_LIST,STR_ARTIST_LIST,STR_GENRE_LIST};
		int i,count = 7;
		int mask = LVIF_ITEXT | LVIF_IMAGE | LVIF_PARAM;
#if 0		
		mListView->deleteAllItems();
		for(i=count-1;i>=0;i--){
			insertFileToListView(mask, NULL, img[i], (void*)i,til[i],LIST_PARAM_MAIN);
		}
		mListView->invalidateRect();
#else
		if(mMainListAdapter == NULL){
			mMainListAdapter = new MainListAdapter(mListView,ADAPTER_PLAYING);
			mMainListAdapter->setData(img,img,til,count);
		}else{
			log_i("mMainListAdapter->refresh()");
			mListView->deleteAllItems();
			mMainListAdapter->refresh();
		}
		//mListView->setZoneY(mMainListAdapter->getYoffset(),false);
#endif
		mTitle->setTextResoure(MUSIC_MY_MUSIC);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1000);
	}

	void MediaView::initPlayingList(){
		mListView->deleteAllItems();

		if(mPlayingListAdapter == NULL){
			
			mPlayingListAdapter = new PlayingListAdapter(mListView,ADAPTER_MUSIC); 
		}
		
		mPlayingListAdapter->refresh();
		mPlayingListAdapter->setPaintStartOffset(0);
		
		mTitle->setTextResoure(STR_PLAYING_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1100);
	}
	void MediaView::initPlayList(){
		mListView->deleteAllItems();

		if(mPlayListAdapter == NULL){
			
			mPlayListAdapter = new PlayListAdapter(mListView); 
		}
		mPlayListAdapter->refresh();
		
		mTitle->setTextResoure(STR_LAYLIST_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1600);
	}
	void MediaView::initPlayListMem(PlayListItem& item){
		ArrayMediaInfo array;
		char *ptr,sql[255];

		ptr = sql;
		ptr+=sprintf(ptr,"select music.* from  music,playlistmem where playlistmem.playlist_id=%d and music._id=playlistmem.audio_id",
			item.id);
		
		mListView->deleteAllItems();

		if(mPlayListMemAdapter == NULL){
			
			mPlayListMemAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING);
		}
		mPlayListMemAdapter->setSql(sql);
		mCurrentPlaylistId = item.id;

		mTitle->setTextString(item.name);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1610);
	}

	void MediaView::initMusicList(){
		mListView->deleteAllItems();
		
		if(mMusicAdapter == NULL){
			mMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING);
		}
		mMusicAdapter->refresh();

		mTitle->setTextResoure(STR_MUSIC_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1300);
	}

	void MediaView::initAlbumList(){
		mListView->deleteAllItems();
		if(mAlbumAdapter == NULL){
			mAlbumAdapter = new AlbumAdapter(mListView,ADAPTER_ALBUM); 
		}
		mAlbumAdapter->refresh();
		mListView->setZoneY(mAlbumAdapter->getYoffset(),false);
		
		mTitle->setTextResoure(STR_ALBUM_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1400);
	}
	void MediaView::initArtistList(){
		
		mListView->deleteAllItems();
		
		if(mArtistAdapter == NULL){

			mArtistAdapter = new ArtistAdapter(mListView,ADAPTER_ALBUM); 
		}
		mArtistAdapter->refresh();
		mListView->setZoneY(mArtistAdapter->getYoffset(),false);
		mTitle->setTextResoure(STR_ARTIST_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1500);
	}
	void MediaView::initArtistAlbumList(char* artist){
		mListView->deleteAllItems();
		if(mArtistAlbumAdapter == NULL){
			mArtistAlbumAdapter = new AlbumAdapter(mListView,ADAPTER_ALBUM); 
		}
		if(artist==NULL){
			mArtistAlbumAdapter->refresh();
			mTitle->setTextString(mArtistAlbumAdapter->getArtist());
		}else{
			mArtistAlbumAdapter->setArtist(artist);
			mTitle->setTextString(artist);
		}
		
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1510);
	}
	void MediaView::initArtistAlbumMusicList(char* artist,char* album){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;
		
		ptr = where;
		mediaList = new ArrayMediaInfo();
				
		mListView->deleteAllItems();
		
		if(mArtistMusicAdapter == NULL){		
			mArtistMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}
		
		ptr += sprintf(ptr,"where artist = '%s' and album = '%s' ",artist,album);
		
		mArtistMusicAdapter->setWhere(where);
		
		setMainState(0x1511);
	}

	void MediaView::initGenreList(){
		
		mListView->deleteAllItems();
		
		if(mGenreAdapter == NULL){
			mGenreAdapter = new GenreAdapter(mListView,ADAPTER_ALBUM); 
		}
		mGenreAdapter->refresh();
		mListView->setZoneY(mGenreAdapter->getYoffset(),false);
		mTitle->setTextResoure(STR_GENRE_LIST);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		
		setMainState(0x1700);
	}

	void MediaView::initAlbumMusicList(mediainfo* info){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;

		ptr = where;
		mediaList = new ArrayMediaInfo();
		
		mListView->deleteAllItems();

		if(mAlbumMusicAdapter == NULL){
			
			mAlbumMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where album='%s' ",info->album,info->artist);

		mAlbumMusicAdapter->setWhere(where);
		
		setMainState(0x1410);
		
	}

	void MediaView::initArtistMusicList(char* key,char* value,int state){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;

		ptr = where;
		mediaList = new ArrayMediaInfo();
		
		mListView->deleteAllItems();

		if(mArtistMusicAdapter == NULL){
			
			mArtistMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where %s = '%s' ",key,value);

//		count = gmediaprovider.queryMusicArray(where,mediaList);
//		mArtistMusicAdapter->setListData(mediaList);

		mArtistMusicAdapter->setWhere(where);
		//mAlbumMusicAdapter->refresh();

		setMainState(state);
	}
	
	void MediaView::initGenreMusicList(char* key,char* value,int state){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;

		ptr = where;
		mediaList = new ArrayMediaInfo();
		
		mListView->deleteAllItems();

		if(mGenreMusicAdapter == NULL){
			
			mGenreMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where %s = '%s' ",key,value);

		mGenreMusicAdapter->setWhere(where);

		setMainState(state);
	}	
	bool MediaView::isRootDirectory()
	{
#ifdef WIN32
		if (String::lstrcmpi(mCurrentPath, TEXT("E:")) == 0)
			return true;
#else
		if (String::lstrcmpi(mCurrentPath, TEXT("/mnt")) == 0)
			return true;
#endif
		return false;
	}
	bool MediaView::isVolumRootpath(char *path){
		bool ret;
		if(path == NULL)
			ret = false;
		if(strcmp(path,"/mnt/sdcard") == 0){
			ret = true;
		}else if(strcmp(path,"/mnt/external_sd") == 0){
			ret = true;
		}else if(strcmp(path,"/mnt/usbotg") == 0){
			ret = true;
		}else{
			ret = false;
		}
		log_i("isVolumRootpath ret=%d:%s",ret,path);
		return ret;
	}


	void MediaView::backToParentDirectory()
	{
		File::pathRemoveFileSpec(mCurrentPath);
		renewFillViewList();
	}
	void MediaView::playMediaInfo(mediainfo* info,int display){
		mPlayinglist->playMediaInfo(info);
		gPlayer.showPlayingView();
	}

	int MediaView::onNotify(View* fromView, int code, void* parameter)
	{
		log_i("MediaView::onNotify code = %d",code);
		if (fromView == mListView && 
			code == NM_CLICK) {
			int type,index;
			
			LISTVIEW_RECORD* record;
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;
			type = (int)(record->m_lvItem.paramType);
			index = (int)(record->m_lvItem.lParam);
			log_i("MediaView::onNotify m_lvItem.paramType = %d,getMainState()=0x%x",type,getMainState());
			switch(type){
				case LIST_PARAM_FILE:
					if (index & FILE_ATTRIBUTE_DIRECTORY)
					{
						File::pathAddBackslash(mCurrentPath);
						String::lstrcat(mCurrentPath, record->m_lvItem.pszText);
						renewFillViewList();
					} 
					else //if(type & FILE_ATTRIBUTE_NORMAL)
					{

						TCHAR path[MAX_PATH];
						char utf8Path[300],*ptr,parent[255];
						char *where = NULL;
						char sqlpath[300];
						int count;
						ArrayMediaInfo *pinfo;
						
						pinfo = new ArrayMediaInfo();
						ptr = where = new char[300];
						
						String::copy(path, mCurrentPath);
						File::pathAddBackslash(path);
						String::lstrcat(path, record->m_lvItem.pszText);

						Charset::wideCharToMultiByte(CP_UTF8, path, String::lstrlen(path), utf8Path, MAX_PATH * 3);
						mediaprovider::slqFormatOut(utf8Path,sqlpath);
						ptr += sprintf(ptr," where path = '%s' ",sqlpath);

						count = gmediaprovider.queryMusicArray(where,pinfo);
						
						safeDelete(where);

						getparent(utf8Path,parent);
						
						if(count>0){
							//if(mPlayinglist->isItemExsit(pinfo->getMediaInfo(0))<0){
							if(1){
								ArrayMediaInfo arrayInfo;
								char sqlParent[350];
								
								if(mPlayinglist == NULL)
									mPlayinglist = new Playinglist();
								
								mPlayinglist->clearAll();
								
								mediaprovider::slqFormatOut(parent,sqlParent);
								ptr = where = new char[300];
								ptr += sprintf(ptr," where path like '%s/%%' and not path like '%s/%%/%%' ",sqlParent,sqlParent);
								
								count = gmediaprovider.queryMusicArray(where,&arrayInfo);
								arrayInfo.sort();
								mPlayinglist->addArrayItem(arrayInfo);
							}
							mPlayinglist->playMediaInfo(pinfo->getMediaInfo(0));
							gPlayer.showPlayingView();
						}
						else if(ismusic(utf8Path)){
							int len = strlen(utf8Path)+1;
							
							gmediaprovider.externFileScanner(parent);
							
							mNeedPlayPath = new char[len];
							
							memcpy(mNeedPlayPath,utf8Path,len);
						}
						else if(isCueFile(utf8Path))
						{
#if 0
							CCue mCue;
							int i;
							int ret = -1;
							ret = mCue.file_load(utf8Path);
							//log_i("file_load ret=%d",ret);
							if(ret>=0){
								for(i=0;i<mCue.m_total_song;i++){
									song_t song;
									mCue.get_cert_song(i,&song);
									//log_i("song.star=%d",song.star);
									log_i("song.star=%d,%s",song.star,song.m_strname.string);
								}
							}
#endif
						}
						
						delete pinfo;
					}
					break;
				case LIST_PARAM_MUSIC:
					switch(getMainState()){
						case 0x1000:
							switch(record->m_lvItem.iItem){
								case 0:
									initPlayingList();	break;
								case 1:
									initRootDirect(); 	break;
								case 2:
									initPlayList();		break;	
								case 3:
									initMusicList();	break;
								case 4:
									initAlbumList();	break;
								case 5:
									initArtistList();	break;
								case 6:
									initGenreList();	break;
								default:
									break;
							}
							break;
						case 0x1100:
							mPlayinglist->moveToPosition(record->m_lvItem.iItem);
							mPlayinglist->startPlay();
							mPlayingListAdapter->refresh();
							//gPlayer.showPlayingView();
							break;
						case 0x1300:
						case 0x1410:
						case 0x1511:
						case 0x1610:
						case 0x1710:{
								Point pt =  mListView->getTouchPrevPosition();
								int iconRight = LIST_MUSIC_ICON_LEFT+40;
								MusicAdapter *adapter;
									
								adapter = (MusicAdapter *)mListView->getListAdapter();
								
								if(pt.x>iconRight){
									if(adapter != NULL){
										//if(getMainState() != 0x1300){
											mPlayinglist->clearAll();
											mPlayinglist->addArrayItem(*(adapter->mMusicArrayList));		
										//}
										playMediaInfo(adapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem),1);
									}
								}else if(pt.x>LIST_MUSIC_ICON_LEFT-10){
									log_i("---");
									if(getMainState() == 0x1610)
										gPlayer.showMusicOperateView( *(adapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)) ,OPERATE_TYPE_MUSIC_PLAYLIST);
									else
										gPlayer.showMusicOperateView( *(adapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)) );
								}
								break;
							}
						case 0x1400:
							initAlbumMusicList(mAlbumAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem));
							break;
						case 0x1700:
							initGenreMusicList("genre",mGenreAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->genre,0x1710);
							break;
						case 0x1600:{
								Point pt =  mListView->getTouchPrevPosition();
								int iconRight = LIST_MUSIC_ICON_LEFT+40;
								PlayListItem item;
								
								item = mPlayListAdapter->mArrayPlayList.getItem(record->m_lvItem.iItem);
								
								log_i("item.id=%d",item.id);
								
								if(item.id == -1){
									PlayList::newPlaylist();
									mPlayListAdapter->refresh();
								}else{
									if(pt.x>iconRight){
										initPlayListMem(item);
									}else if(pt.x>LIST_MUSIC_ICON_LEFT-10){
										gPlayer.showPlaylistOperateView(item);
									}
								}
								break;
							}
						case 0x1500:
							//initArtistMusicList("artist",mArtistAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->artist,0x1510);
							initArtistAlbumList(mArtistAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->artist);
							break;
						case 0x1510:
							initArtistAlbumMusicList(mArtistAlbumAdapter->getArtist(),
								mArtistAlbumAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->album);
							break;
						case 0x1200:{
							switch(record->m_lvItem.iItem){
								case 0:
									String::copy(mCurrentPath, TEXT("/mnt/sdcard"));
									break;
								case 1:
									String::copy(mCurrentPath, TEXT("/mnt/external_sd"));
									break;
								case 2:
									String::copy(mCurrentPath, TEXT("/mnt/usb_storage"));
									break;
							}
							renewFillViewList();
							break;
						}
							
					}
					break;
				}
		}
		else if(fromView == NULL && code == NM_DISPLAY){
			log_i("NM_DISPLAY");
			if(mNeedPlayPath != NULL){
				char *ptr,parent[255];
				char *where = NULL;
				char sqlpath[300];
				int count;
				ArrayMediaInfo *pinfo;
		
				pinfo = new ArrayMediaInfo();
				ptr = where = new char[300];
						
				mediaprovider::slqFormatOut(mNeedPlayPath,sqlpath);
						
				ptr += sprintf(ptr," where path = '%s' ",sqlpath);
						
				count = gmediaprovider.queryMusicArray(where,pinfo);
						
				safeDelete(where);

				getparent(mNeedPlayPath,parent);
						
				if(count>0){
					char* name;
					ArrayMediaInfo arrayInfo;
					char sqlParent[350];
					
					if(mPlayinglist == NULL)
						mPlayinglist = new Playinglist();
					mPlayinglist->clearAll();
					

					mediaprovider::slqFormatOut(parent,sqlParent);
					ptr = where = new char[300];
					ptr += sprintf(ptr," where path like '%s/%%' and not path like '%s/%%/%%' ",sqlParent,sqlParent);
					count = gmediaprovider.queryMusicArray(where,&arrayInfo);	
					arrayInfo.sort();
					mPlayinglist->addArrayItem(arrayInfo);
					mPlayinglist->playMediaInfo(pinfo->getMediaInfo(0));
				}
				
				delete pinfo;
				delete mNeedPlayPath;
				mNeedPlayPath = NULL;
				gPlayer.showPlayingView();
			}
			if(getMainState() != 0x1210)
				mListView->refresh();
		}
		else if(code == SDCARD_START_UNMOUNT){
			char *ret = NULL;
			char *ptr = NULL;
			char utf8Path[300];
							
			Charset::wideCharToMultiByte(CP_UTF8, mCurrentPath, String::lstrlen(mCurrentPath), utf8Path, MAX_PATH * 3);

			ret = strstr(utf8Path,"/mnt/external_sd");
			ptr = strstr(utf8Path,"/mnt/usb_storage");
			if(ret || ptr){
				log_i("ret = %s",ret);
				initRootDirect();
			}
		}
		else if(code == NM_REMOVE_FROM_PLAYLIST){
			int id = (int)parameter;
			PlayList::delAudioFromPlaylist(mCurrentPlaylistId,id);
			//mPlayListMemAdapter.refresh();
		}

		return 0;
	}

	int MediaView::getArrayInfoFromFile(char *path,ArrayMediaInfo& array){
		char direct[255];
		int     fileAttribute;
		File    file;
		mediainfo info;
		String str;
		DIR* d;
		struct dirent* de;
		
		if(path == NULL)
			return 0;

		array.clear();
		
		d = opendir(path);

		while((de = readdir(d)) != NULL)
		{
			int name_len = strlen(de->d_name);
			
			strcpy(direct,path);
			strcat(direct,"/");
			strcat(direct,de->d_name);
			//log_i("opendir DT_DIR :%s\n",direct);

			//if (de->d_type == DT_DIR) {
			if(File::isDirect(direct)){
				;
			}else if(ismusic(de->d_name)){
				if(de->d_name[0] == '.')
					continue;
				mediaprovider::FilePathToInfo(direct,info);
				array.addMediaInfo(&info);
			}
		}

		closedir(d);
		return array.getCount();
	}

	int MediaView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode)
		{
		case KEYCODE_BACK:
			break;
		}

		return 0;
	}


	int MediaView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode)
		{
		case KEYCODE_BACK:
			break;
		}

		return 0;
	}

	void MediaView::backEvent(){
		switch(getMainState()){
			case 0x1000:
				gPlayer.showPlayingView();
				break;
			case 0x1210:
				backToParentDirectory();
				break;
			case 0x1200:	
			case 0x1100:
			case 0x1300:
			case 0x1400:
			case 0x1500:
			case 0x1600:
			case 0x1700:
				initMainList();
				break;
			case 0x1410:
				initAlbumList();
				break;
			case 0x1510:
				initArtistList();
				break;
			case 0x1610:
				initPlayList();
				break;
			case 0x1710:
				initGenreList();
				break;
			case 0x1511:
				initArtistAlbumList(NULL);
				break;
		}				

	}
	int MediaView::onTouchDispatch(int x,int y, int action){
		//log_i("MediaViewx=%d,y=%d,action=%d",x,y,action);
		switch(action){
			case VM_TOUCHUP:
				if(mTitle->mRect.ptIn(x, y))
					break;
				if(!mOrderMenuBk->mRect.ptIn(x, y)){
					dismissOrderByMenu();
				}
				break;
		}
	}

	void MediaView::onMainStateChange(int mainState){
		if(mainState == 0x1300){
			mTitle->setShowIcon(true);
		}else{
			mTitle->setShowIcon(false);
		}
	}
	PlayingListAdapter::PlayingListAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;
	}
	void PlayingListAdapter::setPaintStartOffset(int offset){
		int y,pos,count;
		
		count = mPlayinglist->getCount();
		pos = mPlayinglist->mCurrent;
		
		if(count>4){
			if(pos > count-4)
				pos = count-4;
			y = pos*53;
			mlist->setZoneY(-y,false);
		}
	}
	void PlayingListAdapter::refresh(){
		int count,i;
		
		mlist->setListAdapter(this);
		mlist->deleteAllRecord();

		count = mPlayinglist->getCount();
		
		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
			//log_i("PlayingListAdapter insertItem i=%d",i);
		}
		
		mlist->invalidateRect();
	}

	
	void PlayingListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		Rect yrect;
		
		x = rect.left;
		y = rect.top;
		info = (mediainfo *)getItem(lvitem->iItem);
		if(info == NULL)
			return;
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(MediaView::getMusicIcon(info->path),x,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		x = x+40;
		//canvas.drawText(info->name,strlen(info->title),x,y+5);
		yrect.setEx(x,y+5,308-x,30);
		canvas.drawText(info->name,strlen(info->title),yrect,0);
		
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		if(info->artist!=NULL){
			//canvas.drawText(info->artist,strlen(info->artist),x,y+28);
			yrect.setEx(x,y+28,308-x,20);
			canvas.drawText(info->artist,strlen(info->artist),yrect,0);
		}
		x = x+220;
		if(lvitem->iItem == mPlayinglist->mCurrent)
			canvas.drawImageResource(IDP_LISTICON_PLAYING,x,y+13);
	}

	void* PlayingListAdapter::getItem(int index){
		return (void*)mPlayinglist->getItem(index);
	}
	
	int PlayingListAdapter::getCount(){
		return mPlayinglist->getCount();
	}

	MusicAdapter::MusicAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;
		mWhere = NULL;
		mSql = NULL;
		
		mMusicArrayList = new ArrayMediaInfo();
	}
	void MusicAdapter::setMusicOrderby(int order){
		char *ptr,where[255];
		char *orderby;
		int count,i;
		
		ptr = where;

		switch(order){
			case MEDIA_ORDER_TILE:
				orderby = MUSIC_TITLE_KEY;
				break;
			case MEDIA_ORDER_ALBUM:
				orderby = MUSIC_ALBUM_KEY;
				break;
			case MEDIA_ORDER_ARTIST:
				orderby = MUSIC_ART_KEY;
				break;
			case MEDIA_ORDER_TIME:
				orderby = MUSIC_TIMES;
				break;
		}
		
		ptr += sprintf(ptr," order by %s ",orderby);
		
		setWhere(where);
		
	}
	void MusicAdapter::refresh(){
		int count,i;
		
		mMusicArrayList->clear();		
		if(mSql == NULL){
			count = gmediaprovider.queryMusicArray(mWhere,mMusicArrayList);
		}else{
			count = gmediaprovider.queryArrayMedia(mSql,mMusicArrayList);
		}
		if(count>=0)
			rePaintList();
		else{
			mlist->deleteAllItems();
			mlist->invalidateRect();
		}
	}
	void MusicAdapter::setListData(ArrayMediaInfo* info){
		if(info!=NULL){
			*mMusicArrayList = *info;
			rePaintList();
		}
	}
	void MusicAdapter::setWhere(char* wh){
		int len = strlen(wh);
		if(mWhere!=NULL)
			delete mWhere;
		mWhere = new char[len+1];
		strcpy(mWhere,wh);
		refresh();
	}
	void MusicAdapter::setSql(char* sql){
		int len;
		
		safeDelete(mSql);

		if(sql != NULL){
			len = strlen(sql) + 1;
			mSql = new char[len];
			memcpy(mSql,sql,len);
		}
		
		refresh();
	}

	void MusicAdapter::rePaintList(){
		int count,i;
		
		mlist->setListAdapter(this);
		mlist->deleteAllRecord();
		
		count = mMusicArrayList->getCount();
		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
		}
		log_i("MusicAdapter::rePaintList count=%d",count);
		mlist->invalidateRect();
	}
	
	void MusicAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		Rect yrect;
		
		x = rect.left;
		y = rect.top;

		info = mMusicArrayList->getMediaInfo(lvitem->iItem);

		if(info == NULL){
			log_i("----");
			return;
		}
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(MediaView::getMusicIcon(info->path),x,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		x= x+40;
		canvas.setTextSize(16);
		//canvas.drawText(info->name,strlen(info->title),x,y+5);
		yrect.setEx(x,y+5,296-x+12,40);
		canvas.drawText(info->name,strlen(info->title),yrect,0 );
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		//canvas.drawText(info->artist,strlen(info->artist),x,y+28);
		yrect.setEx(x,y+28,296-x+12,40);
		canvas.drawText(info->artist,strlen(info->artist),yrect,0 );
	}

	AlbumAdapter::AlbumAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;
		mMSkBitmap = NULL;
		sqlWhere = NULL;
		mArtist = NULL;
		mMusicArrayList = new ArrayMediaInfo();
	}
	void AlbumAdapter::setWhere(char* where){
		safeDelete(sqlWhere);
		if(where!=NULL){
			int len = strlen(where);
			sqlWhere = new char[len+1];
			memcpy(sqlWhere,where,len+1);
			refresh();
		}
	}
	char* AlbumAdapter::getArtist(){
		return mArtist;
	}
	void AlbumAdapter::setArtist(char* art){
		safeDelete(mArtist);
		if(art != NULL){
			int len = strlen(art);
			mArtist = new char[len+1];
			memcpy(mArtist,art,len+1);
			refresh();
		}
	}
	void AlbumAdapter::refresh(){
		int count,i;
		char *ptr,where[255];
		
		ptr = where;
		
		if(mArtist != NULL)
			ptr+=sprintf(ptr,"where artist='%s'",mArtist);
		
		ptr+=sprintf(ptr," group by album order by album_key");
		
		mlist->setListAdapter(this);
		mlist->deleteAllRecord();
		mMusicArrayList->clear();
		delete[] mMSkBitmap;

		if(sqlWhere == NULL)
			count = gmediaprovider.queryMusicArray(where,mMusicArrayList);
		else
			count = gmediaprovider.queryArrayMedia(where,mMusicArrayList);
		
		mMSkBitmap = new MSkBitmap[count];
		
		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
			mMSkBitmap[i].release();
		}
		mlist->invalidateRect();
	}
	void AlbumAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		mediainfo *info;
		Rect yrect;
		
		x = rect.left;
		y = rect.top;
		index = lvitem->iItem;
		info = mMusicArrayList->getMediaInfo(index);
		x = LIST_MUSIC_ICON_LEFT;
		
		if(!mMSkBitmap[index].isVaild()){
			if(info->img_path!=NULL && info->img_path[0]=='/')
				BitmapFactory::genBitmapFromFile(&mMSkBitmap[index],info->img_path,36,36);
		}
		//log_i("mMSkBitmap[%d] vai,%d,%s",index,info->id,info->img_path);

		if(mMSkBitmap[index].isVaild())
			canvas.drawBitmap(mMSkBitmap[index].mBits,x,y+5,mMSkBitmap[index].width,mMSkBitmap[index].height);
		else
			canvas.drawImageResource(IDP_LISTICON_ALBUM,x,y+5);
		
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		x = x+40;
		//canvas.drawText(info->album,strlen(info->album),x,y+5);
		yrect.setEx(x,y+5,308-x,30);
		canvas.drawText(info->album,strlen(info->album),yrect,0);
		
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		//canvas.drawText(info->artist,strlen(info->artist),x,y+28);
		yrect.setEx(x,y+28,308-x,20);
		canvas.drawText(info->artist,strlen(info->artist),yrect,0);
	}
	void GenreAdapter::refresh(){
		int count,i;
		char *ptr,where[255];
		ptr = where;
		sprintf(ptr,"group by genre");
		
		mlist->setListAdapter(this);
		mlist->deleteAllRecord();
		mMusicArrayList->clear();
		
		count = gmediaprovider.queryMusicArray(where,mMusicArrayList);
		
		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
		}
		mlist->invalidateRect();
	}
	void GenreAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int  x, y;
		mediainfo *info;
		Rect yrect;
				
		x = rect.left;
		y = rect.top;
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(IDP_LIST_GENRE_ICON,x,y+1);
		
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(18);

		x+=45;
		//canvas.drawText(info->genre,strlen(info->genre),x,y+10);
		yrect.setEx(x,y+10,308-x,30);
		canvas.drawText(info->genre,strlen(info->genre),yrect,0);
	}

	
	ArtistAdapter::ArtistAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;
		
		mMusicArrayList = new ArrayMediaInfo();
	}
	void ArtistAdapter::refresh(){
		int count,i;
		char *ptr,where[255];
		ptr = where;
		sprintf(ptr,"group by artist order by artist_key");
		
		mlist->setListAdapter(this);
		mlist->deleteAllRecord();
		mMusicArrayList->clear();

		count = gmediaprovider.queryMusicArray(where,mMusicArrayList);

		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
		}
		mlist->invalidateRect();
	}
	void ArtistAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		Rect yrect;
		
		x = rect.left;
		y = rect.top;
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(IDP_LISTICON_ARTIST,x,y+5);

		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		
		canvas.setTextSize(18);

		x+=40;
		//canvas.drawText(info->artist,strlen(info->artist),x,y+10);
		yrect.setEx(x,y+10,296+12-x,30);
		canvas.drawText(info->artist,strlen(info->artist),yrect,0);
		
		canvas.setTextColor(RGB(255,255,255));
	}

	void MainListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=100;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
	}
	int MediaView::getMusicIcon(char* name){
		char *type = getfiletype(name);
		if(strcmp(type,"mp3") == 0){
			return IDP_LISTICON_MUSIC;
		}else if(strcmp(type,"aac") == 0){
			return IDP_MUSIC_AAC;
		}else if(strcmp(type,"aif") == 0){
			return IDP_MUSIC_AIF;
		}else if(strcmp(type,"aiff") == 0){
			return IDP_MUSIC_AIFF;
		}else if(strcmp(type,"ape") == 0){
			return IDP_MUSIC_APE;
		}else if(strcmp(type,"flac") == 0){
			return IDP_MUSIC_FLAC;
		}else if(strcmp(type,"m4a") == 0){
			return IDP_MUSIC_M4A;
		}else if(strcmp(type,"ogg") == 0){
			return IDP_MUSIC_OGG;
		}else if(strcmp(type,"wav") == 0){
			return IDP_MUSIC_WAV;
		}else if(strcmp(type,"wma") == 0){
			return IDP_MUSIC_WMA;
		}else if(strcmp(type,"aac") == 0){
			return IDP_MUSIC_AAC;
		}else
			return IDP_MUSIC_ICON;
	}
};
