#ifdef WIN32
#include <Windows.h>
#endif
#include "player.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE( x )  \
    if( NULL != x )       \
    {                     \
        delete x;		  \
        x = NULL;         \
    }
#endif


#ifndef SAFE_FREE
#define SAFE_FREE( x )  \
    if( NULL != x )       \
    {                     \
        free(x) ;		  \
        x = NULL;         \
    }
#endif



#ifdef WIN32
#pragma pack(1)


typedef struct tagCHUNK_HEADER
{
	char mID[4];
	unsigned long long mSize;

}  CHUNK_HEADER;

typedef struct tagDSD_CHUNK
{
	unsigned long long mFileSize;
	unsigned long long mMetadataOffset;

}   DSD_CHUNK;


typedef struct tagFMT_CHUNK
{
	int	mFormatVersion;
	int mFormatID;
	int mChannelType;
	int mChannelNum;
	int mSamplingFreq;
	int mBitsPerSample;

	unsigned long long mSampleCount;
	int mBlockSizePerChannel;
	int mReserved;

}   FMT_CHUNK;

#else


typedef struct tagCHUNK_HEADER
{
	char mID[4];
	unsigned long long mSize;

} __attribute__ ((packed))  CHUNK_HEADER;

typedef struct tagDSD_CHUNK
{
	unsigned long long mFileSize;
	unsigned long long mMetadataOffset;

} __attribute__ ((packed))  DSD_CHUNK;


typedef struct tagFMT_CHUNK
{
	int	mFormatVersion;
	int mFormatID;
	int mChannelType;
	int mChannelNum;
	int mSamplingFreq;
	int mBitsPerSample;

	unsigned long long mSampleCount;
	int mBlockSizePerChannel;
	int mReserved;

} __attribute__ ((packed))  FMT_CHUNK;

#endif


#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164



DsfID3::DsfID3(void)
{
	mId3Buf = NULL;
	mId3BufSize = 0;
	mTagBuf = NULL;
	mTagBufSize = 0;

	mTagNull[0] = '\0';
	mTagNull[1] = '\0';
}


DsfID3::~DsfID3(void)
{
	SAFE_FREE(mId3Buf);
	SAFE_FREE(mTagBuf);
}

int DsfID3::setFile(FILE *stream)
{
	int ret = -1;
	CHUNK_HEADER  mChunkHeader;
	DSD_CHUNK	  mDsdChunk;
	FMT_CHUNK     mFmtChunk;

	CHUNK_HEADER  chunkHeader;
	int dataBeginPos;
	int dataEndPos;

	reset();

	fseek(stream, 0, SEEK_SET);
	if (fread(&chunkHeader, sizeof(CHUNK_HEADER), 1, stream) < 1)
		return -1;

	if (strnicmp(chunkHeader.mID, "dsd ", 4) == 0)
	{
		fread(&mDsdChunk, sizeof(DSD_CHUNK), 1, stream);
	}
	else
		return -1;

	while (1)
	{
		if (fread(&chunkHeader, sizeof(CHUNK_HEADER), 1, stream) < 1)
			break;

		 if (strnicmp(chunkHeader.mID, "fmt ", 4) == 0)
		{
			fread(&mFmtChunk, sizeof(FMT_CHUNK), 1, stream);
		}
		else if (strnicmp(chunkHeader.mID, "data", 4) == 0)
		{
			dataBeginPos = ftell(stream);

			dataEndPos = dataBeginPos + (int)(chunkHeader.mSize) - 12;
			ret = 1;
			break;
		}
	}

	if (ret == -1)
		return ret ;

	//id3
	char buffer[10];
	int id3TagTotalSize;
	fseek(stream, dataEndPos, SEEK_SET);

	//header
	if (fread(buffer, 1, 10, stream) < 10)
		return 1;

	 if (strnicmp(buffer, "ID3", 3) != 0)
		 return 1;

	 //ver buffer[3]
	 //Revision buffer[4]
	 //Flag buffer[5]
	 //size buffer[6 ~ 9]
	 id3TagTotalSize = (unsigned short)(unsigned char)(buffer[6] & 0x7f);
	 id3TagTotalSize = (id3TagTotalSize << 7) + (unsigned short)(unsigned char)(buffer[7] & 0x7f);
	 id3TagTotalSize = (id3TagTotalSize << 7) + (unsigned short)(unsigned char)(buffer[8] & 0x7f);
	 id3TagTotalSize = (id3TagTotalSize << 7) + (unsigned short)(unsigned char)(buffer[9] & 0x7f);


	 if (id3TagTotalSize > 10 * 1024 * 1024)
		 return 1;

	 if (mId3Buf == NULL || mId3BufSize < id3TagTotalSize)
	 {
		SAFE_FREE(mId3Buf);
		mId3Buf = (char *)malloc(id3TagTotalSize);
		mId3BufSize = id3TagTotalSize;
	 }

	 if (!mId3Buf)
		 return 1;

	 if (fread(mId3Buf, 1, id3TagTotalSize, stream) < (unsigned int)id3TagTotalSize)
		 return 1;

	 analyseId3Buffer(mId3Buf, id3TagTotalSize);

	 if (mFmtChunk.mSamplingFreq > 0)
		mDuration = (int)(mFmtChunk.mSampleCount * 1000 / mFmtChunk.mSamplingFreq);

	 return 1;
}


void DsfID3::reset()
{
	memset(mTagData, 0, sizeof(char *) * DSFTAG_COUNT);
	memset(mTagDataSize, 0, sizeof(int) * DSFTAG_COUNT);

	mDuration = 0;
}


#define ID3_TAG_HEADER_SIZE 10

#define ID3_TAG_ID(a,b,c,d) (unsigned int)(((unsigned int)a << 24) + ((unsigned int)b << 16) + ((unsigned int)c << 8) + ((unsigned int)d))

void DsfID3::analyseId3Buffer(char *buf, int size)
{
	char *pTag = buf;
	unsigned int tagSize;
	unsigned int tagId;
	unsigned short tagFlag;

	while (size > ID3_TAG_HEADER_SIZE)
	{
		tagId = (unsigned short)(unsigned char)pTag[0];
		tagId = (tagId << 8) + (unsigned short)(unsigned char)pTag[1];
		tagId = (tagId << 8) + (unsigned short)(unsigned char)pTag[2];
		tagId = (tagId << 8) + (unsigned short)(unsigned char)pTag[3];

		tagSize = (unsigned short)(unsigned char)pTag[4];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[5];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[6];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[7];

		//tagSize = (unsigned int)(pTag[4] & 0x7f);
		//tagSize = (tagSize << 7) + (unsigned int)(pTag[5] & 0x7f);
		//tagSize = (tagSize << 7) + (unsigned int)(pTag[6] & 0x7f);
		//tagSize = (tagSize << 7) + (unsigned int)(pTag[7] & 0x7f);

		tagFlag = (unsigned short)(unsigned char)pTag[8];
		tagFlag = (tagFlag << 8) + (unsigned short)(unsigned char)pTag[9];

		switch(tagId)
		{
		case ID3_TAG_ID('T', 'I', 'T', '2'):
			mTagData[IID3_TITLE] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_TITLE] = (int)tagSize;
			break;

		case ID3_TAG_ID('T', 'P', 'E', '1'):
			mTagData[IID3_ARTIST] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_ARTIST] = tagSize;
			break;
	
		case ID3_TAG_ID('T', 'A', 'L', 'B'):
			mTagData[IID3_ALBUM] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_ALBUM] = tagSize;
			break;

		case ID3_TAG_ID('T','Y','E','R'):
			mTagData[IID3_DATE] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_DATE] = tagSize;
			break;

		case ID3_TAG_ID('T', 'C', 'O', 'N'):
			mTagData[IID3_GENRE] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_GENRE] = tagSize;
			break;

		case ID3_TAG_ID('C', 'O', 'M', 'M'):
			mTagData[IID3_COMMENT] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_COMMENT] = tagSize;
			break;

		case ID3_TAG_ID('T', 'R', 'C', 'K'):
			mTagData[IID3_TRACKNUMBER] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_TRACKNUMBER] = tagSize;
			break;

		case ID3_TAG_ID('A', 'P', 'I', 'C'):
			mTagData[IID3_APIC] = pTag + ID3_TAG_HEADER_SIZE;
			mTagDataSize[IID3_APIC] = tagSize;
			break;

		}

		size -= (int)(tagSize + ID3_TAG_HEADER_SIZE);
		pTag += (tagSize + ID3_TAG_HEADER_SIZE);
	}

}


int DsfID3::getTag(int id, char **data)
{
	*data = mTagNull;
	
	if (id >= DSFTAG_COUNT)
		return -1;

	if (id == IID3_DURATION)
		return mDuration;

	*data = mTagData[id];

	return mTagDataSize[id];
}

#if 0

void tag_print(DsfID3  *dsf, int id, const char *idName)
{
	char *tagData;
	int   tagDataSize;

	tagDataSize = dsf->getTag(id, &tagData);

	//tagData[0] -- Text encoding  $xx
	//$00 - ASCII
	//$01 - UNICODE
	if (tagDataSize <= 0)
	{
		printf("%s[size=%d]:\n", idName, tagDataSize);
	}
	else if (tagData[0] == 0)
	{
		printf("%s[size=%d]:%s\n", idName, tagDataSize, tagData + 1);
	}
	else if (tagData[0] == 1 && tagData[1] == (char)0xff && tagData[2] == (char)0xfe)
	{
		printf("%s", idName);
		wprintf(L"[size=%d]:%s\n", tagDataSize, tagData + 3);
	}
	else 
		printf("%s[size=%d]: !!!!!!!!!!!error code\n", idName, tagDataSize);

}


int main(int argc, char *argv[])
{
	int result;
	FILE *stream;

//	char path[] = "E:\\dsd\\sample\\hmcj10025-5_1_02.dsf";
//	char path[] = "E:\\dsd\\sample\\twl0615-5_1_01.dsf";
	char path[] = "H:\\DSD\\famc0995-5_1_02a.dsf";

	stream = fopen(path, "rb");

	DsfID3  dsf;

	result = dsf.setFile(stream);
	printf("dsf.setFile(%s) ret = %d\n", path, result);
	if (result < 0)
	{
		fclose(stream);
		return 0;
	}


	tag_print(&dsf, DsfID3::IID3_TITLE, "title");
	tag_print(&dsf, DsfID3::IID3_ARTIST, "artist");
	tag_print(&dsf, DsfID3::IID3_ALBUM, "album");
	
	tag_print(&dsf, DsfID3::IID3_DATE, "date");
	tag_print(&dsf, DsfID3::IID3_COMMENT, "comment");


	tag_print(&dsf, DsfID3::IID3_TRACKNUMBER, "tracknumber");
	tag_print(&dsf, DsfID3::IID3_GENRE, "genre");

	char *tagData;
	int   tagDataSize;

	int duration = dsf.getTag(DsfID3::IID3_DURATION, &tagData);
	printf("duration = %d\n", duration);


	tagDataSize = dsf.getTag(DsfID3::IID3_APIC, &tagData);
	printf("apic[size=%d]:\n", tagDataSize);
	if (tagDataSize > 0)
	{
		int headerSize = 0;
		FILE *file;

		//tagData[0] --  Text encoding
		if (strnicmp(tagData + 1, "image/jpeg", 10) == 0)
		{
			file = fopen("e:\\a.jpg", "wb");

			headerSize = 1 + 11 + 2;
	
			fwrite(tagData + headerSize, 1, tagDataSize - headerSize, file);
			fclose(file);
		}
		else if(strnicmp(tagData + 1, "image/png", 9) == 0)
		{
			file = fopen("e:\\a.png", "wb");
			headerSize = 1 + 10 + 2;
	
			fwrite(tagData + headerSize, 1, tagDataSize - headerSize, file);
			fclose(file);
		}
	}


	fclose(stream);

	return 0;
}

#endif
