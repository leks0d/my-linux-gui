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
	}

	SettingsView::~SettingsView(void)
	{

	}
	
	int SettingsView::onCreate()
	{
		Rect rect;

		rect.setEx(0,25,320,215);
		mListView = new MediaListView(TEXT("Media List"), this, &rect, LVS_LIST);
		mListView->setListItemBackground(IDP_LISTITEM_BGD,IDP_LISTITEM_BGD_SEC);
		mListView->setTextColor(RGB(255,255,255));
		mListView->onCreate();

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
		canvas.drawImageResource(IDP_SETTING_BGD,0,0,false);
		canvas.drawImageResource(IDP_MUSIC_TITLEBAR,0,0,false);
		return 0;
	}


	void SettingsView::initAdvanceList(){
		int img[]={IDP_ADVANCE_LANGUGE,IDP_ADVANCE_DISPLAY,IDP_ADVANCE_BATTARY,IDP_ADVANCE_SYSTEM_INFO,IDP_ADVANCE_DISPLAY,IDP_ADVANCE_SYSTEM_INFO};
		int imgsec[]={IDP_ADVANCE_LANGUGE_S,IDP_ADVANCE_DISPLAY_S,IDP_ADVANCE_BATTARY_S,IDP_ADVANCE_SYSTEM_INFO_S,IDP_ADVANCE_DISPLAY_S,IDP_ADVANCE_SYSTEM_INFO_S};
		int text[]={STR_ADVANCE_LANGUAGE,STR_ADVANCE_DISPLAY,STR_ADVANCE_POWER,STR_ADVANCE_SYSINFO,STR_ADVANCE_SHOWTOUCH,STR_ADVANCE_SCANNER};
		int i,count = 6;
		log_i("SettingsView::initAdvanceList");
		if(mAdvanceListAdapter == NULL){
			log_i("SettingsView::mListView->deleteAllItems()");
			mListView->deleteAllItems();
			log_i("SettingsView::new SettingListAdapter");
			mAdvanceListAdapter = new SettingListAdapter(mListView,ADAPTER_PLAYING);
			mAdvanceListAdapter->setData(img,imgsec,text,count);
		}else{
			mAdvanceListAdapter->refresh();
		}
		mTitle->setTextResoure(STR_SETTING_ADVANCED);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1400);
	}
	void SettingsView::initMainList(){
		int img[]={IDP_SETTING_EQ,IDP_SETTING_PLAYORDER,IDP_SETTING_GAPLESS,IDP_SETTING_MUSICINFO,IDP_SETTING_ADVANCED};
		int imgsec[]={IDP_SETTING_EQ_S,IDP_SETTING_PLAYORDER_S,IDP_SETTING_GAPLESS_S,IDP_SETTING_MUSICINFO_S,IDP_SETTING_ADVANCED_S};
		int text[]={STR_SETTING_EQ,STR_SETTING_PLAYOODER,STR_SETTING_GAPLESS,STR_SETTING_MUSICINFO,STR_SETTING_ADVANCED};
		int i,count = 5;
		
		if(mSettingListAdapter == NULL){
			mListView->deleteAllItems();
			mSettingListAdapter = new SettingListAdapter(mListView,ADAPTER_PLAYING);
			mSettingListAdapter->setData(img,imgsec,text,count);
		}else
			mSettingListAdapter->refresh();
		
		mTitle->setTextResoure(MUSIC_MUSIC_FUN);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1000);
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
		
		mTitle->setTextResoure(MUSIC_MUSIC_FUN);
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
		
		mTitle->setTextResoure(STR_SETTING_GAPLESS);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1430);
	}
	void SettingsView::initLanguageList(){
		int img[]={0,0,0,0,0};
		int imgsec[]={0,0,0,0,0};
		int text[]={STR_LANGUAGE_CN,STR_LANGUAGE_EN,STR_LANGUAGE_JA};
		int i,count = 3;
		
		if(mLanguageListAdapter == NULL){
			mListView->deleteAllItems();
			mLanguageListAdapter = new LanguageListAdapter(mListView,ADAPTER_PLAYING);
			mLanguageListAdapter->setData(img,imgsec,text,count);
		}else
			mLanguageListAdapter->refresh();
		
		mTitle->setTextResoure(STR_SETTING_GAPLESS);
		mTitle->setTextLayoutType(TEXT_LAYOUT_CENTER);
		mTitle->invalidateRect();
		setMainState(0x1410);
	}

	
	void SettingsView::initGaplessList(){
		int img[]={0,0,0,0};
		int imgsec[]={0,0,0,0};
		int text[]={STR_GAPLESS_0,STR_GAPLESS_1,STR_GAPLESS_2};
		int i,count = 3;
		
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
							gPlayer.showMusicInfoView();	break;
						case 4:
							initAdvanceList();	break;
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
							initAutoSleepList(); break;
						case 3:
							gPlayer.showSystemInfoView(); break;
						case 4:
							gPlayer.showPointDrawView(); break;
						case 5:
							gmediaprovider.mediascanner("/mnt/sdcard");
							gPlayer.showMediaView();
							break;
						}
					break;
				case 0x1430:
					gPowerManager->setAutoSleepTime(index);
					mAutoSleepListAdapter->refresh();
					break;					
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
				initMainList(); 	
				break;
			case 0x1410:	
			case 0x1430:
				initAdvanceList();	
				break;
		}
	}
	
	SettingListAdapter::SettingListAdapter(ListView* list,int id){
		mId = id;
		mlist = list;	
		mImgRes = NULL;
		mTextRes = NULL;
		mSecImgRes = NULL;
		mCount = 0;
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

		refresh();	
	}
	
	void SettingListAdapter::refresh(){
		int i;
		log_i("SettingListAdapter::refresh()");
		mlist->setListAdapter(this);
		mlist->deleteAllItems();

		for(i=0;i<mCount;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mlist->insertItem(&lvItem);
			log_i("PlayingListAdapter insertItem i=%d",i);
		}
		mlist->invalidateRect();
	}
	
	void SettingListAdapter::PaintView(Canvas& canvas,Rect& rect,ListViewItem* lvitem,int isSec){
		int	 x, y,index;
		
		x = rect.left;
		y = rect.top;

		index = lvitem->iItem;
		x+=100;
		if(isSec)
			canvas.drawImageResource(mSecImgRes[index],x,y+10);
		else
			canvas.drawImageResource(mImgRes[index],x,y+10);
		x+=33;
		if(isSec)
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));
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
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
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
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == gPowerManager->getAutoSleepTime())
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
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
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
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);
		x+=150;
		if(index == 2)
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
			canvas.setTextColor(RGB(255,149,0));
		else
			canvas.setTextColor(RGB(255,255,255));	
		canvas.setTextSize(18);
		canvas.drawTextResource(mTextRes[index],x,y+13);

	}
};
