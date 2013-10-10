
#include "player.h"

namespace mango
{
	CursorItem::CursorItem(){
		mKey = NULL;
		mValue = NULL;
	}
	CursorItem::CursorItem(char* key,char* value){
		mKey = key;
		mValue = value;
	}
	CursorItem::CursorItem(char* key,int val){
		mKey = key;
		mValue = val;
	}
	CursorItem& CursorItem::operator=(CursorItem& item){
		mKey = item.getKey();
		mValue = item.getValue();
		return *this;
	}
	Cursor::Cursor(){
		mList = NULL;
		mMax=mLen=0;
	}
	void Cursor::addItem(CursorItem& item){
		if(mLen>=mMax){
			CursorItem *temp;
			int i,count;
			
			if(mMax == 0){
				mMax = 8;
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
};


