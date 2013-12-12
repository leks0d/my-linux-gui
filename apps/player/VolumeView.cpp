#include "player.h"

#define VOLUME_REFRESH 100
#define VOLUME_TIME 1000/VOLUME_REFRESH
#define NOCHANG 0

namespace mango
{

	enum
	{
		ADAPTER_PLAYING = 0xf0c0,	
		ADAPTER_FILE = 0xf0c1,
		ADAPTER_ALBUM = 0xf0c2,
		ADAPTER_ARTIST = 0xf0c3,
		ADAPTER_MUSIC = 0xf0c4,
	};
	VolumeView::VolumeView(void)
	{

	}

	VolumeView::VolumeView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		count = mMax = 0;
		mpx = mpy = NULL;
		isNeedPaint = 1;
		closeCount = VOLUME_TIME;
		setThreadState(false);
	}

	VolumeView::~VolumeView(void)
	{

	}
	
	int VolumeView::onCreate()
	{
		Rect rect;
		int firstLeft = 98;
		
		rect.setEx((320-60)/2,95,60,15);
		mVolumeStr = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mVolumeStr->setTextColor(COLOR_WHITE);
		mVolumeStr->setTextResoure(STR_VOLUME);
		mVolumeStr->setTextSize(15);
		mVolumeStr->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mVolumeStr->onCreate();
		
		rect.setEx((320-60)/2,105,60,60);
		mVolumeText = new TextView(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mVolumeText->setTextColor(COLOR_WHITE);
		mVolumeText->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mVolumeText->setTextSize(32);
		mVolumeText->onCreate();
		
		initView();	

		setFocus(this);
		invalidateRect();

		return 0;
	}

	void VolumeView::setVolume(int vol){
		Mstring* mstr;
		int volume;
		
		mstr = new Mstring(10);
		volume = vol;
		mVolume = vol;
		mstr->mSprintf("%d",volume);
		
		mVolumeText->setTextString(mstr->mstr);

		delete mstr;
	}
	
	unsigned int VolumeView::VolumeRunning(void* p){
		VolumeView* iVolume = (VolumeView*)p;
		int vol,closeCount;

		iVolume->setThreadState(true);
		
		closeCount = VOLUME_TIME;
		vol = -1;
		while (1) {
			Thread::sleep(VOLUME_REFRESH);
			
			log_i("isKeyDown=%d,mDownKeyCode=%d",iVolume->isKeyDown,iVolume->mDownKeyCode);

			if(iVolume->isKeyDown){
				
				vol = gPlayer.getVolume();
				
				switch(iVolume->mDownKeyCode){
					case KEYCODE_VOLUMEUP:					
						vol++;
						break;
					case KEYCODE_VOLUMEDOWN:
						vol--;
						break;
				}
				
				if(vol>=0&&vol<=255){
					gPlayer.setVolume(vol);
					iVolume->setVolume(vol);
				}

				closeCount = VOLUME_TIME;
			}
			else{
				closeCount--;
				if(closeCount<=0)
					break;
			}			
		}
		gSettingProvider.update(SETTING_VOLUME_ID,gPlayer.getVolume());
		gMessageQueue.post(iVolume,VM_NOTIFY,NM_DISMISS,0);
		
		iVolume->setThreadState(false);
	}
	
	void VolumeView::initView()
	{
		Mstring* mstr;
		int brightness;
		int volume;
		
		mstr = new Mstring(5);
		volume = gPlayer.getVolume();
		mVolume = volume;
		mstr->mSprintf("%d",volume);
		
		mVolumeText->setTextString(mstr->mstr);

		if(getThreadState() == false)
			mVolumeThread.create(VolumeRunning,this);
		else
			log_i("Volume Thread running");
	}

	int VolumeView::onDestroy()
	{
		return 0;
	}

	int VolumeView::onPaint(Canvas& canvas)
	{
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_VULUME_ICON_BK,(320-204)/2,(240-204)/2,true);
		drawVolumeImage(canvas, (320-204)/2,(240-204)/2, mVolume);
		//drawVolumeImage(canvas, (320-204)/2,(240-204)/2, 255);
		return 0;
	}

	int VolumeView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == NULL && code == NM_DISMISS){
			gPlayer.dismissView(this);
			isShow = 0;
		}else if(code == NM_KEY_LOCK){
			isKeyDown = 0;
		}
		
		return 0;
	}
	void VolumeView::onKeyAction(int keyCode,int action){
		log_i("onKeyAction isKeyDown=%d",isKeyDown);
		if(action == VM_KEYUP){
			switch(keyCode){
				case KEYCODE_VOLUMEUP:
				case KEYCODE_VOLUMEDOWN:
					isKeyDown = 0;
					mDownKeyCode = keyCode;
					log_i("-------------------isKeyUP------------------------");
					break;
			}
		}else if(action == VM_KEYDOWN){
			switch(keyCode){
				case KEYCODE_VOLUMEUP:
				case KEYCODE_VOLUMEDOWN:
					isKeyDown = 1;
					mDownKeyCode = keyCode;
					log_i("-------------------isKeyDOWN------------------------");
					break;
			}	
		}
	}
	
	int VolumeView::onKeyDown(int keyCode, int flag)
	{
#if 0
		switch(keyCode){
			case KEYCODE_VOLUMEUP:
			case KEYCODE_VOLUMEDOWN:
				isKeyDown = 1;
				mDownKeyCode = keyCode;
				break;
		}
#endif
		return 0;
	}

	int VolumeView::onKeyUp(int keyCode, int flag)
	{
#if 0
		switch(keyCode){
			case KEYCODE_VOLUMEUP:
			case KEYCODE_VOLUMEDOWN:
				isKeyDown = 0;
				mDownKeyCode = keyCode;
				break;
		}
#endif
		return 0;
	}

	int VolumeView::onTouchDown(int x, int y, int flag)
	{
		return -1;
	}
	int VolumeView::onTouchMove(int x, int y, int flag)
	{
		return -1;

	}
	int VolumeView::onTouchUp(int x, int y, int flag)
	{
		return -1;
	}

	int VolumeView::volumeToGuage(int volume)
	{
#if NOCHANG
		return volume;		
#else
		int guage = 0;

		if (volume > 224){
			guage = 128 * 3 + (volume - 224) * 8;
		}else if (volume > 192){
			guage = 128 * 2 + (volume - 192) * 4;
		}else if (volume > 128){
			guage = 128 + (volume - 128) * 2;
		}else{
			guage = volume;
		}
		return guage*255/632;
#endif
	}
	#define VOLUME_IMAGE_MAX_BYTES  (204 * 204 * 4 + 32)

	void VolumeView::loadVolumeImage(void)
	{
		mVolumeImageBuffer = malloc(VOLUME_IMAGE_MAX_BYTES);
		if (mVolumeImageBuffer == NULL)
			return;

		if (gSessionLocal.mResource.loatBitmap(IDP_VULUME_ICON, &mVolumeImageHeader, mVolumeImageBuffer, VOLUME_IMAGE_MAX_BYTES) <= 0)
			return;
	}

	void VolumeView::releaseVolumeImage(void)
	{
		safeFree(mVolumeImageBuffer);
	}

	int  VolumeView::getAngleFromVolume(int volume)
	{
#if NOCHANG
		return 360 - (360 * volume / 255);
#else
		int offset = 36;
		return 360 - ((360-offset) * volumeToGuage(volume) / 255+offset/2);
#endif		
	}

	int VolumeView::getQuadrant(int angle)
	{
		if (angle >= 360 || angle < 0)
			return 0;

		return angle / 90 + 1;
	}

	int VolumeView::getQuadrant(int x, int y)
	{
		if (x > 0 && y >= 0)
			return 1;
		else if (x <= 0 && y > 0)
			return 2;
		else if (x < 0 && y <= 0)
			return 3;
		else
			return 4;
	}

	void VolumeView::drawVolumeImage(Canvas& canvas, int x0, int y0, int volume)
	{
		loadVolumeImage();

		int width  = mVolumeImageHeader.m_iWidth;
		int height = mVolumeImageHeader.m_iHeight;
		int originX = width  / 2;
		int originY = height / 2;
		int relativeX;
		int relativeY;
		int x, y;
		void* imageBuffer;

		int volumeAngle = getAngleFromVolume(volume);
		int sinDB;
		int volumeQuadrant = getQuadrant(volumeAngle);
		int quadrant;
		int rDB;
		int volumeSin = sin(volumeAngle * 3.1415926 / 180.0) * 1000;
		int volumeSinDB = volumeSin * volumeSin;

		int* destImageBuffer = (int *)malloc(VOLUME_IMAGE_MAX_BYTES);
		int  pixl = 0;
		int clr = 0x0000ff;


		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				relativeX = y - originY;//x - originX;
				relativeY = x - originX;//originY - y;

				quadrant = getQuadrant(relativeX, relativeY);
				imageBuffer = NULL;

				if (quadrant > volumeQuadrant)
					imageBuffer = mVolumeImageBuffer;
				else if (quadrant < volumeQuadrant)
				{
					//imageBuffer = mVolumeBkImageBuffer;
				}
				else
				{
					//same quadrant, ÄÚ°ë¾¶82, Íâ97
					rDB = relativeX * relativeX + relativeY * relativeY;
					if (rDB > 100 * 100 || rDB < 80 * 80)
					{
						//imageBuffer = mVolumeImageBuffer;
					}
					else 
					{
						sinDB = relativeY * relativeY * 100 * 100 * 10 / rDB * 10;
						if (sinDB > volumeSinDB)
						{
							if (quadrant == 1 || quadrant == 3)
								imageBuffer = mVolumeImageBuffer;
						}
						else if (quadrant == 2 || quadrant == 4)
						{
							imageBuffer = mVolumeImageBuffer;
						}
					}

				}

				if (imageBuffer == mVolumeImageBuffer)
					*(destImageBuffer + pixl) = 0xff0000ff;
				else
					*(destImageBuffer + pixl) = 0xff00ff00;

				if (imageBuffer == NULL)
					*(destImageBuffer + pixl) = 0x00;
				else
					*(destImageBuffer + pixl) = *(((int*)imageBuffer) + pixl); 
				
				pixl++;
			}
		}


		canvas.drawBitmap(destImageBuffer, x0, y0, width, height);
		releaseVolumeImage();
		safeFree(destImageBuffer);
	}

	bool VolumeView::getThreadState(){
		bool ret;

		mThreadMutex.lock();
		
		ret = isThreadRunning;

		mThreadMutex.unlock();
		
		return ret;
	}
	void VolumeView::setThreadState(bool run){
		mThreadMutex.lock();
		isThreadRunning = run;
		mThreadMutex.unlock();
	}
};
