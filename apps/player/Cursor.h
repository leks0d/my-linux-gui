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
			void addItem(char* key,char* value);
			bool getValue(char* key,CString& outValue);
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
			bool getValueCstring(int i,char* key,CString& outCStr);
	};
	class CursorMediaInfo{
		public:
			mediainfo mInfo;
			CursorMediaInfo();
			~CursorMediaInfo();
			void setCursorItem(CursorItem& cursorItem);
	};

};


