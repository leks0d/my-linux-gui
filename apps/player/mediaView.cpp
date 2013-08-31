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
/*
	static int ismusic(char *file)
	
	{
		char *type;
		char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","m4a","wma","\0"};
		char **mtype;
		mtype = music_type;
		//log_i("getfiletype filename:%s\n",file);
		type = getfiletype(file);
		//log_i("getfiletype :%s\n",type);
		if(type == 0)
			return 0;
		while(*mtype!="\0"){
			if(strcmp(*mtype,type) == 0)
				return 1;
			mtype++;
		}
		return 0;
	}
*/	
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
		mArtistAdapter = NULL;
		mMainListAdapter = NULL;
		mRootDirectListAdapter = NULL;
		mAlbumMusicAdapter = NULL;
		mArtistMusicAdapter = NULL;
		
		mOrderMenuShow = 0;
	}


	MediaView::~MediaView(void)
	{

	}
	

	int MediaView::onCreate()
	{	
		Rect rect;
		int menuBkX = 5,orderTextX = menuBkX+3,orderDiv = 78,orderTextY = 21;

		rect.setEx(0,21,320,219);
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

			if (fileAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(cName[0] == '.')
					continue;
			
				if (String::lstrcmp (fileName, TEXT (".")) == 0)
					continue;

				if (String::lstrcmp (fileName, TEXT ("..")) == 0)
					continue;

				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_FOLDER_ICON, (void*)FILE_ATTRIBUTE_DIRECTORY);
				count++;
			}
			else if (ismusic(cName)) //PathMatchSpecEx (FindFileData.cFileName, pctl->lpofn->lpstrFilter, PMSF_MULTIPLE))
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
		
		int img[]={IDP_PLAYING_LIST,IDP_FILE_LIST,IDP_ALL_MUSIC,IDP_ALBUM_LIST,IDP_ARTIST_LIST};
		int til[]={STR_PLAYING_LIST,STR_FILE_LIST,STR_MUSIC_LIST,STR_ALBUM_LIST,STR_ARTIST_LIST};
		int i,count = 5;
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
		
		mTitle->setTextResoure(STR_PLAYING_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1100);
	}

	void MediaView::initMusicList(){
		
		mListView->deleteAllItems();
		
		if(mMusicAdapter == NULL){
			mMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING);
		}
		mMusicAdapter->refresh();

		mTitle->setTextResoure(STR_MUSIC_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1300);
	}

	void MediaView::initAlbumList(){
		mListView->deleteAllItems();
		if(mAlbumAdapter == NULL){
			
			mAlbumAdapter = new AlbumAdapter(mListView,ADAPTER_ALBUM); 
		}
		mAlbumAdapter->refresh();
		
		mTitle->setTextResoure(STR_ALBUM_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1400);
	}
	void MediaView::initArtistList(){
		
		mListView->deleteAllItems();
		
		if(mArtistAdapter == NULL){

			mArtistAdapter = new ArtistAdapter(mListView,ADAPTER_ALBUM); 
		}
		mArtistAdapter->refresh();
		
		mTitle->setTextResoure(STR_ARTIST_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1500);
	}

	

	void MediaView::initAlbumMusicList(char* album){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;

		ptr = where;
		mediaList = new ArrayMediaInfo();
		
		mListView->deleteAllItems();

		if(mAlbumMusicAdapter == NULL){
			
			mAlbumMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where album = '%s' ",album);

		//count = gmediaprovider.queryMusicArray(where,mediaList);
		mAlbumMusicAdapter->setWhere(where);
		//mAlbumMusicAdapter->refresh();
		//mAlbumMusicAdapter->setListData(mediaList);

		setMainState(0x1410);
		
	}

	void MediaView::initSpecMusicList(char* key,char* value,int state){
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
		if (fromView == mListView && code == NM_CLICK) {
			int type,index;
			
			LISTVIEW_RECORD* record;
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;
			type = (int)(record->m_lvItem.paramType);
			index = (int)(record->m_lvItem.lParam);
			log_i("MediaView::onNotify m_lvItem.paramType = %d",type);
			switch(type){
				case LIST_PARAM_FILE:
					if (index & FILE_ATTRIBUTE_DIRECTORY)
					{
						File::pathAddBackslash(mCurrentPath);
						String::lstrcat(mCurrentPath, record->m_lvItem.pszText);
						renewFillViewList();
					} else //if(type & FILE_ATTRIBUTE_NORMAL)
					{

						TCHAR path[MAX_PATH];
						char utf8Path[300],*where,*ptr;
						int count;
						ArrayMediaInfo *pinfo;
						
						pinfo = new ArrayMediaInfo();
						ptr = where = new char[300];
						
						String::copy(path, mCurrentPath);
						File::pathAddBackslash(path);
						String::lstrcat(path, record->m_lvItem.pszText);

						Charset::wideCharToMultiByte(CP_UTF8, path, String::lstrlen(path), utf8Path, MAX_PATH * 3);
						ptr += sprintf(ptr," where path = '%s' ",utf8Path);

						count = gmediaprovider.queryMusicArray(where,pinfo);
						
						delete where;
						
						if(count>0){
							char* name,parent[255];
							ArrayMediaInfo arrayInfo;
							
							if(mPlayinglist == NULL)
								mPlayinglist = new Playinglist();
							mPlayinglist->clearAll();
							mPlayinglist->playMediaInfo(pinfo->getMediaInfo(0));	
							
							getparent(utf8Path,parent);
							//if(!isVolumRootpath(parent)){
								ptr = where = new char[300];
								ptr += sprintf(ptr," where path like '%s/%%' and not path like '%s/%%/%%' ",parent,parent);
								count = gmediaprovider.queryMusicArray(where,&arrayInfo);					
								mPlayinglist->addArrayItem(arrayInfo);
							//}
						}else{
							mediainfo info;
							char* name,parent[255];					
							ArrayMediaInfo arrayInfo;
							memset(&info,0,sizeof(mediainfo));
							
							getparent(utf8Path,parent);
							
							log_i("getparent parent=%s",parent);
							
							getArrayInfoFromFile(parent,arrayInfo);
							mPlayinglist->clearAll();
							mPlayinglist->addArrayItem(arrayInfo);
							
							mediaprovider::FilePathToInfo(utf8Path,info);
							mPlayinglist->playMediaInfo(&info);
						}

						delete pinfo;
						gPlayer.showPlayingView();
					}
					break;
				case LIST_PARAM_MAIN:
					log_i("MediaView::onNotify LIST_PARAM_MAIN index=%d",index);
					switch(index){
						case 0:
							initPlayingList();	break;
						case 1:
							initRootDirect(); break;
						case 2:
							initMusicList();	break;
						case 3:
							initAlbumList();	break;
						case 4:
							initArtistList();	break;
						default:
							break;
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
									initMusicList();	break;
								case 3:
									initAlbumList();	break;
								case 4:
									initArtistList();	break;
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
						case 0x1510:{
								Point pt =  mListView->getTouchPrevPosition();
								int iconRight = LIST_MUSIC_ICON_LEFT+40;
								
								if(pt.x>iconRight){
									MusicAdapter *adapter;
									
									adapter = (MusicAdapter *)mListView->getListAdapter();
									
									if(adapter != NULL){
										if(getMainState() != 0x1300){
											mPlayinglist->clearAll();
											mPlayinglist->addArrayItem(*(adapter->mMusicArrayList));		
										}
										playMediaInfo(adapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem),1);
									}
								}else if(pt.x>LIST_MUSIC_ICON_LEFT){
									gPlayer.showMusicOperateView( *(mMusicAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)) );
								}
								break;
							}
						case 0x1400:
							initAlbumMusicList(mAlbumAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->album);
							break;
						case 0x1500:
							initSpecMusicList("artist",mArtistAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem)->artist,0x1510);
							break;
						case 0x1200:{
							switch(record->m_lvItem.iItem){
								case 0:
									String::copy(mCurrentPath, TEXT("/mnt/sdcard"));
									log_i("lstrcmpi  /mnt/sdcard");
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
			if(getMainState() != 0x1210)
				mListView->refresh();
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

			if (de->d_type == DT_DIR) {
				;
			}else if( ismusic(de->d_name) ){
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
				initMainList();
				break;
			case 0x1410:
				initAlbumList();
				break;
			case 0x1510:
				initArtistList();
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
		
		x = rect.left;
		y = rect.top;
		info = (mediainfo *)getItem(lvitem->iItem);
		if(info == NULL)
			return;
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(IDP_LISTICON_MUSIC,x,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		x = x+40;
		canvas.drawText(info->name,strlen(info->title),x,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		if(info->artist!=NULL)
			canvas.drawText(info->artist,strlen(info->artist),x,y+28);
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
		
		log_i("MusicAdapter::refresh");
		
		mMusicArrayList->clear();		
		
		count = gmediaprovider.queryMusicArray(mWhere,mMusicArrayList);
		
		if(count>=0)
			rePaintList();
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
		
		x = rect.left;
		y = rect.top;
		
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(MediaView::getMusicIcon(info->name),x,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		x= x+40;
		canvas.setTextSize(16);
		canvas.drawText(info->name,strlen(info->title),x,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		canvas.drawText(info->artist,strlen(info->artist),x,y+28);
	}

	AlbumAdapter::AlbumAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;
		
		mMusicArrayList = new ArrayMediaInfo();
	}
	void AlbumAdapter::refresh(){
		int count,i;
		char *ptr,where[255];
		ptr = where;
		sprintf(ptr,"group by album");
		
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
	void AlbumAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		
		x = rect.left;
		y = rect.top;
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		x = LIST_MUSIC_ICON_LEFT;
		canvas.drawImageResource(IDP_LISTICON_ALBUM,x,y+5);

		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		x = x+40;
		canvas.drawText(info->album,strlen(info->album),x,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		canvas.drawText(info->artist,strlen(info->artist),x,y+28);
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
		sprintf(ptr,"group by artist");
		
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
#if 0
		log_i("info->artist=%s,len=%d",info->artist,strlen(info->artist));
		if(Charset::isTextUtf8(info->artist)){
			log_i("isTextUtf8 true");
		}else{
			log_i("isTextUtf8 false");
		}
#endif	
		x+=40;
		canvas.drawText(info->artist,strlen(info->artist),x,y+10);
		
		canvas.setTextColor(RGB(255,255,255));
		//canvas.setTextSize(12);
		//canvas.drawText(info->artist,strlen(info->artist),50,y+28);
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
