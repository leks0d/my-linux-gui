#include "player.h"
#include <sys/reboot.h>
#include <signal.h>
 
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
		powerState = 0;
		isBootLock = 0;
	}


	Player::~Player(void)
	{
		safeDelete(mMeidaView);
	}

	int Player::main()
	{
		int i,ret;
		setBootWakeLock(1);
		initialize();
		gSettingProvider.initialize();
		initSettings();
		holdKeyProbe();
		spdifProbe();
		gmediaprovider.initialize();
		
		mPlayinglist = new Playinglist();
		mPlayinglist->initPlayintList();

		mango::Thread::sleep(1000 * 3);
		
		mSpdifSwitch = new PlayerSwitch();
		mHeadestSwitch = new PlayerSwitch();
		mPlayerEventInterface = new PlayerEventInterface();
		gSession.setUseEventInterface((UseEventInterface*)mPlayerEventInterface);

		showPlayingView();

		gPowerManager = new PowerManager();
		
		mSocketDetect = new SocketDetect();
		mSocketDetect->setPlayerEventInterface((UseEventInterface*)mPlayerEventInterface);
		
		ret = mSocketDetect->SocketInit();
		if(ret>0){log_i("mSocketDetect->SocketInit() sucess!");}else{log_i("mSocketDetect->SocketInit() fail.ret=%d",ret);}
		
		for(i=0;i<50;i++){
			//signal(i,&sig_int); 
		}
		log_i("signal &sig_int");
		return messageLoop();
	}
	void Player::setBootWakeLock(int en){
		log_i("Player::setBootWakeLock en=%d,isBootLock=%d",en,isBootLock);
		if(en && !isBootLock){
			if(wakeLock(BootUp) == 0){
				isBootLock = 1;
				bootLockCount = 20;
			}
		}else if(isBootLock){
			if(wakeUnlock(BootUp) == 0)
				isBootLock = 0;
		}
	}

	int Player::initSettings(){
		int value;
		
		if(gSettingProvider.query(SETTING_VOLUME_ID,&value))
			setVolume(value);
		
		if(gSettingProvider.query(SETTING_BRIGHTNESS_ID,&value))
			ioctrlBrightness(IOCTRL_BRIGTNESS_WRITE,&value);
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
	int Player::showMediaView()
	{
		if (mMeidaView == NULL) {
			mMeidaView = new MediaView (TEXT("Media"), NULL, NULL, 0, SW_NORMAL);
			mMeidaView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mMeidaView);
		}

		if (mMeidaView)	{
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
		mPlayinglist->stopPlayer();
		mPlayinglist->savePlayintList();
		mango::Thread::sleep(1000 * 6);
		reboot(RB_POWER_OFF);		
	}
	void Player::dismissView(View *view){
		View *displayView;
		
		if(view != NULL&&view == gSession.mViewZAxis.getDisplayingView()){
			log_i("dismissView:%s",view->name);
			gSession.mViewZAxis.bringViewToBottom(view);
			displayView = gSession.mViewZAxis.getDisplayingView();
			displayView->invalidateRect();
			displayView->setFocus();
			gMessageQueue.post(displayView,VM_NOTIFY,NM_DISPLAY,0);
		}
	}
	
	int  Player::getVolume(void)
	{
#ifndef WIN32
		FILE* file;
		char buffer[20];
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
		log_i ("%s", buffer);
		sscanf(buffer, "%d,%d,%d\n", &currentVolume, &minVolume, &maxVolume);

		fclose(file);
		log_i("Player::getVolume currentVolume=%d",currentVolume);
		return currentVolume;
#endif
		return 0;
	}


	void Player::setVolume(int volume)
	{
#ifndef WIN32
		FILE* file;
		char buffer[20];
		int ret;

		file  = fopen("/dev/codec_volume", "wt");
		if (file == NULL) {
			log_e("/dev/codec_volume");
			return;
		}

		sprintf(buffer, "%d", volume);


		ret = fwrite(buffer, 1, strlen(buffer) + 1, file);
		log_i ("setVolume:%s,ret=%d",buffer,ret);
		fclose(file);
#endif
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

	void Player::holdKeyProbe(){
		int fd=0,state;
		char* path = "/sys/class/axppower/holdkey";
		char rbuf;
		
		fd = open(path,O_RDWR, 0);
		read(fd,&rbuf,1);
		state = rbuf&0x10;
		close(fd);
		
		log_i("/sys/class/axppower/holdkey:0x%x",state);
		
		gSession.setHoldKeyState(state);
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
		
		log_i("spdif openCodecPower enable=%d",enable);
		
		fd = open(path,O_RDWR, 0);
		
		rbuf = enable?1:0;
		
		write(fd,&rbuf,1);

		if(enable){
			gSettingProvider.query(SETTING_VOLUME_ID,&volume);
			setVolume(volume);
		}	
		close(fd);
	}
	int PlayerEventInterface::onKeyDispatch(int keyCode,int action, int flag){
		//log_i("PlayerEventInterface::onKeyDispatch keyCode=%d,action=%d",keyCode,action);
		if((keyCode == KEYCODE_VOLUMEUP||keyCode == KEYCODE_VOLUMEDOWN)&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_SHOW_VOLUME,keyCode);
		}else if(keyCode == KEYCODE_PREV&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_PREV,NULL);
		}else if(keyCode == KEYCODE_NEXT&& action == VM_KEYDOWN)
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_NEXT,NULL);
		else if(keyCode == KEYCODE_PLAY&& action == VM_KEYDOWN)
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_IDB_PLAY,NULL);
		else if(action == VM_CAPACITY){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_BATTERY_UPDATE,keyCode);
			if(gPowerManager!=NULL)
				gPowerManager->PowerManagerCount();
		}else if(keyCode == KEYCODE_SHORT_POWER&&action == VM_KEYUP){
			if(gPowerManager!=NULL)
				gPowerManager->setPowerState();
		}else if(keyCode == KEYCODE_LONG_POWER&&action == VM_KEYDOWN){
			gPlayer.showShutDownView();
		}else if(action == VM_MEDIA){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,keyCode,0);
		}else if(keyCode == KEYCODE_HOLD&& action == VM_KEYDOWN){
			gPlayer.holdKeyProbe();
		}else if(keyCode == KEYCODE_SPIDF&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_SPIDF,keyCode);
		}else if(keyCode == KEYCODE_HEADEST&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_HEADEST,keyCode);
		}
		
		if(action!=VM_CAPACITY&&gPowerManager!=NULL)
			gPowerManager->resetCount();
		return 0;
	}

	int PlayerEventInterface::onTouchDispatch(int x,int y, int action){
		if(gPowerManager!=NULL)
			gPowerManager->resetCount();
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
