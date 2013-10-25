
namespace mango
{

	class PowerManager
	{	
	public:	
		int sleepTime;
		int atuoCount;
		int poweroffTime;
		int atuoPoweroffCount;
		int isSleep;
	
		PowerManager();
		void PowerManagerCount();
		void initAutoSleepTime();
		void resetCount();
		void setPowerState();
		void setPowerState(int n);
		void setAutoSleepTime(int time);
		void setAutoPoweroffTime(int index);
		int getPowerState(){return isSleep;}
		int getAutoSleepTime();
		int getPoweroffTime();
	};
	extern PowerManager *gPowerManager;
};
