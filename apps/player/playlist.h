namespace mango
{
#define TABLE_PLAYLIST "playlist"
		
#define PLAYLIST_TABLE_CREATE "CREATE TABLE IF NOT EXISTS playlist(\
_id INTEGER PRIMARY KEY autoincrement,\
name TEXT,\
remark TEXT\
);"
		class PlayListItem{
			public:
				int id;
				char *name;
				char *remark;
			public:
				PlayListItem();
				~PlayListItem();
				void setId(int id);
				void setName(char *name);
				void setRemark(char *remark);
				PlayListItem& operator =(PlayListItem& item);
		};
	
		class ArrayPlayListItem{
			private:
				PlayListItem *mList;
				int mLen;
				int mMax;
			public:
				ArrayPlayListItem();
				~ArrayPlayListItem();
				int addItem(PlayListItem& item);
				PlayListItem& getItem(int pos);
				int getCount();
		};
		
#define TABLE_PLAYLIST_MEM "playlistmem"
		
#define PLAYLISTMEM_TABLE_CREATE "CREATE TABLE IF NOT EXISTS playlistmem(\
_id INTEGER PRIMARY KEY autoincrement,\
playlist_id INTEGER,\
audio_id INTEGER\
);" 
		class PlayListMem{
			public:
				int id;
				int playlist_id;
				int audio_id;
			public:
				PlayListMem();
				~PlayListMem();
		};
	
		class PlayList{
			public:
				static int sql_callback(void * use, int argc, char ** argv, char ** szColName);
				static void createTable();
				static void newPlaylist();
				static void genPlaylistName(char* name,int len);
				static void queryPlaylist(ArrayPlayListItem& array,char *arg);
				static void insertToPlaylist(int playlist_id,int audio_id);
				static void delAudioFromPlaylist(int audio_id);
				static void delAudioFromPlaylist(int playlist_id,int audio_id);
				static void delPlaylist(int playlist_id);
				static void queryPlaylistMem(ArrayMediaInfo& array,int playlist_id);
		};

		class PlayListAdapter : public BaseAdapter{
			public:
				ArrayPlayListItem mArrayPlayList;
				char *mSql;
				ListView *mList;
			public:
				PlayListAdapter();
				PlayListAdapter(ListView* list);
				~PlayListAdapter();
				void refresh();
				int getCount();
				void setDataSql(char *sql){}
				void PaintView(Canvas & canvas,Rect & rect,ListViewItem * lvitem,int isSec);
		};

};
