
namespace mango
{

	class ChosenView: public View
	{
	public:
		const static int CHOSEN_POWEROFF = 1;
		const static int CHOSEN_RECOVERY = 2;
		const static int CHOSEN_SYSTEMUPDATE = 3;
		const static int CHOSEN_DELETFILE = 4;
	public:
		ChosenView(void);
		ChosenView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~ChosenView(void);
		void initView();
		void dispatchEvent();
		void setType(int type){mType = type;}
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
		static unsigned int PoweroffRunnig(void *parameter);
	public:
		MediaListView*  mListView;
		mediainfo mCurrentInfo;
		TextView* mTitle;
		Button* mBack;
		Button* mHome;
		ImageView* mDisplayIcon;
		SeekBar* mBrightnessBar;
		TextView* mBrightnessValue;
		TextView* mMyMusicText;
		TextView* mSettingText;
		Thread mPoweroffThread;
	private:
		int		mMode;					//menu菜单 媒体库index 收藏夹 或 浏览路径
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int mpx;
		int mpy;
		int mType;
	};

};
