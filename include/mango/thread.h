
#ifndef _MANGO_THREAD_H
#define _MANGO_THREAD_H

namespace mango 
{

#ifndef WIN32
#define THREAD_BASE_PRIORITY_LOWRT  15  // value that gets a thread to LowRealtime-1
#define THREAD_BASE_PRIORITY_MAX    2   // maximum thread base priority boost
#define THREAD_BASE_PRIORITY_MIN    -2  // minimum thread base priority boost
#define THREAD_BASE_PRIORITY_IDLE   -15 // value that gets a thread to idle

#define THREAD_PRIORITY_LOWEST          THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL    (THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL          0
#define THREAD_PRIORITY_HIGHEST         THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL    (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN    (0x7fffffff)
#define THREAD_PRIORITY_TIME_CRITICAL   THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE            THREAD_BASE_PRIORITY_IDLE
#endif

#ifdef WIN32
#define SCHED_OTHER 0
#define SCHED_FIFO  1
#define SCHED_RR    2
#endif
	class EXPORT_CLASS Thread; 

	typedef unsigned int (*THREAD_START_ROUTINE)(void*);

	class Thread :	public Object
	{
	public:
		Thread(void);
		virtual ~Thread(void);

	private:

#ifdef WIN32
		HANDLE mThread;
#endif
		DWORD mId;

		//List<TTHREAD_TVIEW_ITEM> mViewList;
		//MessageQueue*			  mMsgQueue;

	public:
		unsigned int create(THREAD_START_ROUTINE startAddress, void* param);
		unsigned int open(DWORD id);

		BOOL createMsgQueue();
		BOOL isExistMsgQueue();
		BOOL destoryMsgQueue();

		void close();
		void exit(DWORD exitCode);
		int  join();

		BOOL setPriority(int priority);
		int  getPriority();

		DWORD getId();

		static class Thread* getCurrent();
		static DWORD getCurrentId();
		static void sleep(DWORD milliseconds);

		static void  setCurrentName(const char *name);
		static const char* getCurrentName(void);

		static void print_sched(int pid);
		static void set_sched(int pid, int policy, int priority);
		
		static BOOL setCPUfreqPerformance(BOOL performance);  
	private:
		static int  mCPUfreqPerformanceCount;
		static Mutex mCPUfreqMutex;
		static void setCPUfreqPolicy(const char* policy);

	} ;

}

#endif