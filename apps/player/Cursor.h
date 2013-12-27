namespace mango
{
	class CursorItem{
		public:
			CStringArray mKey;
			CStringArray mValue;
			int mLen;
		public:
			CursorItem();
			~CursorItem();
			void addItem(CString& key,CString& value);
			void addItem(const char* key,const char* value);
			void addItem(const char* key,int value);
			void setValue(const char* key,CString& value);
			void setValue(const char* key,const char* value);
			void setValue(const char* key,int value);
			bool getValue(const char* key,CString& outValue);
			void clearAll();
			bool removeItem(const char* key);
			CursorItem& operator=(CursorItem& cursorItem);
	};
	
	class Cursor{
		public:
			CursorItem* mList;
			int mMax;
		public:
			int mLen;
		public:
			Cursor();
			~Cursor();
			int addCursorItem(CursorItem& item);
			bool getValueCstring(int i,const char* key,CString& outCStr);
	};
	class CursorMediaInfo{
		public:
			mediainfo mInfo;
			CursorMediaInfo();
			~CursorMediaInfo();
			void setCursorItem(CursorItem& cursorItem);
	};

};


