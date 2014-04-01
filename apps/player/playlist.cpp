#include "player.h"

namespace mango
{
	PlayListItem::PlayListItem(){
		id = 0;
		name = NULL;
		remark = NULL;
	}
	PlayListItem::~PlayListItem(){
		id = 0;
		safeDelete(name);
		safeDelete(remark);
	}
	void PlayListItem::setId(int id){
		this->id = id;
	}
	void PlayListItem::setName(char *name){	
		if(name == NULL){
			safeDelete(this->name);
		}else{
			this->name = new char[strlen(name)+1];
			memcpy(this->name,name,strlen(name)+1);
		}
	}
	void PlayListItem::setRemark(char *remark){	
		if(name == NULL){
			safeDelete(this->remark);
		}else{
			this->remark = new char[strlen(remark)+1];
			memcpy(this->remark,remark,strlen(remark)+1);
		}
	}
	PlayListItem& PlayListItem::operator =(PlayListItem& item){
		setId(item.id);
		setName(item.name);
		setRemark(item.remark);
		return *this;
	}
	ArrayPlayListItem::ArrayPlayListItem(){
		mList = NULL;
		mLen = mMax = 0;
	}
	ArrayPlayListItem::~ArrayPlayListItem(){
		//log_i("------");
		mLen = mMax = 0;
		if(mList!=NULL){
			delete[] mList;
			mList = NULL;
		}
	}
	PlayListItem& ArrayPlayListItem::getItem(int pos){
		PlayListItem *tmp;
		tmp = new PlayListItem();
		
		if(pos>=0 && pos<mLen){
			*tmp = mList[pos];
		}
		return *tmp;
	}
	int ArrayPlayListItem::addItem(PlayListItem& item){
		if(mLen>=mMax){
			PlayListItem *temp;
			int i,count;
			if(mMax == 0)
				mMax = 8;
			else
				mMax*=2;

			temp = new PlayListItem[mMax];
			for(i=0;i<mLen;i++){
				temp[i] = mList[i];
			}
			delete[] mList;
			mList = temp;
		}
		mList[mLen] = item;
		mLen++;
	}
	int ArrayPlayListItem::getCount(){
		return mLen;
	}
	
	int PlayList::sql_callback(void * use, int argc, char ** argv, char ** szColName)
	{
		ArrayPlayListItem* array = (ArrayPlayListItem*)use;
		PlayListItem item;
		log_i("argc=%d,szColName=%s",argc,*szColName);
		item.setId(mediaprovider::str_to_int(*argv++));
		item.setName(*argv++);
		item.setRemark(*argv);
		
		array->addItem(item);
		//log_i("item.name = %s,len=%d",item.name,array->getCount());
		return 0;
	}
	void PlayList::createTable(){
		gmediaprovider.exec(PLAYLIST_TABLE_CREATE,0,0);
		gmediaprovider.exec(PLAYLISTMEM_TABLE_CREATE,0,0);
	}
	void PlayList::newPlaylist(){
		char *ptr,sql[255],name[100];
		
		ptr = sql;
		genPlaylistName(&name[0],100);
		ptr+=sprintf(ptr,"insert into playlist (name,remark) values('%s','(null)')",name);
		
		gmediaprovider.exec(sql,0,0);
	}
	void PlayList::genPlaylistName(char* name,int len){
		int i,count;
		char *ptr,where[1024];

		i=1;
		ptr = where;
		
		while(1){
			ArrayPlayListItem array;

			memset(where,0,255);
			sprintf(ptr,"where name='Playlist%d'",i);
			
			queryPlaylist(array,where);
			
			if(array.getCount() == 0)
				break;
			
			i++;
		}

		memset(name,0,len);
		sprintf(name,"Playlist%d",i);
		log_i("i=%d,%s",i,name);
	}
	void PlayList::queryPlaylist(ArrayPlayListItem& array,char *arg){
		char *ptr,sql[255];

		array.~ArrayPlayListItem();
		ptr = sql;
		ptr+=sprintf(ptr,"select * from playlist");
		if(arg != NULL)
			ptr+=sprintf(ptr," %s",arg);
		
		gmediaprovider.exec(sql,&array,PlayList::sql_callback);
		//log_i("array.getcount()=%d",array.getCount());
	}
	void PlayList::insertToPlaylist(int playlist_id,const char* md5){
		char *ptr,sql[255];

		ptr = sql;
		ptr+=sprintf(ptr,"insert into playlistmem (playlist_id,audio_md5) values(%d,'%s')",
			playlist_id,md5);
		
		gmediaprovider.exec(sql,0,0);
	}
	void PlayList::delAudioFromPlaylist(const char* md5){
		char *ptr,sql[255];
		
		ptr = sql;
		ptr+=sprintf(ptr,"delete from playlistmem where audio_md5=%d",md5);
		
		gmediaprovider.exec(sql,0,0);	
	}
	void PlayList::delAudioFromPlaylist(int playlist_id,const char* md5){
		char *ptr,sql[255];
		
		ptr = sql;
		ptr+=sprintf(ptr,"delete from playlistmem where audio_md5='%s' and playlist_id=%d",md5,playlist_id);
		
		gmediaprovider.exec(sql,0,0);	
	}
	void PlayList::delPlaylist(int playlist_id){
		char *ptr,sql[255];
		
		log_i("playlist_id=%d",playlist_id);
		
		ptr = sql;
		ptr+=sprintf(ptr,"delete from playlist where _id=%d",playlist_id);
		gmediaprovider.exec(sql,0,0);	

		memset(sql,0,255);
		ptr = sql;
		ptr+=sprintf(ptr,"delete from playlistmem where playlist_id=%d",playlist_id);
		gmediaprovider.exec(sql,0,0);	
	}
	void PlayList::queryPlaylistMem(ArrayMediaInfo& array,int playlist_id){
		char *ptr,sql[255];
		
		array.clear();
		ptr = sql;
		ptr+=sprintf(ptr,"select music.* from  music,playlistmem where playlistmem.playlist_id=%d and music.md5=playlistmem.audio_md5",
			playlist_id);
		
		gmediaprovider.queryArrayMedia(sql,&array);
	}
	PlayListAdapter::PlayListAdapter(){
		mSql = NULL;
	}
	PlayListAdapter::PlayListAdapter(ListView* list){
		mSql = NULL;
		mList = list;
	}
	PlayListAdapter::~PlayListAdapter(){
		safeDelete(mSql);
	}
	int PlayListAdapter::getCount(){
		return 	mArrayPlayList.getCount();
	}
	void PlayListAdapter::refresh(){
		int count,i;
		
		PlayList::queryPlaylist(mArrayPlayList,NULL);
		
		mList->setListAdapter(this);
		mList->deleteAllRecord();

		PlayListItem item;
		item.setId(-1);
		log_i("mArrayPlayList.getcount=%d:item.id=%d",mArrayPlayList.getCount(),item.id);
		mArrayPlayList.addItem(item);
		log_i("mArrayPlayList.getcount=%d",mArrayPlayList.getCount());
		count = mArrayPlayList.getCount();
		for(i=0;i<count;i++){
			ListViewItem  lvItem;
			lvItem.mask     = LVIF_ADAPTER;
			lvItem.iItem    = i;
			lvItem.iSubItem = 0;
			lvItem.paramType = LIST_PARAM_MUSIC;
			mList->insertItem(&lvItem);
		}
		mList->invalidateRect();
	}
	void PlayListAdapter::PaintView(Canvas & canvas,Rect & rect,ListViewItem * lvitem,int isSec){
		int	 x, y;
		int index = lvitem->iItem;
		PlayListItem item = mArrayPlayList.getItem(index);

		x = rect.left;
		y = rect.top;
		
		if(isSec)
			canvas.setTextColor(COLOR_PLAY_ORANGE);
		else
			canvas.setTextColor(COLOR_TEXT);
		
		canvas.setTextSize(18);

		x=20;

		if(item.id>0){
			canvas.drawImageResource(IDP_MUSIC_PLAYLIST_ICO,x,y,true);
			x+=50;y+=10;
			canvas.drawText(item.name,-1,x,y);
		}else{
			canvas.drawImageResource(IDP_MUSIC_NEW_PLAYLIST,x,y,true);
			x+=50;y+=10;
			canvas.drawTextResource(STR_NEW_LAYLIST_LIST,x,y);
		}

	}
};

