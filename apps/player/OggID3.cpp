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


typedef struct tagPAGE_HEADER
{
	char mID[4];
	char mVersion;
	char mType;
	char mPosition[8];
	char mSerNum[4];
	char mSeqNum[4];
	char mCRCCheck[4];
	unsigned char mSegNum;
}  PAGE_HEADER;

#else


typedef struct tagPAGE_HEADER
{
	char mID[4];
	char mVersion;
	char mType;
	char mPosition[8];
	char mSerNum[4];
	char mSeqNum[4];
	char mCRCCheck[4];
	unsigned char mSegNum;

} __attribute__ ((packed))  PAGE_HEADER;

#endif




OggID3::OggID3(void)
{
	mId3Buf = NULL;
	mId3BufSize = 0;
	mTagBuf = NULL;
	mTagBufSize = 0;

	mTagNull[0] = '\0';
	mTagNull[1] = '\0';
}


OggID3::~OggID3(void)
{
	SAFE_FREE(mId3Buf);
	SAFE_FREE(mTagBuf);
}



int OggID3::getPageBodySize(unsigned char segNum, unsigned char *segTable)
{
	int count = (int)(unsigned int)segNum;
	int size = 0;
	int i;

	for (i = 0; i < count; i++)
	{
		size += (int)(unsigned int)segTable[i];
	}

	return size;
}



int OggID3::setFile(FILE *stream)
{
	int ret = -1;
	int pageCount = 0;
	PAGE_HEADER  pageHeader;

	int dataBeginPos;
	int dataEndPos;
	int streamPos;
	int pageBodySize;
	unsigned char segTable[256];
	char segData[8];
	int id3TagTotalSize =0;

	reset();

	streamPos = 0;
	pageBodySize = 0;

	while (pageCount < 6)
	{
		 fseek(stream, streamPos + pageBodySize, SEEK_SET);
		 pageCount++;

		 if (fread(&pageHeader, sizeof(PAGE_HEADER), 1, stream) < 1)
			break;

		if (strnicmp(pageHeader.mID, "OggS", 4) != 0)
			break;

		if (fread(segTable,	1, pageHeader.mSegNum, stream) < 1)
			break;

		pageBodySize = getPageBodySize(pageHeader.mSegNum, segTable);
		streamPos    = ftell(stream);

		 if (fread(segData, 1, 7, stream) < 1)
			break;

		 if (segData[0] != 0x03)
			 continue;

		if (strnicmp(segData+1, "vorbis", 6) != 0)
			continue;

		id3TagTotalSize = pageBodySize - 7;
		ret = 1;
		break;
	}

	if (ret == -1)
		return ret ;

	
	 if (id3TagTotalSize > 1024 * 16)
		 id3TagTotalSize = 1024 * 16;

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

	 mId3Buf[id3TagTotalSize - 1] = '\0';
	 analyseId3Buffer(mId3Buf, id3TagTotalSize);
}


void OggID3::reset()
{
	memset(mTagData, 0, sizeof(char *) * IID3TAG_COUNT);
	memset(mTagDataSize, 0, sizeof(int) * IID3TAG_COUNT);

	mDuration = 0;
}


#define ID3_TAG_HEADER_SIZE 10

#define ID3_TAG_ID(a,b,c,d) (unsigned int)(((unsigned int)a << 24) + ((unsigned int)b << 16) + ((unsigned int)c << 8) + ((unsigned int)d))

void OggID3::analyseId3Buffer(char *buf, int size)
{
	char *pTag = buf;
	unsigned int tagSize;
	unsigned int tagId;
	unsigned short tagFlag;
	int tagCount;
	int status  = 0;

	while (size > 4)
	{
		tagSize = (unsigned short)(unsigned char)pTag[3];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[2];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[1];
		tagSize = (tagSize << 8) + (unsigned short)(unsigned char)pTag[0];

		pTag[0] = '\0'; //为字符串提供结束符

		if (status == 0)
		{
			status = 1;
		}
		else if (status == 1)
		{
			tagCount = tagSize; 
			tagSize  = 0;

			status   = 2;
		}
		else if (strnicmp(pTag+4, "album=", 6) == 0)
		{
			mTagData[IID3_ALBUM] = pTag + 4 + 5;
			pTag[4 + 5] = 0x03;
			mTagDataSize[IID3_ALBUM] = tagSize - 5;
		}
		else if (strnicmp(pTag+4, "title=", 6) == 0)
		{
			mTagData[IID3_TITLE] = pTag + 4 + 5;
			pTag[4 + 5] = 0x03;
			mTagDataSize[IID3_TITLE] = (int)tagSize - 5;

		}
		else if (strnicmp(pTag+4, "artist=", 7) == 0)
		{
			mTagData[IID3_ARTIST] = pTag + 4 + 6;
			pTag[4 + 6] = 0x03;
			mTagDataSize[IID3_ARTIST] = tagSize - 6;

		}
		else if (strnicmp(pTag+4, "genre=", 6) == 0)
		{
			mTagData[IID3_GENRE] = pTag + 4 + 5;
			pTag[4 + 5] = 0x03;
			mTagDataSize[IID3_GENRE] = tagSize - 5;

		}
		else if (strnicmp(pTag+4, "comment=", 8) == 0)
		{
			mTagData[IID3_COMMENT] = pTag + 4 + 7;
			pTag[4 + 7] = 0x03;
			mTagDataSize[IID3_COMMENT] = tagSize - 7;
		}
		else if (strnicmp(pTag+4, "tracknumber=", 12) == 0)
		{
			mTagData[IID3_TRACKNUMBER] = pTag + 4 + 11;
			pTag[4 + 11] = 0x03;
			mTagDataSize[IID3_TRACKNUMBER] = tagSize - 11;
		}

		size -= (int)(tagSize + 4);
		pTag += (tagSize + 4);
	}

}


int OggID3::getTag(int id, char **data)
{
	*data = mTagNull;

	if (id >= IID3TAG_COUNT)
		return -1;

	if (id == IID3_DURATION)
		return -1;

	*data = mTagData[id];
	return mTagDataSize[id];
}

