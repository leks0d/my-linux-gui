
namespace mango
{
	class PositionManage{
		private:
			CStringArray mPathList;
			CStringArray mPositionList;
		public:
			PositionManage(){}
			void savePosition(char *path ,int pos);
			void savePosition(PTCHAR path ,int pos);
			int getPosition(char *path);
			int getPosition(PTCHAR path);
	};

	class SettingListAdapter : public BaseAdapter
	{
	public:
		SettingListAdapter(void){}
		~SettingListAdapter(void){}
		SettingListAdapter(ListView* list,int id);
		void refresh();
		void setData(int* img,int*imgsec,int* text,int count);
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
	public:
		ListView* mlist;
		int* mImgRes;
		int* mSecImgRes;
		int* mTextRes;
		int mCount;
		int mLayoutleft;
	};
	class RootDirectListAdapter : public SettingListAdapter
	{
		public:
			RootDirectListAdapter(void){}
			~RootDirectListAdapter(void){}
			RootDirectListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};

	class MainListAdapter : public SettingListAdapter
	{
		public:
			MainListAdapter(void){}
			~MainListAdapter(void){}
			MainListAdapter(ListView* list,int id): SettingListAdapter(list,id){}
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};


	class PlayingListAdapter : public BaseAdapter
	{
	public:
		PlayingListAdapter(void){}
		~PlayingListAdapter(void){}
		PlayingListAdapter(ListView* list,int id);
		void setPaintStartOffset(int offset);
		void refresh();
		void backEvent();
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount();
		virtual void* getItem(int index);
		ListView* mlist;
		
	};
	class MusicAdapter : public BaseAdapter
	{
	public:
		MusicAdapter(void){}
		~MusicAdapter(void){}
		MusicAdapter(ListView* list,int id);
		void refresh();
		void setMusicOrderby(int order);
		void rePaintList();
		void setListData(ArrayMediaInfo* info);
		void setWhere(char* wh);
		void setSql(char* sql);
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;
		char *mWhere;
		char *mSql;
		int mOrderBy;
	};

	class AlbumAdapter : public BaseAdapter
	{
	public:
		AlbumAdapter(void){}
		~AlbumAdapter(void){}
		AlbumAdapter(ListView* list,int id);
		void refresh();
		void setWhere(char* where);
		void setArtist(char* art);
		void setGenre(char* genre);
		char* getArtist();
		char* getGenre(){return mGenre.string;}
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
	public:
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;
		MSkBitmap *mMSkBitmap;
		char* sqlWhere;
		char* mArtist;
		CString mGenre;
	};
	class GenreAdapter : public AlbumAdapter
	{
	public:

		GenreAdapter(ListView* list,int id):AlbumAdapter(list,id){}
		void refresh();
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
	};

	class ArtistAdapter : public BaseAdapter
	{
	public:
		ArtistAdapter(void){}
		~ArtistAdapter(void){}
		ArtistAdapter(ListView* list,int id);
		void refresh();
		void setGenre(char *str);
		char* getGenre(){return mGenre.string;}
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;
		CString mGenre;
	};

	class MediaListView : public ListView
	{
	public:
		MediaListView(void);
		MediaListView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~MediaListView(void);

	public:
		virtual int  compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2);
	};


	class MediaView: public View
	{
	public:
		MediaView(void);
		MediaView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		static int getMusicIcon(char* name);
		virtual ~MediaView(void);
		int onTouchDispatch(int x,int y, int action);
		static int getArrayInfoFromFile(char *file,ArrayMediaInfo& array,CString& cover);
		void setMediaViewDisplay(int type);
	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onPaint(Canvas& canvas);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 
		virtual int onCommand(int id, int code, View* fromView);

	private:
		void fillDirectoryFile(TCHAR*  path);
		void renewFillViewList();

		void insertFileToListView(UINT mask, TCHAR* name, int iconId, void* param,int itext = 0,int paramtype = 1);
		bool isRootDirectory();
		bool isVolumRootpath(char *path);
		void backToParentDirectory();
		void initRootDirect();
		void initMainList();
		void initPlayingList();
		void initPlayList();
		void initPlayListMem(PlayListItem& item);
		void initMusicList();
		void initAlbumList();
		void initArtistList();
		void initArtistAlbumList(char* artist);
		void initArtistAlbumMusicList(char* artist,char* album);
		void initGenreArtistAlbumMusicList(char* artist,char* album,char* genre);
		void initGenreList();
		void initGenreArtistList(char* genre = NULL);
		void initGenreArtistAlbumList(char* genre=NULL,char* artist=NULL);
		void initAlbumMusicList(mediainfo* info);
		void initArtistMusicList(char* key,char* value,int state);
		void initGenreMusicList(char* key,char* value,int state);
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);onMainStateChange(mainState);}
		void onMainStateChange(int mainState);
		int getMainState(){return mainState;}
		void playMediaInfo(mediainfo* info,int display);
		void backEvent()	;
		void showOrderByMenu();
		void dismissOrderByMenu();
		bool menuIsShow();
		void initGroupView(char *artist,char *album,char *genre);
	public:
		MediaListView*  mListView;
		TextView* mTitle;
		TextView* mEqState;
		Button* mBack;
		Button* mHome;
		ImageView* mTitleImageView;
		
		PlayingListAdapter* mPlayingListAdapter;
		RootDirectListAdapter* mRootDirectListAdapter;
		MusicAdapter* mMusicAdapter;
		MusicAdapter* mAlbumMusicAdapter;
		MusicAdapter* mArtistMusicAdapter;
		MusicAdapter* mGenreMusicAdapter;
		AlbumAdapter* mAlbumAdapter;
		AlbumAdapter* mArtistAlbumAdapter;
		ArtistAdapter* mArtistAdapter;
		MainListAdapter* mMainListAdapter;
		PlayListAdapter* mPlayListAdapter;
		MusicAdapter* mPlayListMemAdapter;
		GenreAdapter* mGenreAdapter;
		ArtistAdapter* mGenreArtistAdapter;
		AlbumAdapter* mGenreArtistAlbumAdapter;
		MusicAdapter* mGenreArtistAlbumMusicAdapter;
		
		ImageView* mOrderMenuBk;
		TextView* mOrderByTitle;
		TextView* mOrderByAlbum;
		TextView* mOrderByArtist;
		TextView* mOrderByTime;
	private:
		int		mMode;					//menu²Ëµ¥ Ã½Ìå¿âindex ÊÕ²Ø¼Ð »ò ä¯ÀÀÂ·¾¶
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int     mOrderMenuShow;
		char    *mNeedPlayPath;
		int     mCurrentPlaylistId;
		PositionManage mPositionManage;
	};


};
