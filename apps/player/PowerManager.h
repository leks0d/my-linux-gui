
namespace mango
{

	class PowerManager
	{	
		int sleepTime;
		int atuoCount;
		int isSleep;
	public:
		PowerManager();
		void PowerManagerCount();
		void resetCount();
		void setPowerState();
		void setPowerState(int n);
		void setAutoSleepTime(int time);
		int getAutoSleepTime();
	};
	extern PowerManager *gPowerManager;
};
