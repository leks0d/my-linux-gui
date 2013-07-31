#include "player.h"

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
		log_i("data = %d",data);
		
		data->info.id = mediaprovider::str_to_int(*argv++);
		data->info.path = getstr(*(argv++));
		data->info.name = getstr(*(argv++));
		data->info.name_key = getstr(*(argv++));
		data->info.title = getstr(*(argv++));
		data->info.title_key = getstr(*(argv++));
		data->info.artist = getstr(*(argv++));
		data->info.artist_key = getstr(*(argv++));
		data->info.album = getstr(*(argv++));	
		data->info.album_key = getstr(*(argv++));
		data->info.track = mediaprovider::str_to_int(*(argv++));
		data->info.img_path = getstr(*(argv++));
		data->info.add_time = mediaprovider::str_to_int(*(argv++));
		data->info.duration = mediaprovider::str_to_int(*(argv++));
		data->info.inPlay = mediaprovider::str_to_int(*(argv));
		data->next = 0;
		
		if((*ppt) == 0){
			(*ppt) = data;			
		}else{
			while(pt->next != 0){
				pt = pt->next;
			}
			pt->next = data;			
		}		
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
		while( (*string) != '\0'){
			if(*string <= 'Z' && *string >= 'A')
				*string += 'a' - 'A';
			string++;
		}
			
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
	static int ismusic(char *file)
	{
		char *type;
		char *music_type[] = {"mp3","wav","flac","aac","ogg","ape","\0"};
		char **mtype;
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

	int mediaprovider::music_exsit_db(char *path){
		mediainfo *infolist;
		int count,i;
		char *ptr,sql[1024];
		
		ptr = sql;
		ptr += sprintf(ptr,"path = '%s'",path);
		count = querymusic(sql,&infolist);

		if(count>0)
			free(infolist);
		
		return count;
	}
	
	mediaprovider::mediaprovider(void)
	{
		db = 0;
		
	}

	int mediaprovider::mediascanner()
	{
		log_i("-------------mediascanner checkfile");
		checkfile();
		log_i("-------------mediascanner filescanner");
		filescanner("/mnt/sdcard");
		filescanner("/mnt/external_sd");
		log_i("-------------mediascanner end");
	}
	
	int mediaprovider::getmediainfo(char *path,mediainfo *info)
	{
		ID3INFO m_id3(path);
		char *value;
		int len;
		value = (char*)malloc(256);
		
		memset(info, 0, sizeof(mediainfo));
		info->path = path;
		info->name = getfilename(path);
		info->inPlay = 0;

		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_CD_TRACK_NUMBER, value)){
			info->track = str_to_int(value);
		}

		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ALBUM, value)){
			strdelspa(value);
			len = strlen(value)+1;
			info->album = new char[len];
			memcpy(info->album,value,len);			
		}
		
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_ARTIST, value)){
			strdelspa(value);
			len = strlen(value)+1;
			//log_i("artist len=%d,value=%s",len,value);
			info->artist = new char[len];
			memcpy(info->artist,value,len);
		}
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_TITLE, value)){
			strdelspa(value);
			len = strlen(value)+1;
			//log_i("artist len=%d,value=%s",len,value);
			info->title = new char[len];
			memcpy(info->title,value,len);
		}
		memset(value, 0, 256);
		if (m_id3.GetTags(METADATA_KEY_DURATION, value)){
			info->duration = str_to_int(value);
		}

		return 0;
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
		d = opendir(path);

		while((de = readdir(d)) != NULL)
		{
			int name_len = strlen(de->d_name);
			
			strcpy(direct,path);
			strcat(direct,"/");
			strcat(direct,de->d_name);
			//log_i("opendir DT_DIR :%s\n",direct);

			if (de->d_type == DT_DIR) {				
				if (name_len == 1 && de->d_name[0] == '.') continue;
            	if (name_len == 2 && de->d_name[0] == '.' &&
                de->d_name[1] == '.') continue;
				filescanner(direct);
				count++;
			}
			else if(ismusic(de->d_name)&&(!music_exsit_db(direct)))
			{				
				getmediainfo(direct,&info);
				insert("music",&info);
				count++;
			}

		}

		closedir(d);	
		
	}
	int mediaprovider::checkfile(){
		mediainfo *infolist;
		int count,i;
		
		count = querymusic(0,&infolist);
		
		for(i=0;i<count;i++){
			log_i("checkfile exsit:%s",infolist[i].path);
			if(access(infolist[i].path,F_OK) != 0){
				int id = infolist[i].id;
				del("music",id);
				log_i("delete file in db at:%s",infolist[i].path);				
			}
		}
		if(count>0)
			free(infolist);
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
			log_i("sqlite3_exec success : %s",sql);
		return ret;	
	}

	int mediaprovider::insert(char *table,mediainfo *info)
	{
		char *ptr,sql[1024];
		
		ptr = sql;
		ptr += sprintf(ptr,"insert into %s (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",
			table,MUSIC_PTAH,MUSIC_NAME,MUSIC_NAME_KEY,MUSIC_TITLE,MUSIC_TITLE_KEY,MUSIC_ART,MUSIC_ART_KEY,
			MUSIC_ALBUM,MUSIC_ALBUM_KEY,MUSIC_TRACK,MUSIC_ART_IMG,MUSIC_ADD_TIME,MUSIC_DURATION,MUSIC_IN_PLAY);

		ptr += sprintf(ptr,"values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%d','%s','%d','%d','%d');",
				info->path,info->name,info->name_key,info->title,info->title_key,info->artist_id,info->artist,
				info->artist_key,info->album_id,info->album,info->album_key,info->track,info->img_path,info->add_time,info->duration,info->inPlay);

		exec(sql,0,0);

		return 0;
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
			
			arraylist = (ArrayMediaInfo*)array;
			info = 0;
			ptr = sql;
			
			ptr += sprintf(ptr,"select * from music ");
			if(where!=0)
				ptr += sprintf(ptr,"%s;",where);
			
			exec(sql,&info,_sql_callback);
	
			pt = info;

			log_i("exec complete");
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
		log_i("del return.");
		return 0;
	}
	mediaprovider::~ mediaprovider(void)
	{
		db = 0;
		
	}
	mediaprovider gmediaprovider;
};

