
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
		char *artist;
		char *artist_key;
		char *album;
		char *album_key;
		char *genre;
		char *genre_key;
		int track;
		char *img_path;
		int add_time;
		int duration;
		int isInPlayList;
		int inPlay;
		int isPlayed;
		int times;
		int isCue;
		int cueStart;
	}mediainfo;
};
