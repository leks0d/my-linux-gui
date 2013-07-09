
#include "player.h"

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
	struct {
		char *key;
		char *value;
	}Map;
	
	class Cursor{
		private:
			Map **maplist;
			int pos;
			int len;
			int mMax;
		public:
			Cursor(){
				pos = 0;
				len = 0;
				mMax = 0;
			};
			int setPos(int p){
				if(p>=0&&p<len){
					pos = p;
					return 0;	
				}else
					return -1;
			}
			int moveToNext(){
				int p = pos;
				p++;
				if(p<len){
					pos = p;
					return 0;
				}else
					return -1;
			}
			
			int moveToFisrt(){
				pos = 0;
			}

			void add(char *k,char * val){
				Map mMap;
				int kc,vc;

				if (len >= mMax) {
			        Map* tmp;

			        if (mMax == 0)
			            mMax = 16;      // initial storage
			        else
			            mMax *= 2;

			        tmp = new Map[mMax];
			        if (tmp == 0)
			            return 0;

			        memcpy(tmp, maplist, len * sizeof(Map));
			        delete[] maplist;
			        mArray = tmp;
		    	}
				
				kc = strlen(k)+1;
				vc = strlen(val)+1;

				mMap = new Map;
				mMap.key = new char[kc];
				memcpy(mMap.key,k,kc);

				mMap.value = new char[vc];
				memcpy(mMap.value,val,vc);

				maplist[len] = mMap;
				len++;
			}
			int getInt(char key){
				
			}
			
						
	}	
	
};


