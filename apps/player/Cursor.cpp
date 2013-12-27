#include "player.h"

namespace mango
{
	CursorItem::CursorItem(){
		mLen = 0;
	}
	CursorItem::~CursorItem(){
	}
	void CursorItem::setValue(const char* key,CString& value){
		int i;
		bool ret = false;
		
		for(i=0;i<mLen;i++){
			CString cstr;
			mKey.getCString(i,cstr);
			if(cstr == key){
				mValue.setCString(i,value);
				ret = true;
				break;
			}
		}
	}
	void CursorItem::setValue(const char* key,const char* value){
		CString cstr;
		cstr = value;
		setValue(key,cstr);
	}
	void CursorItem::setValue(const char* key,int value){
		CString cstr;
		cstr = value;
		setValue(key,cstr);
	}
	void CursorItem::addItem(CString& key,CString& value){
		mKey.addCString(key);
		mValue.addCString(value);	
		mLen++;
	}
	void CursorItem::addItem(const char* key,const char* value){
		//log_i("key=%s,val=%s",key,value);
		mKey.addString(key);
		mValue.addString(value);
		mLen++;
	}
	void CursorItem::addItem(const char* key,int value){
		mKey.addString(key);
		mValue.addInteger(value);
		mLen++;
	}
	bool CursorItem::getValue(const char* key,CString& outValue){
		int i;
		bool ret;

		ret = false;
		
		for(i=0;i<mLen;i++){
			CString cstr;
			mKey.getCString(i,cstr);
			//log_i("i=%d,cstr=%s,key=%s",i,cstr.string,key);
			if(cstr == key){
				mValue.getCString(i,outValue);
				ret = true;
				break;
			}
		}
		//log_i("mLen=%d,key=%s,out=%s",mLen,key,outValue.string);
		return ret;
	}
	bool CursorItem::removeItem(const char* key){
		int i;
		bool ret = false;
		
		for(i=0;i<mLen;i++){
			CString cstr;
			mKey.getCString(i,cstr);
			if(cstr == key){
				mKey.remove(i);
				mValue.remove(i);
				mLen-- ;
				ret = true;
				break;
			}
		}
		return ret;		
	}
	void CursorItem::clearAll(){
		mKey.removeAll();
		mValue.removeAll();
		mLen = 0;
	}
	CursorItem& CursorItem::operator=(CursorItem& cursorItem){
		mKey = cursorItem.mKey;
		mValue = cursorItem.mValue;
		mLen = cursorItem.mLen;
		return *this;
	}
	Cursor::Cursor(){
		mList = NULL;
		mLen = mMax = 0;
	}
	Cursor::~Cursor(){
		if(mList != NULL){
			delete[] mList;
			mList = NULL;
		}
		mLen = mMax = 0;
	}
	int Cursor::addCursorItem(CursorItem& item){
		if(mLen>=mMax){
			CursorItem *temp;
			int i,count;
			
			if(mMax == 0){
				mMax = ARRAY_LIST_NUM;
			}else{
				mMax*=2;
			}
		
			temp = new CursorItem[mMax];
			
			for(i=0;i<mLen;i++){
				temp[i] = mList[i];
			}
		
			delete[] mList;
			mList = temp;
		}
		mList[mLen] = item;
		mLen++;
		return mLen;
	}
	bool Cursor::getValueCstring(int i,const char* key,CString& outCStr){
		
		if(i<0 && i>=mLen)
			return false;
		
		return mList[i].getValue(key,outCStr);
	}
	CursorMediaInfo::CursorMediaInfo(){
		memset(&mInfo,0,sizeof(mediainfo));
	}
	CursorMediaInfo::~CursorMediaInfo(){
		safefreeMediainfo(&mInfo);
	}
	void CursorMediaInfo::setCursorItem(CursorItem& item){
		int len;
		CString out;
		
		item.getValue("_id",out);
		if(out.toIneger(&mInfo.id) == 0){
			log_i("CursorMediaInfo error");
		}

		item.getValue("path",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.path = new char[len];
			memcpy(mInfo.path,out.string,len);
		}

		item.getValue("name",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.name = new char[len];
			memcpy(mInfo.name,out.string,len);
		}

		item.getValue("name_key",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.name_key = new char[len];
			memcpy(mInfo.name_key,out.string,len);
		}
		item.getValue("title",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.title = new char[len];
			memcpy(mInfo.title,out.string,len);
		}
		item.getValue("title_key",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.title_key = new char[len];
			memcpy(mInfo.title_key,out.string,len);
		}
		item.getValue("artist",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.artist = new char[len];
			memcpy(mInfo.artist,out.string,len);
		}
		item.getValue("artist_key",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.artist_key = new char[len];
			memcpy(mInfo.artist_key,out.string,len);
		}
		item.getValue("album",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.album = new char[len];
			memcpy(mInfo.album,out.string,len);
		}
		item.getValue("album_key",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.album_key = new char[len];
			memcpy(mInfo.album_key,out.string,len);
		}
		
		item.getValue("track",out);
		if(out.toIneger(&mInfo.track) == 0){
			log_i("CursorMediaInfo error");
		}

		item.getValue("img_path",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.img_path = new char[len];
			memcpy(mInfo.img_path,out.string,len);
		}
		
		item.getValue("add_time",out);
		if(out.toIneger(&mInfo.add_time) == 0){
			//log_i("CursorMediaInfo error");
		}

		item.getValue("duration",out);
		if(out.toIneger(&mInfo.duration) == 0){
			log_i("CursorMediaInfo error");
		}
		item.getValue("inPlay",out);
		if(out.toIneger(&mInfo.inPlay) == 0){
			log_i("CursorMediaInfo error");
		}
		item.getValue("times",out);
		if(out.toIneger(&mInfo.times) == 0){
			log_i("CursorMediaInfo error");
		}
		item.getValue("iscue",out);
		if(out.toIneger(&mInfo.isCue) == 0){
			//log_i("CursorMediaInfo error");
		}
		item.getValue("cuestart",out);
		if(out.toIneger(&mInfo.cueStart) == 0){
			//log_i("CursorMediaInfo error");
		}
		item.getValue("md5",out);
		if(out != NULL){
			len = strlen(out.string)+1;
			mInfo.md5 = new char[len];
			memcpy(mInfo.md5,out.string,len);
		}
	}

};

