
class DsfID3 : public InterfaceID3
{
public:

#define DSFTAG_COUNT 9

public:
	DsfID3(void);
	~DsfID3(void);

public:
	int setFile(FILE *stream);
	int getTag(int id3, char **data);

private:
	void reset();
	void analyseId3Buffer(char *buf, int size);

private:
	char *mTagData[DSFTAG_COUNT];
	int  mTagDataSize[DSFTAG_COUNT];

	int  mDuration;

	char *mId3Buf;
	int  mId3BufSize;

	char *mTagBuf;
	char mTagBufSize;

	char mTagNull[2];

};
