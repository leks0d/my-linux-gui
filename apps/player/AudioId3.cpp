#ifdef WIN32
#include <Windows.h>
#endif
#include "player.h"
#include "Id3info.h"
#include <cutils/jstring.h>

namespace mango
{
	typedef uint16_t char16_t;
	extern size_t strnlen16to8(const char16_t* utf16Str, size_t len)
	{
		size_t utf8Len = 0;
	
		/* A small note on integer overflow. The result can
		 * potentially be as big as 3*len, which will overflow
		 * for len > SIZE_MAX/3.
		 *
		 * Moreover, the result of a strnlen16to8 is typically used
		 * to allocate a destination buffer to strncpy16to8 which
		 * requires one more byte to terminate the UTF-8 copy, and
		 * this is generally done by careless users by incrementing
		 * the result without checking for integer overflows, e.g.:
		 *
		 *	 dst = malloc(strnlen16to8(utf16,len)+1)
		 *
		 * Due to this, the following code will try to detect
		 * overflows, and never return more than (SIZE_MAX-1)
		 * when it detects one. A careless user will try to malloc
		 * SIZE_MAX bytes, which will return NULL which can at least
		 * be detected appropriately.
		 *
		 * As far as I know, this function is only used by strndup16(),
		 * but better be safe than sorry.
		 */
	
		/* Fast path for the usual case where 3*len is < SIZE_MAX-1.
		 */
		if (len < (SIZE_MAX-1)/3) {
			while (len--) {
				unsigned int uic = *utf16Str++;
	
				if (uic > 0x07ff)
					utf8Len += 3;
				else if (uic > 0x7f || uic == 0)
					utf8Len += 2;
				else
					utf8Len++;
			}
			return utf8Len;
		}
	
		/* The slower but paranoid version */
		while (len--) {
			unsigned int  uic	  = *utf16Str++;
			size_t		  utf8Cur = utf8Len;
	
			if (uic > 0x07ff)
				utf8Len += 3;
			else if (uic > 0x7f || uic == 0)
				utf8Len += 2;
			else
				utf8Len++;
	
			if (utf8Len < utf8Cur) /* overflow detected */
				return SIZE_MAX-1;
		}
	
		/* don't return SIZE_MAX to avoid common user bug */
		if (utf8Len == SIZE_MAX)
			utf8Len = SIZE_MAX-1;
	
		return utf8Len;
	}
	
	
	/**
	 * Convert a Java-Style UTF-16 string + length to a JNI-Style UTF-8 string.
	 *
	 * This basically means: embedded \0's in the UTF-16 string are encoded
	 * as "0xc0 0x80"
	 *
	 * Make sure you allocate "utf8Str" with the result of strlen16to8() + 1,
	 * not just "len".
	 *
	 * Please note, a terminated \0 is always added, so your result will always
	 * be "strlen16to8() + 1" bytes long.
	 */
	extern char* strncpy16to8(char* utf8Str, const char16_t* utf16Str, size_t len)
	{
		char* utf8cur = utf8Str;
	
		/* Note on overflows: We assume the user did check the result of
		 * strnlen16to8() properly or at a minimum checked the result of
		 * its malloc(SIZE_MAX) in case of overflow.
		 */
		while (len--) {
			unsigned int uic = *utf16Str++;
	
			if (uic > 0x07ff) {
				*utf8cur++ = (uic >> 12) | 0xe0;
				*utf8cur++ = ((uic >> 6) & 0x3f) | 0x80;
				*utf8cur++ = (uic & 0x3f) | 0x80;
			} else if (uic > 0x7f || uic == 0) {
				*utf8cur++ = (uic >> 6) | 0xc0;
				*utf8cur++ = (uic & 0x3f) | 0x80;
			} else {
				*utf8cur++ = uic;
	
				if (uic == 0) {
					break;
				}
			}
		}
	
	   *utf8cur = '\0';
	
	   return utf8Str;
	}
	
	/**
	 * Convert a UTF-16 string to UTF-8.
	 *
	 */
	char * strndup16to8 (const char16_t* s, size_t n)
	{
		char*	ret;
		size_t	len;
	
		if (s == NULL) {
			return NULL;
		}
	
		len = strnlen16to8(s, n);
	
		/* We are paranoid, and we check for SIZE_MAX-1
		 * too since it is an overflow value for our
		 * strnlen16to8 implementation.
		 */
		if (len >= SIZE_MAX-1)
			return NULL;
	
		ret = (char*)malloc(len + 1);
		if (ret == NULL)
			return NULL;
	
		strncpy16to8 (ret, s, n);
	
		return ret;
	}

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
		case METADATA_KEY_DURATION:
			ret = IID3_DURATION;break;
	}
	return ret;
}

AudioId3::AudioId3(char* file)
{
	FILE *f =NULL;
	
	mInterfaceID3 = NULL;
	mFFmepgId3 = NULL;
	picdata = NULL;
	piclength = 0;
	
	f = fopen(file,"r");
	if(f>0){
		
		if(isOGGFile(file)){
			mInterfaceID3 = (InterfaceID3 *)new OggID3();	
			mInterfaceID3->setFile(f);
			fclose(f);
			log_i("----------OggID3");
		}else if(isDSFFile(file)){
			mInterfaceID3 = (InterfaceID3 *)new DsfID3();
			if(mInterfaceID3->setFile(f)<0)
				safeDelete(mInterfaceID3);		
			fclose(f);
			log_i("----------DsfID3");
		}else{
			fclose(f);
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
	char *val;
	int ret = -1;
	if(mInterfaceID3!=NULL){
		ret = mInterfaceID3->getTag(tagConvert(id),&val);
		if(id == METADATA_KEY_DURATION){
			ret = sprintf(data,"%d",ret);
		}else{
			if(ret<=0){
				ret = 0;
			}else if (val[0] == 0 || val[0] == 0x03){
				strcpy(data,val+1);
			}else if(val[0] == 1 && val[1] == (char)0xff && val[2] == (char)0xfe){
				char *szBuf = strndup16to8((const char16_t*)(val + 3), (ret-3)/2);
				strcpy(data,szBuf);
			}else{
				ret = 0;
			}
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
	if(mInterfaceID3 != NULL){
		char *tagData;
		int   tagDataSize;
		
		tagDataSize = mInterfaceID3->getTag(IID3_APIC, &tagData);
		
		if (tagDataSize > 0)
		{
			int headerSize = 0;

			if (strnicmp(tagData + 1, "image/jpeg", 10) == 0)
			{
				headerSize = 1 + 11 + 2;

			}
			else if(strnicmp(tagData + 1, "image/png", 9) == 0)
			{
				headerSize = 1 + 10 + 2;
			}
			
			int dataSize = tagDataSize - headerSize;
			
		   	if(dataSize > 0){
		   		piclength = dataSize;
				picdata = tagData + headerSize;
				ret = 1;
		   	}
			
		}			
	}
	return ret;
}
};
