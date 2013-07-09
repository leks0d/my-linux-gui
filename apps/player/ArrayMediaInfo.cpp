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
				if(pos>=0&&pos<len)
					return &mplaylist[pos];
				else
					return NULL;
		}

	
}
