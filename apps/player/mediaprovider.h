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
									genre TEXT,\
									genre_key TEXT,\
									track TEXT,\
									img_path TEXT,\
									add_time INTEGER,\
									duration INTEGER,\
									inplay INTEGER,\
									times INTEGER\
									);"
	#define IMG_PATH "/mnt/sdcard/.album_img"
	
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
	}mediainfo;

	static void safedelete(char *str){
		if(str != NULL){
			//log_i("str=0x%x,%s",str,str);
			delete str;
			str = NULL;
		}
	}
	static void safefreeMediainfo(mediainfo *info){
		if(info!=NULL){
			//log_i("enter free mediainfo");
			safedelete(info->path);
			safedelete(info->name);
			safedelete(info->name_key);
			safedelete(info->title);
			safedelete(info->title_key);
			safedelete(info->artist);
			safedelete(info->artist_key);
			safedelete(info->album);
			safedelete(info->album_key);
			safedelete(info->img_path);
			memset(info,0,sizeof(mediainfo));
			//log_i("leave free mediainfo");
		}
	}

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
		static unsigned int VolumeScannerRunnig(void *parameter);
		static unsigned int FileScannerRunnig(void *parameter);
		static unsigned int FileCheckRunnig(void *parameter);
		void externFileCheck();
		void externFileScanner(char *filepath);
		void ScannerDirectory(char* file);
		void albumImageSync();
		void mTimesSync();
		int initialize(void);
		int exec(char *sql,void *arg,int (*callback)(void*,int,char**,char**));
		int insert(char *table,mediainfo *info);
		int querymusic(char *where,mediainfo **info);
		int queryMusicArray(char *where, void* array);
		int queryArrayMedia(char *where, void *array);
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
		int scanTime;
		Thread mScannerThread;
		Thread mFileScanThread;
		Thread mFileCheckThread;
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
static char * getfiletype(char *file);

static int ismusic(char *file)
{
	char *type;
	const char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","m4a","wma","aif","aiff","\0"};
	const char **mtype;
	mtype = music_type;
	//log_i("getfiletype filename:%s\n",file);
	type = getfiletype(file);
	//log_i("getfiletype :%s\n",type);
	if(type == 0)
		return 0;
	while(*mtype!="\0"){
		if(strcmp(*mtype,type) == 0)
			return 1;
		mtype++;
	}
	return 0;
}


extern mediaprovider gmediaprovider;

};
