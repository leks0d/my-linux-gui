#include "player.h"


namespace mango
{

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
		mListView = new MediaListView(TEXT("Media List"), this, &mRect, LVS_LIST);
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
		canvas.drawImageResource(IDP_BACKGROUND,0,0,false);
		return 0;
	}


	void MediaView::insertFileToListView(UINT mask, TCHAR* name, int iconId, int fileAttributes,int itext)
	{
		ListViewItem  lvItem;

		lvItem.mask     = mask;//LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvItem.iItem    = 0;
		lvItem.iSubItem = 0;
		lvItem.pszText  = name;
		lvItem.iImage   = iconId;
		lvItem.iText   = itext;
		lvItem.lParam   = (void *)fileAttributes;

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

				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_FOLDER_ICON, FILE_ATTRIBUTE_DIRECTORY);
				count++;
			}
			else if (1) //PathMatchSpecEx (FindFileData.cFileName, pctl->lpofn->lpstrFilter, PMSF_MULTIPLE))
			{
			//	mListView->insertString(0, fileName);
				insertFileToListView(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, fileName, IDP_MUSIC_ICON, FILE_ATTRIBUTE_NORMAL);
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
		mListView->deleteAllItems();
		for(i=0;i<count;i++){
			insertFileToListView(LVIF_ITEXT | LVIF_IMAGE | LVIF_PARAM, NULL, img[i], NULL,til[i]);
		}
		mListView->invalidateRect();
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
			
			LISTVIEW_RECORD* record;
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;

			if ((int)(record->m_lvItem.lParam) & FILE_ATTRIBUTE_DIRECTORY)
			{
				File::pathAddBackslash(mCurrentPath);
				String::lstrcat(mCurrentPath, record->m_lvItem.pszText);
				renewFillViewList();
			} else if((int)(record->m_lvItem.lParam) & FILE_ATTRIBUTE_NORMAL){

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