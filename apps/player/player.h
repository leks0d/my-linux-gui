#include <sys/time.h>
#include <signal.h>
#include <sys/system_properties.h>
#include <sys/vfs.h>
#include <sys/reboot.h>
#include "mango.h"
#include "resource.h"
#include "mediainfo.h"
#include "AlarmManager.h"
#include "CCue.h"
#include "Cursor.h"
#include "KernelMsgGet.h"
#include "SocketDetect.h"
#include "mediaprovider.h"
#include "ArrayMediaInfo.h"
#include "playlist.h"
#include "SettingProvider.h"
#include "PowerManager.h"
#include "playinglist.h"
#include "BitmapFactory.h"
#include "mediaView.h"
#include "SettingsView.h"
#include "playingView.h"
#include "MusicInfoView.h"
#include "DisplaySettingView.h"
#include "PointDrawView.h"
#include "VolumeView.h"
#include "SystemInfoView.h"
#include "EqSettingView.h"
#include "ShutDownView.h"
#include "UsmConnectView.h"
#include "SdcardInsertView.h"
#include "MediaScannerView.h"
#include "MusicOperateView.h"
#include "KeyLockView.h"
#include "ChosenView.h"
#include "PlaylistOperateView.h"
#include "GroupOperateView.h"
#include "InterfaceID3.h"
#include "OggID3.h"
#include "DsfID3.h"
#include "AudioId3.h"
#include <dlfcn.h>
#include "Uvcontert.h"
#include "USBHiFi.h"
#include "USBAudioConnectView.h"

#define ARRAY_LIST_NUM 100
//#define CONVERT_UTF8
#define ES9018_VOLUME "/sys/class/codec/es9018_volume"
#define CODEC_VOLUME	1

namespace mango
{	
//#define NEED_SLEEP
	enum {
		kEncodingNone				= 0,
		kEncodingShiftJIS			= (1 << 0),
		kEncodingGBK				= (1 << 1),
		kEncodingBig5				= (1 << 2),
		kEncodingEUCKR				= (1 << 3),
		kEncodingUTF8				= (1<<4),
		kEncodingWin1251		   = (1<<5),
		kEncodingWin1252		   = (1<<6),
		kEncodingAll				= (kEncodingShiftJIS | kEncodingGBK | kEncodingBig5 | kEncodingEUCKR|kEncodingUTF8|kEncodingWin1251|kEncodingWin1252),
	};


	enum{
		IOCTRL_BRIGTNESS_READ=0,
		IOCTRL_BRIGTNESS_WRITE
	};
	enum{
		POWER_STATE_ON = 0,
		POWER_STATE_STANDBY
	};
	enum
	{
		SETTING_BACK = 0x10,
		SETTING_HOME,
		SETTING_TITLE,
		SETTING_EQ_RESET,
		SETTING_EQ_OPENORCLOSE,
		PLAYING_IDB_NEXT,
		PLAYING_IDB_PREV,
		PLAYING_IDB_PLAY,
		PLAYING_IDB_PLAY_MODE,
		PLAYING_IDB_ADD_TO_PLAYLIST,
		PLAYING_IDB_MUSIC,
		PLAYING_IDB_SETTING,
		PLAYING_IDB_VOLUME,
		PLAYING_IDB_ALBUM_IMAGE,
		PLAYING_IDB_MUSIC_NAME,
		PLAYING_SHOW_VOLUME,
		PLAYING_UPDATE_BATTERY,
		MEDIA_ORDER_TILE,
		MEDIA_ORDER_ALBUM,
		MEDIA_ORDER_ARTIST,
		MEDIA_ORDER_TIME,
	};
	enum{
		MEDIAVIEW_DISPLAY_TYPE_SDCARD = 1,
		MEDIAVIEW_DISPLAY_TYPE_ADD_TO_PLAYLIST,
	};

	class PlayerEventInterface : public UseEventInterface{
		public:
		PlayerEventInterface(){}
		~PlayerEventInterface(){}
		virtual int onKeyDispatch(int keyCode,int action, int flag);
		virtual int onTouchDispatch(int x,int y, int action);
	};
	class PlayerSwitch{
		private:	
			int isSet;
			Mutex	mMutex;
			unsigned int lastTime;
		public:
			PlayerSwitch(){
				resetSwicth();
			}
			void setPlayerSwitch(){
				mMutex.lock();
				isSet = 1;
				lastTime = (unsigned int)Time::getMillisecond();
				log_i("setPlayerSwitch lastTime=%d",lastTime);
				mMutex.unlock();
			}
			int isToSwicth(){
				int ret;
				unsigned int now;
				
				mMutex.lock();
				
				if(isSet == 1){
					now = (unsigned int)Time::getMillisecond() - 500;
					log_i("setPlayerSwitch lastTime=%d,now=%d",lastTime,now);
					if(now>=lastTime){
						ret = 1;
					}
				}else{
					ret = 0;
				}
				
				mMutex.unlock();
				
				return ret;
			}
			void resetSwicth(){
				mMutex.lock();
				log_i("setPlayerSwitch resetSwicth");
				isSet = 0;
				lastTime = -1;
				mMutex.unlock();
			}
	};

	class FileAttr{
		public:
		char *path;
		
		FileAttr(){
			path = NULL;
		}
		FileAttr(char *str){
			int len = strlen(str);
			path = new char[len+1];
			memcpy(path,str,len+1);
		}
		~FileAttr(){
			delete path;
		}
		int isExist(){
			if(path == NULL)
				return 0;			
			return access(path,F_OK) == 0;
		}
		static int FileExist(const char *str){
			if(str == NULL)
				return 0;
			return access(str,F_OK) == 0;

		}
	};
	
	class Player: public Party
	{
	public:
		Player(void);
		virtual ~Player(void);

	public:
		int main();
		int initSettings();
		int showPlayingView();
		int showMediaView(int type=-1);
		int showSettingsView();
		int showMusicInfoView();
		int showMusicInfoView(mediainfo* info);
		int showDisplaySettingView();
		int showPointDrawView();
		int showSystemInfoView();
		int showVolumeView();
		int showEqSettingView();
		int showShutDownView();
		int showUsmConnectView();
		int showMediaScannerView();
		int showSdcardInsertView();
		int showMusicOperateView(mediainfo& info);
		int showMusicOperateView(mediainfo& info,int type,int start=0);
		int showKeyLockView();
		int showChosenView(int type,View *call=NULL);
		int showPlaylistOperateView(PlayListItem& info);
		void showGroupOperateView();
		void showUSBAudioConnectView();
		void dismissView(View *view);
		int  getVolume(void);
		void setVolume(int volume);
		void setHardwareVolume(int volume);
		void setPowerState();
		void ioctrlBrightness(int cmd,int* brightness);
		int holdKeyProbe();
		void spdifProbe();
		bool isSpdifIn();
		bool isHeadestIn();
		void openCodecPower(bool enable);
		void openOrCloseMute(bool enable);
		void openOrCloseWm8740Mute(bool enable);
		void openWm8740Mute();
		void closeWm8740Mute();
		void setBootWakeLock(int en);
		void shutDown();
		void VolumeCheck();
		void initBoardInfo();
	public:
		MediaView*  mMeidaView;
		PlayingView* mPlayingView;
		SettingsView* mSettingsView;
		MusicInfoView* mMusicInfoView;
		DisplaySettingView* mDisplaySettingView;
		PointDrawView* mPointDrawView;
		SystemInfoView* mSystemInfoView;
		VolumeView* mVolumeView;
		EqSettingsView* mEqSettingsView;
		ShutDownView* mShutDownView;
		GroupOperateView* mGroupOperateView;
		MediaScannerView *mMediaScannerView;
		SdcardInsertView *mSdcardInsertView;
		MusicOperateView *mMusicOperateView;
		UsmConnectView *mUsmConnectView;
		KeyLockView *mKeyLockView;
		ChosenView *mChosenView;
		PlaylistOperateView *mPlaylistOperateView;
		USBAudioConnectView *mUSBAudioConnectView;
		KernelMsgGet *mKernelMsgGet;
		USBHiFi *mUSBHiFi;
		Mutex muteMutex;
		Mutex volumeMutex;
		int muteCount;
		
		PlayerEventInterface* mPlayerEventInterface;
		int powerState;
		int isBootLock;
		int bootLockCount;
		SocketDetect *mSocketDetect;
		PlayerSwitch *mSpdifSwitch;
		PlayerSwitch *mHeadestSwitch;

		int mPlayerVolume;
		bool volumeInitFail;
		int mBoardType;//-1-unknow,0-rk2926,1-rk3026.
		int mCodecType;//-1-unknow,0-wm8740,1-es9018.
		int mProductType;//0-dx50,1-dx90.
	};
	
	extern Player  gPlayer;
	extern MSkBitmap gWallpaperBitmap;
	extern uint32_t (*possibleEncodings)(const char*);
	extern void (*ponvertValues)(uint32_t,const char*,char*);
	extern uint32_t mLocaleEncoding;
};
