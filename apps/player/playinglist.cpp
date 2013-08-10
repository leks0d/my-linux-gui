
#include "player.h"


namespace mango
{
static const char *PlayerLock = "playerlock";
	enum 
		{
			MEDIA_PLAYER_ERROR = 1,
			MEDIA_PLAYER_EOF,
	
		};

#ifndef WIN32
	//	particle::MediaPlayerInterface*  mParticleplayer = NULL; // = particle::createMediaPlayer();
#endif
	

			Playinglist::Playinglist(){
				particle::MediaPlayerInterface*  mtemp;
				
				len = 0;
				mCurrent = 0;
				mMax = 0;
				playMode = 0;
				mplaylist = NULL;
				inPause = 0;
				mParticleplayer = NULL;
				isWakeLock = 0;
			}

			void Playinglist::initPlayintList(){
				int count,i;
				ArrayMediaInfo *pinfo;
				pinfo = new ArrayMediaInfo();
				count = gmediaprovider.queryMusicArray(NULL,pinfo);
				for(i=0;i<count;i++){
					addItem(pinfo->getMediaInfo(i));
				}
				gSettingProvider.query(SETTING_PLAYMODE_ID,&playMode);
			}

			void Playinglist::checkPlayintList(){
				int i;
				for(i=0;i<getCount();i++){
					if(access(getItem(i)->path,F_OK) != 0){
						removeItem(i);
						if(mCurrent>i)
							mCurrent--;
						else if(mCurrent == i)
							mCurrent = 0;
						i--;
					}
				}
			}

			void Playinglist::savePlayintList(){
				gmediaprovider.updateInPlay(0);
			}
			
			void Playinglist::removeItem(int n){
				int i;
				int count = getCount();
				mediainfo *ptr = mplaylist;
				for(i=n;i<count-1;i++){
					memcpy(&ptr[i],&ptr[i+1],sizeof(mediainfo));
				}
				memset(&ptr[len-1],0,sizeof(mediainfo));
				len--;
			}
			
			void Playinglist::addItem(mediainfo *item	){

				if(len>=mMax){
					mediainfo *temp;

					if(mMax == 0)
						mMax = 16;
					else
						mMax*=2;
					
					temp = new mediainfo[mMax];
					memcpy(temp,mplaylist,len*sizeof(mediainfo));
					delete mplaylist;
					mplaylist = temp;
				}

				memcpy(&mplaylist[len],item,sizeof(mediainfo));
				mplaylist[len].inPlay = 0;
				len++;
			}

			int Playinglist::moveToNext(){
				int p = mCurrent;

				if(len==0)
					return 0;

				p++;

				if(p<len)
					mCurrent = p;
				else
					mCurrent = 0;

				return 1;
			}
			int Playinglist::moveToPrev(){
				int p = mCurrent;
				
				p--;
				
				if(len == 0)
					return 0;
				else if(p<0)
					mCurrent = len - 1;
				else
					mCurrent = p;

				return 1;
			}

			int Playinglist::moveRandom(){
				int i,random;
				IntegerArray *array;

				if(len<=0)
					return 0;
				
				array = new IntegerArray();
				
				for(i=0;i<len;i++){
					if(getItem(i)->inPlay == 0)
						array->addItem(i);
				}
				
				if(array->getCount() == 0){
					for(i=0;i<len;i++){
						getItem(i)->inPlay = 0;
						array->addItem(i);
					}
				}
				
				random = randomInt(array->getCount());
				moveToPosition(array->getItem(random));
				
				return 1;
			}
			
			void Playinglist::moveToLast(){
				if(len > 0)
					mCurrent = len - 1;
			}

			void Playinglist::moveToPosition(int pos){
				if(pos>=0&&pos<len)
					mCurrent = pos;
			}

			void Playinglist::playNext(){
				int ret;
				if(playMode != MODE_PLAY_RANDOM){
					ret = moveToNext();
				}else
					ret = moveRandom();
				if(ret)
					startPlay();
			}
			void Playinglist::playPrev(){
				int ret;
				if(moveToPrev())
				startPlay();
			}			
			mediainfo* Playinglist::getPlayingItem(){
				if(mCurrent<len)
					return &mplaylist[mCurrent];
				else
					return mplaylist;
			}

			int Playinglist::playMediaInfo(mediainfo *info){
				int pos;
				log_i("Playinglist::startPlay");
				pos = isItemExsit(info);
				log_i("Playinglist::isItemExsit=%d",pos);
				if(pos>=0){
					moveToPosition(pos);
					log_i("Playinglist::moveToPosition");
				}else{
					addItem(info);
					log_i("Playinglist::addItem");			
					moveToLast();
					log_i("Playinglist::moveToLast");	
				}
				startPlay();
			}
			int Playinglist::randomInt(int n){
				long random;
				srand( (unsigned int)time(0) );
				random = rand();
				return random%n;
			}

			int Playinglist::startPlay(){
				startPlayPosition(0,true,true);
			}
			
			int Playinglist::startPlayPosition(int mesc,bool needStart,bool needGapless){
				if(mParticleplayer == NULL){
					mParticleplayer = particle::createMediaPlayer();
					mParticleplayer->setEventCallback(Playinglist::playerCallback,(void *)this);
					PlayerInit();
				}
				
				char *playPath = getPlayingItem()->path;
				
				log_i("Playinglist::startPlayPosition needStart=%d, %d/%d:%s",needStart,mCurrent,len,playPath);
				
				if(needGapless&&mGapless>0&&mParticleplayer!=NULL&&mParticleplayer->setNextSongForGapless(playPath)){
					
					if(mParticleplayer->gaplessPlay(playPath)){log_i("gaplessPlay() success!");}
					else{log_i("gaplessPlay() fail!");}
					
				}else if(mParticleplayer!=NULL){
				
					if(mParticleplayer->stop()){log_i("stop() success!");}else{log_i("stop() fail!");return -1;}
					if(mParticleplayer->setSource(playPath)){log_i("setSource() success!");}else{log_i("setSource() fail!");return -1;}
					if(mParticleplayer->prepare()){log_i("prepare() success!");}else{log_i("prepare() fail!");return -1;}
					//if(mParticleplayer->seekTo(mesc)){log_i("seekTo() success!");}else{log_i("seekTo() fail!");return -1;}
					if(needStart)
						if(mParticleplayer->start()){log_i("start() success!");}else{log_i("start() fail!");return -1;}
				}
				
			}
			
			int Playinglist::isItemExsit(mediainfo *info){
				int i;
				for(i=0;i<len;i++){
					
					if(mplaylist[i].id == info->id && info->id!=-1)
						return i;
					
				}
				return -1;
			}
			
			Playinglist::~Playinglist(){
				len = 0;
				mCurrent = 0;
				mMax = 0;
				delete mplaylist;
			}

			int Playinglist::getDuration(){
				if(mParticleplayer!=NULL)
					return mParticleplayer->getDuration();
				else
					return 1;
			}

			int Playinglist::getCurrent(){
				if(mParticleplayer!=NULL)
					return mParticleplayer->getCurrentPosition();
				else
					return 0;
			}

			int Playinglist::getCount(){
				return len;
			}


			int Playinglist::isPlaying(){
				if(mParticleplayer != NULL)
					return mParticleplayer->isPlaying();
				else
					return 0;
			}
			bool Playinglist::isSpdifOut(){
				if(mParticleplayer != NULL)
					return mParticleplayer->isSpdifOut();
				else
					return false;
			}

			void Playinglist::setSpdifOut(bool isSpdif){
				int playPostion;
				if(mParticleplayer != NULL){
					if(mParticleplayer->isSpdifOut() != isSpdif){
						mParticleplayer->enableSpdifOut(isSpdif);
						if(!isSpdif && (isPlaying()||inPause)){
							playPostion = mParticleplayer->getCurrentPosition();
							startPlayPosition(playPostion,inPause?false:true,false);
						}
					}
				}
			}
			mediainfo* Playinglist::getItem(int index){
				if(index<len)
					return &mplaylist[index];
				else
					return NULL;
			}

			void Playinglist::playerCallback(void* calldata, int evnet, int param0, int param1){
				Playinglist *playlist = (Playinglist *)calldata;
				log_i("Playinglist::playerCallback evnet = %d",evnet);
				switch(evnet){
					case MEDIA_PLAYER_EOF:
						//playlist->playNext();
						gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,NM_PLAY_COM,(unsigned int)calldata);
						break;
					}
			}

		void Playinglist::callbackPlay(){
			switch(playMode){
				case MODE_PLAY_ORDER:
					if(len>0&&mCurrent==(len - 1)){
						if(mParticleplayer->stop()){
							releaseWakeLock();
						}
						break;
					}
				case MODE_PLAY_LOOP:
				case MODE_PLAY_RANDOM:	
					playNext();
					break;
				case MDOE_PLAY_REPEAT:
					startPlay();
					break;
			}
		}
		
		void Playinglist::playPauseOrStart(){
			if(mParticleplayer == NULL){
				log_i("mParticleplayer=NULL");
				if(len>0)
					startPlay();
				return;
			}else if(mParticleplayer->isPlaying()){
				log_i("mParticleplayer->isPlaying");
				mParticleplayer->pause();
				releaseWakeLock();
				inPause = 1;
			}else if(inPause == 1){
				log_i("mParticleplayer inPause");
				mParticleplayer->start();
				inPause = 0;
			}else{
				log_i("mParticleplayer startPlay");
				if(len>0)
					startPlay();
			}
		}
		void Playinglist::setEq(int *val){
			int i;
			if(mParticleplayer != NULL){
				for(i=0;i<8;i++){
					log_i("equalizerSetBandLevel:i=%d,val=%d",i,val[i]);
					mParticleplayer->equalizerSetBandLevel(i,val[i]);
				}
			}
		}
		
		void Playinglist::setEqBandLevel(int index,int val){
			if(mParticleplayer != NULL){
					log_i("equalizerSetBandLevel:index=%d,val=%d",index,val);
					mParticleplayer->equalizerSetBandLevel(index,val);
			}
		}
		void Playinglist::getAudioInfo(int *info){
			if(mParticleplayer != NULL){
				mParticleplayer->getAudioParameter(info,info+1,info+2,info+3,info+4,info+5);
			}
		}
		void Playinglist::PlayerInit(){
			int eqOpen;
			int gaplessEn;
			int EqValue[8];
			
			if(mParticleplayer == NULL)
				return;

			gSettingProvider.query(SETTING_EQSTATE_ID,&eqOpen);
			gSettingProvider.query(SETTING_GAPLESS_ID,&gaplessEn);
			gSettingProvider.EqQuery(SETTING_GAPLESS_ID,EqValue);

			log_i("PlayerInit eqOpen=%d,gaplessEn=%d",eqOpen,gaplessEn);
			
			mParticleplayer->audioEqualizerEnable(eqOpen);
			mParticleplayer->setGaplessDuration(GaplessValue[gaplessEn]);
			setEq(EqValue);
			mGapless = gaplessEn;
		}
		void Playinglist::AudioEqualizerEnable(int enable){
			if(mParticleplayer != NULL)
				mParticleplayer->audioEqualizerEnable(enable);
		}
		void Playinglist::setGapless(int gapless){
			int gaplessEn;
			gaplessEn = gapless;
			if(mParticleplayer!=NULL)
				mParticleplayer->setGaplessDuration(GaplessValue[gaplessEn]);
			gSettingProvider.update(SETTING_GAPLESS_ID,gaplessEn);
			mGapless = gaplessEn;
		}

		void Playinglist::setPlayMode(int mode){
			if(mode>=0&&mode<=3){
				playMode=mode;
				gSettingProvider.update(SETTING_PLAYMODE_ID,playMode);
			}
		}
		void Playinglist::setPlayPause(){
			if(mParticleplayer!=NULL&&mParticleplayer->isPlaying()){
				mParticleplayer->pause();
				inPause = 1;
			}
		}
		void Playinglist::setWakeLock(){
			log_i("Playinglist::setWakeLock isWakeLock=%d",isWakeLock);
			if(!isWakeLock){
				if(gPlayer.wakeLock(PlayerLock) == 0)
					isWakeLock = 1;
			}
		}

		void Playinglist::releaseWakeLock(){
			log_i("Playinglist::releaseWakeLock isWakeLock=%d",isWakeLock);
			if(isWakeLock){
				if(gPlayer.wakeUnlock(PlayerLock) == 0)
					isWakeLock = 0;
			}
		}
		
		Playinglist *mPlayinglist = NULL;
};


