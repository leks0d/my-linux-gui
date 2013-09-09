
namespace mango
{

	class OperateMuiscListAdapter : public SettingListAdapter
	{
		public:
			OperateMuiscListAdapter(void){}
			~OperateMuiscListAdapter(void){}
			OperateMuiscListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};


	class MusicOperateView: public View
	{
	public:
		MusicOperateView(void);
		MusicOperateView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~MusicOperateView(void);
		void setMusicInfo(mediainfo& info){mCurrentInfo = info;initView();}
		void initView();
		void initOperateMuiscList();
		void initPlayList();
	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onPaint(Canvas& canvas);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 
		virtual int onTouchDown(int x, int y, int flag);
		virtual int onTouchMove(int x, int y, int flag);
		virtual int onTouchUp(int x, int y, int flag);

	private:
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);}
		int getMainState(){return mainState;}
	public:
		MediaListView*  mListView;
		mediainfo mCurrentInfo;
		TextView* mTitle;
		Button* mBack;
		Button* mHome;
		ImageView* mDisplayIcon;
		SeekBar* mBrightnessBar;
		TextView* mBrightnessValue;
		
		OperateMuiscListAdapter *mOperateMuiscListAdapter;
		PlayListAdapter *mPlayListAdapter;
	private:
		int		mMode;					//menu²Ëµ¥ Ã½Ìå¿âindex ÊÕ²Ø¼Ð »ò ä¯ÀÀÂ·¾¶
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int mpx;
		int mpy;
	};

};
