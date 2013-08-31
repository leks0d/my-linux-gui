
#ifndef _MANGO_SESSION_H
#define _MANGO_SESSION_H

namespace mango
{

	//定义每一帧的层数
	#define  SESSION_LAYERS_PER_FRAME		1
	//定义帧数
	#define  SESSION_FRAMES					1	//前台一帧, 后台一帧, GDI 帧 
	//表面数量
	//主表面PRIMARYSURFACE(前台front buffer)
	//离屏OFFSCREENPLAIN表面(后台back buffer) 
	#define  SESSION_SURFACE_COUNT   2   

	#define LANG_NEUTRAL                     0x00
	#define LANG_CHINESE                     0x04
	#define LANG_ENGLISH                     0x09
	#define LANG_JAPANESE                    0x11
			

	class Session 
	{
	public:
		Session();
		~Session();

	public:
		int initialize(class Party* party);
		void start();

		void getScreenInvalidRect(Rect &rect);
		void validateScreen();
		void invalidateScreen(const Rect* rect);
		void getScreenRect(Rect &rect);

		void updateRoutine();
		static unsigned int updateThreadRoutine(void *parameter);

		void keyRoutine();
		static unsigned int keyThreadRoutine(void *parameter);

		void touchRoutine();
		static unsigned int touchThreadRoutine(void *parameter);
		
		void batteryRoutine();
		static unsigned int batteryThreadRoutine(void *parameter);
		void powerKeyRoutine();
		static unsigned int powerKeyThreadRoutine(void *parameter);
		
		void setUseEventInterface(UseEventInterface* i){mUseEventInterface = i;}

		void dispatchTouch(Point &pt, bool pressed);
		void dispatchKeycode(int keycode, bool pressed);
		void setHoldKeyState(int n){holdKetState = n;log_i("setHoldKeyState = %d",n);}
		int getHoldKeyState(){return holdKetState;}
		
		bool mTouchPressed;
		bool mKeyPressed;
		View* mTouchView;
		UseEventInterface* mUseEventInterface;
		Point mPoint;
		int holdKetState;
		int lowVolCount;
	public:
		Screen		mScreen;

		ViewZAxis   mViewZAxis;

		Thread	    mUpdateThread;
		Thread	    mTouchThread;
		Thread	    mKeyThread;
		Thread	    mBatteryThread;		
		Thread	    mPowerKeyThread;
		
		class Party* mParty;

		Mutex		mMutex;  
	};


	typedef struct _StockGraphic
	{
		Canvas  mCanvas;
		Bitmap  mBitmap;
		Pen     mPen;
		Brush   mBrush;
		Font    mFont;

	} StockGraphic;


	class SessionLocal 
	{
	public:
		SessionLocal();
		~SessionLocal();

	public:
		int  initialize(class Party* party);
		int  initializeStockGraphic();
		void  initializeFont();
		void showBootLogo(unsigned char* addr);
		void swapScreenFrontBuffer(); 
		void writeBuffer(void* buf,int len);
		void copyScreenFrontToBack();
		bool loadLanguageCodePage(int langid);
		int  getLangId();
		bool setLangId (int langid);

	public:
		Surface	mSurface[SESSION_SURFACE_COUNT];
		StockGraphic  mStockGraphic;

		int		mfbDevice;		//FB 显示设备
		void   *mfbBuffer;

		#define STOCK_FONT_OBJECT_NUM  10
		unsigned char* mFontWidthBuffer[STOCK_FONT_OBJECT_NUM];
		int      mFontSize[STOCK_FONT_OBJECT_NUM];


		class Party* mParty;
		Mutex mCanvasMutex;
#ifdef WIN32
		EM_FRAMEBUFFER* mEmFrameBuffer;
#endif

		Resource  mResource;

		int			mLanguageId;
		unsigned short *mLanguageCodePage;
	public:
	};

extern Session			gSession;
extern SessionLocal		gSessionLocal;

}

#endif