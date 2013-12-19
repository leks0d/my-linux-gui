#include "player.h"

#define DECODE_IMG 0
#define SQLITE_LOG 0

namespace mango
{
	unsigned long t0=0,t1=0,t2=0;

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
		data->info.times = mediaprovider::str_to_int(*(argv++));
		data->info.isCue = mediaprovider::str_to_int(*(argv++));
		data->info.cueStart = mediaprovider::str_to_int(*(argv++));
		data->info.md5 = getstr(*(argv));
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
		
		if(path!=NULL){
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
		int value,ret;
		ret = sscanf(arg,"%d\n",&value);
		if(ret == 0)
			value = 0;
		return value;
	}
	static char* getstr(char *arg){
		int len;
		char *str;
		
		if(arg == NULL){
			len = 7;
			str = (char*)malloc(len);
			memcpy(str,"(null)",len);
		}else{
			len = strlen(arg)+1;
			str = (char*)malloc(len);
			memcpy(str,arg,len);
		}
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
		
		if(arg){
			src = arg;
			while(*src){
				if(*src == '\''){
					*src = '\`';
				}
				src++;
			}
		}
		return 0;
	}
	
	int mediaprovider::power_operation(int ary,int th){
		int value ,i;
		value = 1;
		
		if(th == 0){
			return 1;
		}else{
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
	static char * getfilename(char *path,char *out){
		int i,len = 0;
		
		if(path == 0)
			return 0;
		
		len = strlen(path);
		
		for(i = len-1 ; i > 0 ; i--){
			if(*(path+i) == '/' )
				break;			
		}

		strcpy(out,path+i+1);
		return 0;
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
	static char * getfiletype(const char *file,char *type){
		int i,len = 0;
		//char type[10];
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
		
		if(file == NULL)
			return;
		
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
		CString md5;
		int count,i;
		char *ptr,sql[1024];
		char sqlPath[300];
		
		ptr = sql;
#if 1
		Environment::MD5(path,md5);
		ptr += sprintf(ptr,"md5 = '%s'",md5.string);
#else
		slqFormatOut(path,sqlPath);
		ptr += sprintf(ptr,"path = '%s'",sqlPath);
#endif

		count = querymusic(sql,&infolist);
		if(count>0)
			free(infolist);
		return count;
	}
	void mediaprovider::mTimesSync(){
		char *ptr,sql[256];
		IntegerArray array;
		
		ptr = sql;
		ptr += sprintf(ptr,"select times from music order by times desc limit 1");
		
		exec(sql,&array,SettingProvider::sql_callback);

		mCurrentTimes = array.getItem(0) + 1;
	}
	void mediaprovider::albumImageSync(){
		char *ptr,sql[4024];
		ArrayMediaInfo array;
		int i,count;
		
		ptr = sql;
		ptr += sprintf(ptr,"group by img_path");
		
		queryMusicArray(sql,&array);

		count = array.getCount();
		
		for(i=1;i<count;i++){	//'(null)' is first,so start at 1.
			mediainfo *info = array.getMediaInfo(i);
			
			memset(sql,0,4024);
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
		scannerStop = true;
		scanCanStop = false;
		isWakeLock = false;
	}

	void mediaprovider::externFileScanner(char *filepath){
		ScanInfo *info;

		info = new ScanInfo();
		info->media = this;
		strcpy(info->path,filepath);

		mScannerThread.create(mediaprovider::FileScannerRunnig, info);
	}
	unsigned int mediaprovider::FileScannerRunnig(void *parameter){
			ScanInfo *info = (ScanInfo*)parameter;
			mediaprovider *media = info->media;
			char *path = info->path;
			
			media->sendMsgStart();
			
//			media->mTimesSync();
//			media->ScannerDirectory(path);
//			media->albumImageSync();

			media->mediascanner(path,false);
			
			media->sendMsgEnd();
				
			return 0;
	}
	void mediaprovider::ScannerDirectory(char* file){
		ArrayMediaInfo array;
		mediainfo info;
		CString cover,albumImg;
		int count,i;
		
		memset(&info,0,sizeof(mediainfo));
		count = MediaView::getArrayInfoFromFile(file,array,cover);
		
		mMutex.lock();
		
		for(i=0;i<count;i++){
			char *dirpath;
			dirpath = array.getMediaInfo(i)->path;
			if(!music_exsit_db(dirpath)){
				
				getmediainfo(array.getMediaInfo(i)->path,&info,cover,albumImg);
				
				if(!cueCheck(info.path,&info)){
					insert("music",&info);
				}

				safefreeMediainfo(&info);
			}
		}
		safefreeMediainfo(&info);
		mMutex.unlock();
	}

	void mediaprovider::externFileCheck(){
		ScanInfo *info;

		info = new ScanInfo();
		info->media = this;
		strcpy(info->path,SDCARD_PATH);

		mFileCheckThread.create(mediaprovider::FileCheckRunnig, info);
	}
	unsigned int mediaprovider::FileCheckRunnig(void *parameter){
		ScanInfo *info = (ScanInfo*)parameter;
		mediaprovider *media = info->media;
		
		log_i("------------info->path=%s",info->path);
		media->sendMsgStart();
		media->mMutex.lock();
		media->checkfile(info->path,true);
		media->mMutex.unlock();
		media->sendMsgEnd();
	}
#if 0
	int mediaprovider::mediascanner(char *path)
	{
		mMutex.lock();
		log_i("------------- checkfile %s",path);
		checkfile(path);
		log_i("------------- filescanner %s",path);
		scannerStop = false;
		filescanner(path);
		log_i("------------- albumImageSync");
		albumImageSync();
		log_i("------------- end %s",path);
		mMutex.unlock();
	}
#else
	int mediaprovider::mediascanner(char *path,bool recursion)
	{
		int i;
		int progress = 0;		
		unsigned long t0,t1,t2,t3,tt0,tt1,tt2,tt3,tt4;
		unsigned long ts,te,td=0,ts_,te_,td_=0;
		AudioFileArray fileArray;
		SdcardAudioData sdcard(path);

		mMutex.lock();
		
		scannerStop = false;
		
		tt0 = Time::getMicrosecond();
		
		checkfile(path);
		mTimesSync();
		fileArray.startScanFile(path,recursion);

		tt1 = Time::getMicrosecond();
			
		for(i=0;i<fileArray.mLen;i++){
			int p;
			int existSDcard = 0;
			CursorItem curItem;
			
			if(sdcard.existBack&&sdcard.queryFile(curItem,fileArray.mList[i])){
				curItem.setValue("times",mCurrentTimes);
				existSDcard = 1;
			}else{
				//ts = Time::getMicrosecond();
				analyzeAudioID3(curItem,fileArray.mList[i]);
				//te = Time::getMicrosecond();
				//td += te - ts;
			}
			
			//ts_ = Time::getMicrosecond();
			if(!cueCheckCursor(curItem)){
				insertCursorItem(curItem);
				if(existSDcard == 0)
					sdcard.insertCursor(curItem);
			}
			//te_ = Time::getMicrosecond();
			//td_ += te_ - ts_;
			
			p = (i+1)*100/fileArray.mLen;
			
			if(progress<p){
				progress = p;
				sendMsgProgress(progress);
			}
			
			if(scannerStop)
				break;
		}
		
		tt2 = Time::getMicrosecond();
		
		//albumImageSync();
		
		tt3 = Time::getMicrosecond();

		t0 = tt1 - tt0;
		t1 = tt2 - tt1;
		t2 = tt3 - tt2;

		if(fileArray.mLen == 0){
			sendMsgProgress(100);
		}
		log_i("time,t0=%ldms,t1=%ldms,td_=%ldms,td=%ldms",t0/1000,t1/1000,td_/1000,td/1000);
		Thread::sleep(100);
		mMutex.unlock();
	}
#endif
	int mediaprovider::externVolumeScanner(char *path){
		ScanInfo *info;
		char *file;
		
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
			
			media->scanCanStop = true;
			media->sendMsgStart();
			
			media->mediascanner(path,true);
			
			media->sendMsgEnd();
			media->scanCanStop = false;
			
			return 0;
	}
	int mediaprovider::sendMsgStart(){
		log_i("sendMsgStart:show MediaScannerView");
		scanTime = Time::getMillisecond();
		getWakeLock();
		gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,MEDIA_SCANNER_START,0);
		return 0;
	}
	
	int mediaprovider::sendMsgEnd(){
		int dur = Time::getMillisecond() - scanTime;
		log_i("sendMsgEnd:dismissView MediaScannerView,Time:%dms",dur);
		releaseWakeLock();
		gMessageQueue.post(gPlayer.mPlayingView,VM_NOTIFY,MEDIA_SCANNER_END,0);
		return 0;
	}
	int mediaprovider::sendMsgProgress(int progress){
		log_i("progress = %d",progress);
		gMessageQueue.post(gPlayer.mMediaScannerView,VM_NOTIFY,MEDIA_SCANNER_PROGRESS,progress);
	}
	int mediaprovider::analyzeAudioID3(CursorItem& item,AudioFileInfo& info){
//		ID3INFO m_id3(info.path.string);
		AudioId3  m_id3(info.path.string);
		char *value;
		int len = 1024;
		char sqlStr[1024]={0},filename[1024]={0};
		CString md5;
		
		item.addItem("times",mCurrentTimes);
		
		value = (char*)malloc(len);
		
		slqFormatOut(info.path.string,sqlStr);
		item.addItem("path",sqlStr);
		
		Environment::MD5(info.path.string,md5);	
		item.addItem("md5",md5.string);
		
		getfilename(info.path.string,filename);
		slqCheck(filename);

		item.addItem("name",filename);
		
		strlwr(filename,sqlStr);
		item.addItem("name_key",sqlStr);

		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_TITLE, value)){
			slqCheck(value);
		}else{
			getFileTitle(filename,value);
		}
		item.addItem("title",value);
		
		strlwr(value,sqlStr);
		item.addItem("title_key",sqlStr);
		
		item.addItem("inPlay",0);

		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_CD_TRACK_NUMBER, value)){
			item.addItem("track",str_to_int(value));
		}else{
			item.addItem("track",1000);
		}
		
		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_ALBUM, value)){

		}else{
			getFileParentName(info.path.string,value);
		}
		slqCheck(value);
		item.addItem("album",value);
		
		strlwr(value,sqlStr);
		item.addItem("album_key",sqlStr);
		
		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_ARTIST, value)){
		}else{
			sprintf(value,"(null)");
		}
		slqCheck(value);
		item.addItem("artist",value);
		
		strlwr(value,sqlStr);
		item.addItem("artist_key",sqlStr);
		
		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_GENRE, value)){
		}else{
			sprintf(value,"(null)");
		}
		slqCheck(value);
		item.addItem("genre",value);

		strlwr(value,sqlStr);
		item.addItem("genre_key",value);
		
		memset(value, 0, len);
		if (m_id3.GetTags(METADATA_KEY_DURATION, value)){
			
			item.addItem("duration",str_to_int(value));
		}else{
			item.addItem("duration",0);
		}
		if( m_id3.PicValid() >= 0 ){
			CString imgpath;
			MSkBitmap mMSkBitmap;
			
			genMd5ImgPath(md5,imgpath);
			
			if(FileAttr::FileExist(imgpath.string)){
				item.addItem("img_path",imgpath.string);
			}else{
				if(m_id3.piclength > 0)
					BitmapFactory::decodeBuffer(&mMSkBitmap,(void*)m_id3.picdata,m_id3.piclength,109,109);
				
				int ret = mMSkBitmap.saveToFile(imgpath.string);
				if(ret == 1)
					item.addItem("img_path",imgpath.string);
				else
					item.addItem("img_path","(null)");
			}
		}else if(info.cover != NULL){
			CString imgpath,coverMd5;
			MSkBitmap mMSkBitmap;
			
			Environment::MD5(info.cover.string,coverMd5);
			genMd5ImgPath(coverMd5,imgpath);

			if(FileAttr::FileExist(imgpath.string)){
				item.addItem("img_path",imgpath.string);
			}else{
				BitmapFactory::decodeFile(&mMSkBitmap,info.cover.getString(),109,109);
				
				int ret = mMSkBitmap.saveToFile(imgpath.string);
				if(ret == 1)
					item.addItem("img_path",imgpath.string);
				else
					item.addItem("img_path","(null)");
			}
		}else{
			item.addItem("img_path","(null)");
		}

		free(value);
		value = NULL;
	}

	int mediaprovider::getmediainfo(char *path,mediainfo *info,CString& cover,CString& genImg)
	{
		ID3INFO m_id3(path);
		CString md5;
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

		Environment::MD5(path,md5);
		info->md5 = new char[33];
		memset(info->md5,0,33);
		memcpy(info->md5,md5.string,32);
//		log_i("path='%s'",path);
//		log_i("md5=%s",md5.string);
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
		}else{
			info->track = 10000;
		}
/*------------------------------album---------------------------------------*/		
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ALBUM, value)){

		}else{
			getFileParentName(path,value);
			log_i("id3 album is null;getname=%s",value);
		}
		slqCheck(value);
		len = strlen(value)+1;
		info->album = new char[len];
		memcpy(info->album,value,len);

		info->album_key = new char[len];
		strlwr(value,info->album_key);	
/*------------------------------artist---------------------------------------*/				
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ARTIST, value)){
			slqCheck(value);
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
			slqCheck(value);
		}else{
			log_i("title gettag fail");
			getFileTitle(info->name,value);
		}
		len = strlen(value)+1;
			
		info->title = new char[len];
		memcpy(info->title,value,len);

		info->title_key = new char[len];
		strlwr(value,info->title_key);
/*--------------------------------genre---------------------------------------------*/
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_GENRE, value)){
			log_i("title gettag:title=%s",value);
		}else{
			log_i("title gettag fail");
			sprintf(value,"(null)");
		}
		slqCheck(value);
		len = strlen(value)+1;
		
		info->genre = new char[len];
		memcpy(info->genre,value,len);
		
		info->genre_key = new char[len];
		strlwr(value,info->genre_key);
		
/*--------------------------------duration--------------------------------------------*/

		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_DURATION, value)){
			info->duration = str_to_int(value);
		}
		
		if(m_id3.PicValid() >= 0 || (cover!=NULL && genImg==NULL)){
			if(m_id3.piclength > 0 || cover!=NULL){
				char *imgPath,*fileTitle;
				MSkBitmap mMSkBitmap;
				
				if(m_id3.piclength > 0)
					BitmapFactory::decodeBuffer(&mMSkBitmap,(void*)m_id3.picdata,m_id3.piclength,109,109);
				else if(cover!=NULL)
					BitmapFactory::decodeFile(&mMSkBitmap,cover.getString(),109,109);
				
				imgPath = new char[255];
				fileTitle = new char[strlen(info->name)];
				
				getFileTitle(info->name,fileTitle);
				genImgPath(fileTitle,imgPath);
				
				int ret = mMSkBitmap.saveToFile(imgPath);
				
				if(ret == 1){
					int pathlen = strlen(imgPath);
					info->img_path = new char[pathlen+1];
					memcpy(info->img_path,imgPath,pathlen+1);
					if(cover!=NULL){
						genImg = imgPath;
					}
					log_i("album img save sucess.");
				}else
					log_i("fwrite fail,ret=%d",ret);
			}
		}
		else if(cover!=NULL && genImg!=NULL){
			int pathlen = strlen(genImg.getString());
			info->img_path = new char[pathlen+1];
			memcpy(info->img_path,genImg.getString(),pathlen+1);		
		}


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
			slqCheck(path);
			if(FileAttr::FileExist(path)){
				inf++;
				memset(ptr,0,strlen(ptr));
			}else{
				log_i("genImgPath:%s",path);
				break;
			}
		}
	}
	void mediaprovider::genMd5ImgPath(CString& md5,CString& out){
		char path[1024];
		
		sprintf(path,"%s/%s",IMG_PATH,md5.string);
		out = path;
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
	void mediaprovider::stopMediaScanner(){
		log_i("stopMediaScanner");
		scannerStop = true;
	}
	bool mediaprovider::mediaCanStop(){
		return scanCanStop;
	}

	int mediaprovider::filescanner(char *path)
	{
		int		count,i;
		char 	direct[255];
		File    file;
		String 	str;
		DIR* 	d;
		mediainfo info;
		struct dirent* de;
		CStringArray musicList;
		CStringArray cueList;
		CStringArray pictureList;
		unsigned long tt0,tt1,tt2,tt3;

		memset(&info,0,sizeof(mediainfo));

		tt0 = Time::getMicrosecond();
		
		d = opendir(path);

		while((de = readdir(d)) != NULL)
		{
			int name_len = strlen(de->d_name);
			
			strcpy(direct,path);
			strcat(direct,"/");
			strcat(direct,de->d_name);
			
			if(File::isDirect(direct)){
				if (scannerStop || de->d_name[0] == '.'){
					continue;
				}
				filescanner(direct);
				count++;
			}else if(ismusic(de->d_name)&&(de->d_name[0]!='.')&&(!music_exsit_db(direct))){
				musicList.addString(direct);
			}else if((de->d_name[0]!='.')&&isCueFile(de->d_name)){
				cueList.addString(direct);
			}else if(isPictureFile(de->d_name)){
				pictureList.addString(direct);
			}
		}
		t0 += Time::getMicrosecond() - tt0;
		
		CString cover,genImg;
		
		pictureList.getCString(0,cover);
		
		for(i=0;i<musicList.getCount();i++){
			CString path;
			
			tt1 = Time::getMicrosecond();
			
			musicList.getCString(i,path);
			
			getmediainfo(path.getString(),&info,cover,genImg);
			
			tt2 = Time::getMicrosecond();
			t1 += (tt2-tt1);

			if(!cueCheck(info.path,&info)){
				insert("music",&info);
			}
			tt3 = Time::getMicrosecond();
			
			t2 += tt3 - tt2;
			safefreeMediainfo(&info);
		}
		
		
		safefreeMediainfo(&info);
		closedir(d);
		
		return 0;
	}
	bool mediaprovider::cueCheckCursor(CursorItem& item){
		CString dir;
		char cuePath[300]={0};
		bool ret = false;
		CursorMediaInfo curMedia;
		
		item.getValue("path",dir);
		getCuePath(dir.string,cuePath);
		
		if(FileAttr::FileExist(cuePath)){
			curMedia.setCursorItem(item);
			ret = loadCueFile(cuePath,&curMedia.mInfo);
		}
		
		return ret;	
	}
	bool mediaprovider::cueCheck(char *direct,mediainfo *info){
		char cuePath[300];
		bool ret = false;
		
		memset(cuePath,0,300);
		getCuePath(direct,cuePath);
		
		if(FileAttr::FileExist(cuePath)){
			ret = loadCueFile(cuePath,info);
		}

		return ret;
	}
	void mediaprovider::getCuePath(char* src,char * out){
		if(src){
			//log_i("in=%s",src);
			int len = strlen(src);
			int i; 
			for(i=len-1;i>0;i--){
				if(src[i] == '.')
					break;
			}
			memcpy(out,src,i+1);
			strcat(out,"cue");
			//log_i("out=%s",out);
		}
	}
	bool mediaprovider::loadCueFile(char* path,mediainfo *info){
		CCue mCCue;
		ArrayMediaInfo *list;
		int i,count,ret;
		
		ret = mCCue.file_load(path);	
		count = mCCue.m_total_song;
		
		log_i("ret=%d,file=%s",ret,path);
		
		if(ret<0 || count<=0)
			return false;
		
		for(i=0;i<count;i++){
			mediainfo mInfo;
			song_t song;
			song_t prev_song;
			int len,ldur;

			memset(&mInfo,0,sizeof(mediainfo));
			mediainfocpy(info,mInfo);
			
			mCCue.get_cert_song(i,&song);
			
			mInfo.isCue = 1;
			mInfo.cueStart = song.star;
			
			if(i == count-1){
				mCCue.get_cert_song(i-1,&prev_song);
				ldur = info->duration - mInfo.cueStart;
				if(ldur>0){
					mInfo.duration = ldur;
				}else{
					mInfo.duration = 0;
				}
			}else{
				mInfo.duration = song.len;
			}
			mInfo.track = i+1;
			if(song.m_strname.string != NULL){
				len = strlen(song.m_strname.string);
				slqCheck(song.m_strname.string);
//---------------------Name-------------------------------------------------------
				//log_i("tag");
				if(mInfo.name != NULL){
					delete mInfo.name;
					mInfo.name = NULL;
				}
				mInfo.name = new char[len+1];
				memcpy(mInfo.name,song.m_strname.string,len+1);
				
				if(mInfo.name_key != NULL){
					delete mInfo.name_key;
					mInfo.name_key = NULL;
				}
				mInfo.name_key = new char[len+1];
				strlwr(song.m_strname.string,mInfo.name_key);
//---------------------title-------------------------------------------------------			
				if(mInfo.title != NULL){
					delete mInfo.title;
					mInfo.title = NULL;
				}
				mInfo.title = new char[len+1];
				memcpy(mInfo.title,song.m_strname.string,len+1);
				
				if(mInfo.title_key != NULL){
					delete mInfo.title_key;
					mInfo.title_key = NULL;
				}
				mInfo.title_key = new char[len+1];
				strlwr(song.m_strname.string,mInfo.title_key);
			}
//---------------------artist-------------------------------------------------------
			//log_i("tag");
			if(song.m_strart.string){
				len = strlen(song.m_strart.string);
				slqCheck(song.m_strart.string);
				
				if(mInfo.artist != NULL){
					delete mInfo.artist;
					mInfo.artist = NULL;
				}
				mInfo.artist = new char[len+1];
				memcpy(mInfo.artist,song.m_strart.string,len+1);
				
				if(mInfo.artist_key != NULL){
					delete mInfo.artist_key;
					mInfo.artist_key = NULL;
				}
				mInfo.artist_key = new char[len+1];
				strlwr(song.m_strart.string,mInfo.artist_key);
			}
//---------------------end------------------------------------------------------- 
			
			insert("music",&mInfo);
			safefreeMediainfo(&mInfo);
		}
	}
	void mediaprovider::strCopy(char *src,char *out){
		int len;
		
		len = strlen(src);
		safeDelete(out);
		
		out = new char[len+1];
		memcpy(out,src,len+1);
	}
/*
	int mediaprovider::checkfile(const char *dir,bool display){
		char sql[100],*sp;
		mediainfo *infolist;
		int count,i,progress=0;

		sp = sql;
		mCurrentTimes = 0;
		if(dir == NULL){
			count = querymusic(0,&infolist);
		}else{
			sprintf(sp,"path like '%s/%%'",dir);
			count = querymusic(sql,&infolist);
		}
		
		for(i=0;i<count;i++){
			int p;
			//log_i("checkfile exsit:%s",infolist[i].path);
			
			if(access(infolist[i].path,F_OK) != 0){
				int id = infolist[i].id;
				
				//remove(infolist[i].img_path);
				del("music",id);
				
				//log_i("del file in db:%s",infolist[i].path);
				
			}else if(infolist[i].times > mCurrentTimes){
				mCurrentTimes = infolist[i].times;
			}

			if(display){
				p = (i+1)*100/count;
				
				if(progress<p){
					progress = p;
					sendMsgProgress(progress);
				}
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
*/
	int mediaprovider::checkfile(const char *dir,bool display){
		int count,i,progress=0;
		unsigned long t0,t1,tt0,tt1,tt2;
		Cursor cur;
		char sql[SQL_LEN];

		sprintf(sql,"select _id,path,times from music where path like '%s/%%'",dir);
		
		tt0 = Time::getMicrosecond();
		
		querySqlCursor(sql,&cur);
		count = cur.mLen;

		tt1 = Time::getMicrosecond();
		
		for(i=0;i<count;i++){
			CString path;
			CString timeStr;
			int times = 0,p;
			
			cur.getValueCstring(i,"path",path);
			cur.getValueCstring(i,"times",timeStr);
			timeStr.toIneger(&times);

			if(!FileAttr::FileExist(path.string)){
				CString idStr;
				int id = 0;
				
				cur.getValueCstring(i,"_id",idStr);
				idStr.toIneger(&id);
				
				del("music",id);
			}else if(times > mCurrentTimes){
				mCurrentTimes = times;
			}

			if(display){
				p = (i+1)*100/count;
				
				if(progress<p){
					progress = p;
					sendMsgProgress(progress);
				}
			}
		}

		tt2 = Time::getMicrosecond();
		t0 = tt1 - tt0;
		t1 = tt2 - tt1;
		log_i("t0=%ldms,t1=%ld",t0/1000,t1/1000);
		
		if(!FileAttr::FileExist(IMG_PATH))
			mkdir(IMG_PATH,0777);
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
		}else{
			log_i("MUSIC_TABLE_CREATE : %s\n",MUSIC_TABLE_CREATE);	
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
		//log_i("sqlite3_exec");
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
		char *ptr,sql[1024*3];

		ptr = sql;
		ptr += sprintf(ptr,"insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",
			table,MUSIC_PTAH,MUSIC_NAME,MUSIC_NAME_KEY,MUSIC_TITLE,MUSIC_TITLE_KEY,MUSIC_ART,MUSIC_ART_KEY,
			MUSIC_ALBUM,MUSIC_ALBUM_KEY,"genre","genre_key",MUSIC_TRACK,MUSIC_ART_IMG,MUSIC_ADD_TIME,
			MUSIC_DURATION,MUSIC_IN_PLAY,MUSIC_TIMES,"iscue","cuestart","md5");
		
		ptr += sprintf(ptr,"values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s','%d','%d','%d','%d','%d','%d','%s');",
				info->path,info->name,info->name_key,info->title,info->title_key,info->artist,
				info->artist_key,info->album,info->album_key,info->genre,info->genre_key,info->track,info->img_path,
				info->add_time,info->duration,info->inPlay,info->times,info->isCue,info->cueStart,info->md5);
		
		exec(sql,0,0);
		
		return 0;
	}
	int mediaprovider::insertCursorItem(CursorItem& item){
		int i;
		char *ptr,sql[1024*3];
		
		ptr = sql;
		ptr += sprintf(ptr,"insert into music (");
		
		for(i=0;i<item.mKey.getCount();i++){
			CString key;
			
			item.mKey.getCString(i,key);
			
			ptr += sprintf(ptr,"%s",key.string);
			
			if(item.mKey.getCount() != i + 1)
				ptr += sprintf(ptr,",",key.string);
		}

		ptr += sprintf(ptr,") values(");

		for(i=0;i<item.mValue.getCount();i++){
			CString value;
			
			item.mValue.getCString(i,value);
			
			ptr += sprintf(ptr,"'%s'",value.string);
			
			if(item.mKey.getCount() != i + 1)
				ptr += sprintf(ptr,",",value.string);
		}

		ptr += sprintf(ptr,");");
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
	int mediaprovider::deleteMusicOnDir(const char *dir){
		char sql[1024];
		sprintf(sql,"delete from music where path like '%s/%%'",dir);
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
			if(where != NULL)
				ptr += sprintf(ptr,"%s;",where);
			else
				return 0;

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
	int mediaprovider::queryCursor(char *where, Cursor* cur){
			char *ptr,sql[1024];
			int count = 0;
			
			log_i("-------queryCursor");
			
			ptr = sql;
			
			ptr += sprintf(ptr,"select * from music ");
			if(where!=0)
				ptr += sprintf(ptr,"%s;",where);

			log_i("queryMusicArray:sql=%s",sql);
			
			return exec(sql,cur,cursor_sql_callback);		
	}
	int mediaprovider::querySqlCursor(char *sql, Cursor* cur){
		log_i("queryMusicArray:sql=%s",sql);
		return exec(sql,cur,cursor_sql_callback);	

	}
	int mediaprovider::del(char *table,int id){
		char *ptr,sql[256];
		ptr = sql;
		ptr += sprintf(ptr,"delete from music where _id=%d",id);
		
		exec(sql,0,0);
		
		return 0;
	}
	static const char* mediaLock = "mediascanner";
	void mediaprovider::getWakeLock(){
		if(!isWakeLock){
			if(gPlayer.wakeLock(mediaLock) == 0){
				isWakeLock = true;
			}
		}
	}
	void mediaprovider::releaseWakeLock(){
		if(isWakeLock){
			if(gPlayer.wakeUnlock(mediaLock) == 0){
				isWakeLock = false;
			}
		}
	}
	mediaprovider::~ mediaprovider(void)
	{
		db = 0;
		
	}
	int mediaprovider::queryMusicNum(){
		char *ptr,sql[256];
		IntegerArray array;
		
		ptr = sql;
		ptr += sprintf(ptr,"select count(_id) as num from music");
		
		exec(sql,&array,SettingProvider::sql_callback);

		return array.getItem(0);
	}
	int mediaprovider::queryAllMusicId(void *arg){
		char *ptr,sql[256];
		IntegerArray *array;
		array = (IntegerArray *)arg;
		ptr = sql;
		ptr += sprintf(ptr,"select _id from music");
		
		exec(sql,array,SettingProvider::sql_callback);
		
		return array->getCount();
	}
	AudioFileArray::AudioFileArray(){
		mList = NULL;
		mLen=mMax=0;
		needStop = false;
	}
	AudioFileArray::~AudioFileArray(){
		if(mList!=NULL){
			delete[] mList;
			mList = NULL;
		}
		mLen=mMax=0;
		log_i("-");
	}
	void AudioFileArray::addItem(AudioFileInfo& item){
		if(mLen>=mMax){
			AudioFileInfo *temp;
			int i,count;
			
			if(mMax == 0){
				mMax = ARRAY_LIST_NUM;
			}else{
				mMax*=2;
			}
		
			temp = new AudioFileInfo[mMax];
			
			for(i=0;i<mLen;i++){
				temp[i] = mList[i];
			}
		
			delete[] mList;
			mList = temp;
		}
		mList[mLen] = item;
		mLen++;
	}
	
	void AudioFileArray::startScanFile(const char *file,bool recursion){
		this->~AudioFileArray();
		needStop = false;
		listFile(file,recursion);
	}
	
	void AudioFileArray::listFile(const char *file,bool recursion){
		int 	i;
		DIR* 	d;
		char 	direct[255];
		struct dirent* de;
		CString cover;
		CStringArray musicList;
		CStringArray cueList;
		CStringArray pictureList;

		d = opendir(file);

		while((de = readdir(d)) != NULL)
		{
			int name_len = strlen(de->d_name);
			
			strcpy(direct,file);
			strcat(direct,"/");
			strcat(direct,de->d_name);
			
			if(File::isDirect(direct)){
				if (needStop || de->d_name[0] == '.'){
					continue;
				}
				if(recursion)
					listFile(direct,recursion);
			}else if(ismusic(de->d_name)&&(de->d_name[0]!='.')&&(!gmediaprovider.music_exsit_db(direct))){
				musicList.addString(direct);
			}else if((de->d_name[0]!='.')&&isCueFile(de->d_name)){
				cueList.addString(direct);
			}else if(isPictureFile(de->d_name)){
				pictureList.addString(direct);
			}
		}

		closedir(d);
		
		pictureList.getCString(0,cover);
		
		for(i=0;i<musicList.getCount();i++){
			AudioFileInfo item;
			musicList.getCString(i,item.path);
			item.cover = cover;
			Environment::MD5(item.path.string,item.md5);
			addItem(item);
		}
	}
	SdcardAudioData::SdcardAudioData(const char* path){
		int ret;
		char file[255];
		char dir[255];
		char cmd[255];
		CString scanDir;

		insertItemCount = 0;
			
		sprintf(dir,"%s/.audio_data",SDCARD_PATH);
		sprintf(file,"%s/.audio_data/audio.db",SDCARD_PATH);
		dataPath = file;
		datadir = dir;
		scanDir = path;
		
		if(scanDir.Find(SDCARD_PATH,0) == 0){
			isSdcard = true;
			if(FileAttr::FileExist(file)){
				sprintf(cmd,"./system/bin/busybox cp '%s' '%s'",file,SDCARD_DATA_PATH);
				ret = system(cmd);
				existBack = true;
			}else{
				remove(SDCARD_DATA_PATH);
				existBack = false;
			}
			
			ret = sqlite3_open(SDCARD_DATA_PATH,&db);

			if(ret != SQLITE_OK){
				log_e("sqlite3_exec open error path: %s\n",TABLE_PATH);
				db = NULL;
			}

			ret = sqlite3_exec(db,MUSIC_TABLE_CREATE,NULL,NULL,NULL);
			if(ret != SQLITE_OK){
				log_e("MUSIC_TABLE_CREATE error");
				db = NULL;
			}			

		}
		else{
			db = NULL;
			isSdcard = false;
		}
	}
	bool SdcardAudioData::queryFile(CursorItem& item,AudioFileInfo& info){
		bool ret;
		char *pErrMsg = 0;
		CString md5;
		char *ptr,sql[1024],sqlPath[1024];
		Cursor cur;
		
		if(db == NULL)
			return false;
				
		ptr = sql;
		ptr += sprintf(ptr,"select * from music where ");
		ptr += sprintf(ptr,"md5 = '%s'",info.md5.string);
		
		sqlite3_exec(db,sql,cursor_sql_callback,(void*)&cur,&pErrMsg);
		
		if(cur.mLen > 0){
			CString val;
			
			item = cur.mList[0];
			
			item.getValue("path",val);
			mediaprovider::slqFormatOut(val.string,sqlPath);
			item.setValue("path",sqlPath);
			
			item.removeItem("_id");
			
			ret = true;
		}else{
			ret = false;
		}
		//log_i("ret=%d,%s",ret,info.path.string);
		return ret;
	}
	void SdcardAudioData::insertCursor(CursorItem& item){
		int i,ret;
		char *pErrMsg = 0;
		char *ptr,sql[1024*5];

		if(db == NULL)
			return;
			
		ptr = sql;
		ptr += sprintf(ptr,"insert into music (");
		
		for(i=0;i<item.mKey.getCount();i++){
			CString key;
			
			item.mKey.getCString(i,key);
			
			ptr += sprintf(ptr,"%s",key.string);
			
			if(item.mKey.getCount() != i + 1)
				ptr += sprintf(ptr,",",key.string);
		}

		ptr += sprintf(ptr,") values(");

		for(i=0;i<item.mValue.getCount();i++){
			CString value;
			
			item.mValue.getCString(i,value);
			
			ptr += sprintf(ptr,"'%s'",value.string);
			
			if(item.mKey.getCount() != i + 1)
				ptr += sprintf(ptr,",",value.string);
		}

		ptr += sprintf(ptr,");");
		
		ret = sqlite3_exec(db,sql,NULL,NULL,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s",sql);
			log_e("sqlite3_exec pErrMsg : %s",pErrMsg);
		}
		insertItemCount++;
	}
	void SdcardAudioData::copyData(){
		char cmd[300],ptr;
		
		log_i("isSdcard=%d;existBack=%d,insertItemCount=%d",isSdcard,existBack,insertItemCount);
		
		if(isSdcard  
			&& (!existBack || insertItemCount>0)	){
			
			if(!FileAttr::FileExist(datadir.string)){
				sprintf(cmd,"./system/bin/mkdir '%s'",datadir.string);
				log_i("%s",cmd);
				system(cmd);
			}

			sprintf(cmd,"./system/bin/busybox cp '%s' '%s'",SDCARD_DATA_PATH,dataPath.string);
			
			log_i("%s",cmd);
			system(cmd);
			Environment::sync();
		}

	}
	SdcardAudioData::~SdcardAudioData(){
		if(db != NULL){
			sqlite3_close(db);
			db = NULL;
		}
		copyData();
		log_i("---~SdcardAudioData()--");
	}

	mediaprovider gmediaprovider;
};

