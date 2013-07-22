
namespace mango
{

	class VolumeView: public View
	{
	public:
		VolumeView(void);
		VolumeView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~VolumeView(void);
		void initView();
		void addPoint(int x,int y);
		void pointClear();
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
		int isShowing(){return isShow;}

	private:
		void setMainState(int state){ mainState = state;log_i("mainState = 0x%x",mainState);}
		int getMainState(){return mainState;}
		static unsigned int VolumeRunning(void* p);
		void setVolume(int vol);
	public:
		MediaListView*  mListView;
		mediainfo mCurrentInfo;
		TextView* mVolume;
		TextView* mVolumeStr;
		Button* mBack;
		Button* mHome;
		ImageView* mDisplayIcon;
		SeekBar* mBrightnessBar;
		TextView* mBrightnessValue;
		int isShow;
		Thread mVolumeThread;
	private:
		int		mMode;					//menu菜单 媒体库index 收藏夹 或 浏览路径
		int		mPath;
		int		mSubindex;
		TCHAR	mCurrentPath[MAX_PATH];
		int 	mainState;
		int* mpx;
		int* mpy;
		int count;
		int mMax;
		int isNeedPaint;
		int isKeyDown;
		int mDownKeyCode;
		int closeCount;
	};

};
