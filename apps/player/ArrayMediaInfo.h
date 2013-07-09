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

	};
	
	typedef ArrayMediaInfo* ARRAYMINFO;
}
