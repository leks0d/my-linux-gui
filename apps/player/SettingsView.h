
namespace mango
{
	int isHifiMode();


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
	class DigitalFilterListAdapter : public SettingListAdapter
	{
		public:
			DigitalFilterListAdapter(void){}
			~DigitalFilterListAdapter(void){}
			DigitalFilterListAdapter(ListView* list,int id);
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
	class PoweroffListAdapter : public SettingListAdapter
	{
		public:
			PoweroffListAdapter(void){}
			~PoweroffListAdapter(void){}
			PoweroffListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};
	class ForcepoweroffListAdapter : public SettingListAdapter
	{
		public:
			ForcepoweroffListAdapter(void){}
			~ForcepoweroffListAdapter(void){}
			ForcepoweroffListAdapter(ListView* list,int id);
			virtual void PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec);
	};
	class USBSettingListAdapter : public SettingListAdapter
	{
		public:
			USBSettingListAdapter(void){}
			~USBSettingListAdapter(void){}
			USBSettingListAdapter(ListView* list,int id);
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
		void initPowerList();
		void initPlayOrderList();
		void initGaplessList();
		void initAdvanceList();
		void initAutoSleepList();
		void initDigitalFilterList();
		void initLanguageList();
		void initAutoPoweroffList();
		void initForcePoweroffList();
		void initUSBSettingList();
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);}
		int getMainState(){return mainState;}
		void backEvent();
	public:
		static int getLeftByLanguage(int languageId);
	public:
		MediaListView*  mListView;
		TextView* mTitle;
		Button* mBack;
		Button* mHome;
		SettingListAdapter* mPowerListAdapter;
		SettingListAdapter* mSettingListAdapter;
		PlayModeListAdapter* mPlayModeListAdapter;
		GaplessListAdapter* mGaplessListAdapter;
		SettingListAdapter* mAdvanceListAdapter;
		AutoSleepListAdapter* mAutoSleepListAdapter;
		DigitalFilterListAdapter* mDigitalFilterListAdapter;
		LanguageListAdapter* mLanguageListAdapter;
		PoweroffListAdapter* mPoweroffListAdapter;
		ForcepoweroffListAdapter* mForcepoweroffListAdapter;
		USBSettingListAdapter* mUSBSettingListAdapter;
	private:
		int		mMode;					//menu�˵� ý���index �ղؼ� �� ���·��
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
	};

};
