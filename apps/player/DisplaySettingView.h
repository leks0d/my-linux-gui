
namespace mango
{

	class DisplaySettingView: public View
	{
	public:
		DisplaySettingView(void);
		DisplaySettingView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~DisplaySettingView(void);
		void setDisplayValue(int n);
		void initView();
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
	
	private:
		int		mMode;					//menu�˵� ý���index �ղؼ� �� ���·��
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int mpx;
		int mpy;
	};

};
