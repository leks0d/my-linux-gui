#ifdef WIN32
#include <Windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "InterfaceID3.h"
namespace mango
{

InterfaceID3::InterfaceID3(void)
{

}


InterfaceID3::~InterfaceID3(void)
{

}


int InterfaceID3::setFile(FILE *stream)
{
	return -1;
}


int InterfaceID3::getTag(int id3, char **data)
{
	return -1;
}


#ifdef WIN32

FILE *log_stream = NULL;

void tag_print(InterfaceID3  *id3, int id, const char *idName)
{
	char *tagData;
	int   tagDataSize;

	tagDataSize = id3->getTag(id, &tagData);

	//tagData[0] -- Text encoding  $xx
	//$00 - ASCII
	//$01 - UNICODE
	//$03 - UTF8

	if (tagDataSize <= 0)
	{
		printf("%s[size=%d]:\n", idName, tagDataSize);
		if (log_stream)
			fprintf(log_stream, "%s[size=%d]:\n", idName, tagDataSize);
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
	else if (tagData[0] == 0x03)
	{
		if (log_stream)
			fprintf(log_stream, "%s[size=%d]: %s\n", idName, tagDataSize, tagData + 1);
	}
	else 
		printf("%s[size=%d]: !!!!!!!!!!!error code\n", idName, tagDataSize);

}


#include "DsfID3.h"
#include "OggID3.h"


int main(int argc, char *argv[])
{
	int result;
	FILE *stream;

	unsigned char uft8Tag[3] = {0xef, 0xbb, 0xbf};

//	char path[] = "E:\\dsd\\sample\\hmcj10025-5_1_02.dsf";
//	char path[] = "E:\\dsd\\sample\\twl0615-5_1_01.dsf";
//	char path[] = "H:\\DSD\\famc0995-5_1_02a.dsf";
	log_stream = fopen("e:\\id3_log.txt",  "w+");

	fwrite(uft8Tag, 1, 3, log_stream);


	char path[][256] = 
	{
		"H:\\music\\周杰伦青花瓷·ogg.ogg",
		"H:\\music\\魔性之宴·ogg.ogg",
		"H:\\music\\11 城里的月光.ogg",
		"H:\\music\\14 快乐无罪.ogg",
		"H:\\music\\周华健-真心英雄.ogg",
		"H:\\music\\周华健-忘忧草.ogg",
		"H:\\music\\周华健 - 风雨无阻.ogg",
	};


	InterfaceID3 *id3;

	//DsfID3  dsf;

	id3 = new OggID3;
	int i;
	int wideChars;
	int chars;
	WCHAR widePath[256];
	char uft8Path[256 * 3];

	for (i = 0; i < sizeof (path) / sizeof(path[0]); i++)
	{

		stream = fopen(path[i], "rb");


		wideChars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, path[i], strlen(path[i]), widePath, 256);
		widePath[wideChars] = '\0';
		chars = WideCharToMultiByte(CP_UTF8, 0, widePath, wideChars, uft8Path, 256 * 3, NULL, NULL);
		uft8Path[chars] = '\0'; 

		fprintf(log_stream, "\n\n*** %s *** \n\n", uft8Path);


		result = id3->setFile(stream);
		printf("id3->setFile(%s) ret = %d\n", path, result);
		if (result < 0)
		{
			delete id3;
			fclose(stream);
			return 0;
		}


		tag_print(id3, InterfaceID3::IID3_TITLE, "title");
		tag_print(id3, InterfaceID3::IID3_ARTIST, "artist");
		tag_print(id3, InterfaceID3::IID3_ALBUM, "album");

		tag_print(id3, InterfaceID3::IID3_DATE, "date");
		tag_print(id3, InterfaceID3::IID3_COMMENT, "comment");


		tag_print(id3, InterfaceID3::IID3_TRACKNUMBER, "tracknumber");
		tag_print(id3, InterfaceID3::IID3_GENRE, "genre");

		char *tagData;
		int   tagDataSize;

		int duration = id3->getTag(InterfaceID3::IID3_DURATION, &tagData);
		printf("duration = %d\n", duration);


		tagDataSize = id3->getTag(InterfaceID3::IID3_APIC, &tagData);
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
	}

	delete id3;
	fclose(log_stream);
	return 0;
}
#endif
};