#include "player.h"

namespace mango
{	
	
		ArrayMediaInfo::ArrayMediaInfo(){
			len = mMax = 0;
			mplaylist = NULL;
		}
		
		ArrayMediaInfo::~ArrayMediaInfo(){
			delete mplaylist;
		}
			
		void ArrayMediaInfo::addMediaInfo(mediainfo *info){
			
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

				memcpy(&mplaylist[len],info,sizeof(mediainfo));
				len++;
			
		}
		
		mediainfo* ArrayMediaInfo::getMediaInfo(int pos){
				
				if(pos>=0&&pos<len){
					log_i("ArrayMediaInfo::getMediaInfo len=%d,pos=%d,val=0x%x",len,pos,mplaylist+pos);
					return mplaylist+pos;
				}else
					return NULL;
		}

		ArrayMediaInfo& ArrayMediaInfo::operator =(ArrayMediaInfo& info){
			mediainfo *temp;
			log_i("ArrayMediaInfo::operator = mplaylist=0x%x",mplaylist);

			mMax = info.mMax;
			len = info.len;
			
			temp = new mediainfo[mMax];
			
			log_i("new mediainfo[mMax] temp=0x%x",temp);
			
			memcpy(temp,info.mplaylist,len*sizeof(mediainfo));
			delete mplaylist;
			mplaylist = temp;
			
			return *this;
		}
			
		void ArrayMediaInfo::clear(){
			len = mMax = 0;
			if(mplaylist!=NULL)
				delete mplaylist;
			mplaylist = NULL;
		}
	
}
