#ifndef ID3INFO_
#define ID3INFO_
#ifndef WIN32
#define __UINT64_C(c)     c ## ULL
#define UINT64_C(c)       __UINT64_C(c)
#endif
#ifdef __cplusplus
extern "C" 
{
#endif
//#include "libavcodec/avfft.h"
//#include "libavutil/mem.h"
//#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
//#include "libavformat/metadata.h"
//#include "libswscale/swscale.h"
//#include "libavformat/avio.h"
//#include "libavutil/avutil.h"
#ifdef __cplusplus
}
#endif
enum ID3KEY
{
	 METADATA_KEY_CD_TRACK_NUMBER=0,
	 METADATA_KEY_DISC_NUMBER,
	 METADATA_KEY_ALBUM,
	 METADATA_KEY_ARTIST,
	 METADATA_KEY_ALBUMARTIST,
	 METADATA_KEY_COMPOSER,
	 METADATA_KEY_GENRE,
	 METADATA_KEY_TITLE,
	 METADATA_KEY_YEAR,
	 METADATA_KEY_DURATION,
	 METADATA_KEY_WRITER
};

class ID3INFO
{
public:
		ID3INFO(char* inputfile);
		~ID3INFO();
private:
	int vedioStream;
	AVFormatContext *format_ctx; 
  int PicValided;
public:
	int	GetTags(enum ID3KEY id3_key,char *value);
	const char * Getmimetype();
	int PicValid();
	void dump_tags();
	int piclength;
  char *picdata;
  
		
};
#endif