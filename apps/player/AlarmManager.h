
namespace mango
{

	class AlarmManager{
		int mFd;
		struct timespec ts;
		Thread mSeekBarUpdateThread;
	public:	
		AlarmManager(){
			mFd = 0;
		}
		
		void initialize();
		
		void setAlarmWakeup(unsigned int time);
		void cancelAlarm();

		static unsigned int AlarmManagerRunnig(void *parameter);

		void AlarmLoop();
	};
	extern AlarmManager *gAlarmManager;
};
