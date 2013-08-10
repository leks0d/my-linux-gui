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
	}


	MediaView::~MediaView(void)
	{

	}
	

	int MediaView::onCreate()
	{	
		Rect rect;

		rect.setEx(0,25,320,215);
		mListView = new MediaListView(TEXT("Media List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->onCreate();
		
		/*rect.setEx(0,0,320,26);
		mTitleImageView = new ImageView(SETTING_BACK, TEXT("Media List"), this, &rect, LVS_LIST);
		mTitleImageView->setImageResoure(IDP_MUSIC_TITLEBAR);
		mTitleImageView->onCreate();*/

		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(RGB(255,255,255));
		mTitle->onCreate();

		
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
		
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
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
		
		
		if (!file.findOpen(mCurrentPath)) {
			log_e ("findOpen \n") ;
			return ;
		}

		while(file.findNext(fileName, &fileAttribute))
		{
			if (fileAttribute & FILE_ATTRIBUTE_HIDDEN)
				continue ;

			if (fileAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (String::lstrcmp (fileName, TEXT (".")) == 0)
					continue ;

				if (String::lstrcmp (fileName, TEXT ("..")) == 0)
					continue ;

				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_FOLDER_ICON, (void*)FILE_ATTRIBUTE_DIRECTORY);
				count++;
			}
			else if (1) //PathMatchSpecEx (FindFileData.cFileName, pctl->lpofn->lpstrFilter, PMSF_MULTIPLE))
			{
			//	mListView->insertString(0, fileName);
				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_MUSIC_ICON, (void*)FILE_ATTRIBUTE_NORMAL);
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
		}else
			mRootDirectListAdapter->refresh();
		setMainState(0x1200);
	}
	void MediaView::initMainList(){
		int img[]={IDP_PLAYING_LIST,IDP_FILE_LIST,IDP_ALL_MUSIC,IDP_ALBUM_LIST,IDP_ARTIST_LIST};
		int til[]={STR_PLAYING_LIST,STR_FILE_LIST,STR_MUSIC_LIST,STR_ALBUM_LIST,STR_ARTIST_LIST};
		int i,count = 5;
		int mask = LVIF_ITEXT | LVIF_IMAGE | LVIF_PARAM;
		
		mListView->deleteAllItems();
		for(i=count-1;i>=0;i--){
			insertFileToListView(mask, NULL, img[i], (void*)i,til[i],LIST_PARAM_MAIN);
		}
		mListView->invalidateRect();

		mTitle->setTextResoure(MUSIC_MY_MUSIC);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1000);
	}

	void MediaView::initPlayingList(){

		if(mPlayingListAdapter == NULL){
			mListView->deleteAllItems();
			mPlayingListAdapter = new PlayingListAdapter(mListView,ADAPTER_MUSIC); 
		}
		mPlayingListAdapter->refresh();
		
		mTitle->setTextResoure(STR_PLAYING_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1100);
	}

	void MediaView::initMusicList(){
		
		if(mMusicAdapter == NULL){
			mListView->deleteAllItems();
			mMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}
		mMusicAdapter->refresh();

		mTitle->setTextResoure(STR_MUSIC_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1300);
	}

	void MediaView::initAlbumList(){
		if(mAlbumAdapter == NULL){
			mListView->deleteAllItems();
			mAlbumAdapter = new AlbumAdapter(mListView,ADAPTER_ALBUM); 
		}
		mAlbumAdapter->refresh();
		
		mTitle->setTextResoure(STR_ALBUM_LIST);
		mTitle->setTextLayoutType(STR_PLAYING_LIST);
		mTitle->invalidateRect();
		setMainState(0x1400);
	}
	void MediaView::initArtistList(){
		if(mArtistAdapter == NULL){
			mListView->deleteAllItems();
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

		if(mMusicAdapter == NULL){
			mListView->deleteAllItems();
			mMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where album = '%s' ",album);

		count = gmediaprovider.queryMusicArray(where,mediaList);

		mMusicAdapter->setListData(mediaList);

		setMainState(0x1410);
		
	}

	void MediaView::initSpecMusicList(char* key,char* value,int state){
		int count,i;
		char *ptr,where[255];
		ArrayMediaInfo *mediaList;

		ptr = where;
		mediaList = new ArrayMediaInfo();

		if(mMusicAdapter == NULL){
			mListView->deleteAllItems();
			mMusicAdapter = new MusicAdapter(mListView,ADAPTER_PLAYING); 
		}

		ptr += sprintf(ptr,"where %s = '%s' ",key,value);

		count = gmediaprovider.queryMusicArray(where,mediaList);

		mMusicAdapter->setListData(mediaList);

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


	void MediaView::backToParentDirectory()
	{
	//	TCHAR*  fileName;

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
						log_i("MusicArray count=%d",count);
						
						if(count>0){
							
							if(mPlayinglist == NULL)
								mPlayinglist = new Playinglist();
							log_i("pinfo->getMediaInfo(0)->artist=%s",pinfo->getMediaInfo(0)->artist);
							mPlayinglist->playMediaInfo(pinfo->getMediaInfo(0));
							
							
						}else{
							mediainfo info;
							char* name;
							info.id = -1;
							info.path = new char[strlen(utf8Path)+1];
							
							name = getfilename(utf8Path);
							info.name = new char[strlen(utf8Path)+1]; 				
							memcpy(info.name,name,strlen(name)+1);
							memcpy(info.path,utf8Path,strlen(utf8Path)+1);
							info.artist = NULL;
							mPlayinglist->playMediaInfo(&info);
							
						}
						
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
						case 0x1100:
							mPlayinglist->moveToPosition(record->m_lvItem.iItem);
							mPlayinglist->startPlay();
							mPlayingListAdapter->refresh();
							//gPlayer.showPlayingView();
							break;
						case 0x1300:
						case 0x1410:
						case 0x1510:	
							playMediaInfo(mMusicAdapter->mMusicArrayList->getMediaInfo(record->m_lvItem.iItem),1);
							break;
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

		return 0;
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

	
	PlayingListAdapter::PlayingListAdapter(ListView* list,int id){
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
			log_i("PlayingListAdapter insertItem i=%d",i);
		}
		mlist->invalidateRect();
	}

	
	void PlayingListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		
		x = rect.left;
		y = rect.top;
		info = (mediainfo *)getItem(lvitem->iItem);
		//log_i("PlayingListAdapter::PaintView lvitem->iItem=%d",lvitem->iItem);
		canvas.drawImageResource(IDP_LISTICON_MUSIC,10,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		canvas.drawText(info->name,strlen(info->name),50,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		if(info->artist!=NULL)
			canvas.drawText(info->artist,strlen(info->artist),50,y+28);
		if(lvitem->iItem == mPlayinglist->mCurrent)
			canvas.drawImageResource(IDP_LISTICON_PLAYING,270,y+13);
	}

	void* PlayingListAdapter::getItem(int index){
		return (void*)mPlayinglist->getItem(index);
	}
	
	int PlayingListAdapter::getCount(){
		return mPlayinglist->getCount();
	}

	MusicAdapter::MusicAdapter(ListView* list,int id){
		mId = id;
		mlist = list;
		
		mMusicArrayList = new ArrayMediaInfo();
	}
	void MusicAdapter::refresh(){
		int count,i;
		
		log_i("MusicAdapter::refresh");
		
		mMusicArrayList->clear();		
		
		count = gmediaprovider.queryMusicArray(0,mMusicArrayList);
		
		if(count>=0)
			rePaintList();
	}

	void MusicAdapter::setListData(ArrayMediaInfo* info){
		mlist->deleteAllItems();
		if(info!=NULL){
			*mMusicArrayList = *info;
			rePaintList();
		}
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
		
		//log_i("MusicAdapter::PaintView lvitem->iItem=%d",lvitem->iItem);
		
		
	#if 0	
		canvas.drawText("aaaaa",5,50,y+5);
	#else
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		canvas.drawImageResource(IDP_LISTICON_MUSIC,10,y+5);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));

		canvas.setTextSize(16);
		canvas.drawText(info->name,strlen(info->name),50,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		canvas.drawText(info->artist,strlen(info->artist),50,y+28);
	#endif	
	}

	AlbumAdapter::AlbumAdapter(ListView* list,int id){
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
			log_i("PlayingListAdapter insertItem i=%d",i);
		}
		mlist->invalidateRect();
	}
	void AlbumAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		
		x = rect.left;
		y = rect.top;
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		//log_i("PlayingListAdapter::PaintView lvitem->iItem=%d",lvitem->iItem);
		canvas.drawImageResource(IDP_MUSIC_ICON,10,y+5);
		log_i("AlbumAdapter::PaintView isSec=%d",isSec);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(16);
		canvas.drawText(info->album,strlen(info->album),50,y+5);
		canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(12);
		canvas.drawText(info->artist,strlen(info->artist),50,y+28);
	}

	
	ArtistAdapter::ArtistAdapter(ListView* list,int id){
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
			log_i("PlayingListAdapter insertItem i=%d",i);
		}
		mlist->invalidateRect();
	}
	void ArtistAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y;
		mediainfo *info;
		
		x = rect.left;
		y = rect.top;
		info = mMusicArrayList->getMediaInfo(lvitem->iItem);
		//log_i("PlayingListAdapter::PaintView lvitem->iItem=%d",lvitem->iItem);
		canvas.drawImageResource(IDP_LISTICON_ARTIST,10,y+5);
		log_i("ArtistAdapter::PaintView isSec=%d",isSec);
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
		canvas.setTextSize(18);
		canvas.drawText(info->artist,strlen(info->artist),50,y+10);
		canvas.setTextColor(RGB(255,255,255));
		//canvas.setTextSize(12);
		//canvas.drawText(info->artist,strlen(info->artist),50,y+28);
	}
};
