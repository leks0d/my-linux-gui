
namespace mango
{

	class BitmapFactory{
		
	public:	
		static bool decodeFile(MSkBitmap* mMSkBitmap,char* path,int w,int h);
		static bool decodeBuffer(MSkBitmap* mMSkBitmap,void* buf,int size,int w,int h);
		static bool genBitmapFromFile(MSkBitmap* mMSkBitmap,char* path,int w,int h);
	};

	class Environment{
		public:	
			static void space_info(char *path,int& toatl,int& avail,int& free);
			static void memSizeToStr(int size, char *space);
			static void recovery();
			static void install();
			static void reboot();
			static void openMute();
	};
};
