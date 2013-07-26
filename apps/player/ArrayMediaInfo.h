namespace mango
{	
	class ArrayMediaInfo{

		private:
			mediainfo *mplaylist;
			int len;
			int mMax;
		public:
			ArrayMediaInfo();
			~ArrayMediaInfo();
			void addMediaInfo(mediainfo *info);
			mediainfo* getMediaInfo(int pos);
			int getCount(){return len;}
			void clear();
			ArrayMediaInfo& operator =(ArrayMediaInfo& info);
	};
	
	typedef ArrayMediaInfo* ARRAYMINFO;
}
