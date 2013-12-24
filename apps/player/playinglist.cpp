
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
			static	char* mstrcpy(char *str,char *arg){
				int len;		
				if(arg == NULL)
					return NULL;
				len = strlen(arg)+1;
				str = (char *)malloc(len);
				memcpy(str,arg,len);
				
				return 0;
			}

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

				clearPlay();
			}

			void Playinglist::initPlayintList(){
				int count,i,playpost;
				ArrayMediaInfo *pinfo;
				mediainfo *info;

				log_i("-------------Playinglist::initPlayintList");
				pinfo = new ArrayMediaInfo();
				
				count = gmediaprovider.queryMusicArray("order by inplay",pinfo);
				count = pinfo->getCount();
				
				for(i=0;i<count;i++){
					info = pinfo->getMediaInfo(i);
					if(info->inPlay){
						addItem(pinfo->getMediaInfo(i));
					}
				}
				//checkPlayintList();
				gSettingProvider.query(SETTING_PLAYMODE_ID,&playMode);
				gSettingProvider.query(SETTING_PLAYPOS_ID,&playpost);
				gSettingProvider.query(SETTING_GAPLESS_ID,&mGapless);
				
				log_i("playpost = %d,countd",playpost);
				moveToPosition(playpost);
			}
			void Playinglist::cursorInit(){
				int i,playpost;
				Cursor cur;
				
				gmediaprovider.queryCursor("where inplay>0 order by inplay",&cur);

				for(i=0;i<cur.mLen;i++){
					int inplay;
					CString cstr;
					cur.getValueCstring(i,"inplay",cstr);
					
					if(cstr.toIneger(&inplay)>0){
						if(inplay>0){
							CursorMediaInfo curMedia;
							curMedia.setCursorItem(cur.mList[i]);
							addItem(&curMedia.mInfo);
						}
					}
				}
				
				gSettingProvider.query(SETTING_PLAYMODE_ID,&playMode);
				gSettingProvider.query(SETTING_PLAYPOS_ID,&playpost);
				gSettingProvider.query(SETTING_GAPLESS_ID,&mGapless);
				
				moveToPosition(playpost);		
			}
			void Playinglist::checkPlayintList(const char *dir){
				int i;
				for(i=0;i<getCount();i++){
					CString path;
					path = getItem(i)->path;
					if(dir == NULL || ( path.Find(dir,0) == 0) ){
						if(access(path.string,F_OK) != 0){
							removeItem(i);
							if(mCurrent>i)
								mCurrent--;
							else if(mCurrent == i)
								mCurrent = 0;
							i--;
						}
					}
				}
			}

			void Playinglist::savePlayintList(){
				int i,count = getCount();
				int pos;
				mediainfo *info;
				IntegerArray array;
				
				log_i("--------------------savePlayintList-----------");
				
				if(count > gmediaprovider.queryMusicNum()/2){
					gmediaprovider.updateInPlay(1);
					gmediaprovider.queryAllMusicId((void*)&array);
					
					count = array.getCount();
					
					for(i=0;i<count;i++){
						pos = isItemExsitAudioId(array.getItem(i));
						if(pos<0)
							gmediaprovider.updateInPlay(0,array.getItem(i));	
					}
				}else{
					gmediaprovider.updateInPlay(0);
				
					for(i=0;i<count;i++){
						info = getItem(i);
						gmediaprovider.updateInPlay(i+1,info->id);
					}
				}
				
				if(getCount()>0){
					gSettingProvider.update(SETTING_PLAYPOS_ID,mCurrent);
				}
				
			}
			void Playinglist::removeItem(int n){
				int i;
				int count = getCount();
				mediainfo *ptr = mplaylist;

				if(count <= 0 || n < 0)
					return;
				safefreeMediainfo(&mplaylist[n]);
				for(i=n;i<count-1;i++){
					memcpy(&ptr[i],&ptr[i+1],sizeof(mediainfo));
				}
				
				memset(&ptr[len-1],0,sizeof(mediainfo));
				len--;
			}
			void Playinglist::addArrayItem(ArrayMediaInfo& array){
				int count = array.getCount();
				int i;
				for(i=0;i<count;i++){
					if(isItemExsit(array.getMediaInfo(i))<0)
						addItem(array.getMediaInfo(i));
				}
			}
			void Playinglist::addItem(mediainfo *item	){

				if(len>=mMax){
					mediainfo *temp;

					if(mMax == 0)
						mMax = ARRAY_LIST_NUM;
					else
						mMax*=2;
					
					temp = new mediainfo[mMax];
					memcpy(temp,mplaylist,len*sizeof(mediainfo));
					delete mplaylist;
					mplaylist = temp;
				}

				//memcpy(&mplaylist[len],item,sizeof(mediainfo));
				
				mediaInfoCpy(len,item);

				mplaylist[len].isPlayed = 0;
				len++;
			}
			void Playinglist::clearPlay(){
				mIsCue = 0;
				mPlayingPath = "null";
				mCueStart = 0;
			}
			void Playinglist::mediaInfoCpy(int des,mediainfo *src){
				//log_i("enter src=0x%x",src);
				memset(&mplaylist[des],0,sizeof(mediainfo));
				mplaylist[des].id = src->id;
				mplaylist[des].track = src->track;
				mplaylist[des].add_time = src->add_time;
				mplaylist[des].duration = src->duration;
				mplaylist[des].isInPlayList = src->isInPlayList;
				mplaylist[des].inPlay = src->inPlay;
				mplaylist[des].isPlayed = src->isPlayed;
				mplaylist[des].times = src->times;
				mplaylist[des].isCue = src->isCue;
				mplaylist[des].cueStart = src->cueStart;
				
				if(src->path == NULL)
					return;
				mplaylist[des].path = new char[strlen(src->path)+1];
				strcpy(mplaylist[des].path,src->path);
				
				//log_i("tag src->name_key=0x%x",src->name_key);
				if(src->name != NULL){
					mplaylist[des].name = new char[strlen(src->name)+1];
					strcpy(mplaylist[des].name,src->name);
				}
				
				//log_i("tag ");
				if(src->name_key != NULL){
					mplaylist[des].name_key = new char[strlen(src->name_key)+1];
					strcpy(mplaylist[des].name_key,src->name_key);
				}
				
				//log_i("tag");
				if(src->title != NULL){
					mplaylist[des].title = new char[strlen(src->title)+1];
					strcpy(mplaylist[des].title,src->title);
				}else{
					//log_i("mplaylist[des].title=0x%x",mplaylist[des].title);
				}
				
				//log_i("tag");
				if(src->title_key != NULL){
					mplaylist[des].title_key = new char[strlen(src->title_key)+1];
					strcpy(mplaylist[des].title_key,src->title_key);
				}
				
				//log_i("tag");
				if(src->artist != NULL){
					mplaylist[des].artist = new char[strlen(src->artist)+1];
					strcpy(mplaylist[des].artist,src->artist);
				}
				
				//log_i("tag");
				if(src->artist_key != NULL){
					mplaylist[des].artist_key = new char[strlen(src->artist_key)+1];
					strcpy(mplaylist[des].artist_key,src->artist_key);
				}
				
				//log_i("tag");
				if(src->album != NULL){
					mplaylist[des].album = new char[strlen(src->album)+1];
					strcpy(mplaylist[des].album,src->album);
				}
				
				//log_i("tag");
				if(src->album_key != NULL){
					mplaylist[des].album_key = new char[strlen(src->album_key)+1];
					strcpy(mplaylist[des].album_key,src->album_key);
				}
				
				//log_i("tag");
				if(src->img_path != NULL){
					mplaylist[des].img_path = new char[strlen(src->img_path)+1];
					strcpy(mplaylist[des].img_path,src->img_path);
				}
				if(src->md5 != NULL){
					mplaylist[des].md5 = new char[strlen(src->md5)+1];
					strcpy(mplaylist[des].md5,src->md5);
				}
				//log_i("leave");
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
					if(getItem(i)->isPlayed == 0){
						array->addItem(i);
						log_i("array->addItem(%d)",i);
					}
				}
				log_i("moveRandom : array->getCount()=%d",array->getCount());
				if(array->getCount() == 0){
					log_i("moveRandom getCount() reset");
					for(i=0;i<len;i++){
						getItem(i)->isPlayed = 0;
						array->addItem(i);
					}
				}
				
				random = randomInt(array->getCount());
				moveToPosition(array->getItem(random));

				delete array;
				
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

			void Playinglist::playNext(bool needstart){
				int ret;
				if(playMode != MODE_PLAY_RANDOM){
					ret = moveToNext();
				}else
					ret = moveRandom();
				if(ret&&needstart)
					startPlay();
				else
					stopPlayer();
			}
			void Playinglist::playPrev(bool needstart){
				int ret;
				if(moveToPrev()&&needstart)
					startPlay();
				else
					stopPlayer();
			}
			mediainfo* Playinglist::getPlayingItem(){
				if(len == 0)
					return NULL;
				//log_i("mCurrent=%d,len=%d",mCurrent,len);
				if(mCurrent<len)
					return &mplaylist[mCurrent];
				else
					return NULL;
			}

			int Playinglist::playMediaInfo(mediainfo *info){
				int pos;
				
				pos = isItemExsit(info);
				
				if(pos>=0){
					moveToPosition(pos);
				}else{
					addItem(info);
					moveToLast();
				}
				
				return startPlay();
			}
			int Playinglist::randomInt(int n){
				long random;
				srand( (unsigned int)time(0) );
				random = rand();
				return random%n;
			}
			
			int Playinglist::startPlay(){
				return startPlayPosition(0,true,true);
			}
			
			int Playinglist::startPlayPosition(int mesc,bool needStart,bool needGapless){
				mediainfo *info;
				
				if(mParticleplayer == NULL){
					mParticleplayer = particle::createMediaPlayer();
					mParticleplayer->setEventCallback(Playinglist::playerCallback,(void *)this);
					PlayerInit();
					Environment::openMute();
				}
				
				if(mParticleplayer == NULL)
					return -1;
				
				if(getPlayingItem() == NULL)
					return -1;
				
				char *playPath = getPlayingItem()->path;
				
				if(getPlayingItem()->isCue == 0 
					&& mPlayingPath != NULL 
					&& mPlayingPath == playPath
					&& isPlaying()){
					return -2;
				}else if(getPlayingItem()->isCue == mIsCue 
					&& mCueStart == getPlayingItem()->cueStart 
					&& mPlayingPath == playPath
					&& isPlaying()){
					return -2;
				}else if(getPlayingItem()->isCue == 1 
					&& mCueStart != getPlayingItem()->cueStart 
					&& mPlayingPath == playPath
					&& isPlaying()){
					
					if(mParticleplayer->seekTo(getPlayingItem()->cueStart)){log_i("seekTo() success!");}else{log_i("seekTo() fail!");return -1;}

					mPlayingPath = playPath;
					mIsCue = getPlayingItem()->isCue;
					mCueStart = getPlayingItem()->cueStart;					
					return 0;
				}
				
				mPlayingPath = playPath;
				mIsCue = getPlayingItem()->isCue;
				mCueStart = getPlayingItem()->cueStart;

				if(playPath == NULL || !FileAttr::FileExist(playPath) || Environment::get_file_size(playPath)<10)
					return -3;
				
				log_i("Playinglist::startPlayPosition needStart=%d, %d/%d:%s",needStart,mCurrent,len,playPath);
				
				if(needGapless&&mGapless>0&&mParticleplayer->setNextSongForGapless(playPath)){
				
					if(mParticleplayer->gaplessPlay(playPath)){log_i("gaplessPlay() success!");}
					else{log_i("gaplessPlay() fail!");stopPlayer();return -1;}
				
				}else{
					gPlayer.openWm8740Mute();
					if(mParticleplayer->stop()){log_i("stop() success!");}else{log_i("stop() fail!");goto Exit;}
					if(mParticleplayer->setSource(playPath)){log_i("setSource() success!");}else{log_i("setSource() fail!");goto Exit;}
					if(mParticleplayer->prepare()){log_i("prepare() success!");}else{log_i("prepare() fail!");goto Exit;}
					//if(mParticleplayer->seekTo(15*1000)){log_i("seekTo() success!");}else{log_i("seekTo() fail!");return -1;}
					if(needStart)
						if(mParticleplayer->start()){log_i("start() success!");}else{log_i("start() fail!");goto Exit;}
					
					
					
					if(getPlayingItem()->isCue)
					{
						if(getPlayingItem()->cueStart>1000){
							mango::Thread::sleep(1000);
							
							if(mParticleplayer->seekTo(getPlayingItem()->cueStart)){
								log_i("seekTo() cueStart=%d",getPlayingItem()->cueStart);
							}else{
								log_i("seekTo fail.");
							}
							mThread.create(Playinglist::CloseMuteRunnig,(void*)1000);
						}else{
							mThread.create(Playinglist::CloseMuteRunnig,(void*)200);
						}
					}else{
						mThread.create(Playinglist::CloseMuteRunnig,(void*)200);
					}
					gPlayer.VolumeCheck();
				
				}
				if(needGapless&&mGapless>0)
					mParticleplayer->setNextSongForGapless(playPath);//(getItem(mCurrent+1)->path);
				//log_i("gapless no change");
				getPlayingItem()->isPlayed = 1;
				setWakeLock();
				return 0;
Exit:
				gPlayer.closeWm8740Mute();
				return -4;
			}
			unsigned int Playinglist::CloseMuteRunnig(void *parameter){
				int sleepTime = (int)parameter;
				mango::Thread::sleep(sleepTime);
				log_i("sleepTime=%d",sleepTime);
				gPlayer.closeWm8740Mute();
			}
			int Playinglist::isItemExsit(mediainfo *info){
				int i;
				for(i=0;i<len;i++){
					
					if(mplaylist[i].id == info->id && info->id!=-1){
						return i;
					}else if(info->id == -1 || mplaylist[i].id == -1){
						if(strcmp(mplaylist[i].path,info->path) == 0)
							return i;
					}
				}
				return -1;
			}
			int Playinglist::isItemExsitAudioId(int id){
				int i;
				for(i=0;i<len;i++){
					if(mplaylist[i].id == id){
						return i;
					}
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
				int dur;
				if(mParticleplayer!=NULL){
					dur = mParticleplayer->getDuration();
				}else{
					dur = 0;
				}
				log_i("Playinglist::getDuration dur=%d",dur);
				return dur;
			}

			int Playinglist::getCurrent(){
				int cur;
				unsigned long t,s;

				t = Time::getMicrosecond();
				
				if(mParticleplayer!=NULL){
					cur = mParticleplayer->getCurrentPosition();
				}else{
					cur = 0;
				}
				s = Time::getMicrosecond()-t;
				//log_i("cur=%d,spec=%ld",cur,s);
				return cur;
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
						if(!isSpdif && (isPlaying())){
							//playPostion = mParticleplayer->getCurrentPosition();
							//startPlayPosition(playPostion,inPause?false:true,false);

							playPauseOrStart();
							mango::Thread::sleep(500);
							playPauseOrStart();
						}else if(isSpdif && isPlaying()){
							playPauseOrStart();
							mango::Thread::sleep(500);
							playPauseOrStart();
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
					if(len>0 && mCurrent==(len - 1)){
						stopPlayer();	
						break;
					}
				case MODE_PLAY_LOOP:
					if(len>0 && mCurrent==(len - 1)){
						clearPlay();
					}
				case MODE_PLAY_RANDOM:	
					playNext();
					break;
				case MDOE_PLAY_REPEAT:
					clearPlay();
					if(startPlay() == -2){
						seekTo(600);
					}
					break;
			}
		}
		
		int Playinglist::playPauseOrStart(){
			int ret = 0;
			if(mParticleplayer == NULL){
				log_i("mParticleplayer=NULL");
				if(len>0){
					startPlay();
					ret = 1;
				}
			}else if(mParticleplayer->isPlaying()){
				log_i("mParticleplayer->isPlaying");
				mParticleplayer->pause();
				releaseWakeLock();
				inPause = 1;
				ret = 2;
			}else if(inPause == 1){
				log_i("mParticleplayer inPause");
				mParticleplayer->start();
				setWakeLock();
				inPause = 0;
				ret = 3;
			}else{
				log_i("mParticleplayer startPlay");
				if(len>0){
					startPlay();
					ret = 1;
				}
			}
			return ret;
		}
		void Playinglist::setEq(int *val){
			int i;
			if(mParticleplayer != NULL){
				for(i=0;i<8;i++){
					//log_i("equalizerSetBandLevel:i=%d,val=%d",i,val[i]);
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
			int eqOpen,eqMode;
			int gaplessEn;
			int EqValue[8];
			
			if(mParticleplayer == NULL)
				return;

			gSettingProvider.query(SETTING_EQSTATE_ID,&eqOpen);
			gSettingProvider.query(SETTING_GAPLESS_ID,&gaplessEn);
			gSettingProvider.query(SETTING_EQMODE_ID,&eqMode);
			gSettingProvider.EqQuery(eqMode,EqValue);

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
			if(mParticleplayer!=NULL && gaplessEn>= 0)
				mParticleplayer->setGaplessDuration(GaplessValue[gaplessEn]);
			gSettingProvider.update(SETTING_GAPLESS_ID,gaplessEn);
			mGapless = gaplessEn;
			log_i("GaplessValue[%d]=%d",mGapless,GaplessValue[mGapless]);
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
				if(gPlayer.wakeLock(PlayerLock) == 0){
#ifndef NEED_SLEEP					
					gPlayer.openCodecPower(true);
#endif
					isWakeLock = 1;
				}
			}
		}

		void Playinglist::releaseWakeLock(){
			log_i("Playinglist::releaseWakeLock isWakeLock=%d",isWakeLock);
			if(isWakeLock){
				if(gPlayer.wakeUnlock(PlayerLock) == 0){
#ifndef NEED_SLEEP
					gPlayer.openCodecPower(false);
#endif
					isWakeLock = 0;
				}
			}
		}
		void Playinglist::stopForSdcardEject(){
			log_i("------------------Playinglist::stopForSdcardEject");
			if((mParticleplayer != NULL) && (mParticleplayer->isPlaying() || inPause)){
				log_i("-----mParticleplayer isplayong or pause");
				mediainfo *info = getPlayingItem();
				log_i("-----mParticleplayer getPlayingItem()");
				if(info == NULL)
					return;
				
				log_i("info->path = %s",info->path);
				
				char *ret = strstr(info->path,"/mnt/external_sd");
				if(ret){
					log_i("strstr ret = %s",ret);
					stopPlayer();
				}
			}
		}
		void Playinglist::seekTo(int n){
			if(mParticleplayer!=NULL){
				log_i("seek to n=%d",n);
				mParticleplayer->seekTo(n);
			}
		}

		void Playinglist::stopPlayer(){
			if(mParticleplayer != NULL){
				mParticleplayer->stop();
			}
			inPause = 0;
			releaseWakeLock();
		}
		void Playinglist::clearAll(){
			int i;
			log_i("enter len = %d",len);
			if(len>0){
				for(i=0;i<len;i++){
					safefreeMediainfo(&mplaylist[i]);
				}
				memset(mplaylist,0,sizeof(mediainfo)*len);
				len = 0;
			}
		}
		#define WEST_TIME 1000*10
		
		void Playinglist::fastForward(){
			int cur,dur,pos;
			if(mParticleplayer != NULL&&mParticleplayer->isPlaying()){
				cur = mParticleplayer->getCurrentPosition();
				dur = mParticleplayer->getDuration();
				pos = cur + WEST_TIME;
				
				log_i("cur=%d,dur=%d",cur/1000,dur/1000);
				
				if(pos >= dur)
					pos = dur;
				
				mParticleplayer->seekTo(pos);
			}
		}
		void Playinglist::fastRewind(){
			int cur,dur,pos;
			if(mParticleplayer != NULL&&mParticleplayer->isPlaying()){
				cur = mParticleplayer->getCurrentPosition();
				dur = mParticleplayer->getDuration();
				pos = cur - WEST_TIME;
				
				if(pos <= 0)
					return;
				
				mParticleplayer->seekTo(pos);
			}
		}

		Playinglist *mPlayinglist = NULL;
};


