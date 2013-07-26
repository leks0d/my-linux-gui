
#include "mango.h"
#include "resource.h"
#include "mediaView.h"
#include "playingView.h"
#include "playlist.h"
#include "mediaprovider.h"
#include "playinglist.h"
#include "ArrayMediaInfo.h"


namespace mango
{
	typedef struct
	{
		int id;
		char *path;
		char *name;
		char *name_key;
		char *title;
		char *title_key;
		int artist_id;
		char *artist;
		char *artist_key;
		int album_id;
		char *album;
		char *album_key;
		int track;
		char *img_path;
		int add_time;
		int duration;
		int inPlay;
	}mediainfo;
	
};
