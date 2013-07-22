
#ifndef WIN32
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "MediaPlayerInterface.h"
#endif


namespace mango
{

	class Playinglist{

		public:
			particle::MediaPlayerInterface*  mParticleplayer;
			mediainfo *mplaylist;
			int len;
			int mCurrent;
			int mMax;
			int playMode;
			int mGapless;
			Playinglist();
			~Playinglist();
			mediainfo *getPlayingItem();
			mediainfo *getItem(int index);
			int playMediaInfo(mediainfo *info);
			int isItemExsit(mediainfo *info);
			void moveToLast();
			void moveToNext();
			void moveToPosition(int pos);
			int  startPlay();
			void addItem(mediainfo *item);
			int getDuration();
			int getCurrent();
			int getCount();
			int isPlaying();
			void setPlayMode(int mode){if(mode>=0&&mode<=3)playMode=mode;}
			int getPlayMode(){return playMode;}
			void setGapless(int gapless){if(mGapless != gapless){mGapless = gapless;}}
			int getGapless(){return mGapless;}
			static void playerCallback(void* calldata, int evnet, int param0, int param1);
	};		
	extern Playinglist *mPlayinglist;

};


