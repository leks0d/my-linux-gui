#include "player.h"
#define DECODE_IMG 0
#define SQLITE_LOG 0

namespace mango
{

	static void strlwr(char *string);
	static char * getfiletype(char *file);
	static char* getstr(char *arg);
	
	static int _sql_callback(void * use, int argc, char ** argv, char ** szColName)
	{
		int i;
		Musicdbinfo **ppt,*pt,*data;
		
		ppt = (Musicdbinfo **)use;
		pt = *ppt;
		data = (Musicdbinfo *)malloc(sizeof(Musicdbinfo));
		memset(data,0,sizeof(Musicdbinfo));
#if SQLITE_LOG
		log_i("data = %d",data);
#endif
		data->info.id = mediaprovider::str_to_int(*argv++);*szColName++;
		data->info.path = getstr(*(argv++));*szColName++;
		data->info.name = getstr(*(argv++));*szColName++;
		data->info.name_key = getstr(*(argv++));*szColName++;
		data->info.title = getstr(*(argv++));*szColName++;
		data->info.title_key = getstr(*(argv++));*szColName++;
#if SQLITE_LOG
		log_i("artist argv = %s,szColName=%s",*argv,*szColName);
#endif
		data->info.artist = getstr(*(argv++));
		data->info.artist_key = getstr(*(argv++));
		data->info.album = getstr(*(argv++));	
		data->info.album_key = getstr(*(argv++));
		data->info.genre = getstr(*(argv++));
		data->info.genre_key = getstr(*(argv++));
		data->info.track = mediaprovider::str_to_int(*(argv++));
		data->info.img_path = getstr(*(argv++));
		data->info.add_time = mediaprovider::str_to_int(*(argv++));
		data->info.duration = mediaprovider::str_to_int(*(argv++));
		data->info.inPlay = mediaprovider::str_to_int(*(argv++));
		data->info.times = mediaprovider::str_to_int(*(argv));
		data->next = 0;
		
#if SQLITE_LOG
		log_i("*ppt = %d",*ppt);
#endif

		if((*ppt) == 0){		
			(*ppt) = data;			
		}else{
			while(pt->next != 0){
				pt = pt->next;
			}
			pt->next = data;			
		}
		
#if SQLITE_LOG
		log_i("_sql_callback return.");
#endif
		return 0;
	}

	static void relese_db(Musicdbinfo *db){
		Musicdbinfo *pt;
		log_i("relese_db");
		pt =  db;
		while(db != 0){
			pt = db;
			db = db->next;
			free(pt);
		}
	}
	static void strdelspa(char *str){
		int i,len,count;
		return;
		len = strlen(str);
		if(len<=2)
			return;
		for(i = len-2;i>0;i--){
			//log_i("strdelspa str[%d]=%d",i,str[i]);
			if(str[i] != 32){
				count = i;
				break;
			}
		}
		str[count+1] = '\0';
	};
	static void getFileParentName(char *path,char *name ){
		int i,len,start,end;
		
		len = strlen(path);
		start = end = -1;
		for(i=len-1;i>=0;i--){
			if(path[i] == '/'){
				if(end == -1){
					end = i;
				}else if(start == -1){
					start = i+1;
					break;
				}
			}
		}
		if(start>=0&&end>0&&end-start>0){
			memcpy(name,&path[start],end-start);
		}
	}
	int mediaprovider::str_to_int(char *arg){
		/*int value,len,i;
		char ch;
		value = 0;
		
		len = strlen(arg);

		for(i=len-1;i>=0;i--){
			ch = arg[i];
			if(ch>='0'&&ch<='9'){
				value+=(ch-48)*power_operation(10,len-1-i);				
			}else{
				//log_e("str_to_int error arg[%d]=%c",i,arg[i]);
				return -1;
			}
		}*/
		if(arg == NULL)
			return 0;
		int value;
		sscanf(arg,"%d\n",&value);
		return value;		
	}
	static char* getstr(char *arg){
		int len;
		char *str;
		
		len = strlen(arg)+1;
		str = (char*)malloc(len);
		memcpy(str,arg,len);
		
		return str;
	}

	static void str_cpy(char *pt,char *val){
		int len = strlen(val);
		pt = new char[len+1];
		memcpy(pt,val,len+1);
		log_i("str_cpy:%s",pt);
	}
	char* mediaprovider::slqFormat(char *arg){
		int i,len;
		char *ptr,*src,*des;
		char ch = '\'';
		
		if(arg){
			src = arg;
			
			len = strlen(src);
			des = ptr = new char[len*2];
			memset(ptr,0,len*2);
			
			while(*src){
				
				if(*src == '\''){
					*ptr++ = *src;
					*ptr = *src;
				}else{
					*ptr = *src;
				}
				
				ptr++;src++;
			}
		}
		return des;
	}
	char* mediaprovider::slqFormatOut(char *arg,char *out){
		int i,len;
		char *ptr,*src,*des;
		char ch = '\'';
		
		if(arg){
			src = arg;
			ptr = out;
			memset(ptr,0,len*2);
			
			while(*src){
				
				if(*src == '\''){
					*ptr++ = *src;
					*ptr = *src;
				}else{
					*ptr = *src;
				}
				
				ptr++;src++;
			}
			*ptr = *src;
		}
		return 0;
	}

	char* mediaprovider::slqCheck(char *arg){
		int i,len;
		char *src;
		return 0;
		if(arg){
			src = arg;	
			while(*src){		
				if(*src == '\''){
					*src = '\`';
				}
				src++;
			}
		}
		
	}
	
	int mediaprovider::power_operation(int ary,int th){
		int value ,i;
		value = 1;
		if(th == 0)
			return 1;
		else{
			for(i=0;i<th;i++)
				value*=ary;
		}
		return value;
			
	}
	static void strlwr(char *string){
		int wid = 'a' - 'A';
		while( (*string) != '\0'){
			if(*string <= 'Z' && *string >= 'A')
				*string += wid;
			string++;
		}
	}
	static void strlwr(char *string,char *out){
		int wid = 'a' - 'A';
		if(string == NULL || out == NULL)
			return;
		while( (*string) != '\0'){
			if(*string <= 'Z' && *string >= 'A')
				*out = *string + wid;
			else
				*out = *string;
			string++;
			out++;
		}
		*out = *string;
	}

	static char * getfilename(char *path){
		int i,len = 0;
		if(path == 0)
			return 0;
		len = strlen(path);
		for(i = len-1 ; i > 0 ; i--){
			if(*(path+i) == '/' )
				return path+i+1;
		}
	}	
	static char * getfiletype(char *file){
		int i,len = 0;
		char type[10];
		if(file == 0)
			return 0;
		len = strlen(file);
		i = len-1;
		while(*(file+i) != '.'){
			//log_i("*(file+i)=%c\n",*(file+i))
			i--;
			if(i==0)
				break;
		}
		if(len-i>5||i == 0)
			return 0;
		strcpy(type,file+i+1);
		strlwr(type);
		return type;
	}
	static void getFileTitle(char *file,char *title){
		char *temp;
		int i,len;
		
		len = strlen(file);
		temp = new char[len+1];
		
		memcpy(temp,file,len+1);

		for(i=len; i>=0; i--){
			if(temp[i] == '.'){
				temp[i] = '\0';
				break;
			}
		}
		memcpy(title,temp,i+1);
		delete temp;
		log_i("getFileTitle title=%s",title);
	}

	int mediaprovider::music_exsit_db(char *path){
		mediainfo *infolist;
		int count,i;
		char *ptr,sql[1024];
		char sqlPath[300];

		slqFormatOut(path,sqlPath);
		ptr = sql;
		ptr += sprintf(ptr,"path = '%s'",sqlPath);
		count = querymusic(sql,&infolist);

		if(count>0)
			free(infolist);
		
		return count;
	}
	void mediaprovider::mTimesSync(){
		char *ptr,sql[1024];
		ArrayMediaInfo array;
		int i,count;

		mCurrentTimes = 0;
		
		ptr = sql;
		ptr += sprintf(ptr,"order by times desc limit 1");
		
		queryMusicArray(sql,&array);
		
		if(array.getCount()>=1)
			mCurrentTimes = array.getMediaInfo(0)->times+1;
	}
	void mediaprovider::albumImageSync(){
		char *ptr,sql[1024];
		ArrayMediaInfo array;
		int i,count;
		
		ptr = sql;
		ptr += sprintf(ptr,"group by img_path");
		
		queryMusicArray(sql,&array);

		count = array.getCount();
		
		for(i=1;i<count;i++){
			mediainfo *info=array.getMediaInfo(i);
			
			memset(sql,0,1024);
			ptr = sql;
			
			ptr += sprintf(ptr,"update music set img_path='%s'\
				where album='%s' and artist='%s' and img_path='(null)'",
				info->img_path,info->album,info->artist);
			
			exec(sql,0,0);
		}
	}
	mediaprovider::mediaprovider(void)
	{
		db = 0;
		scanPath = NULL;
		mCurrentTimes = 0;
	}

	void mediaprovider::externFileScanner(char *filepath){
		ScanInfo *info;

		info = new ScanInfo();
		info->media = this;
		strcpy(info->path,filepath);

		mScannerThread.create(mediaprovider::FileScannerRunnig, info);
	}
	void mediaprovider::externFileCheck(){
		ScanInfo *info;

		info = new ScanInfo();
		info->media = this;
		//info->path = "(null)";

		mFileCheckThread.create(mediaprovider::FileCheckRunnig, info);
	}
	unsigned int mediaprovider::FileScannerRunnig(void *parameter){
			ScanInfo *info = (ScanInfo*)parameter;
			mediaprovider *media = info->media;
			char *path = info->path;
			
			media->sendMsgStart();
			
			media->mTimesSync();
			media->ScannerDirectory(path);
			media->albumImageSync();
			
			media->sendMsgEnd();
				
			return 0;		
	}
	unsigned int mediaprovider::FileCheckRunnig(void *parameter){
		ScanInfo *info = (ScanInfo*)parameter;
		mediaprovider *media = info->media;
		
		media->sendMsgStart();
		media->checkfile();
		//Thread::sleep(2000);
		media->sendMsgEnd();
	}
	void mediaprovider::ScannerDirectory(char* file){
		ArrayMediaInfo array;
		mediainfo info;
		int count,i;
		memset(&info,0,sizeof(mediainfo));
		count = MediaView::getArrayInfoFromFile(file,array);
		
		mMutex.lock();
		
		for(i=0;i<count;i++){
			char *dirpath;
			dirpath = array.getMediaInfo(i)->path;
			if(!music_exsit_db(dirpath)){
				log_i("getmediainfo info");
				getmediainfo(array.getMediaInfo(i)->path,&info);
				log_i("inster info");
				insert("music",&info);
				log_i("safefreeMediainfo info");
				safefreeMediainfo(&info);
			}
		}
		safefreeMediainfo(&info);
		mMutex.unlock();
	}

	int mediaprovider::mediascanner(char *path)
	{
		mMutex.lock();
		log_i("-------------mediascanner checkfile %s",path);
		checkfile();
		log_i("-------------mediascanner filescanner %s",path);
		filescanner(path);
		log_i("-------------mediascanner albumImageSync");
		albumImageSync();
		log_i("-------------mediascanner end %s",path);
		mMutex.unlock();
	}
	int mediaprovider::externVolumeScanner(char *path){
		ScanInfo *info;
		char *file;
		
		sendMsgStart();
		
		info = new ScanInfo();
		info->media = this;
		file = info->path;
		strcpy(file,path);
		
		mScannerThread.create(mediaprovider::VolumeScannerRunnig, info);
	}
	
	unsigned int mediaprovider::VolumeScannerRunnig(void *parameter){
			ScanInfo *info = (ScanInfo*)parameter;
			mediaprovider *media = info->media;
			char *path = info->path;

			media->mediascanner(path);
			
			media->sendMsgEnd();
				
			return 0;
	}
	int mediaprovider::sendMsgStart(){
		log_i("sendMsgStart:show MediaScannerView");
		//gPlayer.showMediaScannerView();
		//scanTime = Time::getMillisecond();
		gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,MEDIA_SCANNER_START,0);
	}
	int mediaprovider::sendMsgEnd(){
		log_i("sendMsgEnd:dismissView MediaScannerView");
		//gPlayer.dismissView(gPlayer.mMediaScannerView);
		//int dur = Time::getMillisecond() - scanTime;
		//if(dur<50)
		//	mango::Thread::sleep(500);
		gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,MEDIA_SCANNER_END,0);
	}
	int mediaprovider::getmediainfo(char *path,mediainfo *info)
	{
		ID3INFO m_id3(path);
		char *value;
		char *filename;
		int len,charCount;
		TCHAR	fileName[255];
		log_i("tag");		
		value = (char*)malloc(256);
		
		filename = getfilename(path);
		info->times = mCurrentTimes + 1;
		
		len = strlen(path)+1;
//**************************
		//info->path = new char[len];
		//memcpy(info->path,path,len);
//-----------------------------
		info->path = new char[len*2];
		slqFormatOut(path,info->path);
//**************************

		len = strlen(filename)+1;
//**************************
		//info->name = new char[len];
		//memcpy(info->name,filename,len);
//-----------------------------
		info->name = new char[len*2];
		slqFormatOut(filename,info->name);
//**************************

		
		info->name_key = new char[strlen(info->name)+1];
		strlwr(info->name,info->name_key);
		
		info->inPlay = 0;

		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_CD_TRACK_NUMBER, value)){
			info->track = str_to_int(value);
		}
/*------------------------------album---------------------------------------*/		
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ALBUM, value)){

		}else{
			getFileParentName(path,value);
			log_i("id3 album is null;getname=%s",value);
		}
		//slqCheck(value);
		len = strlen(value)+1;
		info->album = new char[len];
		memcpy(info->album,value,len);

		info->album_key = new char[len];
		strlwr(value,info->album_key);	
/*------------------------------album---------------------------------------*/				
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ARTIST, value)){
			strdelspa(value);
			len = strlen(value)+1;
			
			info->artist = new char[len];
			memcpy(info->artist,value,len);

			info->artist_key = new char[len];
			strlwr(value,info->artist_key);			
		}
		
/*------------------------------title---------------------------------------*/		
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_TITLE, value)){
			/*strdelspa(value);
			len = strlen(value)+1;
			
			info->title = new char[len];
			memcpy(info->title,value,len);

			info->title_key = new char[len];
			strlwr(value,info->title_key);*/
			log_i("title gettag:title=%s",value);
		}else{
			log_i("title gettag fail");
			getFileTitle(info->name,value);
		}
		len = strlen(value)+1;
			
		info->title = new char[len];
		memcpy(info->title,value,len);

		info->title_key = new char[len];
		strlwr(value,info->title_key);
/*--------------------------------title---------------------------------------------*/
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_GENRE, value)){
			log_i("title gettag:title=%s",value);
		}else{
			log_i("title gettag fail");
			sprintf(value,"(null)");
		}
		len = strlen(value)+1;
			
		info->genre = new char[len];
		memcpy(info->genre,value,len);

		info->genre_key = new char[len];
		strlwr(value,info->genre_key);

/*--------------------------------genre--------------------------------------------*/

		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_DURATION, value)){
			info->duration = str_to_int(value);
		}
#if 0
		if(m_id3.PicValid() >= 0)//save picture
		{
			log_i("m_id3.PicValid() m_id3.piclength=%d",m_id3.piclength);
			
			if(m_id3.piclength > 0){
				char *imgPath,*fileTitle;
#if DECODE_IMG
				SkBitmap *skBitmap;
				
				skBitmap = new SkBitmap();	
				
				bool ret = SkImageDecoder::DecodeMemory(m_id3.picdata,m_id3.piclength,skBitmap,
					SkBitmap::kRGB_565_Config,SkImageDecoder::kDecodePixels_Mode);
			
				if(ret){		
					log_i("skBitmap->width()=%d,skBitmap->height()=%d",skBitmap->width(),skBitmap->height());
					int len = skBitmap->width()*skBitmap->height()*skBitmap->bytesPerPixel();
#else
					int len = m_id3.piclength;
#endif						
					FILE *fb;

					imgPath = new char[255];
					fileTitle = new char[strlen(info->name)];
					
					getFileTitle(info->name,fileTitle);
					genImgPath(fileTitle,imgPath);
					
					
					fb = fopen(imgPath, "wb");
					if(fb>0){
#if DECODE_IMG							
						int ret = fwrite((unsigned char *)skBitmap->getPixels(),1,len,fb);
#else
						int ret = fwrite((unsigned char *)m_id3.picdata, 1, m_id3.piclength, fb);
#endif						
						if(ret == len){
							int pathlen = strlen(imgPath);
							info->img_path = new char[pathlen+1];
							memcpy(info->img_path,imgPath,pathlen+1);
							log_i("album img save sucess.");
						}else
							log_i("fwrite fail,ret=%d",ret);
						
						fclose(fb);
					}else{
						log_i("fopen %s fail.",imgPath);
					}

					
					delete imgPath;
					delete fileTitle;
#if DECODE_IMG					
				}else{
					log_i("SkImageDecoder::DecodeMemory fail.");
				}
#endif				
			}
		}else{
			log_i("m_id3 not PicValid()");
		}
#else
		if(m_id3.PicValid() >= 0){
			if(m_id3.piclength > 0){
				char *imgPath,*fileTitle;
				MSkBitmap mMSkBitmap;
				
				BitmapFactory::decodeBuffer(&mMSkBitmap,(void*)m_id3.picdata,m_id3.piclength,109,109);

				imgPath = new char[255];
				fileTitle = new char[strlen(info->name)];
					
				getFileTitle(info->name,fileTitle);
				genImgPath(fileTitle,imgPath);
					
				int ret = mMSkBitmap.saveToFile(imgPath);

				if(ret == 1){
					int pathlen = strlen(imgPath);
					info->img_path = new char[pathlen+1];
					memcpy(info->img_path,imgPath,pathlen+1);
					log_i("album img save sucess.");
				}else
					log_i("fwrite fail,ret=%d",ret);				
			}
		}
#endif
		delete value;
		return 0;
	}

	void mediaprovider::genImgPath(char *title,char *path)
	{	
		int inf = 0;
		unsigned int ti;
		char *ptr;
		
		ti = (unsigned int)Time::getMillisecond();
		ptr = path;

		while(1){
			sprintf(ptr,"%s/%s_%d_%d",IMG_PATH,title,ti,inf);
			if(FileAttr::FileExist(path)){
				inf++;
				memset(ptr,0,strlen(ptr));
			}else{
				log_i("genImgPath:%s",path);
				break;
			}
		}
	}
	int mediaprovider::FilePathToInfo(char *path,mediainfo& info){
		char* name;					
							
		memset(&info,0,sizeof(mediainfo));
		info.id = -1;
		
		name = getfilename(path);
							
		info.name = new char[strlen(path)+1]; 
		info.path = new char[strlen(path)+1];
		info.title = new char[strlen(path)+1];
		
		memcpy(info.name,name,strlen(name)+1);
		memcpy(info.title,name,strlen(name)+1);
		memcpy(info.path,path,strlen(path)+1);

		log_i("info.title=0x%x",info.title);
	}
	int mediaprovider::filescanner(char *path)
	{	
		int		count = 0;
		char direct[255];
		int     fileAttribute;
		File    file;
		mediainfo info;
		String str;
		DIR* d;
		struct dirent* de;

		memset(&info,0,sizeof(mediainfo));
		//log_i("opendir DT_DIR :%s\n",path);
		d = opendir(path);

		while((de = readdir(d)) != NULL)
		{
			int name_len = strlen(de->d_name);
			
			strcpy(direct,path);
			strcat(direct,"/");
			strcat(direct,de->d_name);
			//log_i("scan DT_DIR=%d :%s,de->d_type=%d",DT_DIR,direct,de->d_type);
			
//			if (de->d_type == DT_DIR || de->d_type == 0 ) {	
			if(File::isDirect(direct)){
#if 0				
				if (name_len == 1 && de->d_name[0] == '.') continue;
            	if (name_len == 2 && de->d_name[0] == '.' &&
                de->d_name[1] == '.') continue;
#endif
				if (de->d_name[0] == '.'){
					//log_i("scan DT_DIR continue");
					continue;
				}
				//if(File::isDirect(direct))
				filescanner(direct);
				count++;
			}
			else if(ismusic(de->d_name)&&(de->d_name[0]!='.')&&(!music_exsit_db(direct)))
			{

				getmediainfo(direct,&info);
				insert("music",&info);
				safefreeMediainfo(&info);
				count++;
			}

		}
		safefreeMediainfo(&info);
		closedir(d);	
		
		return 0;
	}
	int mediaprovider::checkfile(){
		mediainfo *infolist;
		int count,i;
		
		mCurrentTimes = 0;
		
		count = querymusic(0,&infolist);
		
		for(i=0;i<count;i++){
			
			//log_i("checkfile exsit:%s",infolist[i].path);
			
			if(access(infolist[i].path,F_OK) != 0){
				int id = infolist[i].id;
				
				//remove(infolist[i].img_path);
				del("music",id);
				
				log_i("del file in db:%s",infolist[i].path);
				
			}else if(infolist[i].times > mCurrentTimes){
				mCurrentTimes = infolist[i].times;
			}
		}
		if(count>0)
			free(infolist);

		if(!FileAttr::FileExist(IMG_PATH)){
			
			if(mkdir(IMG_PATH,0777)){
				log_i("Create file fail:%s",IMG_PATH);
			}else
				log_i("Create file sucess:%s",IMG_PATH);
		}else
			log_i("FileExist:%s",IMG_PATH);
		
		log_i("mCurrentTimes = %d",mCurrentTimes);
		
		return 0;
	}

	
	int mediaprovider::initialize(void){
		int ret = 0;
		char *pErrMsg = 0;
		
		ret = sqlite3_open(TABLE_PATH,&db);
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec open error path: %s\n",TABLE_PATH);
			return ret;
		}
		
		ret = sqlite3_exec( db, MUSIC_TABLE_CREATE, 0, 0, &pErrMsg );
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",MUSIC_TABLE_CREATE);			
		}

		PlayList::createTable();
			
		return ret;
	}

	int mediaprovider::exec(char *sql,void *arg,int (*callback)(void*,int,char**,char**))
	{
		int ret = 0;
		char *pErrMsg = 0;
		
		if(db == 0)
			return SQLITE_ERROR;
		
		ret = sqlite3_exec( db, sql, callback, arg, &pErrMsg );
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s",sql);
			log_e("sqlite3_exec pErrMsg : %s",pErrMsg);
		}else
			//log_i("sqlite3_exec success : %s",sql);
			;
		return ret;	
	}

	int mediaprovider::insert(char *table,mediainfo *info)
	{
		char *ptr,sql[1024];
		log_i("tag");
		ptr = sql;
		ptr += sprintf(ptr,"insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",
			table,MUSIC_PTAH,MUSIC_NAME,MUSIC_NAME_KEY,MUSIC_TITLE,MUSIC_TITLE_KEY,MUSIC_ART,MUSIC_ART_KEY,
			MUSIC_ALBUM,MUSIC_ALBUM_KEY,"genre","genre_key",MUSIC_TRACK,MUSIC_ART_IMG,MUSIC_ADD_TIME,MUSIC_DURATION,MUSIC_IN_PLAY,MUSIC_TIMES);
		log_i("tag");
		ptr += sprintf(ptr,"values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s','%d','%d','%d','%d');",
				info->path,info->name,info->name_key,info->title,info->title_key,info->artist,
				info->artist_key,info->album,info->album_key,info->genre,info->genre_key,info->track,info->img_path,
				info->add_time,info->duration,info->inPlay,info->times);
		log_i("tag");
		exec(sql,0,0);

		return 0;
	}

	int mediaprovider::updateInPlay(int value,int id){
		char *ptr,sql[1024];
		
		ptr = sql;
		if(id>0)
			ptr += sprintf(ptr,"update music set inplay=%d where _id=%d",value,id);
		else
			ptr += sprintf(ptr,"update music set inplay=%d",value);

		return exec(sql,0,0);
	}
	
	int mediaprovider::querymusic(char *where, mediainfo **minfo)
	{
		char *ptr,sql[1024];
		Musicdbinfo *info,*pt;
		mediainfo *infolist;
		int count = 0;
		
		info = 0;
		ptr = sql;
		
		ptr += sprintf(ptr,"select * from music ");
		if(where!=0)
			ptr += sprintf(ptr,"where %s;",where);
		
		exec(sql,&info,_sql_callback);

		pt = info;
		
		if(pt == 0)
			return 0;
		else{
			count++;
			while(pt->next!=0){				
				pt = pt->next;
				count++;
			}			
		}

		infolist = (mediainfo*)malloc(sizeof(mediainfo)*count);
		
		pt = info;
		count = 0;
		if(pt == 0)
			return 0;
		else{
			infolist[count] = pt->info;
			count++;
			while(pt->next!=0){	
				//log_i("pt->info.path=%s,",pt->info.path);
				pt = pt->next;
				infolist[count] = pt->info;				
				count++;
			}
			//log_i("pt->info.path=%s",pt->info.path);
		}
		log_i("infolist count=%d",count);
		if(minfo!=0)
			*minfo = infolist;
//		relese_db(info);
		return count;
	}

		int mediaprovider::queryMusicArray(char *where, void *array)
		{
			char *ptr,sql[1024];
			Musicdbinfo *info,*pt;
			ArrayMediaInfo *arraylist;
			int count = 0;
			
			//log_i("-------queryMusicArray");
			
			arraylist = (ArrayMediaInfo*)array;
			info = 0;
			ptr = sql;
			
			ptr += sprintf(ptr,"select * from music ");
			if(where!=0)
				ptr += sprintf(ptr,"%s;",where);

			log_i("queryMusicArray:sql=%s",sql);
			
			exec(sql,&info,_sql_callback);
	
			pt = info;

			//log_i("exec complete");
			if(pt == 0){
				return 0;
			}else{
				arraylist->addMediaInfo(&(pt->info));
				count++;	
				while(pt->next!=0){
					
					pt = pt->next;
					arraylist->addMediaInfo(&(pt->info));
					count++;
				}			
			}
			
			relese_db(info);
			log_i("MusicArray count=%d",count);
			return count;
		}
		int mediaprovider::queryArrayMedia(char *where, void *array)
		{
			char *ptr,sql[1024];
			Musicdbinfo *info,*pt;
			ArrayMediaInfo *arraylist;
			int count = 0;
			
			//log_i("-------queryMusicArray");
			
			arraylist = (ArrayMediaInfo*)array;
			info = 0;
			ptr = sql;
			
			//ptr += sprintf(ptr,"select * from music ");
			if(where!=0)
				ptr += sprintf(ptr,"%s;",where);

			log_i("queryMusicArray:sql=%s",sql);
			
			exec(sql,&info,_sql_callback);
	
			pt = info;

			//log_i("exec complete");
			if(pt == 0){
				return 0;
			}else{
				arraylist->addMediaInfo(&(pt->info));
				count++;	
				while(pt->next!=0){
					
					pt = pt->next;
					arraylist->addMediaInfo(&(pt->info));
					count++;
				}			
			}
			
			relese_db(info);
			log_i("MusicArray count=%d",count);
			return count;
		}

	
	int mediaprovider::del(char *table,int id){
		char *ptr,sql[256];
		ptr = sql;
		ptr += sprintf(ptr,"delete from music where _id=%d",id);
		
		exec(sql,0,0);
		
		PlayList::delAudioFromPlaylist(id);
		
		return 0;
	}
	mediaprovider::~ mediaprovider(void)
	{
		db = 0;
		
	}
	mediaprovider gmediaprovider;
};

