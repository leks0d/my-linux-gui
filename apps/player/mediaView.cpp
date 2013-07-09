#include "player.h"


namespace mango
{
	enum
	{
		SETTING_BACK = 10,
		SETTING_HOME,
		SETTING_TITLE,
		PLAYING_IDB_PLAY,
		PLAYING_IDB_MUSIC,
		PLAYING_IDB_SETTING,
		PLAYING_IDB_VOLUME,
		PLAYING_IDB_ALBUM_IMAGE,
		PLAYING_IDB_MUSIC_NAME
	};
	
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

	}


	MediaView::~MediaView(void)
	{

	}
	

	int MediaView::onCreate()
	{	
		Rect rect;



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
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();

		rect.setEx(0,25,320,215);
		mListView = new MediaListView(TEXT("Media List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->onCreate();
		
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
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		return 0;
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
		mListView->deleteAllItems();
		fillDirectoryFile(mCurrentPath);
		mListView->sort();
		mListView->invalidateRect();
	}

	void MediaView::initMainList(){
		int img[]={IDP_PLAYING_LIST,IDP_FILE_LIST,IDP_ALL_MUSIC,IDP_ALBUM_LIST,IDP_ARTIST_LIST};
		int til[]={STR_PLAYING_LIST,STR_FILE_LIST,STR_MUSIC_LIST,STR_ALBUM_LIST,STR_ARTIST_LIST};
		int i,count = 5;
		int mask = LVIF_ITEXT | LVIF_IMAGE | LVIF_PARAM;
		mListView->deleteAllItems();
		for(i=count-1;i>0;i--){
			insertFileToListView(mask, NULL, img[i], NULL,til[i],LIST_PARAM_MAIN);
		}
		mListView->invalidateRect();

		mTitle->setTextResoure(MUSIC_MY_MUSIC);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
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



	int MediaView::onNotify(View* fromView, int code, void* parameter)
	{
		if (fromView == mListView && code == NM_CLICK) {
			int type;
			LISTVIEW_RECORD* record;
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;
			type = (int)(record->m_lvItem.lParam);
			switch(type){
				case LIST_PARAM_FILE:
					if (type & FILE_ATTRIBUTE_DIRECTORY)
					{
						File::pathAddBackslash(mCurrentPath);
						String::lstrcat(mCurrentPath, record->m_lvItem.pszText);
						renewFillViewList();
					} else if(type & FILE_ATTRIBUTE_NORMAL){

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
						ptr += sprintf(ptr," path = '%s' ",utf8Path);

						count = gmediaprovider.queryMusicArray(where,pinfo);
						log_i("MusicArray count=%d",count);
						
						if(count>0){
							
							if(mPlayinglist == NULL)
								mPlayinglist = new Playinglist();
							mPlayinglist->startPlay(pinfo->getMediaInfo(0));
							
							gPlayer.showPlayingView();
						}
					}
					break;
				case LIST_PARAM_MAIN:
					log_i("MediaView::onNotify LIST_PARAM_MAIN index=%d",parameter);
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
			if (!isRootDirectory())
				backToParentDirectory();

			break;
		}

		return 0;
	}

	
};
