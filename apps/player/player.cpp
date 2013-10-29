#include "player.h"
#include "cutils/android_reboot.h"

namespace mango
{

	static const char *BootUp = "bootup";

	void sig_int(int sig)
	{
 		log_i("Catch a termination single = %d/n",sig);   
	}
	Player::Player(void)
	{
		mMeidaView = NULL;
		mPlayingView = NULL;
		mSettingsView = NULL;
		mMusicInfoView = NULL;
		mDisplaySettingView = NULL;
		mPointDrawView = NULL;
		mSystemInfoView = NULL;
		mVolumeView = NULL;
		mEqSettingsView = NULL;
		mShutDownView = NULL;
		mMediaScannerView = NULL;
		mSdcardInsertView = NULL;
		mMusicOperateView = NULL;
		mUsmConnectView = NULL;
		mKeyLockView = NULL;
		mChosenView = NULL;
		mPlaylistOperateView = NULL;
		
		powerState = 0;
		isBootLock = 0;
		muteCount = 0;
		volumeInitFail = true;

	}


	Player::~Player(void)
	{
		safeDelete(mMeidaView);
	}

	int Player::main()
	{
#if 1	
		int i,ret;
		setBootWakeLock(1);
		initialize();
		gSettingProvider.initialize();
		initSettings();
		
		//spdifProbe();
		gmediaprovider.initialize();

		mPlayinglist = new Playinglist();
		mPlayinglist->initPlayintList();		
		
		//mango::Thread::sleep(1000 * 3);
		
		mSpdifSwitch = new PlayerSwitch();
		mHeadestSwitch = new PlayerSwitch();
		mPlayerEventInterface = new PlayerEventInterface();
		gSession.setUseEventInterface((UseEventInterface*)mPlayerEventInterface);

		showPlayingView();
		openCodecPower(false);
		openCodecPower(true);
		gPowerManager = new PowerManager();
		
		mSocketDetect = new SocketDetect();
		mSocketDetect->setPlayerEventInterface((UseEventInterface*)mPlayerEventInterface);
		
		ret = mSocketDetect->SocketInit();
		if(ret>0){log_i("mSocketDetect->SocketInit() sucess!");}else{log_i("mSocketDetect->SocketInit() fail.ret=%d",ret);}
		
		for(i=0;i<50;i++){
			//signal(i,&sig_int); 
		}
#else
		gAlarmManager = new AlarmManager();
		gAlarmManager->initialize();
		gAlarmManager->setAlarmWakeup(6);
#endif
		holdKeyProbe();
		//openOrCloseMute(true);
		//Environment::openMute();
		return messageLoop();
	}
	void Player::setBootWakeLock(int en){
		log_i("Player::setBootWakeLock en=%d,isBootLock=%d",en,isBootLock);
		if(en && !isBootLock){
			if(wakeLock(BootUp) == 0){
				isBootLock = 1;
				bootLockCount = 20;
			}
		}
		else if(isBootLock){
			if((!mPlayinglist->isPlaying()) && (mPlayinglist->inPause == 0))
				Environment::openMute();
#ifdef NEED_SLEEP
			if(wakeUnlock(BootUp) == 0)
				isBootLock = 0;
#endif
		}

	}

	int Player::initSettings(){
		int value;
		
		VolumeCheck();
		
		if(gSettingProvider.query(SETTING_BRIGHTNESS_ID,&value))
			ioctrlBrightness(IOCTRL_BRIGTNESS_WRITE,&value);
		
		if(gSettingProvider.query(SETTING_LANGUAGE_ID,&value))
			gSessionLocal.setLangId(value);
	}
	
	int Player::showPlayingView()
	{
		if (mPlayingView == NULL) {
			mPlayingView = new PlayingView(TEXT("Playing"), NULL, NULL, 0, SW_NORMAL);
			log_i("new PlayingView");
			mPlayingView->onCreate();
			log_i("mPlayingView->onCreate()");
		} else {
			gSession.mViewZAxis.bringViewToTop(mPlayingView);
		}

		if (mPlayingView) {
			mPlayingView->invalidateRect();
			mPlayingView->setFocus();
		}
		log_i("Player::showPlayingView");
		return 0;
	}

	int Player::showSettingsView()
	{
		if (mSettingsView == NULL) {
			mSettingsView = new SettingsView(TEXT("Settings"), NULL, NULL, 0, SW_NORMAL);
			mSettingsView->onCreate();	
			log_i("showSettingsView ");
		} else {
			gSession.mViewZAxis.bringViewToTop(mSettingsView);
		}

		if (mSettingsView) {
			mSettingsView->invalidateRect();
			mSettingsView->setFocus();
		}

		return 0;
	}
	int Player::showMediaView(int type)
	{
		if (mMeidaView == NULL) {
			mMeidaView = new MediaView (TEXT("Media"), NULL, NULL, 0, SW_NORMAL);
			mMeidaView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mMeidaView);
		}

		if (mMeidaView)	{
			mMeidaView->setMediaViewDisplay(type);
			mMeidaView->invalidateRect();
			mMeidaView->setFocus();
		}

		return 0;
	}
	int Player::showMusicInfoView()
	{
		if (mMusicInfoView == NULL) {
			mMusicInfoView = new MusicInfoView (TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
			mMusicInfoView->onCreate();
			log_i("mSystemInfoView->onCreate();");
		} else {
			gSession.mViewZAxis.bringViewToTop(mMusicInfoView);
		}

		if (mMusicInfoView)	{
			mMusicInfoView->invalidateRect();
			mMusicInfoView->setFocus();
		}

		return 0;
	}

	int Player::showMusicInfoView(mediainfo* info){
		if (mMusicInfoView == NULL) {
			mMusicInfoView = new MusicInfoView (TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
			mMusicInfoView->onCreate();
		}
		//log_i("info->title=%s",info->title);
		//log_i("info->path=%s",info->path);
		mMusicInfoView->setMusicInfo(info);
		showMusicInfoView();
		return 0;
	}
	int Player::showDisplaySettingView()
	{
		if (mDisplaySettingView == NULL) {
			mDisplaySettingView = new DisplaySettingView (TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
			mDisplaySettingView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mDisplaySettingView);
		}

		if (mDisplaySettingView){
			mDisplaySettingView->invalidateRect();
			mDisplaySettingView->setFocus();
		}

		return 0;
	}
	int Player::showPointDrawView(){
		if (mPointDrawView == NULL) {
			mPointDrawView = new PointDrawView (TEXT("PointDraw"), NULL, NULL, 0, SW_NORMAL);
			mPointDrawView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mPointDrawView);
		}

		if (mPointDrawView)	{
			mPointDrawView->invalidateRect();
			mPointDrawView->setFocus();
		}
	}

	int Player::showSystemInfoView(){
		if (mSystemInfoView == NULL) {
			mSystemInfoView = new SystemInfoView(TEXT("SystemInfo"), NULL, NULL, 0, SW_NORMAL);
			mSystemInfoView->onCreate();
			log_i("mSystemInfoView->onCreate();");
		} else {
			gSession.mViewZAxis.bringViewToTop(mSystemInfoView);
		}

		if (mSystemInfoView){
			mSystemInfoView->invalidateRect();
			mSystemInfoView->setFocus();
		}
	}
	int Player::showEqSettingView(){
		if (mEqSettingsView == NULL) {
			mEqSettingsView = new EqSettingsView(TEXT("EqSetting"), NULL, NULL, 0, SW_NORMAL);
			mEqSettingsView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mEqSettingsView);
		}

		if (mEqSettingsView){
			mEqSettingsView->invalidateRect();
			mEqSettingsView->setFocus();
		}
	}
	int Player::showUsmConnectView(){
		if (mUsmConnectView == NULL) {
			mUsmConnectView = new UsmConnectView(TEXT("UsmConnect"), NULL, NULL, 0, SW_NORMAL);
			mUsmConnectView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mUsmConnectView);
		}
		if (mUsmConnectView){
			mUsmConnectView->invalidateRect();
			mUsmConnectView->setFocus();
		}
	}

	int Player::showMediaScannerView(){
		if (mMediaScannerView == NULL) {
			mMediaScannerView = new MediaScannerView(TEXT("MediaScannerView"), NULL, NULL, 0, SW_NORMAL);
			mMediaScannerView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mMediaScannerView);
		}
		if (mMediaScannerView){
			mMediaScannerView->invalidateRect();
			mMediaScannerView->setFocus();
		}
	}
	int Player::showSdcardInsertView(){
		if (mSdcardInsertView == NULL) {
			mSdcardInsertView = new SdcardInsertView(TEXT("SdcardInsertView"), NULL, NULL, 0, SW_NORMAL);
			mSdcardInsertView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mSdcardInsertView);
		}
		if (mSdcardInsertView){
			mSdcardInsertView->invalidateRect();
			mSdcardInsertView->setFocus();
		}
		holdKeyProbe();
	}	
	int Player::showVolumeView(){
		if (mVolumeView == NULL) {
			mVolumeView = new VolumeView(TEXT("Volume"), NULL, NULL, 0, SW_NORMAL);
			mVolumeView->onCreate();
		}else if(mVolumeView->isShowing()){
			return 0;
		}else {
			gSession.mViewZAxis.bringViewToTop(mVolumeView);
		}
		
		if (mVolumeView && !mVolumeView->isShowing()){
			mVolumeView->invalidateRect();
			mVolumeView->setFocus();
			mVolumeView->isShow = 1;
		}
	}
	int Player::showShutDownView(){
		if (mShutDownView == NULL) {
			mShutDownView = new ShutDownView(TEXT("ShutDown"), NULL, NULL, 0, SW_NORMAL);
			mShutDownView->onCreate();
		}else {
			gSession.mViewZAxis.bringViewToTop(mShutDownView);
		}
		if (mShutDownView){
			mShutDownView->invalidateRect();
			mShutDownView->setFocus();		
		}
		//shutDown();
	}
	void Player::shutDown(){	
		mPlayinglist->stopPlayer();
		mPlayinglist->savePlayintList();
		gSettingProvider.dbclose();
		Environment::sync();
	}
	void Player::VolumeCheck(){
		int value;
		
		if(volumeInitFail && gSettingProvider.query(SETTING_VOLUME_ID,&value)){
			int i,count = 3;
			for(i=0;i<count;i++){
				setVolume(value);
				if(getVolume() == value){
					volumeInitFail = false;
					break;
				}
			}
			if(i>=count){
				log_i("error setVolume to system fail.");
				volumeInitFail = true;
			}
		}
	}
	int Player::showMusicOperateView(mediainfo& info){
#if 0		
		if(mMusicOperateView == NULL){
			mMusicOperateView = new MusicOperateView(TEXT("MusicOperate"), NULL, NULL, 0, SW_NORMAL);
			mMusicOperateView->onCreate();
		}else{
			mMusicOperateView->setType(OPERATE_TYPE_MUSIC_NORMAL);
			gSession.mViewZAxis.bringViewToTop(mMusicOperateView);
		}
		
		log_i("---");
		if (mMusicOperateView){
			log_i("---");
			mMusicOperateView->setMusicInfo(info);
			mMusicOperateView->invalidateRect();
			mMusicOperateView->setFocus();
			log_i("---");
		}
#endif
		showMusicOperateView(info,OPERATE_TYPE_MUSIC_NORMAL);	
	}
	int Player::showMusicOperateView(mediainfo& info,int type,int start){
		if(mMusicOperateView == NULL){
			mMusicOperateView = new MusicOperateView(TEXT("MusicOperate"), NULL, NULL, 0, type, SW_NORMAL);
			mMusicOperateView->setOperateViewStart(start);
			mMusicOperateView->onCreate();
		}else{
			mMusicOperateView->setOperateViewStart(start);
			mMusicOperateView->setType(type);
			gSession.mViewZAxis.bringViewToTop(mMusicOperateView);
		}
		log_i("---");
		if (mMusicOperateView){
			log_i("---");
			mMusicOperateView->setMusicInfo(info);
			mMusicOperateView->invalidateRect();
			mMusicOperateView->setFocus();
			log_i("---");
		}
	}
	int Player::showKeyLockView(){
		if(mKeyLockView == NULL){
			mKeyLockView = new KeyLockView(TEXT("MusicOperate"), NULL, NULL, 0, SW_NORMAL);
			mKeyLockView->onCreate();
		}else{
			gSession.mViewZAxis.bringViewToTop(mKeyLockView);
		}

		if (mKeyLockView){
			mKeyLockView->invalidateRect();
			mKeyLockView->setFocus();
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_KEY_LOCK,0);
			gMessageQueue.post(gPlayer.mVolumeView,VM_NOTIFY,NM_KEY_LOCK,0);
		}
	}
	int Player::showChosenView(int type){
		if(mChosenView == NULL){
			mChosenView = new ChosenView(TEXT("mChosenView"), NULL, NULL, 0, SW_NORMAL);
			mChosenView->setType(type);
			mChosenView->onCreate();
		}else{
			mChosenView->setType(type);
			gSession.mViewZAxis.bringViewToTop(mChosenView);
		}
		
		if (mChosenView){
			mChosenView->invalidateRect();
			mChosenView->setFocus();
		}		
	}
	int Player::showPlaylistOperateView(PlayListItem& info){
		if(mPlaylistOperateView == NULL){
			mPlaylistOperateView = new PlaylistOperateView(TEXT("mChosenView"), NULL, NULL, 0, SW_NORMAL);
			mPlaylistOperateView->onCreate();
		}else{
			gSession.mViewZAxis.bringViewToTop(mPlaylistOperateView);
		}
		mPlaylistOperateView->setmPlayListItem(info);
		if (mPlaylistOperateView){
			mPlaylistOperateView->invalidateRect();
			mPlaylistOperateView->setFocus();
		}
	}
	void Player::showGroupOperateView(){
		if (mGroupOperateView == NULL) {
			mGroupOperateView = new GroupOperateView(TEXT("GroupOperate"), NULL, NULL, 0, SW_NORMAL);
			mGroupOperateView->onCreate();
		}else {
			gSession.mViewZAxis.bringViewToTop(mGroupOperateView);
		}
		if (mGroupOperateView){
			mGroupOperateView->invalidateRect();
			mGroupOperateView->setFocus();		
		}
	}
	void Player::dismissView(View *view){
		View *displayView;
		
		if(view != NULL&&view == gSession.mViewZAxis.getDisplayingView()){
			log_i("dismissView:%s",view->name);
			gSession.mViewZAxis.bringViewToBottom(view);
			displayView = gSession.mViewZAxis.getDisplayingView();
			displayView->invalidateRect();
			displayView->setFocus();
			if(displayView!=NULL)
				log_i("dimiss view and show view:%s",displayView->name);
			gMessageQueue.post(displayView,VM_NOTIFY,NM_DISPLAY,0);
		}
	}
	
	int  Player::getVolume(void)
	{
#ifndef WIN32
		FILE* file;
		char buffer[20]={0};
		int currentVolume;
		int maxVolume;
		int minVolume;

		file  = fopen("/dev/codec_volume", "rt");
		if (file == NULL) {
			log_e("/dev/codec_volume");
			return -1;
		}

		fread(buffer, 1, 20, file);
		buffer[19] = '\0';
		//log_i ("%s", buffer);
		sscanf(buffer, "%d,%d,%d\n", &currentVolume, &minVolume, &maxVolume);

		fclose(file);
		//log_i("Player::getVolume currentVolume=%d",currentVolume);
		return currentVolume;
#endif
		return 0;
	}


	void Player::setVolume(int volume)
	{
		FILE* file;
		char buffer[20]={0};
		int ret;
		
		volumeMutex.lock();
		
		file  = fopen("/dev/codec_volume", "wt");
		if (file != NULL) {
			sprintf(buffer, "%d", volume);
			ret = fwrite(buffer, 1, strlen(buffer) + 1, file);	
			fclose(file);
		}
		
		volumeMutex.unlock();
	}
	void Player::ioctrlBrightness(int cmd,int* brightness){
		int fd=0;
		char* path = "/sys/devices/platform/rk29_backlight/backlight/rk28_bl/brightness";
		char rbuf[6]={};
		
		fd = open(path,O_RDWR, 0);
		if(fd==-1)
			return;
		switch(cmd){
			case IOCTRL_BRIGTNESS_READ:
				if (read(fd, rbuf, 5) == -1) 
					return;
				rbuf[5]='\0';
				log_i("read rbuf=%s",rbuf);
				sscanf(rbuf, "%d\n", brightness);
				break;
			case IOCTRL_BRIGTNESS_WRITE:
				sprintf(rbuf, "%d", *brightness);
				log_i("write rbuf=%s,nuflen=%d",rbuf,strlen(rbuf));
				if(write(fd,rbuf,strlen(rbuf)) == -1){
					log_i("ioctrlBrightness write fail.");
				}else
					log_i("ioctrlBrightness write sucess.");
				break;
		}
		close(fd);
	}
	void Player::setPowerState(){
		int fd=0;
		char* path = "/sys/power/state";
		char rbuf[6]={};
		const char *const pm_states[5] = {
			"on",
			"standby",
			"mem"
		};

		if(powerState)
			powerState = 0;
		else
			powerState = 2;
		
		fd = open(path,O_RDWR, 0);
		if(fd==-1)
			return;
		
		if(write(fd,pm_states[powerState],strlen(pm_states[powerState])) == -1){
			log_i("setPowerState write fail.");
		}else
			log_i("setPowerState write sucess.");
		
		close(fd);
	}

	int Player::holdKeyProbe(){
		int fd=0,state;
		char* path = "/sys/class/axppower/holdkey";
		char rbuf;
		
		fd = open(path,O_RDWR, 0);
		read(fd,&rbuf,1);
		state = rbuf&0x10;
		close(fd);
		
		log_i("/sys/class/axppower/holdkey:0x%x",state);
		
		gSession.setHoldKeyState(state);

		if(state){
			gPlayer.dismissView(gPlayer.mKeyLockView);
		}else{
			gPlayer.showKeyLockView();	
		}		
		
		return state;
	}
	void Player::spdifProbe(){
		bool isSpidfIn;
		isSpidfIn = isSpdifIn();
		if(isSpidfIn)
			openCodecPower(!isSpidfIn);
	}
	
	bool Player::isSpdifIn(){
		int fd=0,state;
		char* path = "/sys/class/axppower/holdkey";
		char rbuf;
		
		fd = open(path,O_RDWR, 0);
		
		read(fd,&rbuf,1);
		
		state = rbuf&0x20;
		
		close(fd);
		log_i("spdif Player::isSpdifIn state=%d",state);
		return state?false:true;
		//return true; //同轴关codec 设计有问题，这里屏蔽掉
	}
	bool Player::isHeadestIn(){
		int fd=0,state;
		char* path = "/sys/class/axppower/holdkey";
		char rbuf;
		
		fd = open(path,O_RDWR, 0);
		
		read(fd,&rbuf,1);
		
		state = rbuf&0x40;
		
		close(fd);
		log_i("spdif Player::isHeadestIn state=%d",state);
		return state?false:true;
	}
	void Player::openCodecPower(bool enable){
		int fd=0,state,volume;
		char* path = "/sys/class/codec/power";
		char rbuf;
		
		return;		//not need to open or colse power
		
		log_i("spdif openCodecPower enable=%d",enable);
		
		fd = open(path,O_RDWR, 0);
		
		rbuf = enable?1:0;
		
		write(fd,&rbuf,1);

		if(enable){
			//mango::Thread::sleep(200);
			gSettingProvider.query(SETTING_VOLUME_ID,&volume);
			setVolume(volume);
		}
		close(fd);
	}
	void Player::openOrCloseMute(bool enable){
		int fd=0,state,volume;
		char* path = "/sys/class/codec/mute";
		char rbuf;
		
		log_i("mute openOrCloseMute enable=%d",enable);
		
		fd = open(path,O_RDWR, 0);
		
		rbuf = enable?1:0;
		
		write(fd,&rbuf,1);
		
		close(fd);
	}
	void Player::openOrCloseWm8740Mute(bool enable){
		int fd=0,state,volume;
		char* path = "/sys/class/codec/wm8740_mute";
		char rbuf;
		
		log_i("openOrCloseMute enable=%d",enable);
			
		fd = open(path,O_RDWR, 0);
		
		rbuf = enable?1:0;
		
		write(fd,&rbuf,1);
		
		close(fd);
	}
	void Player::openWm8740Mute(){	
		muteMutex.lock();
		
		muteCount++;
		log_i("openOrCloseMute muteCount=%d",muteCount);
		if(muteCount == 1){
			openOrCloseWm8740Mute(true);
		}
		
		muteMutex.unlock();
	}
	void Player::closeWm8740Mute(){
		muteMutex.lock();
		
		muteCount--;
		log_i("openOrCloseMute muteCount=%d",muteCount);
		if(muteCount == 0){
			openOrCloseWm8740Mute(false);
		}
		
		muteMutex.unlock();
	}

	
	int PlayerEventInterface::onKeyDispatch(int keyCode,int action, int flag){
		KeyCount *keycount = NULL;
		int ret = 0;
		if(gPlayer.mPlayingView != NULL)
			keycount = &(gPlayer.mPlayingView->mKeyCount);
		
		//log_i("PlayerEventInterface::onKeyDispatch keyCode=%d,action=%d",keyCode,action);
		
		if((keyCode == KEYCODE_VOLUMEUP||keyCode == KEYCODE_VOLUMEDOWN)){
			log_i("VOLUMEonKeyDispatch action=%d",action);
			if(action == VM_KEYDOWN)
				gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_SHOW_VOLUME,keyCode);

			while(gPlayer.mVolumeView == NULL)
				mango::Thread::sleep(100);
			
			if(gPlayer.mVolumeView != NULL){
				gPlayer.mVolumeView->onKeyAction(keyCode,action);
			}
			ret = 1;
		}
		else if(keyCode == KEYCODE_PREV||keyCode == KEYCODE_NEXT){
			log_i("enter");
			gMessageQueue.post(gPlayer.mPlayingView, action, keyCode, 0);
			ret = 1;
		}
#if 0		
		else if(keyCode == KEYCODE_PREV&& action == VM_KEYDOWN){
			if(keycount != NULL)
				keycount->initKeyPress(keyCode);
		}else if(keyCode == KEYCODE_NEXT&& action == VM_KEYDOWN){
			if(keycount != NULL)
				keycount->initKeyPress(keyCode);
		}
		else if(keyCode == KEYCODE_PREV&& action == VM_KEYUP){
			if(keycount != NULL && keycount->isKeyPress(keyCode))
				;
			else
				gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_PREV,NULL);
			
		}else if(keyCode == KEYCODE_NEXT&& action == VM_KEYUP){
			if(keycount != NULL && keycount->isKeyPress(keyCode))
				;
			else
				gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_NEXT,NULL);
		}
#endif
		else if(keyCode == KEYCODE_PLAY&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_PLAY,NULL);
			//log_i("gMessageQueue.post PLAYING_IDB_PLAY"); 
		}else if(action == VM_CAPACITY){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_BATTERY_UPDATE,keyCode);
			if(gPowerManager!=NULL)
				gPowerManager->PowerManagerCount();
		}else if(keyCode == KEYCODE_SHORT_POWER&&action == VM_KEYUP){
			if(gPowerManager!=NULL){
				log_i("SHORT_POWER setPowerState()");
				gPowerManager->setPowerState();
			}
		}else if(keyCode == KEYCODE_LONG_POWER&&action == VM_KEYDOWN){
			//gPlayer.showShutDownView();
			gPlayer.showChosenView(ChosenView::CHOSEN_POWEROFF);
		}else if(action == VM_MEDIA){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,keyCode,0);
			gMessageQueue.post(gPlayer.mMeidaView,VM_NOTIFY,keyCode,0);
		}else if(keyCode == KEYCODE_HOLD&& action == VM_KEYDOWN){
			gPlayer.holdKeyProbe();
		}else if(keyCode == KEYCODE_SPIDF&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_SPIDF,keyCode);
		}else if(keyCode == KEYCODE_HEADEST&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_HEADEST,keyCode);
		}else if(action == VM_POWEROFF){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_POWER_OFF,keyCode);
		}
		if(action!=VM_CAPACITY&&gPowerManager!=NULL)
			gPowerManager->resetCount();
		return ret;
	}

	int PlayerEventInterface::onTouchDispatch(int x,int y, int action){
		//log_i("PlayerEventInterface x=%d,y=%d,action=%d",x,y,action);
		if(gPowerManager!=NULL)
			gPowerManager->resetCount();
		if(gPlayer.mMeidaView != NULL)
			gPlayer.mMeidaView->onTouchDispatch(x,y,action);
		return 0;
	}
	
	Player  gPlayer;
};



int main (int argc, char* argv[])
{
#ifndef WIN32
	//mango::Thread::sleep(1000 * 3);
#endif
	return mango::gPlayer.main();
}
