

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "MediaPlayerInterface.h"

#define PLAY_INTERVAL 200 //ms

namespace mango
{
	enum{
		MODE_PLAY_ORDER=0,
		MODE_PLAY_LOOP,
		MODE_PLAY_RANDOM,
		MDOE_PLAY_REPEAT
	};
	
	class Playinglist{

		public:
			particle::MediaPlayerInterface*  mParticleplayer;
			mediainfo *mplaylist;
			Thread mThread;
			CString mPlayingPath;
			int mIsCue;
			int mCueStart;
			int len;
			int mCurrent;
			int mMax;
			int playMode;
			int mGapless;
			int inPause;
			int isWakeLock;
			int mOrderBy;
			ULONGLONG mLastPlayTime;
			
			Playinglist();
			~Playinglist();
			void startSlient();
			void setWakeLock();
			void releaseWakeLock();
			mediainfo *getPlayingItem();
			mediainfo *getItem(int index);
			void initPlayintList();
			void cursorInit();
			int playMediaInfo(mediainfo *info);
			int isItemExsit(mediainfo *info);
			int isItemExsitAudioId(int id);
			void moveToLast();
			int moveToNext();
			int moveToPrev();
			void moveToPosition(int pos);
			int moveRandom();		
			int startPlay();
			int startPlayPosition(int mesc,bool needStart,bool needGapless);
			void playNext(bool needstart = true);
			void playPrev(bool needstart = true);
			void checkPlayintList(const char* dir = NULL);
			void savePlayintList();
			int playPauseOrStart();
			void callbackPlay();
			void addItem(mediainfo *item);
			void addArrayItem(ArrayMediaInfo& array);
			void removeItem(int n);
			int getDuration();
			int getCurrent();
			int getCount();
			int isPlaying();
			int isPause();
			bool isSpdifOut();
			void setSpdifOut(bool isSpdifOut);
			void setPlayMode(int mode);
			int getPlayMode(){return playMode;}
			void setGapless(int gapless);
			int getGapless(){return mGapless;}
			void setEq(int *val);
			void setEqBandLevel(int index,int val);
			void getAudioInfo(int *);
			int randomInt(int n);
			void PlayerInit();
			void AudioEqualizerEnable(int enable);
			void seekTo(int n);
			void stopPlayer();
			void stopForSdcardEject();
			void setPauseToStart(){ if(inPause&&mParticleplayer != NULL){mParticleplayer->start();setInPauseState(0);} }
			void setPlayPause();
			void clearAll();
			void fastForward();
			void fastRewind();
			void mediaInfoCpy(int des,mediainfo *src);
			void clearPlay();
			void sort(int sortby = 0);
			bool needSortByTrack();
			int compare(mediainfo *first,mediainfo *end,int sortby);
			//char* mstrcpy(char *str,char *arg);
			static void playerCallback(void* calldata, int evnet, int param0, int param1);
			static unsigned int CloseMuteRunnig(void *parameter);
			void setInPauseState(int state);
	};		
	extern Playinglist *mPlayinglist;

};


