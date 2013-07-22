
#include "mango.h"
#include "resource.h"
#include "mediaprovider.h"
#include "ArrayMediaInfo.h"
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
#include "NetlinkListener.h"



namespace mango
{	
	enum{
		IOCTRL_BRIGTNESS_READ=0,
		IOCTRL_BRIGTNESS_WRITE
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
		int showPlayingView();
		int showMediaView();
		int showSettingsView();
		int showMusicInfoView();
		int showDisplaySettingView();
		int showPointDrawView();
		int showSystemInfoView();
		int showVolumeView();
		int showEqSettingView();
		int  getVolume(void);
		void setVolume(int volume);
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
		PlayerEventInterface* mPlayerEventInterface;
		sqlite3 *db; 
	};

	extern Player  gPlayer;
};
