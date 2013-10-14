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
									track INTEGER,\
									img_path TEXT,\
									add_time INTEGER,\
									duration INTEGER,\
									inplay INTEGER,\
									times INTEGER,\
									iscue INTEGER,\
									cuestart INTEGER\
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
		int isCue;
		int cueStart;
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
	static void mediainfocpy(mediainfo *src,mediainfo& des){
		if(src != NULL){
			
			memset(&des,0,sizeof(mediainfo));
			
			des.id = src->id;
			des.track = src->track;
			des.add_time = src->add_time;
			des.duration = src->duration;
			des.isInPlayList = src->isInPlayList;
			des.inPlay = src->inPlay;
			des.isPlayed = src->isPlayed;
			des.times = src->times;
			
			if(src->path == NULL)
				return;
			
			des.path = new char[strlen(src->path)+1];
			strcpy(des.path,src->path);
			
			if(src->name != NULL){
				des.name = new char[strlen(src->name)+1];
				strcpy(des.name,src->name);
			}
			
			if(src->name_key != NULL){
				des.name_key = new char[strlen(src->name_key)+1];
				strcpy(des.name_key,src->name_key);
			}
			
			if(src->title != NULL){
				des.title = new char[strlen(src->title)+1];
				strcpy(des.title,src->title);
			}
			
			if(src->title_key != NULL){
				des.title_key = new char[strlen(src->title_key)+1];
				strcpy(des.title_key,src->title_key);
			}
			
			if(src->artist != NULL){
				des.artist = new char[strlen(src->artist)+1];
				strcpy(des.artist,src->artist);
			}
			
			if(src->artist_key != NULL){
				des.artist_key = new char[strlen(src->artist_key)+1];
				strcpy(des.artist_key,src->artist_key);
			}
			
			if(src->album != NULL){
				des.album = new char[strlen(src->album)+1];
				strcpy(des.album,src->album);
			}
			
			if(src->album_key != NULL){
				des.album_key = new char[strlen(src->album_key)+1];
				strcpy(des.album_key,src->album_key);
			}
			
			if(src->img_path != NULL){
				des.img_path = new char[strlen(src->img_path)+1];
				strcpy(des.img_path,src->img_path);
			}

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
		int getmediainfo(char *path,mediainfo *info,CString& cover,CString& genImg);
		static unsigned int VolumeScannerRunnig(void *parameter);
		static unsigned int FileScannerRunnig(void *parameter);
		static unsigned int FileCheckRunnig(void *parameter);
		void externFileCheck();
		void externFileScanner(char *filepath);
		void ScannerDirectory(char* file);
		void albumImageSync();
		void mTimesSync();
		bool cueCheck(char *direct,mediainfo *info);
		void getCuePath(char* src,char * out);
		bool loadCueFile(char* path,mediainfo *info);
		void strCopy(char *des,char *out);
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
		void getWakeLock();
		void releaseWakeLock();
		void stopMediaScanner();
		bool mediaCanStop();
		int queryMusicNum();
		static char* slqFormatOut(char *arg,char *out);
		static char* slqFormat(char *arg);
		static char* slqCheck(char *arg);
		static int str_to_int(char *arg);
		static int power_operation(int ary,int th);
		static int FilePathToInfo(char *path,mediainfo& info);
		
	private:
		sqlite3 * db;
		char *scanPath;
		bool isWakeLock;
		int mCurrentTimes;
		int scanTime;
		bool scannerStop;
		bool scanCanStop;
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
static char * getfiletype(char *file,char *type);
static int fileTypeMatch(char *file,const char *music_type[])
{
	char type[10];
//	const char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","m4a","wma","aif","aiff","\0"};
	const char **mtype;

	mtype = music_type;

	getfiletype(file,type);

	if(type == 0)
		return 0;

	while(*mtype != "\0"){
		if(strcmp(*mtype,type) == 0){
			return 1;
		}
		mtype++;
	}

	return 0;
}

static int ismusic(char *file)
{
#if 0
	char type[10];
	const char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","m4a","wma","aif","aiff","\0"};
	const char **mtype;
	mtype = music_type;
	//log_i("getfiletype filename:%s\n",file);
	getfiletype(file,type);
	//log_i("getfiletype :%s\n",type);
	if(type == 0)
		return 0;
	while(*mtype!="\0"){
		if(strcmp(*mtype,type) == 0)
			return 1;
		mtype++;
	}
	return 0;
#else
	const char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","m4a","wma","aif","aiff","\0"};
	return fileTypeMatch(file,music_type);
#endif		
}
static int isCueFile(char *file)
{
#if 0
	char *type;
	const char *music_type[] = {"cue","\0"};
	const char **mtype;
	mtype = music_type;

	type = getfiletype(file);

	if(type == 0)
		return 0;
	
	while(*mtype != "\0"){
		
		if(strcmp(*mtype,type) == 0)
			return 1;
		
		mtype++;
	}
	
	return 0;
#endif	
	const char *music_type[] = {"cue","\0"};
	return fileTypeMatch(file,music_type);
}
static int isPictureFile(char* file){
	const char *music_type[] = {"jpg","png","bmp","\0"};
	return fileTypeMatch(file,music_type);
}

extern mediaprovider gmediaprovider;

};
