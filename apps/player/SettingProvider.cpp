#include "player.h"


namespace mango
{	
	const static char* volumePath = "/data/volume";
	
	SettingProvider::SettingProvider(){

	}
	SettingProvider::~SettingProvider(){

	}	
	void SettingProvider::initialize(){
		int value,ret = 0;
		char *pErrMsg = 0;
		int *eqValue; 
		
		ret = sqlite3_open(SETTING_TABLE_PATH,&db);
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec open error path: %s\n",SETTING_TABLE_PATH);
			return;
		}

		ret = sqlite3_exec( db, SETTING_TABLE_CREATE, 0, 0, &pErrMsg );

		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}

		ret = query(SETTING_BRIGHTNESS_ID,&value);

		if(ret == 0){
			dataBaseInit();
		}

		ret = sqlite3_exec( db, EQDATA_TABLE_CREATE, 0, 0, &pErrMsg );

		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}
		
		eqValue = new int[8];
		
		ret = EqQuery(EQ_CUSTOM_ID,eqValue);

		if(ret == 0){
			EqInitialize();
		}
	}

	void SettingProvider::dataBaseInit(void){
		insert(SETTING_BRIGHTNESS_ID,127,SETTING_BRIGHTNESS_NAME);
		insert(SETTING_VOLUME_ID,SETTING_DEFAULT_VOLUME,SETTING_VOLUME_NAME);
		insert(SETTING_PLAYMODE_ID,0,SETTING_PLAYMODE_NAME);
		insert(SETTING_GAPLESS_ID,0,SETTING_GAPLESS_NAME);
		insert(SETTING_EQMODE_ID,0,SETTING_EQMODE_NAME);
		insert(SETTING_EQSTATE_ID,0,SETTING_EQSTATE_NAME);
		insert(SETTING_PLAYPOS_ID,0,SETTING_PLAYPOS_NAME);
		insert(SETTING_AUTOSLEEP_ID,30,SETTING_AUTOSLEEP_NAME);
		insert(SETTING_LANGUAGE_ID,0,SETTING_LANGUAGE_NAME);
		insert(SETTING_AUTOPOWEROFF_ID,60*20,SETTING_AUTOPOWEROFF_NAME);
		insert(SETTING_SCANSDCARDSTATE_ID,0,SETTING_SCANSDCARDSTATE_NAME);
		insert(SETTING_FORCE_POWEROFF_ID,0,SETTING_FORCE_POWEROFF_NAME);
		insert(100,100,"reserve");
	}
	void SettingProvider::dbclose(){
		int value;
		update(100,101);
		
		log_i("------------------SettingProvider close-----value=%d",value);
		
		sqlite3_close(db);
	}
	int SettingProvider::insert(int id,int value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		
		ptr = sql;
		sprintf(ptr,"insert into settings values(%d,%d)",id,value);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}

		return ret;
	}
	int SettingProvider::insert(int id,int value,const char *str){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];

		if(id == SETTING_VOLUME_ID){
			saveVolume(value);
		}
		ptr = sql;
		sprintf(ptr,"insert into settings values(%d,%d,'%s')",id,value,str);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}
		
		return ret;	
	}
	void SettingProvider::saveVolume(int vol){
		FILE *fd=NULL;
		Mstring mstr(10);
		
		mstr.mSprintf("%d.",vol);
		
		fd = fopen(volumePath,"w");
		
		if(fd!=NULL){
			fwrite(mstr.mstr,1,mstr.len,fd);
			fclose(fd);
			log_i("saveVolume mstr=%s,vol=%d",mstr.mstr,vol);
		}
		
		Environment::sync();
	}
	int SettingProvider::readVolume(){
		FILE *fd=NULL;
		char buf[10];
		int vol = -1;
		
		fd = fopen(volumePath,"r");
		if(fd != NULL){
			fread(buf,1,10,fd);
			fclose(fd);
			sscanf(buf,"%d.",&vol);
			log_i("readVolume buf=%s,vol=%d",buf,vol);
			return vol;
		}
		return vol;
	}
	int SettingProvider::updateSDcard(__u32 value,char* buf){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		
		ptr = sql;
		sprintf(ptr,"update settings set value=%d,name='%s' where _id=%d",value,buf,SETTING_SCANSDCARDSTATE_ID);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);
		}else{
			log_i("sqlite3_exec sucess:%s",sql);
		}
		
		return ret;
	}

	int SettingProvider::update(int id,int value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024],safe[100];
		
		if(id == SETTING_VOLUME_ID){
			/*int retry = 3;
			while(retry--){
				saveVolume(value);
				if(readVolume() == value)
					break;
			}*/
			value += 300;
		}

		ptr = sql;
		sprintf(ptr,"update settings set value=%d where _id=%d",value,id);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}else
			log_i("sqlite3_exec sucess:%s",sql);

		ptr = safe;
		sprintf(ptr,"update settings set value=100 where _id=100");
		sqlite3_exec(db,safe,0,0,&pErrMsg);
		
		return ret;
	}
	int SettingProvider::queryCursor(int id,Cursor *cur){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];

		ptr = sql;
		sprintf(ptr,"select * from settings where _id=%d",id);
		
		ret = sqlite3_exec(db,sql,cursor_sql_callback,(void*)cur,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}

		return ret;
	}

	int SettingProvider::query(int id,int *value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		IntegerArray *array;
		array = new IntegerArray();

		if(id == SETTING_VOLUME_ID){
			/**value = readVolume();
			if(*value<=255||*value>=0)
				return 1;*/
		}

		ptr = sql;
		sprintf(ptr,"select value from settings where _id=%d",id);
		
		ret = sqlite3_exec(db,sql,SettingProvider::sql_callback,(void*)array,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}
		if(value == NULL)
			return array->getItem(0);
		else{
			*value = array->getItem(0);
			if(id == SETTING_VOLUME_ID){
				*value-=300;
				if(*value<0){
					*value = 170;
				}
			}
			log_i("SettingProvider::query array=%d",array->getCount());
			return array->getCount();
		}
	}

	int SettingProvider::EqQuery(int id,int *value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		IntegerArray *array;
		array = new IntegerArray();
		
		ptr = sql;
		sprintf(ptr,"select val_0,val_1,val_2,val_3,val_4,val_5,val_6,val_7 from eqdata where _id=%d",id);
		
		ret = sqlite3_exec(db,sql,SettingProvider::sql_callback,(void*)array,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}else
			log_i("sqlite3_exec sucess : %s\n",sql);	
		
		array->getIntegerArray(value,8);
		log_i("SettingProvider::query array=%d",array->getCount());
		return array->getCount();
	}
	int SettingProvider::EqInsert(int id,int *value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		
		ptr = sql;
		sprintf(ptr,"insert into eqdata values(%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			id,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
			
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}

		return ret;
	}
	int SettingProvider::EqUpdate(int id,int *value){
		int ret = 0;
		char *pErrMsg = 0;
		char *ptr,sql[1024];
		
		ptr = sql;
		sprintf(ptr,"update eqdata set val_0=%d,val_1=%d,val_2=%d,val_3=%d,val_4=%d,val_5=%d,val_6=%d,val_7=%d where _id=%d",
			value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],id);
		
		ret = sqlite3_exec(db,sql,0,0,&pErrMsg);
		
		if(ret != SQLITE_OK){
			log_e("sqlite3_exec error : %s\n",pErrMsg);			
		}else
			log_i("sqlite3_exec success: %s\n",sql);	
		
		return ret;
	}

	void SettingProvider::EqInitialize(){
		EqInsert(EQ_CUSTOM_ID,EqValue[EQ_CUSTOM_ID]);
		EqInsert(EQ_POP_ID,EqValue[EQ_POP_ID]);
		EqInsert(EQ_ROCK_ID,EqValue[EQ_ROCK_ID]);
		EqInsert(EQ_CLASS_ID,EqValue[EQ_CLASS_ID]);
		EqInsert(EQ_DANCE_ID,EqValue[EQ_DANCE_ID]);
		EqInsert(EQ_BASS_ID,EqValue[EQ_BASS_ID]);
	}
	
	int SettingProvider::sql_callback(void * use, int argc, char ** argv, char ** szColName)
	{
		IntegerArray* array = (IntegerArray*)use;
		int i;

		for(i=0;i<argc;i++,argv++){
			array->addItem(mediaprovider::str_to_int(*argv));
			//log_i("------->sql_callback:%d,%s",mediaprovider::str_to_int(*argv),*argv);
		}
		return 0;
	}
	
	IntegerArray::IntegerArray(){
		list = NULL;
		len = mMax = 0;
	}
	IntegerArray::~IntegerArray(){
	}
	int IntegerArray::addItem(int val){
		if(len>=mMax){
			int *temp;
			if(mMax == 0)
				mMax = 100;
			else
				mMax*=2;
			temp = new int[mMax];
			memcpy(temp,list,len*sizeof(int));
			delete list;
			list = temp;
		}
		list[len] = val;
		len++;
	}
	int IntegerArray::getItem(int val){
		if(val>=len)
			return -1;
		else
			return list[val];
	}
	int IntegerArray::getCount(){
		return len;
	}
	int IntegerArray::getIntegerArray(int *val,int count){
		if(len>=count)
			memcpy(val,list,len*sizeof(int));
		else
			memset(val,0,count);
		return len;
	}

	SettingProvider gSettingProvider;
	int EqValue[6][8]={{0,0,0,0,0,0,0,0},
						{1,0,-1,-2,-2,-1,0,1},
						{0,1,2,-1,-1,0,0,2},
						{4,4,3,0,0,0,0,1},
						{1,2,1,-1,-1,0,0,2},
						{1,2,-1,-1,-2,0,0,3}
						};
	
	int GaplessValue[4]={-1,100,5000,10000};
}

