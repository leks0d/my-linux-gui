
#ifndef _INTERFACEID3_H
#define _INTERFACEID3_H
namespace mango
{


static int strnicmp(const char* src,const char* arg,int len){
	int slen;
	char *temp;

	temp = new char[len+1];
	
	if(src!=NULL){
		slen = sizeof(src);
		if(slen<len){
			return -1;
		}else{
			memcpy(temp,src,len);
			return strcmp(temp,arg);
		}
	}else{
		return -1;
	}

	delete temp;
}

enum 
{
	IID3_TITLE = 0,
	IID3_ARTIST,
	IID3_ALBUM,
	IID3_DATE,
	IID3_COMMENT,
	IID3_TRACKNUMBER,
	IID3_GENRE,
	IID3_APIC,
	IID3_DURATION,
};

class InterfaceID3
{
public:


#define IID3TAG_COUNT 9

public:
	InterfaceID3(void);
	virtual ~InterfaceID3(void);

public:
	virtual int setFile(FILE *stream);
	virtual int getTag(int id3, char **data);

};
};

#endif
