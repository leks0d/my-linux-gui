
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
		
			Playinglist();
			~Playinglist();
			mediainfo *getPlayingItem();
			int startPlay(mediainfo *info);
			int isItemExsit(mediainfo *info);
			void moveToLast();
			void moveToNext();
			void moveToPosition(int pos);
			void addItem(mediainfo *item);
			int getDuration();
			int getCurrent();
	};		
	extern Playinglist *mPlayinglist;

};


