
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
			static void space_info(char *path,__u32& toatl,__u32& avail,__u32& free);
			static void memSizeToStr(int size, char *space);
			static __u32 getSdcardAvailSpace();
			static int getSdcardCid(char *cid);
			static int updateSDcard();
			static bool isSDcardChange();
			static void recovery();
			static void install();
			static void reboot();
			static void openMute();
			static void sync();
			static bool sdcardNeedScanner();
			static unsigned long get_file_size(const char *path);
			static bool isSDcardExist();
			static void MD5(char* data,CString& out);
	};
	
};
