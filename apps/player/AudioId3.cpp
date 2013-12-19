#ifdef WIN32
#include <Windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "AudioId3.h"
#include "mango.h"
#include "resource.h"
#include "mediainfo.h"
#include "SocketDetect.h"
#include "AlarmManager.h"
#include "CCue.h"
#include "Cursor.h"
#include "mediaprovider.h"

namespace mango
{

static int tagConvert(int n){
	int ret = IID3_DURATION;

	switch(n){
		case METADATA_KEY_CD_TRACK_NUMBER:
			ret = IID3_TRACKNUMBER;
		case METADATA_KEY_ALBUM:
			ret = IID3_ALBUM;
		case METADATA_KEY_ARTIST:
			ret = IID3_ARTIST;
		case METADATA_KEY_GENRE:
			ret = IID3_GENRE;
		case METADATA_KEY_TITLE:
			ret = IID3_TITLE;
	}

	return ret;
}

AudioId3::AudioId3(const char* file)
{
	FILE *f =NULL;
	
	mInterfaceID3 = NULL;
	mFFmepgId3 = NULL;
	
	f = fopen(file,"r");

	if(f>0){
		
		if(isOGGFile(file)){
			//mInterfaceID3 = (InterfaceID3 *)new OggID3();
			OggID3 *p=new OggID3();
			mInterfaceID3->setFile(f);
			fclose(f);
		}else if(isDSFFile(file)){
			mInterfaceID3 = (InterfaceID3 *)new DsfID3();
			mInterfaceID3->setFile(f);			
			fclose(f);
		}else{
			mFFmepgId3 = new ID3INFO(file);
		}
		
	}
}


AudioId3::~AudioId3(void)
{
	if(mInterfaceID3 != NULL)
		delete mInterfaceID3;
	if(mFFmepgId3 != NULL)
		delete mFFmepgId3;
}


int AudioId3::GetTags(int id, char *data)
{
	if(mInterfaceID3!=NULL){
		mInterfaceID3->getTag(tagConvert(id),&data);
	}
	if(mFFmepgId3 != NULL){
		mFFmepgId3->GetTags(id,data);
	}
}
int AudioId3::PicValid(){
	int ret = 0;
	if(mFFmepgId3 != NULL){
		ret = mFFmepgId3->PicValid();
		if(ret){
			piclength = mFFmepgId3->piclength;
			picdata = mFFmepgId3->picdata;
		}
	}
	return ret;
}
};
