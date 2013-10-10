
#include "player.h"

namespace mango
{
	class CursorItem{
		private:	
			CString mKey;
			CString mValue;
		public:
			CursorItem();
			CursorItem(char* key,char* value);
			CursorItem(char* key,int val);
			void setKey(char *key){}
			void setValueString(const char* value){}
			void setValueInteger(int value){}
			CString& getKey(){return mKey;}
			CString& getValue(){return mValue;}
			CursorItem& operator=(CursorItem& item);
			~CursorItem();
	};
	
	class Cursor{
		private:
			CursorItem* mList;
			int mLen;
			int mMax;
		public:
			Cursor();
			void addItem(CursorItem& item);
			CursorItem& getItem(int index);
			int getCount();
			~Cursor();
	};
	
};


