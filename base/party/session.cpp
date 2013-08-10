
#include <mango.h>
#ifndef WIN32
#include <linux/input.h>
#endif
namespace mango
{
	#define TCC_LCD_FB_IOCTL_DISP_ONOFF				0x10
	#define TCC_LCD_FB_IOCTL_ALPHA_ONOFF			0x11
	#define TCC_LCD_FB_IOCTL_ALPHA_SELECTION		0x16
	static void print_info(struct fb_var_screeninfo *vinfo);
	static int rgb565To888(int rgb);
//	const TCHAR contcFontWidthFileName[STOCK_FONT_OBJECT_NUM][56] ; 

	Session::Session()
	{
		mViewZAxis.mSession = this;
		mUseEventInterface = NULL;
	}

	Session::~Session()
	{

	}
	

	int Session::initialize(Party* party)
	{
		mScreen.mWidth = SCREEN_CX;
		mScreen.mHeight = SCREEN_CY ;
		mScreen.mBitsPerPixel = SCREEN_BITSPERPXIEL; 	
		mParty = party;


		return 0;
	}

	void Session::start()
	{
		mUpdateThread.create(Session::updateThreadRoutine, this);
		mTouchThread.create(Session::touchThreadRoutine, this); 
		mKeyThread.create(Session::keyThreadRoutine, this);
		mBatteryThread.create(Session::batteryThreadRoutine, this);
		mPowerKeyThread.create(Session::powerKeyThreadRoutine, this);
	}

	void Session::getScreenInvalidRect(Rect &rect)
	{
		mMutex.lock();
		
		rect  = mScreen.mInvalidRect;
		
		mMutex.unlock();
	}


	void Session::validateScreen()
	{
		mMutex.lock();

		mScreen.mInvalidRect.set(0, 0, 0, 0);
		
		mMutex.unlock();
	}


	void Session::invalidateScreen(const Rect* rect)
	{
		mMutex.lock();
		
		if (rect)
			mScreen.mInvalidRect.unionRect(rect);
		else
			mScreen.mInvalidRect.set(0, 0, mScreen.mWidth, mScreen.mHeight);

		mMutex.unlock();
	}


	void Session::getScreenRect(Rect &rect)
	{
		rect.set(0, 0, mScreen.mWidth, mScreen.mHeight);
	}

	

	void Session::updateRoutine()
	{
		while (1) {
			Thread::sleep(30);

			mViewZAxis.invalidScreenRectToView();
			if (!mViewZAxis.mExistInvalidateView)
				continue;

			mViewZAxis.sendPainMessageToAllInvalidateView();

			gSessionLocal.swapScreenFrontBuffer();
		}
	}

	unsigned int Session::updateThreadRoutine(void *parameter)
	{
		Session* session = (Session *)parameter;
		session->updateRoutine();
		return 0;
	}


#define  INPUT_EVENT_BUF_SIZE  32
#ifndef EV_SYN
#define EV_SYN 0
#endif

	void Session::keyRoutine()
	{
#ifdef WIN32
		while(1)
		{
			if (gSessionLocal.mEmFrameBuffer->mKeyFlag)
			{
				dispatchKeycode(gSessionLocal.mEmFrameBuffer->mKeyCode, (bool)gSessionLocal.mEmFrameBuffer->mKeyPress);
				gSessionLocal.mEmFrameBuffer->mKeyFlag = 0;
			}
			Thread::sleep(10);
		}
#else

	struct input_event EventBuf[INPUT_EVENT_BUF_SIZE] ;
	int fd ;
	int i, evnetCount, readBytes ;
	int keyCode = -1;
	bool pressed = false;

	if ((fd = open("/dev/input/event1", O_RDONLY)) < 0) {
		log_e("can't open /dev/input/event1 \n") ;
		return ;
	}

	log_i("Open /dev/input/event1 \n") ;

	while (1) 
	{
		readBytes = read (fd, EventBuf, sizeof(struct input_event) * INPUT_EVENT_BUF_SIZE) ;
		if (readBytes < (int) sizeof(struct input_event)) {
			log_e("reading \n");
			break ;
		}

		evnetCount = readBytes / sizeof(struct input_event);
		for (i = 0 ; i < evnetCount ; i++)
		{
			log_i("type = %d, code = %d, value = %d\n", EventBuf[i].type, EventBuf[i].code, EventBuf[i].value);

			switch (EventBuf[i].type)
			{
			case EV_KEY:
				if (EventBuf[i].value == 1)
				{
					if (keyCode != -1)
						break;

					keyCode = EventBuf[i].code;
					pressed = true;
					dispatchKeycode(keyCode, pressed);
				}
				else if (keyCode == EventBuf[i].code)
				{
					pressed = false;
					dispatchKeycode(keyCode, pressed);
					keyCode = -1;
				}
				break ;

			case EV_SYN:
				break;

			default:
				log_d ("Unknow type %d \n", EventBuf[i].type) ;
				break ;
			}
		}

	}

#endif
	}



	unsigned int Session::keyThreadRoutine(void *parameter)
	{
		Session* session = (Session *)parameter;
		session->keyRoutine();
		return 0;
	}



	void Session::touchRoutine()
	{
#ifdef WIN32
		Point pt;

		while(1)
		{
			if (gSessionLocal.mEmFrameBuffer->mTouchFlag)
			{
				pt.x = gSessionLocal.mEmFrameBuffer->mTouchX;
				pt.y = gSessionLocal.mEmFrameBuffer->mTouchY;
				
				dispatchTouch(pt, gSessionLocal.mEmFrameBuffer->mTouchPress) ;
				gSessionLocal.mEmFrameBuffer->mTouchFlag = 0;
			}

			Thread::sleep(10);
		}

#else
		struct input_event EventBuf[INPUT_EVENT_BUF_SIZE] ;
		int		fd ;
		int		i, iEvent, iBytes ;
		Point	pt ;
		BOOL	bDown  ;	

		if ((fd = open("/dev/input/event2", O_RDONLY)) < 0) 
		{
			log_e ("can't open /dev/input/event2 \n") ;
			return ;
		}

		log_i ("Open /dev/input/event2 \n") ;

		pt.x = 0 ;
		pt.y = 0 ;
		bDown = FALSE ;


		while (1) {
			iBytes = read (fd, EventBuf, sizeof(struct input_event) * INPUT_EVENT_BUF_SIZE) ;
			if (iBytes < (int) sizeof(struct input_event)) {
				log_e ("read bytes less sizeof (struct input_event) \n");
				break ;
			}

			iEvent = iBytes / sizeof(struct input_event) ;
			for (i = 0 ; i < iEvent ; i++)
			{

//struct input_event {
//    struct timeval time;  //按键时间
//    __u16 type; //类型，在下面有定义
//    __u16 code; //要模拟成什么按键
//    __s32 value;//是按下还是释放
//};
#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */

				//log_i("type = %d, code = %d, value = %d", EventBuf[i].type, EventBuf[i].code, EventBuf[i].value);

				switch (EventBuf[i].type)
				{
				case EV_ABS:
					if (EventBuf[i].code == ABS_MT_POSITION_X)
						pt.x = EventBuf[i].value ;
					else if (EventBuf[i].code == ABS_MT_POSITION_Y)
						pt.y = EventBuf[i].value ;
					else if (EventBuf[i].code == ABS_MT_TRACKING_ID)
					{
						if (EventBuf[i].value == 1)
							bDown = TRUE;
						else
							bDown = FALSE;
					}

					break ;

				case EV_KEY:
					if (EventBuf[i].code == 330) {
					}
					break ;

				case EV_SYN:
					if (EventBuf[i].code == 0)
						dispatchTouch(pt, bDown) ;

					break ;
				default:
					log_i ("Unknow type %d \n", EventBuf[i].type) ;
					break ;
				}
			}
		}
#endif
	}


	unsigned int Session::touchThreadRoutine(void *parameter)
	{
		Session* session = (Session *)parameter;
		session->touchRoutine();
		return 0;
	}

	void Session::dispatchTouch(Point &pt, bool pressed)
	{
		Point ptInView;
		//log_i("pt.x=%d pt.y=%d pressed=%d", pt.x, pt.y, pressed);

		if (!mTouchPressed && pressed)
			mTouchView = mViewZAxis.getViewFromPoint(pt);

		if (!mTouchView)
			return;

		ptInView = pt;
		
		ptInView.offset(0 - mTouchView->mRect.left, 0 - mTouchView->mRect.top);
		if (!mTouchPressed && pressed)
		{
			gMessageQueue.post(mTouchView, VM_TOUCHDOWN, ptInView.x,ptInView.y);
			mTouchPressed = true;
		}
		else if (mTouchPressed && pressed)
		{
			gMessageQueue.post(mTouchView, VM_TOUCHMOVE, ptInView.x ,ptInView.y);
		}else if (!pressed)
		{
			gMessageQueue.post(mTouchView, VM_TOUCHUP, ptInView.x,ptInView.y);
			mTouchPressed = false;
		}
		mPoint.x=pt.x;
		mPoint.y=pt.y;
		if(mUseEventInterface!=NULL)
			mUseEventInterface->onTouchDispatch(pt.x,pt.y,VM_TOUCHUP);
	}


	void Session::dispatchKeycode(int keycode, bool pressed)
	{
		View* focusView  = mViewZAxis.getFocus();
		if (!focusView)
			return;
		if(!holdKetState)
			return;
		
		
		if (!mKeyPressed && pressed)
		{
			//log_i("dispatchKeycode code=%d,pressed=%d",keycode,pressed);
			if(mUseEventInterface->onKeyDispatch(keycode,VM_KEYDOWN,0))
				return;
			gMessageQueue.post(focusView, VM_KEYDOWN, keycode, 0);
			mKeyPressed = true;
		}
		else if (mKeyPressed && !pressed)
		{
			//log_i("dispatchKeycode code=%d,pressed=%d",keycode,pressed);
			if(mUseEventInterface->onKeyDispatch(keycode,VM_KEYUP,0))
				return;
			gMessageQueue.post(focusView, VM_KEYUP, keycode, 0);
			mKeyPressed = false;
		}
		log_i("dispatchKeycode code=%d,pressed=%d",keycode,pressed);
	}


	unsigned int Session::batteryThreadRoutine(void *parameter)
	{
		Session* session = (Session *)parameter;
		session->batteryRoutine();
		return 0;
	}

	void Session::batteryRoutine(){
		int fd,count,capacity;
		char *rbuf;
		
		rbuf = new char[5];
		
		while(1){
			
			fd = open("/sys/class/power_supply/battery/capacity", O_RDONLY);
			
			if(fd<=0){
				log_i("open /sys/class/power_supply/battery/capacity fail");
				break;
			}
		
			count = read(fd,rbuf,5);

			sscanf(rbuf,"%d\n",&capacity);
			
			if(mUseEventInterface != NULL)
				mUseEventInterface->onKeyDispatch(capacity,VM_CAPACITY,0);		

			close(fd);
			memset(rbuf,0,5);
			
			Thread::sleep(1000);
		}

		close(fd);
	}

	unsigned int Session::powerKeyThreadRoutine(void *parameter){
		Session* session = (Session *)parameter;
		session->powerKeyRoutine();
		return 0;
	}

	void Session::powerKeyRoutine()
	{
		struct input_event EventBuf[INPUT_EVENT_BUF_SIZE] ;
		int fd ;
		int i, evnetCount, readBytes ;
		int keyCode = -1;
		bool pressed = false;
		
		if ((fd = open("/dev/input/event0", O_RDONLY)) < 0) {
			log_e("can't open /dev/input/event0 \n") ;
			return;
		}
		
		log_i("Open /dev/input/event0 \n") ;
		
		while (1) 
		{
			readBytes = read (fd, EventBuf, sizeof(struct input_event) * INPUT_EVENT_BUF_SIZE) ;
			if (readBytes < (int) sizeof(struct input_event)) {
				log_e("reading \n");
				break ;
			}
			
			evnetCount = readBytes / sizeof(struct input_event);
			for (i = 0 ; i < evnetCount ; i++)
			{
				//log_i("type = %d, code = %d, value = %d", EventBuf[i].type, EventBuf[i].code, EventBuf[i].value);
			
				switch (EventBuf[i].type)
				{
				case EV_KEY:
					if(mUseEventInterface!=NULL)
						mUseEventInterface->onKeyDispatch(EventBuf[i].code,EventBuf[i].value?VM_KEYDOWN:VM_KEYUP,0);
					break ;
				}
			}
		}	
		return;	
	}


	
	SessionLocal::SessionLocal()
	{
		mLanguageId = LANGID_SIMPLIFIED;
		mLanguageCodePage = NULL;
	}


	SessionLocal::~SessionLocal()
	{

	}


	int SessionLocal::initialize(Party* party)
	{
		int i,k;
		struct fb_var_screeninfo info;
		
		mParty = party;

#ifdef WIN32
		HANDLE map ;
		void*  address ;

		map     = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, TEXT("/dev/fb0")) ;
		address = MapViewOfFileEx (map, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EM_FRAMEBUFFER) + SCREEN_BUFFER_BYTES * SESSION_SURFACE_COUNT, NULL) ;

		mfbDevice = (int)map ;
		mfbBuffer = (BYTE *)address + sizeof (EM_FRAMEBUFFER) ;

		mEmFrameBuffer = (EM_FRAMEBUFFER *)address ;
		mEmFrameBuffer->m_bVideo  = FALSE ;
		mEmFrameBuffer->m_bVisible = TRUE ;
#else

		int	   ifb ;
		LPVOID pAddress ;

		ifb      = open("/dev/graphics/fb0", O_RDWR) ;
		if (ifb < 0)
		{
			log_e ("can't open /dev/graphics/fb0 \n") ;
			return FALSE ;
		}
		mfbDevice = ifb ;
		
		ioctl(mfbDevice, FBIOGET_VSCREENINFO, &info) ;
		
		print_info(&info);
		info.reserved[0] = 0;
    	info.reserved[1] = 0;
    	info.reserved[2] = 0;
    	info.xoffset = 0;
    	info.yoffset = 0;
    	info.activate = 0;
		info.yres_virtual = 480;
		info.bits_per_pixel = 32;
		
//		if(ioctl(mfbDevice, FBIOPUT_VSCREENINFO, &info)==-1)
//			log_e("FBIOGET_VSCREENINFO set yres_virtual to 480 fail \n") ;
	
		pAddress = mmap(0, SCREEN_BUFFER_BYTES * 2, PROT_READ | PROT_WRITE, MAP_SHARED, mfbDevice, 0);
		if (pAddress == MAP_FAILED)
		{
			log_e ("mmap /dev/fb0 failed \n") ;
			return FALSE ;
		}
		memset((unsigned char*)pAddress, 0x0, SCREEN_BUFFER_BYTES);

		if(munmap(pAddress,SCREEN_BUFFER_BYTES * 2)==0)
			log_i("munmap pAddress sucess.");
		
		if(ioctl(mfbDevice, FBIOPUT_VSCREENINFO, &info)==-1)
			log_e("FBIOGET_VSCREENINFO set yres_virtual to 480 fail \n") ;

		pAddress = mmap(0, SCREEN_BUFFER_BYTES * 2, PROT_READ | PROT_WRITE, MAP_SHARED, mfbDevice, 0);
		if (pAddress == MAP_FAILED)
		{
			log_e ("mmap /dev/fb0 failed \n") ;
			return FALSE ;
		}
		
		showBootLogo((unsigned char*)pAddress);
		
		ioctl(mfbDevice, FBIOPUT_VSCREENINFO, &info);
		
		ioctl(mfbDevice, FBIOPAN_DISPLAY, &info);
		mfbBuffer = pAddress;
		log_i("fb0 buffer address 0x%x \n", pAddress) ;
#endif

		for (i = 0 ; i < SESSION_SURFACE_COUNT ; i++)
		{
			mSurface[i].mWidth      = SCREEN_CX;
			mSurface[i].mHeight     = SCREEN_CY;
			mSurface[i].mBitsPixel  = SCREEN_BITSPERPXIEL;	
			mSurface[i].mWidthBytes = ((SCREEN_CX * SCREEN_BITSPERPXIEL + 31) & (~31) ) >> 3;

			mSurface[i].mBits = (unsigned char*)mfbBuffer + SCREEN_BUFFER_BYTES * i;
			log_i ("address mSurface[%d] = 0x%x \n", i,mSurface[i].mBits) ;
		}
		
		initializeFont();
		gSession.mScreen.mYOffset = 240;
		mStockGraphic.mBitmap.setBits(mSurface[1].mBits);
		initializeStockGraphic();
		
		setLangId(mLanguageId);
		return 0;
	}

	void SessionLocal::showBootLogo(unsigned char* addr){
		unsigned char* logo;
		logo = (unsigned char*)new char[240*320*2];
		int i;
		FILE *fb;
		char *logopath = "/mnt/sdcard/img_12644"; 
		//fb  = fopen("/system/mango/bootlogo", "rb");
		fb  = fopen(logopath, "rb");
		if(fb<=0){
			memset(addr,0xFF,240*320*4);
			return;
		}
		if(fread(logo,240*320*2,1,fb)==1){
			unsigned short int *bmp = (unsigned short int *)logo;
			unsigned int *buffer = (unsigned int *)(addr);
			for(i=0;i<240*320;i++){
				buffer[i]=rgb565To888(bmp[i]);
			}
		}
		fclose(fb);
	}
	static int rgb565To888(int rgb){
		int ret;
		char *pt;
		pt = (char *)&ret;
		pt[0] = (rgb&0x001F)*0xFF/0x1F;
		pt[1] = ((rgb&0x07E0)>>5)*0xFF/0x3F;
		pt[2] = ((rgb&0xF800)>>11)*0xFF/0x1F;
		pt[3] = 0x0;
		return ret;
	}
	int  SessionLocal::initializeStockGraphic()
	{
		int i;
		FILE *file;

		const char contcFontWidthFileName[STOCK_FONT_OBJECT_NUM][56] = 
		{
#ifdef WIN32
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth8.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth12.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth16.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth20.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth24.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth28.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth32.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth36.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth48.dat",
			"E:\\EmluatorStore\\C\\turnip\\cwidth\\cwidth72.dat",
#else
			MANGO_ROOT"cwidth/cwidth8.dat",
			MANGO_ROOT"cwidth/cwidth12.dat",
			MANGO_ROOT"cwidth/cwidth16.dat",
			MANGO_ROOT"cwidth/cwidth20.dat",
			MANGO_ROOT"cwidth/cwidth24.dat",
			MANGO_ROOT"cwidth/cwidth28.dat",
			MANGO_ROOT"cwidth/cwidth32.dat",
			MANGO_ROOT"cwidth/cwidth36.dat",
			MANGO_ROOT"cwidth/cwidth48.dat",
			MANGO_ROOT"cwidth/cwidth72.dat",
#endif
		} ;


		mFontSize[0] = 8;
		mFontSize[1] = 12;
		mFontSize[2] = 16;
		mFontSize[3] = 20;
		mFontSize[4] = 24;
		mFontSize[5] = 28;
		mFontSize[6] = 32;
		mFontSize[7] = 36;
		mFontSize[7] = 48;
		mFontSize[7] = 72;

		for (i = 0 ; i < STOCK_FONT_OBJECT_NUM ; i++)
		{
			mFontWidthBuffer[i] = (unsigned char *)malloc (FONT_WIDTH_BUFF_BYTES);

			file = fopen(contcFontWidthFileName[i], "rb");
			if (!file) {
				log_e ("Can't open font width file [%d] \n", i);
				safeFree(mFontWidthBuffer[i]);
				continue ;
			}

			fread(mFontWidthBuffer[i], 1, FONT_WIDTH_BUFF_BYTES, file);
			fclose(file) ;
		}


		mStockGraphic.mBitmap.set(mSurface[1].mWidth, mSurface[1].mHeight, mSurface[1].mBitsPixel, mSurface[1].mBits);
		mStockGraphic.mPen.set(0, 1, RGB(0, 0, 0));
		mStockGraphic.mBrush.set(0, RGB(255, 255, 255), 0);
		mStockGraphic.mFont.setSize(16);

		mStockGraphic.mCanvas.select(&mStockGraphic.mBitmap);
		mStockGraphic.mCanvas.select(&mStockGraphic.mPen);
		mStockGraphic.mCanvas.select(&mStockGraphic.mBrush);
		mStockGraphic.mCanvas.select(&mStockGraphic.mFont);
#ifdef WIN32
		mResource.open("E:\\EmluatorStore\\C\\programs\\player.data");
#else
		mResource.open(MANGO_ROOT"apps/player.data");
#endif
		return 0;
	}

	void  SessionLocal::initializeFont()
	{
		int   ift_ex_mem_size = 500 * 1024 ;
		const char *ppszfilename[3] ;

		//字体文件名
#ifdef WIN32
		ppszfilename[0] = "E:\\EmluatorStore\\C\\turnip\\font\\arial.ttf";
		ppszfilename[1] = "E:\\EmluatorStore\\C\\turnip\\font\\simsun.ttc";
#else
		ppszfilename[0] = MANGO_ROOT"font/arial.ttf";
		ppszfilename[1] = MANGO_ROOT"font/simsun.ttc";
#endif
		ft_ex_meminit (malloc (ift_ex_mem_size), ift_ex_mem_size) ;

		ft_ex_TrueTypeInit ((char **)ppszfilename, 2) ;
	}


	void SessionLocal::swapScreenFrontBuffer () 
	{
#ifndef WIN32
		struct fb_var_screeninfo vinfo;
#endif
		int ret=0,k;
#ifdef WIN32
		if (gSession.mScreen.mYOffset)
			mEmFrameBuffer->m_yres = 0 ;
		else
			mEmFrameBuffer->m_yres = gSession.mScreen.mHeight;
#else
		ioctl(mfbDevice, FBIOGET_VSCREENINFO, &vinfo) ;

		if (mfbBuffer == mStockGraphic.mBitmap.getBits())
			vinfo.yoffset = 0;
		else
			vinfo.yoffset = 240;
		
		ioctl(mfbDevice, FBIOPUT_VSCREENINFO, &vinfo);
				
		ret = ioctl(mfbDevice, FBIOPAN_DISPLAY, &vinfo) ;		

#endif

		if (gSession.mScreen.mYOffset)
		{
			gSession.mScreen.mYOffset = 0;
			memset((unsigned char*)mSurface[0].mBits, 0x0, SCREEN_BUFFER_BYTES);
			mStockGraphic.mBitmap.setBits(mSurface[0].mBits);
		}
		else
		{
			gSession.mScreen.mYOffset = 240;
			memset((unsigned char*)mSurface[1].mBits, 0x0, SCREEN_BUFFER_BYTES);
			mStockGraphic.mBitmap.setBits(mSurface[1].mBits);
			
		}
	}
	void SessionLocal::writeBuffer(void* buf,int len){
#ifndef WIN32
		struct fb_var_screeninfo vinfo;
#endif
		int ret=0,k;

		void *mbuf = mStockGraphic.mBitmap.getBits();
			
		ioctl(mfbDevice, FBIOGET_VSCREENINFO, &vinfo);

		if (mfbBuffer == mbuf)
			vinfo.yoffset = 0;
		else
			vinfo.yoffset = 240;

		memcpy(mbuf,buf,len);

		ioctl(mfbDevice, FBIOPUT_VSCREENINFO, &vinfo);

		ret = ioctl(mfbDevice, FBIOPAN_DISPLAY, &vinfo);	

	}

	static void print_info(struct fb_var_screeninfo *vinfo){
		FILE *fb;

		fb  = fopen("/etc/fb0info", "rb");
		if(fread(vinfo,sizeof(fb_var_screeninfo),1,fb)==1)
			log_i("fb_var_screeninfo read etc/fb0info success\n");
		fclose(fb);

		log_i("*********************FBIOGET_VSCREENINFO**************************\n");
		log_i("vinfo->xres=%d\n",vinfo->xres);
		log_i("vinfo->yres=%d\n",vinfo->yres);
		log_i("vinfo->xres_virtual=%d\n",vinfo->xres_virtual);
		log_i("vinfo->yres_virtual=%d\n",vinfo->yres_virtual);
		log_i("vinfo->xoffset=%d\n",vinfo->xoffset);
		log_i("vinfo->yoffset=%d\n",vinfo->yoffset);
		log_i("vinfo->bits_per_pixel=%d\n",vinfo->bits_per_pixel);
		log_i("vinfo->grayscale=%d\n",vinfo->grayscale);		
		log_i("******************************END*********************************\n");
		
	}
	bool SessionLocal::loadLanguageCodePage(int langid)
	{
		const char* path = NULL;
		FILE* file = NULL;
		bool success = false;

#define CODEPAGE_FILE_BYTES   (64*1024*2)
		const char conCodePageFileName[][56] = 
		{
#ifdef WIN32
			"E:\\EmluatorStore\\C\\turnip\\codepage\\c_936.bin",
			"E:\\EmluatorStore\\C\\turnip\\codepage\\c_950.bin",
#else
			MANGO_ROOT"codepage/c_936.bin",
			MANGO_ROOT"codepage/c_950.bin",
#endif
		} ;


		switch (langid)
		{
		case LANGID_SIMPLIFIED:
		case LANGID_ENGLISH:
			path = conCodePageFileName[0] ;
			break ;
		case LANGID_TRADITIONAL:
			path = conCodePageFileName[1] ;
			break ;
		}


		do
		{
			if (!path)
				break  ;

			file = fopen(path, "rb");
			if (file == NULL)
			{
				log_e ("Can't open id 0x%x codepage file\n", langid) ;
				break ;
			}

			if (mLanguageCodePage == NULL)
			{
				mLanguageCodePage = (unsigned short *)malloc(CODEPAGE_FILE_BYTES);
				log_i ("allocate memory for codepage 0x%x\n", mLanguageCodePage);
			}

			if (mLanguageCodePage == NULL)
				break ;

			fread(mLanguageCodePage, 1, CODEPAGE_FILE_BYTES, file);

			success = true;

		} while (0) ;
		
		if (file != NULL)
			fclose (file) ;

		return success ;

	}


	int  SessionLocal::getLangId()
	{
		return mLanguageId;
	}


	bool SessionLocal::setLangId (int langid)
	{
		if (!loadLanguageCodePage(langid))
		{
			return false;
		}
		mLanguageId = langid;

		return true;
	}

	Session			gSession;
	SessionLocal	gSessionLocal;
	
}

