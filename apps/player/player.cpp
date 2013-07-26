#include "player.h"
#include <sys/reboot.h>
#include <sys/cdefs.h>

namespace mango
{


	Player::Player(void)
	{
		mMeidaView = NULL;
		mPlayingView = NULL;
		mSettingsView = NULL;
		mMusicInfoView = NULL;
		powerState = 0;
	}


	Player::~Player(void)
	{
		safeDelete(mMeidaView);
	}

	int Player::main()
	{
		wakeLock();
		initialize();
		gSettingProvider.initialize();
		initSettings();
		
		mango::Thread::sleep(1000 * 2);
		
		gmediaprovider.initialize();
		
		gmediaprovider.mediascanner();

		mPlayinglist = new Playinglist();
		mPlayinglist->initPlayintList();
		
		mPlayerEventInterface = new PlayerEventInterface();
		gSession.setUseEventInterface((UseEventInterface*)mPlayerEventInterface);

		showPlayingView();

		gPowerManager = new PowerManager();
		
		return messageLoop();
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
			mPointDrawView = new PointDrawView (TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
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
			mSystemInfoView = new SystemInfoView(TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
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
			mEqSettingsView = new EqSettingsView(TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
			mEqSettingsView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mEqSettingsView);
		}

		if (mEqSettingsView){
			mEqSettingsView->invalidateRect();
			mEqSettingsView->setFocus();
		}
	}
	int Player::showVolumeView(){
		if (mVolumeView == NULL) {
			mVolumeView = new VolumeView(TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
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
			mShutDownView = new ShutDownView(TEXT("MusicInfo"), NULL, NULL, 0, SW_NORMAL);
			mShutDownView->onCreate();
		}else {
			gSession.mViewZAxis.bringViewToTop(mShutDownView);
		}
		if (mShutDownView){
			mShutDownView->invalidateRect();
			mShutDownView->setFocus();
			mango::Thread::sleep(1000 * 3);
			gPowerManager->setPowerState(2);
			reboot(RB_POWER_OFF);
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
	int PlayerEventInterface::onKeyDispatch(int keyCode,int action, int flag){	
		if((keyCode == KEYCODE_VOLUMEUP||keyCode == KEYCODE_VOLUMEDOWN)&& action == VM_KEYDOWN){
			gMessageQueue.post(gPlayer.mPlayingView,VM_COMMAND,PLAYING_SHOW_VOLUME,NULL);
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
		}else if(keyCode == KEYCODE_LONG_POWER&&action == VM_KEYUP){
			gPlayer.showShutDownView();
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
	//mango::Thread::sleep(1000 * 6);
#endif
	return mango::gPlayer.main();
}