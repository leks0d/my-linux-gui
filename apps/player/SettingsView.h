
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
	};
	class PlayModeListAdapter : public SettingListAdapter
	{
		public:
			PlayModeListAdapter(void){}
			~PlayModeListAdapter(void){}
			PlayModeListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};
	class GaplessListAdapter : public SettingListAdapter
	{
		public:
			GaplessListAdapter(void){}
			~GaplessListAdapter(void){}
			GaplessListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};
	class AutoSleepListAdapter : public SettingListAdapter
	{
		public:
			AutoSleepListAdapter(void){}
			~AutoSleepListAdapter(void){}
			AutoSleepListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};
	class LanguageListAdapter : public SettingListAdapter
	{
		public:
			LanguageListAdapter(void){}
			~LanguageListAdapter(void){}
			LanguageListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};

	class SettingsView: public View
	{
	public:
		SettingsView(void);
		SettingsView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~SettingsView(void);

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
		void initAdvanceList();
		void initAutoSleepList();
		void initLanguageList();
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);}
		int getMainState(){return mainState;}
		void backEvent();
	public:
		MediaListView*  mListView;
		TextView* mTitle;
		Button* mBack;
		Button* mHome;
		SettingListAdapter* mSettingListAdapter;
		PlayModeListAdapter* mPlayModeListAdapter;
		GaplessListAdapter* mGaplessListAdapter;
		SettingListAdapter* mAdvanceListAdapter;
		AutoSleepListAdapter* mAutoSleepListAdapter;
		LanguageListAdapter* mLanguageListAdapter;
	private:
		int		mMode;					//menu²Ëµ¥ Ã½Ìå¿âindex ÊÕ²Ø¼Ð »ò ä¯ÀÀÂ·¾¶
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
	};

};
