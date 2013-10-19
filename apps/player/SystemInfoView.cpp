#include "player.h"


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
	SystemInfoView::SystemInfoView(void)
	{

	}

	SystemInfoView::SystemInfoView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mModelNumber = NULL;
		mFirmwareVersion = NULL;
		mStorageSpace = NULL;
	}

	SystemInfoView::~SystemInfoView(void)
	{

	}
	
	int SystemInfoView::onCreate()
	{
		Rect rect;
		int left = 40;
		int top = 60;
		int dinner = 33;
		int size = 17;
		int color = 255;
		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(RGB(255,255,255));
		mTitle->onCreate();

		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();
		
		rect.setEx(left, top, 320-left, 20);
		mModelNumber = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mModelNumber->setTextResoure(STR_SYSTEM_INFO_MODEL);
		mModelNumber->setTextSize(size);
		mModelNumber->setTextColor(RGB(color,color,color));
		mModelNumber->onCreate();
		top+=dinner;
		
		rect.setEx(left, top, 320-left, 20);
		mFirmwareVersion = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mFirmwareVersion->setTextResoure(STR_SYSTEM_INFO_VERSION);
		mFirmwareVersion->setTextSize(size);
		mFirmwareVersion->setTextColor(RGB(color,color,color));
		mFirmwareVersion->onCreate();
		top+=dinner;
		
		rect.setEx(left, top, 320-left, 20);
		mStorageSpace = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mStorageSpace->setTextResoure(STR_SYSTEM_INFO_INTERNAL);
		mStorageSpace->setTextSize(size);
		mStorageSpace->setTextColor(RGB(color,color,color));
		mStorageSpace->onCreate();
		top+=dinner;
		
		rect.setEx(left, top, 320-left, 20);
		mUesedSpace = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mUesedSpace->setTextResoure(STR_SYSTEM_INFO_EXTERNAL);
		mUesedSpace->setTextSize(size);
		mUesedSpace->setTextColor(RGB(color,color,color));
		mUesedSpace->onCreate();
		top+=dinner;
/*		
		rect.setEx(left, top, 320-left, 20);
		mAvailSpace = new ValueTextView(SETTING_BACK, TEXT("mFilename"), this, &rect, 0);
		mAvailSpace->setTextResoure(STR_SYSTEM_INFO_FREEPACE);
		mAvailSpace->setTextSize(size);
		mAvailSpace->setTextColor(RGB(color,color,color));
		mAvailSpace->onCreate();
	
		rect.setEx(0, 150, 320, 22);
		mStorageSeekBar = new SeekBar(SETTING_BACK, TEXT("mStorageSeekBar"), this, &rect, 0);
		mStorageSeekBar->setBackgroundRes(IDP_STORAGE_SPACE,0,0);
		mStorageSeekBar->setSeekgroundRes(IDP_STORAGE_USE,5,5);
		mStorageSeekBar->setMax(100);
		mStorageSeekBar->setProgress(38);
		mStorageSeekBar->onCreate();
*/
		initView();

		setFocus(this);
		invalidateRect();
		return 0;
	}
	void SystemInfoView::addPoint(int x,int y)
	{

	}

	void SystemInfoView::pointClear(){

	}
	void SystemInfoView::initView()
	{
		mTitle->setTextResoure(STR_ADVANCE_SYSINFO);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mModelNumber->setTextString("DX50");
		mFirmwareVersion->setTextString("V1.2.2");
		
		updateTotalMem();
	}
	void SystemInfoView::updateTotalMem(){
		int total,avail,free;
		char iSpaceAll[10],iSpaceAvail[10],espaceAll[10],eSpaceAvail[10];
		char external[20],internal[20],*ptr;
		char *ipath = "/mnt/sdcard";
		char *epath = "/mnt/external_sd";
		Mstring *mstr;
		mstr = new Mstring(20);
		
		Environment::space_info(ipath,total,avail,free);
		Environment::memSizeToStr(total,iSpaceAll);
		Environment::memSizeToStr(avail,iSpaceAvail);
		mstr->mStringSprintf("%s left/",iSpaceAvail);
		mstr->mStringSprintf("%s",iSpaceAll);
		mStorageSpace->setTextString(mstr->mstr);

		mstr->clear();
		Environment::space_info(epath,total,avail,free);
		Environment::memSizeToStr(total,espaceAll);
		Environment::memSizeToStr(avail,eSpaceAvail);
		mstr->mStringSprintf("%s left/",eSpaceAvail);
		mstr->mStringSprintf("%s",espaceAll);
		
		mUesedSpace->setTextString(mstr->mstr);
		delete mstr;
	}

	int SystemInfoView::onDestroy()
	{
		return 0;
	}

	int SystemInfoView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		canvas.drawImageResource(IDP_PLAYING_BACKGROUND,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);	
		
		return 0;
	}

	int SystemInfoView::onNotify(View* fromView, int code, void* parameter)
	{
		if(fromView == NULL && code == NM_DISPLAY){
			initView();
		}else if(fromView == mBack && NM_CLICK){
			gPlayer.showSettingsView();
		}else if(fromView == mHome && NM_CLICK){
			gPlayer.showPlayingView();
		}
		
		return 0;
	}

	int SystemInfoView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				break;
		}

		return 0;
	}

	int SystemInfoView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode){
			case KEYCODE_BACK:
				//gPlayer.showSettingsView();
				break;
		}
		return 0;
	}

	int SystemInfoView::onTouchDown(int x, int y, int flag)
	{

		return -1;
	}
	int SystemInfoView::onTouchMove(int x, int y, int flag)
	{
		return -1;

	}
	int SystemInfoView::onTouchUp(int x, int y, int flag){
		return -1;
		}
};
