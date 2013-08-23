
namespace mango
{

	class PowerManager
	{	
		int sleepTime;
		int atuoCount;
		int poweroffTime;
		int atuoPoweroffCount;
		int isSleep;
	public:
		PowerManager();
		void PowerManagerCount();
		void resetCount();
		void setPowerState();
		void setPowerState(int n);
		void setAutoSleepTime(int time);
		void setAutoPoweroffTime(int index);
		int getAutoSleepTime();
		int getPoweroffTime();
	};
	extern PowerManager *gPowerManager;
};
