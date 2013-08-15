
namespace mango
{

	class BitmapFactory{
		
	public:	
		static bool decodeFile(MSkBitmap* mMSkBitmap,char* path,int w,int h);
		
	};

	class Environment{
		public:	
			static void space_info(char *path,int& toatl,int& avail,int& free);
			static void memSizeToStr(int size, char *space);
	};
};
