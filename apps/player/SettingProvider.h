#include <sqlite3.h>

namespace mango
{	
	#define SETTING_TABLE_PATH "/data/mangosetting.db"
	#define SETTING_TABLE_NAME "settings"
	
	#define SETTING_BRIGHTNESS_ID 			1
	#define SETTING_VOLUME_ID 				2
	#define SETTING_PLAYMODE_ID 				3
	#define SETTING_GAPLESS_ID 				4
	#define SETTING_EQMODE_ID					5
	#define SETTING_EQSTATE_ID				6
	#define SETTING_PLAYPOS_ID				7
	#define SETTING_AUTOSLEEP_ID				8
	#define SETTING_LANGUAGE_ID				9
	#define SETTING_AUTOPOWEROFF_ID			10
	#define SETTING_SCANSDCARDSTATE_ID		11
	#define SETTING_FORCE_POWEROFF_ID		12

	#define SETTING_DEFAULT_VOLUME	170

	#define SETTING_BRIGHTNESS_NAME				"brightness"
	#define SETTING_VOLUME_NAME					"volume"
	#define SETTING_PLAYMODE_NAME 				"playmode"
	#define SETTING_GAPLESS_NAME					"gapless"
	#define SETTING_EQMODE_NAME					"eqmode"
	#define SETTING_EQSTATE_NAME					"eqopenOrclose"
	#define SETTING_PLAYPOS_NAME					"playposition"
	#define SETTING_AUTOSLEEP_NAME				"autosleepTime"
	#define SETTING_LANGUAGE_NAME 	 			"languageId"
	#define SETTING_AUTOPOWEROFF_NAME  			"autopoweroffTime"
	#define SETTING_SCANSDCARDSTATE_NAME  		"scanSdcardState"
	#define SETTING_FORCE_POWEROFF_NAME			"forcePowerOff"

	#define EQ_CUSTOM_ID		0
	#define EQ_POP_ID			1
	#define EQ_ROCK_ID			2
	#define EQ_CLASS_ID		3
	#define EQ_DANCE_ID		4
	#define EQ_BASS_ID			5
	
	#define SETTING_TABLE_CREATE "CREATE TABLE IF NOT EXISTS settings(\
_id INTEGER PRIMARY KEY,\
value INTEGER,\
name TEXT\
);"
	#define EQDATA_TABLE_CREATE "CREATE TABLE IF NOT EXISTS eqdata(\
_id INTEGER PRIMARY KEY,\
val_0 INTEGER,\
val_1 INTEGER,\
val_2 INTEGER,\
val_3 INTEGER,\
val_4 INTEGER,\
val_5 INTEGER,\
val_6 INTEGER,\
val_7 INTEGER\
);"
	class SettingProvider{
		public:
			SettingProvider(void);
			~SettingProvider(void);
			void initialize(void);
			void dbclose();
			void dataBaseInit(void);
			int insert(int id,int value);
			int insert(int id,int value,const char *str);
			int updateSDcard(__u32 value,char* buf,int id=SETTING_SCANSDCARDSTATE_ID);
			int update(int id,int value);
			int queryCursor(int id,Cursor *cur);
			int query(int id,int *value);
			void saveVolume(int vol);
			int readVolume();
			void EqInitialize();
			int EqInsert(int id,int *value);
			int EqUpdate(int id,int *value);
			int EqQuery(int id ,int *value);	
			static int sql_callback(void * use, int argc, char ** argv, char ** szColName);
		private:
			sqlite3 * db;
			
	};
	
	class IntegerArray{
		private:
			int *list;
			int len,mMax;
		public:
			IntegerArray();
			~IntegerArray();
			int addItem(int val);
			int getItem(int val);
			int getCount();
			int getIntegerArray(int *val,int count);
	};

	extern SettingProvider gSettingProvider;
	extern int EqValue[6][8];
	extern int GaplessValue[4];
}

