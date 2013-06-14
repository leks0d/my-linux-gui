namespace mango
{
	class PlayingView: public View
	{
	public:
		PlayingView(void);
		PlayingView(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~PlayingView(void);

	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onPaint(Canvas& canvas);
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 
		virtual int onCommand(int id, int code, View* fromView);

	private:

		Button*  mNextButton;
		Button*  mPrevButton;

	};
};