namespace mango
{
	class USBHiFi: public View
	{
	public:
		USBHiFi(void);
		USBHiFi(const TCHAR* title, View* parent, Rect* rect, int style, int show = SW_NORMAL);
		virtual ~USBHiFi(void);

	public:
		virtual int onCreate();
		virtual int onDestroy();
		virtual int onPaint(Canvas& canvas);
		virtual int onNotify(View* fromView, int code, void* parameter);
		virtual int onKeyDown(int keyCode, int flag);
		virtual int onKeyUp(int keyCode, int flag); 
		virtual int onCommand(int id, int code, View* fromView);
		virtual int onTouchUp(int x, int y, int flag);


	private:
		int  mTouchCount;
		FILE* mfp;

		Thread mReadUSBThread;
		Thread mSendCodecThread;

	};
	class PcmWrite{
		public:
			PcmWrite(void){}
			~PcmWrite(void){}
			void start();
	};
};