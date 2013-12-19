
#ifndef _OGGID3_H
#define _OGGID3_H
namespace mango
{
class OggID3 : public InterfaceID3
{

public:
	OggID3(void);
	~OggID3(void);

public:
	int setFile(FILE *stream);
	int getTag(int id3, char **data);

private:
	void reset();
	void analyseId3Buffer(char *buf, int size);

	int  getPageBodySize(unsigned char segNum, unsigned char *segTable);


private:
	char *mTagData[IID3TAG_COUNT];
	int  mTagDataSize[IID3TAG_COUNT];

	int  mDuration;

	char *mId3Buf;
	int  mId3BufSize;

	char *mTagBuf;
	char mTagBufSize;

	char mTagNull[2];

};
};
#endif