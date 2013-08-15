#include "player.h"
#include <linux/android_alarm.h>


namespace mango
{

#define RTC_WAKEUP_MASK 0x1

	void AlarmManager::initialize(){
		int fd;

		fd = open("/dev/alarm", O_RDWR);

		if(fd>0){
			mFd = fd;
		}

		mSeekBarUpdateThread.create(AlarmManager::AlarmManagerRunnig, this);
	}

	void AlarmManager::setAlarmWakeup(unsigned int time){
		struct timeval	tv;
		
		gettimeofday(&tv, NULL);
		
		ts.tv_sec = tv.tv_sec + time;
		ts.tv_nsec = 0;

		int result = ioctl(mFd, ANDROID_ALARM_SET(ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP), &ts);
		if(result < 0){
			log_i("set rtc fail!");
		}else{
			log_i("set rtc sucess :time = %d",time);
		}
	}
	void AlarmManager::cancelAlarm(){
		struct timeval	tv;
		
		gettimeofday(&tv, NULL);
		
		if(ts.tv_sec > tv.tv_sec){
			int result = ioctl(mFd, ANDROID_ALARM_CLEAR(ANDROID_ALARM_RTC_WAKEUP), &ts);
			if(result < 0){
				log_i("cancel rtc fail!");
			}else{
				log_i("cancel rtc sucess");
			}
		}
	}
	unsigned int AlarmManager::AlarmManagerRunnig(void *parameter){
		AlarmManager *alarm = (AlarmManager *)parameter;
		alarm->AlarmLoop();
	}

	void AlarmManager::AlarmLoop(){
		int result;
		
		while(1){
			do
			{
				log_i("AlarmLoop ioctl ANDROID_ALARM_WAIT");
				result = ioctl(mFd, ANDROID_ALARM_WAIT);
			} while (result < 0 && errno == EINTR);

			if (result < 0)
			{
		        log_i("Unable to wait on alarm: %s", strerror(errno));
				result = 0;
		    }else{
				log_i("wait a alarm: result = %d",result);
			}

			 if ((result & RTC_WAKEUP_MASK) != 0){
				log_i("wait a alarm: RTC_WAKEUP");
			 }
		}
	}
	
	AlarmManager *gAlarmManager = NULL;
};
