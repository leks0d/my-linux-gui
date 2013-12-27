
namespace mango
{
#define MAX_EQ 24
#define MAX_CN MAX_EQ/2

	class EqSettingsView: public View
	{
	public:
		EqSettingsView(void);
		EqSettingsView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~EqSettingsView(void);
		void initView();
		void addPoint(int x,int y);
		void pointClear();
		void resetEqValue();
		void setCurrentEqMode(int n);
		void setCurrentEqValue(int id,int val);
		void setEqModeToReset();
		int progressToValue(int n){return MAX_CN-n;}
		int valueToProgress(int n){return MAX_CN-n;}
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
		int		mMode;					//menu²Ëµ¥ Ã½Ìå¿âindex ÊÕ²Ø¼Ð »ò ä¯ÀÀÂ·¾¶
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int* mpx;
		int* mpy;
		int count;
		int mMax;
		int isNeedPaint;
		TextView* mEqMode[6];
		TextView* mEqFrequency[8];
		TextView* mEqValue[8];
		VerticalSeekBar* mVerticalSeekBar[8];
		TextView*	mControlButton;
		TextView*	mResetButton;
		int mCurrentMode;
		int *mCurrentValue;
		int mEqOpen;
	};

};
