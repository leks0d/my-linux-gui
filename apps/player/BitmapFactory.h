
namespace mango
{

	class BitmapFactory{
		
	public:	
		static bool decodeFile(MSkBitmap* mMSkBitmap,char* path,int w,int h);
		static bool decodeBuffer(MSkBitmap* mMSkBitmap,void* buf,int size,int w,int h);
		static bool genBitmapFromFile(MSkBitmap* mMSkBitmap,char* path,int w,int h);
		static void caputerScreen();
		static void caputerDrawScreen();
	};

	class Environment{
		public:	
			static void space_info(const char *path,__u32& toatl,__u32& avail,__u32& free);
			static void memSizeToStr(int size, char *space);
			static __u32 getSdcardAvailSpace();
			static int getSdcardCid(char *cid);
			static int updateSDcard();
			static bool isSDcardChange();
			static void recovery();
			static void install();
			static void reboot();
			static void openMute();
			static void openadb();
			static void sync();
			static void logcat();
			static unsigned int logcatRunnig(void *parameter);
			static bool sdcardNeedScanner();
			static unsigned long get_file_size(const char *path);
			static bool isSDcardExist();
			static void MD5(char* data,CString& out);
			static void checkWallpaper();
			static void drawWallpaper(Canvas& canvas);
			static void setDigitalFilter(int val);
	};
};
