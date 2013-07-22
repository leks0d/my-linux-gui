namespace mango
{	
	class ArrayMediaInfo{

		public:
			mediainfo *mplaylist;
			int len;
			int mMax;
		
			ArrayMediaInfo();
			~ArrayMediaInfo();
			void addMediaInfo(mediainfo *info);
			mediainfo* getMediaInfo(int pos);
			void clear();
			ArrayMediaInfo& operator =(ArrayMediaInfo& info);
	};
	
	typedef ArrayMediaInfo* ARRAYMINFO;
}
