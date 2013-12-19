#ifdef WIN32
#include <Windows.h>
#endif
#include "player.h"
#include "Id3info.h"
namespace mango
{	
	static void strlwr(char *string){
		int wid = 'a' - 'A';
		while( (*string) != '\0'){
			if(*string <= 'Z' && *string >= 'A')
				*string += wid;
			string++;
		}
	}

static char * getfiletype(const char *file,char *type){
		int i,len = 0;
		//char type[10];
		if(file == 0)
			return 0;
		len = strlen(file);
		i = len-1;
		while(*(file+i) != '.'){
			//log_i("*(file+i)=%c\n",*(file+i))
			i--;
			if(i==0)
				break;
		}
		if(len-i>5||i == 0)
			return 0;
		strcpy(type,file+i+1);
		strlwr(type);
		return type;
	}

static int tagConvert(int n){
	int ret = 1000;

	switch(n){
		case METADATA_KEY_CD_TRACK_NUMBER:
			ret = IID3_TRACKNUMBER;break;
		case METADATA_KEY_ALBUM:
			ret = IID3_ALBUM;break;
		case METADATA_KEY_ARTIST:
			ret = IID3_ARTIST;break;
		case METADATA_KEY_GENRE:
			ret = IID3_GENRE;break;
		case METADATA_KEY_TITLE:
			ret = IID3_TITLE;break;
	}
	return ret;
}

AudioId3::AudioId3(char* file)
{
	FILE *f =NULL;
	
	mInterfaceID3 = NULL;
	mFFmepgId3 = NULL;
	
	f = fopen(file,"r");

	if(f>0){
		
		if(isOGGFile(file)){
			mInterfaceID3 = (InterfaceID3 *)new OggID3();
			
			mInterfaceID3->setFile(f);
			//	safeDelete(mInterfaceID3);
			
			fclose(f);
			log_i("----------OggID3");
		}else if(isDSFFile(file)){
			mInterfaceID3 = (InterfaceID3 *)new DsfID3();
			
			if(mInterfaceID3->setFile(f)<0)
				safeDelete(mInterfaceID3);		
			
			fclose(f);
			log_i("----------DsfID3");
		}else{
			mFFmepgId3 = new ID3INFO(file);
			log_i("----------ID3INFO");
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
	char *val[]={0};
	int ret = -1;
	if(mInterfaceID3!=NULL){
		ret = mInterfaceID3->getTag(tagConvert(id),val);
		
		if(ret>0){
			strcpy(data,*val+1);
		}
	}
	if(mFFmepgId3 != NULL){
		ret = mFFmepgId3->GetTags((ID3KEY)id,data);
	}
	return ret;
}
int AudioId3::PicValid(){
	int ret = -1;
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
