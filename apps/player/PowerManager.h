
namespace mango
{
	#define FORCE_POWEROFF_FILE "/mnt/time"
	static int write_int(char const *path, int value)
	{
		int fd;
		static int already_warned;
	
		already_warned = 0;
	
		log_i("write_int: path %s, value %d", path, value);
		fd = open(path, O_RDWR|O_CREAT);
	
		if (fd >= 0) {
			char buffer[20];
			int bytes = sprintf(buffer, "%d\n", value);
			int amt = write(fd, buffer, bytes);
			close(fd);
			return amt == -1 ? -errno : 0;
		} else {
			if (already_warned == 0) {
				log_e("write_int failed to open %s\n", path);
				already_warned = 1;
			}
			return -errno;
		}
	}
	static int read_int(char const *path){
		int fd;
		int result = -1;
		
		fd = open(path, O_RDWR);
		
		if (fd >= 0) {
			int len = 20;
			char buffer[len];
			
			int amt = read(fd, buffer, len);
			int ret = sscanf(buffer,"%d",&result);
			
			close(fd);
			
			if(ret == 0)
				result = -1;
		}
		
		log_i("path %s,result=%d", path,result);

		return result;
	}

	class PowerManager
	{	
	public:	
		int sleepTime;
		int atuoCount;
		int poweroffTime;
		int atuoPoweroffCount;		
		int mForcePowerOffTime;
		int mForcePowerOffTimeCount;
		int isSleep;
	
		PowerManager();
		void PowerManagerCount();
		void initAutoSleepTime();
		void resetCount();
		void setPowerState();
		void setPowerState(int n);
		void setAutoSleepTime(int time);
		void setAutoPoweroffTime(int index);
		void setForcePoweroffTime(int index);
		void writeTime(int time);
		int readTime();
		int getPowerState(){return isSleep;}
		int getAutoSleepTime();
		int getPoweroffTime();
		int getForcePoweroffTime();
	};
	extern PowerManager *gPowerManager;
};
