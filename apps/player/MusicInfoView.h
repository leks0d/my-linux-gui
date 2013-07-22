
namespace mango
{

	class MusicInfoView: public View
	{
	public:
		MusicInfoView(void);
		MusicInfoView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~MusicInfoView(void);
		void initView();
	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onPaint(Canvas& canvas);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 


	private:
		void initMainList();
		void initPlayOrderList();
		void initGaplessList();
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);}
		int getMainState(){return mainState;}
	public:
		MediaListView*  mListView;
		mediainfo mCurrentInfo;
		TextView* mTitle;
		Button* mBack;
		Button* mHome;
		ImageView* mAlbumImage;
		ValueTextView* mFilename;
		ValueTextView* mMusicTitle;
		ValueTextView* mAlbum;
		ValueTextView* mArtist;
		ValueTextView* mDuration;
		ValueTextView* mSimpleRate;
		ValueTextView* mSimpleBit;
		ValueTextView* mSimpleBps;
		ValueTextView* mMusicPath;

	private:
		int		mMode;					//menu菜单 媒体库index 收藏夹 或 浏览路径
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
	};

};
