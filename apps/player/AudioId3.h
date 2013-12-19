
#ifndef _OGGID3_H
#define _OGGID3_H

#include "InterfaceID3.h"
#include "OggID3.h"
#include "DsfID3.h"
#include "Id3info.h"

namespace mango
{
class AudioId3
{

public:
	AudioId3(const char* file);
	~AudioId3(void);

public:
	int setFile(FILE *stream);
	int GetTags(int id3, char *data);
	int PicValid();
	int piclength;
 	char *picdata;
private:
	InterfaceID3 *mInterfaceID3;
	ID3INFO *mFFmepgId3;
};
};
#endif