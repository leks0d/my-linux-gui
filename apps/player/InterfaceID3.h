
#ifndef _INTERFACEID3_H
#define _INTERFACEID3_H
//#define strnicmp strncasecmp


static int strnicmp(const char *s1, const char *s2, int len)
{
    unsigned char c1, c2;
    if(!len)
        return 0;
    do{
        c1 = *s1++;
        c2 = *s2++;
        if (!c1 || !c2)
            break;
        if (c1 == c2)
            continue;
        c1 = tolower(c1);
        c2 = tolower(c2);
        if (c1 != c2)
            break;
    }while(--len);
    return (int)c1 - (int)c2;
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


#endif
