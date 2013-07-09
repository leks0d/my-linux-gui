
#include "player.h"


namespace mango
{
#ifndef WIN32
	//	particle::MediaPlayerInterface*  mParticleplayer = NULL; // = particle::createMediaPlayer();
#endif


			Playinglist::Playinglist(){
				len = 0;
				mCurrent = 0;
				mMax = 0;
				playMode = 0;
				mplaylist = NULL;
#ifndef WIN32
				if(mParticleplayer == NULL)
						mParticleplayer = particle::createMediaPlayer();
				//mParticleplayer = (particle::MediaPlayerInterface*)new particle::MediaPlayerService();
				mParticleplayer = NULL;
#endif
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
				len++;
			}

			void Playinglist::moveToNext(){
				int p = mCurrent;
					p++;
				if(p<len)
					mCurrent = p;
				else
					mCurrent = 0;
			}

			void Playinglist::moveToLast(){
				if(len > 0)
					mCurrent = len - 1;
			}

			void Playinglist::moveToPosition(int pos){
				if(pos>0&&pos<len)
					mCurrent = pos;
			}
			
			mediainfo* Playinglist::getPlayingItem(){
				if(mCurrent<len)
					return &mplaylist[mCurrent];
				else
					return mplaylist;
			}

			int Playinglist::startPlay(mediainfo *info){
				int pos;
				log_i("Playinglist::startPlay");
				pos = isItemExsit(info);
				log_i("Playinglist::isItemExsit=%d",pos);
				if(pos>0){
					moveToPosition(pos);
					log_i("Playinglist::moveToPosition");
				}else{	
					addItem(info);	
					log_i("Playinglist::addItem");			
					moveToLast();
					log_i("Playinglist::moveToLast");	
				}
				//log_i("Playinglist::mParticleplayer->stop()");
				log_i("Playinglist::mParticleplayer=0x%x",mParticleplayer);
				//log_i("Playinglist::mParticleplayer->stop=0x%x",*mParticleplayer::stop);
				if(mParticleplayer!=NULL){
				
					if(mParticleplayer->stop()){log_i("stop() success!");}else{log_i("stop() fail!");return -1;}

					if(mParticleplayer->setSource(getPlayingItem()->path)){log_i("setSource() success!");}else{log_i("setSource() fail!");return -1;}

					if(mParticleplayer->prepare()){log_i("prepare() success!");}else{log_i("prepare() fail!");return -1;}

					if(mParticleplayer->start()){log_i("start() success!");}else{log_i("start() fail!");return -1;}
				}

			}

			int Playinglist::isItemExsit(mediainfo *info){
				int i;
				for(i=0;i<len;i++){
					if(mplaylist[i].id == info->id)
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
			
		Playinglist *mPlayinglist = NULL;
};


