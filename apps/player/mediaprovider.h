#include <sqlite3.h>
#include "Id3info.h"


namespace mango
{	
	#define TABLE_PATH "/data/mango.db"

	#define TABLE_NAME "music"
	
	#define MUSIC_ID "_id"
	#define MUSIC_PTAH "path"
	#define MUSIC_NAME "name"	
	#define MUSIC_NAME_KEY "name_key"	
	#define MUSIC_TITLE "title"
	#define MUSIC_TITLE_KEY "title_key"
	#define MUSIC_ART "artist"
	#define MUSIC_ART_KEY "artist_key"
	#define MUSIC_ALBUM "album"
	#define MUSIC_ALBUM_KEY "album_key"
	#define MUSIC_TRACK "track"
	#define MUSIC_ART_IMG "img_path"
	#define MUSIC_ADD_TIME "add_time"
	#define MUSIC_DURATION "duration"
	#define MUSIC_IN_PLAY "inplay"
	#define MUSIC_TIMES "times"

	#define MUSIC_TABLE_CREATE "CREATE TABLE IF NOT EXISTS music(\
									_id INTEGER PRIMARY KEY autoincrement,\
									path TEXT,\
									name TEXT,\
									name_key TEXT,\
									title TEXT,\
									title_key TEXT,\
									artist TEXT,\
									artist_key TEXT,\
									album TEXT,\
									album_key TEXT,\
									track TEXT,\
									img_path TEXT,\
									add_time INTEGER,\
									duration INTEGER,\
									inplay INTEGER,\
									times INTEGER\
									);"
	#define IMG_PATH "/mnt/sdcard/.album_img"

	#define TABLE_PLAYLIST "playlist"
	
	#define PLAYLIST_TABLE_CREATE "CREATE TABLE IF NOT EXISTS playlist(\
									_id INTEGER PRIMARY KEY autoincrement,\
									name TEXT,\
									remak TEXT\
									);"
	#define TABLE_PLAYLIST_MEM "playlistmem"
	
	#define PLAYLISTMEM_TABLE_CREATE "CREATE TABLE IF NOT EXISTS playlistmem(\
									_id INTEGER PRIMARY KEY autoincrement,\
									playlist_id INTEGER,\
									audio_id INTEGER\
									);"	

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
		int track;
		char *img_path;
		int add_time;
		int duration;
		int isInPlayList;
		int inPlay;
		int isPlayed;
		int times;
	}mediainfo;

	struct Musicdb{
		mediainfo info;
		struct Musicdb *next;;
	};
	typedef struct Musicdb Musicdbinfo;
	class mediaprovider
	{
	public:
		mediaprovider(void);
		int mediascanner(char *path);
		int filescanner(char *path);
		int externVolumeScanner(char *path);
		int sendMsgStart();
		int sendMsgEnd();
		int getmediainfo(char *path,mediainfo *info);
		static unsigned int FileScannerRunnig(void *parameter);
//		int ismusic(char *path);
		int initialize(void);
		int exec(char *sql,void *arg,int (*callback)(void*,int,char**,char**));
		int insert(char *table,mediainfo *info);
		int querymusic(char *where,mediainfo **info);
		int queryMusicArray(char *where, void* array);
		int updateInPlay(int value,int id =-1);
		int del(char *table,int id);
		virtual ~mediaprovider(void);
		int checkfile();
		void genImgPath(char *title,char *path);
		int music_exsit_db(char *path);
		static int str_to_int(char *arg);
		static int power_operation(int ary,int th);
		static int FilePathToInfo(char *path,mediainfo& info);
	private:
		sqlite3 * db;
		char *scanPath;
		int mCurrentTimes;
		Thread mScannerThread;
		Mutex	mMutex;
	};

	class ScanInfo{
		public:
			mediaprovider* media;
			char path[255];
			ScanInfo(){log_i("new ScanInfo")}
	};
#if 0
	class SkiaTest{
		
		SkBitmap bp;
		void decode(){
			SkImageDecoder::DecodeFile("aa",&bp);
		}
		
	};
#endif
extern mediaprovider gmediaprovider;

};
