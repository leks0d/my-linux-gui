#include "player.h"


namespace mango
{
#define USB_FUNCTION_PATH 	"/sys/class/android_usb/android0/functions"
#define USB_STATE_PATH 	"/sys/class/android_usb/android0/state"
#define USB_FUNCTION_HIFI "hifi\n"
#define USB_FUNCTION_STORAGE "mass_storage\n"
#define USB_FUNCTION_ADB "adb\n"
#define USB_FUNCTION_NONE "none\n"
#define USB_CONNECT_STAT "DISCONNECTED\n"

	int isHifiMode(){
		int fd = -1;
		int result = 0;
		char buf[20] = {0};
		char board[PROP_VALUE_MAX]={0};
		
		__system_property_get("persist.usb.debug",board);	
			
		fd = open(USB_FUNCTION_PATH, O_RDONLY);
		if(fd<=0){
				log_i("open %s fail",USB_FUNCTION_PATH);
				return 0;
		}
		read(fd,buf,20);
		close(fd);
		
		if(strcmp(buf,USB_FUNCTION_HIFI)==0){
				result = 1;
		}else if(strcmp(buf,USB_FUNCTION_STORAGE)==0 || strcmp(buf,USB_FUNCTION_ADB)==0){
				if(strcmp(board,"0")==0)
					result = 0;
				else
					result = 2;
		}
		return result;
	}	
	void setUsbMode(int mode){
		if(mode == 1){
			log_i("------------>set usb hifi");
			system("stop usbd");
			system("setprop persist.usb.debug 1");
			system("setprop persist.sys.usb.config hifi");
			system("start usbd");
		}else if(mode == 0){
			log_i("------------>set usb mass_storage");
			system("setprop persist.usb.debug 0");
			system("setprop persist.sys.usb.config mass_storage");
		}else if(mode == 2){
			char board[PROP_VALUE_MAX]={0};
			__system_property_get("persist.mango.storage.fun",board);

			if(strcmp(board,"adb")==0){
				log_i("------------>set usb adb");
				system("openadb");
			}else{
				log_i("------------>set usb chager");				
				system("setprop persist.usb.debug 1");
				system("setprop persist.sys.usb.config mass_storage");
			}
		}
	}
	void setDigitalState(int val){
		Environment::setDigitalFilter(val);
		gSettingProvider.update(SETTING_DIGITAL_FILTER_ID,val);
	}
	int getDigitalState(){
		int value = 0;
		if(gSettingProvider.query(SETTING_DIGITAL_FILTER_ID,&value))
			return value;
		return -1;
	}
	enum
	{
		ADAPTER_PLAYING = 0xf0c0,	
		ADAPTER_FILE = 0xf0c1,
		ADAPTER_ALBUM = 0xf0c2,
		ADAPTER_ARTIST = 0xf0c3,
		ADAPTER_MUSIC = 0xf0c4,
	};
	SettingsView::SettingsView(void)
	{

	}

	SettingsView::SettingsView(const TCHAR* title, View* parent, Rect* rect, int style, int show) 
		: View(title, parent, rect, style, show)
	{
		mSettingListAdapter = NULL;
		mGaplessListAdapter = NULL;
		mPlayModeListAdapter = NULL;
		mAdvanceListAdapter = NULL;
		mLanguageListAdapter = NULL;
		mAutoSleepListAdapter = NULL;
		mPowerListAdapter = NULL;
		mPoweroffListAdapter = NULL;
		mForcepoweroffListAdapter = NULL;
		mUSBSettingListAdapter = NULL;
		mDigitalFilterListAdapter = NULL;
	}

	SettingsView::~SettingsView(void)
	{

	}
	
	int SettingsView::onCreate()
	{
		Rect rect;

		rect.setEx(0,21,320,219);
		mListView = new MediaListView(TEXT("Media List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->setListViewBackground(IDP_PLAYING_LIST_BACK);
		mListView->onCreate();

		rect.setEx(0, 0, 41, 22);
		mBack = new Button(SETTING_BACK, TEXT("mBack"), this, &rect, 0);
		mBack->setNormalImageId(IDP_SETTING_BACK);
		mBack->setPressedImageId(IDP_SETTING_BACK_SEC);
		mBack->onCreate();
		
		rect.setEx(45, 0, 230, 20);
		mTitle = new TextView(SETTING_TITLE, TEXT("mTitle"), this, &rect, 0);
		mTitle->setTextColor(COLOR_TEXT);
		mTitle->onCreate();
		
		rect.setEx(280, 0, 41, 22);
		mHome = new Button(SETTING_HOME, TEXT("mHome"), this, &rect, 0);
		mHome->setNormalImageId(IDP_MUSIC_HOME);
		mHome->setPressedImageId(IDP_MUSIC_HOME_SEC);
		mHome->onCreate();
		
		initMainList();

		setFocus(this);
		invalidateRect();
		return 0;
	}


	int SettingsView::onDestroy()
	{
		if (mListView)
			mListView->onDestroy();
		
		safeDelete(mListView);
		return 0;
	}
	
	int SettingsView::onPaint(Canvas& canvas)
	{
		log_i("MediaView::onPaint");
		Environment::drawWallpaper(canvas);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,true);
		return 0;
	}


	void SettingsView::initAdvanceList(){
		int img[]={IDP_ADVANCE_LANGUGE,IDP_ADVANCE_DISPLAY,IDP_ADVANCE_BATTARY,IDP_ADVANCE_LIBRARY,IDP_ADVANCE_SYSTEM_INFO,IDP_SYSTEM_RESET,IDP_ADVANCE_SYSTEM_INFO};
		int imgsec[]={IDP_ADVANCE_LANGUGE_S,IDP_ADVANCE_DISPLAY_S,IDP_ADVANCE_BATTARY_S,IDP_ADVANCE_LIBRARY_S,IDP_ADVANCE_SYSTEM_INFO_S,IDP_SYSTEM_RESET_S,IDP_ADVANCE_SYSTEM_INFO_S};
		int text[]={STR_ADVANCE_LANGUAGE,STR_ADVANCE_DISPLAY,STR_ADVANCE_POWER,STR_RESCAN_LIBRARY,STR_ADVANCE_SYSINFO,STR_SYSTEM_RESTET,STR_SYSTEM_UPDATE};

		int i,count = sizeof(img)/sizeof(int);

		if(FileAttr::FileExist("/mnt/sdcard/update.img")){
			count = count;
		}else{
			count = count-1;
		}
		
		log_i("SettingsView::initAdvanceList");
		
		if(mAdvanceListAdapter == NULL){
			mListView->deleteAllItems();
			mAdvanceListAdapter = new SettingListAdapter(mListView,ADAPTER_PLAYING);
			
		}
		mAdvanceListAdapter->setData(img,imgsec,text,count);
		mAdvanceListAdapter->mLayoutleft = getLeftByLanguage(gSessionLocal.getLangId());
		
		mTitle->setTextResoure(STR_SETTING_ADVANCED);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1400);
	}
	void SettingsView::initMainList(){
		int img[]={IDP_SETTING_EQ,IDP_SETTING_PLAYORDER,IDP_SETTING_GAPLESS,IDP_SETTING_MUSICINFO,IDB_DUSBOTG_ICON,IDP_SETTING_DF,IDP_SETTING_ADVANCED};
		int imgsec[]={IDP_SETTING_EQ_S,IDP_SETTING_PLAYORDER_S,IDP_SETTING_GAPLESS_S,IDP_SETTING_MUSICINFO_S,IDB_DUSBOTG_ICON_S,IDP_SETTING_DF_S,IDP_SETTING_ADVANCED_S};
		int text[]={STR_SETTING_EQ,STR_SETTING_PLAYOODER,STR_SETTING_GAPLESS,STR_SETTING_MUSICINFO,STR_USB_CONECT_SETTING,STR_SETTING_DIGITAL_FILTER,STR_SETTING_ADVANCED};
		int i,count = sizeof(img)/sizeof(int);
		
		mListView->deleteAllItems();
		
		if(mSettingListAdapter == NULL){
			mSettingListAdapter = new SettingListAdapter(mListView,ADAPTER_PLAYING);
			mSettingListAdapter->setData(img,imgsec,text,count);
		}else
			mSettingListAdapter->refresh();

		mSettingListAdapter->mLayoutleft = getLeftByLanguage(gSessionLocal.getLangId());
		
		mTitle->setTextResoure(MUSIC_MUSIC_FUN);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1000);
	}
	void SettingsView::initPowerList(){
		int img[]={0,0};
		int imgsec[]={0,0};
		int text[]={STR_POWER_SCREEN_OFF,STR_POWER_POWER_OFF,STR_POWER_SLEEP};
		int i,count = sizeof(text)/sizeof(text[0]);
		
		if(mPowerListAdapter == NULL){
			mListView->deleteAllItems();
			mPowerListAdapter = new SettingListAdapter(mListView,ADAPTER_PLAYING);
			mPowerListAdapter->setData(img,imgsec,text,count);
		}else
			mPowerListAdapter->refresh();

		mPowerListAdapter->mLayoutleft = getLeftByLanguage(gSessionLocal.getLangId());
		
		mTitle->setTextResoure(STR_ADVANCE_POWER);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1430);
	}
	void  SettingsView::initPlayOrderList(){
		int img[]={IDP_PLAYMODE_0,IDP_PLAYMODE_1,IDP_PLAYMODE_2,IDP_PLAYMODE_3};
		int imgsec[]={IDP_PLAYMODE_0_S,IDP_PLAYMODE_1_S,IDP_PLAYMODE_2_S,IDP_PLAYMODE_3_S};
		int text[]={STR_PLAYMODE_0,STR_PLAYMODE_1,STR_PLAYMODE_2,STR_PLAYMODE_3};
		int i,count = 4;
		
		if(mPlayModeListAdapter == NULL){
			mListView->deleteAllItems();
			mPlayModeListAdapter = new PlayModeListAdapter(mListView,ADAPTER_PLAYING);
			mPlayModeListAdapter->setData(img,imgsec,text,count);
		}else
			mPlayModeListAdapter->refresh();
		
		mTitle->setTextResoure(STR_SETTING_PLAYOODER);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1200);
	}

	void SettingsView::initAutoSleepList(){
		int img[]={0,0,0,0,0};
		int imgsec[]={0,0,0,0,0};
		int text[]={STR_AUTO_SPLEEP_30S,STR_AUTO_SPLEEP_1M,STR_AUTO_SPLEEP_2M,STR_AUTO_SPLEEP_5M,STR_AUTO_SPLEEP_10M};
		int i,count = 5;
		
		if(mAutoSleepListAdapter == NULL){
			mListView->deleteAllItems();
			mAutoSleepListAdapter = new AutoSleepListAdapter(mListView,ADAPTER_PLAYING);
			mAutoSleepListAdapter->setData(img,imgsec,text,count);
		}else
			mAutoSleepListAdapter->refresh();
		
		mTitle->setTextResoure(STR_POWER_SCREEN_OFF);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1431);
	}
	void SettingsView::initAutoPoweroffList(){
		int img[]={0,0,0,0};
		int imgsec[]={0,0,0,0};
		int text[]={STR_POWEROFF_5M,STR_POWEROFF_10M,STR_POWEROFF_20M,STR_POWEROFF_30M,STR_POWEROFF_NEVER};
		int i,count = 5;
		
		if(mPoweroffListAdapter == NULL){
			mListView->deleteAllItems();
			mPoweroffListAdapter = new PoweroffListAdapter(mListView,ADAPTER_PLAYING);
			mPoweroffListAdapter->setData(img,imgsec,text,count);
		}else
			mPoweroffListAdapter->refresh();
		
		mTitle->setTextResoure(STR_POWER_POWER_OFF);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1432);
	}
	void SettingsView::initForcePoweroffList(){
		int img[]={0,0,0,0};
		int imgsec[]={0,0,0,0};
		int text[]={STR_SLEEP_TIME_0,STR_SLEEP_TIME_1,STR_SLEEP_TIME_2,STR_SLEEP_TIME_3};
		int i,count = sizeof(text)/sizeof(text[0]);
		
		if(mForcepoweroffListAdapter == NULL){
			mListView->deleteAllItems();
			mForcepoweroffListAdapter = new ForcepoweroffListAdapter(mListView,ADAPTER_PLAYING);
			mForcepoweroffListAdapter->setData(img,imgsec,text,count);
		}else
			mForcepoweroffListAdapter->refresh();
		
		mTitle->setTextResoure(STR_POWER_SLEEP);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1433);
	}
	void SettingsView::initUSBSettingList(){
		int img[]={0,0,0,0};
		int imgsec[]={0,0,0,0};
		int text[]={STR_USB_MODE_STORAGE,STR_USB_MODE_DAC,STR_USB_MODE_CHANGER};
		int i,count = sizeof(text)/sizeof(text[0]);
		
		if(mUSBSettingListAdapter== NULL){
			mListView->deleteAllItems();
			mUSBSettingListAdapter = new USBSettingListAdapter(mListView,ADAPTER_PLAYING);
			mUSBSettingListAdapter->setData(img,imgsec,text,count);
		}else
			mUSBSettingListAdapter->refresh();
		
		mTitle->setTextResoure(STR_USB_CONECT_SETTING);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1500);		
	}
	void SettingsView::initDigitalFilterList(){
		int img[]={0,0,0,0,0};
		int imgsec[]={0,0,0,0,0};
		int text[]={STR_SETTING_DIGITAL_FILTER_FAST,STR_SETTING_DIGITAL_FILTER_SLOW};
		int i,count = 2;
		
		if(mDigitalFilterListAdapter == NULL){
			mListView->deleteAllItems();
			mDigitalFilterListAdapter = new DigitalFilterListAdapter(mListView,ADAPTER_PLAYING);
			mDigitalFilterListAdapter->setData(img,imgsec,text,count);
		}else
			mDigitalFilterListAdapter->refresh();
		
		mTitle->setTextResoure(STR_SETTING_DIGITAL_FILTER);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1600);
	}
	void SettingsView::initLanguageList(){
		int img[]={0,0,0,0,0};
		int imgsec[]={0,0,0,0,0};
		int text[]={STR_LANGUAGE_EN,STR_LANGUAGE_CN,STR_LANGUAGE_JA,STR_LANGUAGE_JAP,STR_LANGUAGE_KR,
			STR_LANGUAGE_TH,STR_LANGUAGE_RU,STR_LANGUAGE_SP,STR_LANGUAGE_PU,STR_LANGUAGE_DE,
			STR_LANGUAGE_FR,STR_LANGUAGE_DU,STR_LANGUAGE_CZ};
		int i,count = sizeof(text)/sizeof(text[0]);
		
		if(mLanguageListAdapter == NULL){
			mListView->deleteAllItems();
			mLanguageListAdapter = new LanguageListAdapter(mListView,ADAPTER_PLAYING);
			mLanguageListAdapter->setData(img,imgsec,text,count);
		}else
			mLanguageListAdapter->refresh();
		
		mTitle->setTextResoure(STR_ADVANCE_LANGUAGE);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1410);
	}

	int SettingsView::getLeftByLanguage(int languageId){
		int ret = 20;
		
		switch(languageId){
			case LANGID_RU:
			case LANGID_LA:
			case LANGID_PU:
			case LANGID_DE:
			case LANGID_FR:	
				ret = 20;break;
			case LANGID_SIMPLIFIED:
			case LANGID_ENGLISH:
			case LANGID_TRADITIONAL:
			case LANGID_JAPANESE:
			case LANGID_KOREAN:
			case LANGID_TH:	
				ret = 80;break;
		}
		
		return ret;
	}
	void SettingsView::initGaplessList(){
		int img[]={0,0,0,0};
		int imgsec[]={0,0,0,0};
		int text[]={STR_GAPLESS_0,STR_GAPLESS_ON};
		int i,count = sizeof(text)/sizeof(text[0]);
		
		if(mGaplessListAdapter == NULL){
			mListView->deleteAllItems();
			mGaplessListAdapter = new GaplessListAdapter(mListView,ADAPTER_PLAYING);
			mGaplessListAdapter->setData(img,imgsec,text,count);
		}else
			mGaplessListAdapter->refresh();
		
		mTitle->setTextResoure(STR_SETTING_GAPLESS);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1300);
	}

	
	int SettingsView::onNotify(View* fromView, int code, void* parameter)
	{
		if (fromView == mListView && code == NM_CLICK) {
			int type,index;
			
			LISTVIEW_RECORD* record;
			record = mListView->getRecord((int)parameter);
			if (!record)
				return 0;
			type = (int)(record->m_lvItem.paramType);
			index = (int)(record->m_lvItem.iItem);
			
			log_i("SettingsView::onNotify m_lvItem.paramType = %d,index=%d",type,index);
			switch(getMainState()){
				case 0x1000:
					switch(index){
						case 0:
							gPlayer.showEqSettingView();	break;
						case 1:
							initPlayOrderList();	break;
						case 2:
							initGaplessList();		break;
						case 3:
							gPlayer.showMusicInfoView(mPlayinglist->getPlayingItem());	break;
						case 4:
							initUSBSettingList();	break;
						case 5:
							initDigitalFilterList();break;	
						case 6:
							initAdvanceList();		break;
						
					}
					break;
				case 0x1200:
					mPlayinglist->setPlayMode(index);
					mPlayModeListAdapter->refresh();
					break;
				case 0x1300:
					mPlayinglist->setGapless(index);
					mGaplessListAdapter->refresh();
					break;
				case 0x1400:
					switch(index){
						case 0:
							initLanguageList();	break;
						case 1:
							gPlayer.showDisplaySettingView();	break;
						case 2:
							//initAutoSleepList(); 
							initPowerList();
							break;
						case 3:
							gPlayer.showChosenView(ChosenView::CHOSEN_RESCAN);
							break;							
						case 4:
							gPlayer.showSystemInfoView(); break;
						case 5:
							gPlayer.showChosenView(ChosenView::CHOSEN_RECOVERY);
							break;
						case 6:
							gPlayer.showChosenView(ChosenView::CHOSEN_SYSTEMUPDATE);
							break;
						}
					break;
				case 0x1410:
					//gSessionLocal.mStockGraphic.mCanvas.setTextLanguage(index+1);
					gSessionLocal.setLangId(index);
					gSettingProvider.update(SETTING_LANGUAGE_ID,index);
					mListView->invalidateRect();
					break;
				case 0x1430:{
					switch(index){
						case 0:
							initAutoSleepList();
							break;
						case 1:
							initAutoPoweroffList();
							break;
						case 2:
							initForcePoweroffList();
							break;
					}
					break;
				case 0x1431:
						gPowerManager->setAutoSleepTime(index);
						mAutoSleepListAdapter->refresh();
						break;
				case 0x1432:
						gPowerManager->setAutoPoweroffTime(index);
						mPoweroffListAdapter->refresh();
						break;
				case 0x1433:
						gPowerManager->setForcePoweroffTime(index);
						mForcepoweroffListAdapter->refresh();
						break;
				case 0x1500:
						setUsbMode(index);
						mUSBSettingListAdapter->refresh();
						break;
				case 0x1600:
						setDigitalState(index);
						mDigitalFilterListAdapter->refresh();
						break;
				}
			}
			
		}
		else if(fromView == mBack && code == NM_CLICK){
			backEvent();
		}else if(fromView == mHome && code == NM_CLICK){
			gPlayer.showPlayingView();
		}
		return 0;
	}


	int SettingsView::onKeyDown(int keyCode, int flag)
	{
		switch(keyCode)
		{
		case KEYCODE_BACK:
			break;
		}

		return 0;
	}


	int SettingsView::onKeyUp(int keyCode, int flag)
	{
		switch(keyCode)
		{
			case KEYCODE_BACK:
				
				break;
		}
		return 0;
	}
	void SettingsView::backEvent(){
		switch(getMainState()){
			case 0x1000:
				gPlayer.showPlayingView();		
				break;
			case 0x1200:
			case 0x1300:
			case 0x1400:
			case 0x1500:
			case 0x1600:
				initMainList();
				break;
			case 0x1410:	
			case 0x1430:
				initAdvanceList();
				break;
			case 0x1431:
			case 0x1432:
			case 0x1433:
				initPowerList();
				break;
		}
	}
	
	SettingListAdapter::SettingListAdapter(ListView* list,int id):BaseAdapter(){
		mId = id;
		mlist = list;	
		mImgRes = NULL;
		mTextRes = NULL;
		mSecImgRes = NULL;
		mCount = 0;
		mLayoutleft = 100;
	}
	void SettingListAdapter::setData(int* img,int* imgsec,int* text,int count){
		mCount = count;
		log_i("SettingListAdapter::setData");
		if(mImgRes!=NULL && mTextRes!=NULL&&mSecImgRes!=NULL){
			delete mImgRes;
			delete mTextRes;
			delete mSecImgRes;
			mImgRes = NULL;
			mTextRes = NULL;
			mSecImgRes = NULL;
		}
		log_i("SettingListAdapter::setData delete complete.");
		if(count>0){
			
			mImgRes = new int[count];
			memcpy(mImgRes,img,count*sizeof(int));
			
			mTextRes = new int[count];
			memcpy(mTextRes,text,count*sizeof(int));

			mSecImgRes = new int[count];
			memcpy(mSecImgRes,imgsec,count*sizeof(int));
		}
		log_i("SettingListAdapter::setData refresh.");
		refresh();	
	}
	
	void SettingListAdapter::refresh(){
		int i;
		log_i("SettingListAdapter::refresh()");
		mlist->setListAdapter(this);
		log_i("mlist->setListAdapter(this)");
		//mlist->deleteAllRecord();
		mlist->deleteAllItems();
		log_i("mlist->deleteAllItems(this)");

		for(i=0;i<mCount;i++){
			ListViewItem  lvItem;
			
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			
			mlist->insertItem(&lvItem);
		}
		mlist->setZoneY(getYoffset(),false);
		mlist->invalidateRect();
	}
	
	void SettingListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=mLayoutleft;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
	}

	PlayModeListAdapter::PlayModeListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}

	void PlayModeListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == mPlayinglist->getPlayMode())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}

	AutoSleepListAdapter::AutoSleepListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}

	void AutoSleepListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == gPowerManager->getAutoSleepTime())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}

	DigitalFilterListAdapter::DigitalFilterListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}

	void DigitalFilterListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == getDigitalState())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}

	GaplessListAdapter::GaplessListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}

	void GaplessListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == mPlayinglist->getGapless())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}
	
	LanguageListAdapter::LanguageListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}

	void LanguageListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == gSessionLocal.getLangId())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}
	
	PoweroffListAdapter::PoweroffListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}
	void PoweroffListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == gPowerManager->getPoweroffTime())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}
	ForcepoweroffListAdapter::ForcepoweroffListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}
	void ForcepoweroffListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == gPowerManager->getForcePoweroffTime())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}
	USBSettingListAdapter::USBSettingListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}
	void USBSettingListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == isHifiMode())
			canvas.drawImageResource(IDP_LISTITEM_SEC,x,y+13);
		else
			canvas.drawImageResource(IDP_LISTITEM_NO_SEC,x,y+13);
	}

	RootDirectListAdapter::RootDirectListAdapter(ListView* list,int id)
		: SettingListAdapter(list,id){

	}
	void RootDirectListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=50;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);

	}
};
