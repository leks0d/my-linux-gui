#include "player.h"

namespace mango
{	
	
		ArrayMediaInfo::ArrayMediaInfo(){
			len = 0;
			mMax = 0;
			mplaylist = NULL;
			//log_i("ArrayMediaInfo::ArrayMediaInfo %d,len=%d,mMax=%d,",mplaylist,len,mMax);
		}
		
		ArrayMediaInfo::~ArrayMediaInfo(){
			//log_i("enter");
			clear();
			//log_i("leave");
		}
			
		void ArrayMediaInfo::addMediaInfo(mediainfo *info){

				//log_i("ArrayMediaInfo::addMediaInfo %d,len=%d,mMax=%d,",info,len,mMax);
			
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
					//log_i("ArrayMediaInfo::getMediaInfo len=%d,pos=%d,val=0x%x",len,pos,mplaylist+pos);
					return mplaylist+pos;
				}else
					return NULL;
		}
		void ArrayMediaInfo::sort(int sortby){
			mediainfo info;
			int i,count;

			if(needSortByTrack()){
				sortby = 1;
			}
			log_i("ArrayMediaInfo sort by %d,len=%d",sortby,len);
			for(count = len;count>1;count--){
				for(i=0;i<count-1;i++){
					if(compare(&mplaylist[i],&mplaylist[i+1],sortby) > 0){
						info = mplaylist[i];
						mplaylist[i] = mplaylist[i+1];
						mplaylist[i+1] = info;
					}
				}
			}
		}
		bool ArrayMediaInfo::needSortByTrack(){
			int i,count = 0;

			for(i=0;i<len;i++){
				if(mplaylist[i].track>0)
					count++;
			}

			if(count>1)
				return true;
			else
				return false;
		}
		int ArrayMediaInfo::compare(mediainfo *first,mediainfo *end,int sortby){
			int ret=0;
			
			if(sortby == 0){//order by name
				if(first->name_key == NULL)
					ret = -1;
				else if(end->name_key ==NULL)
					ret = 1;
				else
					ret = strcmp(first->name_key,end->name_key);
			}else if(sortby == 1){//order by track
				//ret = strcmp(first->name,end->name);
				if(ret == 0){
					if(first->track < end->track){
						if(first->track != 0)	//zero order in the end.
							ret = -1;
						else
							ret = 1;
					}else if(first->track > end->track){
						if(end->track!=0)		//zero order in the end.
							ret = 1;
						else
							ret = -1;
					}else{
						ret = strcmp(first->name_key,end->name_key);
					}
				}
			}
			//log_i("ArrayMediaInfo::compare ret = %d",ret);
			return ret;
		}

		ArrayMediaInfo& ArrayMediaInfo::operator =(ArrayMediaInfo& info){
			mediainfo *temp;
			//log_i("ArrayMediaInfo::operator = mplaylist=0x%x",mplaylist);

			mMax = info.mMax;
			len = info.len;
			
			temp = new mediainfo[mMax];
			
			//log_i("new mediainfo[mMax] temp=0x%x",temp);
			
			memcpy(temp,info.mplaylist,len*sizeof(mediainfo));
			delete mplaylist;
			mplaylist = temp;
			
			return *this;
		}
			
		void ArrayMediaInfo::clear(){
			int i;
			
			for(i=0;i<len;i++){
				safefreeMediainfo(&mplaylist[i]);
			}
			
			len = mMax = 0;
			
			if(mplaylist!=NULL){
				delete[] mplaylist;
				mplaylist = NULL;
			}
		}
	
}
