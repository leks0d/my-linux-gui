
namespace mango
{
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
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;
		char *mWhere;
	};

	class AlbumAdapter : public BaseAdapter
	{
	public:
		AlbumAdapter(void){}
		~AlbumAdapter(void){}
		AlbumAdapter(ListView* list,int id);
		void refresh();
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;
		MSkBitmap *mMSkBitmap;
	};

	class ArtistAdapter : public BaseAdapter
	{
	public:
		ArtistAdapter(void){}
		~ArtistAdapter(void){}
		ArtistAdapter(ListView* list,int id);
		void refresh();
		virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
		virtual int getCount(){return 0;}
		virtual void* getItem(int index){return 0;}
		ListView* mlist;
		ArrayMediaInfo *mMusicArrayList;	
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
		static int getArrayInfoFromFile(char *file,ArrayMediaInfo& array);
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
		void initMusicList();
		void initAlbumList();
		void initArtistList();
		void initAlbumMusicList(char* album);
		void initSpecMusicList(char* key,char* value,int state);
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);onMainStateChange(mainState);}
		void onMainStateChange(int mainState);
		int getMainState(){return mainState;}
		void playMediaInfo(mediainfo* info,int display);
		void backEvent()	;
		void showOrderByMenu();
		void dismissOrderByMenu();
		bool menuIsShow();
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
		AlbumAdapter* mAlbumAdapter;
		ArtistAdapter* mArtistAdapter;
		MainListAdapter* mMainListAdapter;
		
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
	};


};
