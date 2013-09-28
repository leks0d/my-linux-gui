namespace mango
{	
	class ArrayMediaInfo{

		private:
			mediainfo *mplaylist;
			int len;
			int mMax;
			int sortBy;
		public:
			ArrayMediaInfo();
			~ArrayMediaInfo();
			void addMediaInfo(mediainfo *info);
			mediainfo* getMediaInfo(int pos);
			int getCount(){return len;}
			void sort(int sortby = 0);
			bool needSortByTrack();
			int compare(mediainfo *first,mediainfo *end,int sortby);
			void clear();
			ArrayMediaInfo& operator =(ArrayMediaInfo& info);
	};
	
	typedef ArrayMediaInfo* ARRAYMINFO;
}
