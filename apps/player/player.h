#include <sys/time.h>
#include <signal.h>
#include "mango.h"
#include "resource.h"
#include "SocketDetect.h"
#include "mediaprovider.h"
#include "ArrayMediaInfo.h"
#include "SettingProvider.h"
#include "PowerManager.h"
#include "playinglist.h"
#include "mediaView.h"
#include "SettingsView.h"
#include "playlist.h"
#include "playingView.h"
#include "MusicInfoView.h"
#include "DisplaySettingView.h"
#include "PointDrawView.h"
#include "VolumeView.h"
#include "SystemInfoView.h"
#include "EqSettingView.h"
#include "ShutDownView.h"
#include "UsmConnectView.h"

namespace mango
{	
	enum{
		IOCTRL_BRIGTNESS_READ=0,
		IOCTRL_BRIGTNESS_WRITE
	};

	enum
	{
		SETTING_BACK = 10,
		SETTING_HOME,
		SETTING_TITLE,
		SETTING_EQ_RESET,
		SETTING_EQ_OPENORCLOSE,
		PLAYING_IDB_NEXT,
		PLAYING_IDB_PREV,
		PLAYING_IDB_PLAY,
		PLAYING_IDB_PLAY_MODE,
		PLAYING_IDB_MUSIC,
		PLAYING_IDB_SETTING,
		PLAYING_IDB_VOLUME,
		PLAYING_IDB_ALBUM_IMAGE,
		PLAYING_IDB_MUSIC_NAME,
		PLAYING_SHOW_VOLUME,
		PLAYING_UPDATE_BATTERY
	};

	class PlayerEventInterface : public UseEventInterface{
		public:
		PlayerEventInterface(){}
		~PlayerEventInterface(){}
		virtual int onKeyDispatch(int keyCode,int action, int flag);
		virtual int onTouchDispatch(int x,int y, int action);
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
		int showMediaView();
		int showSettingsView();
		int showMusicInfoView();
		int showDisplaySettingView();
		int showPointDrawView();
		int showSystemInfoView();
		int showVolumeView();
		int showEqSettingView();
		int showShutDownView();
		int showUsmConnectView();
		void dismissView(View *view);
		int  getVolume(void);
		void setVolume(int volume);
		void setPowerState();
		void ioctrlBrightness(int cmd,int* brightness);

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
		PlayerEventInterface* mPlayerEventInterface;
		int powerState;
		SocketDetect *mSocketDetect;
		UsmConnectView *mUsmConnectView;
		sqlite3 *db; 
	};

	extern Player  gPlayer;
};
