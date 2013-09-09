#include "mango.h"

namespace mango 
{

#ifndef WIN32
	typedef void* (*PTHREAD_START_ROUTINE)(void*);
#endif


	Thread::Thread(void)
	{
#ifdef WIN32
		mThread = NULL;
#endif
		mId = -1;
//		mMsgQueue = NULL;
	}


	Thread::~Thread(void)
	{
		close();
	}


	unsigned int Thread::create(THREAD_START_ROUTINE startAddress, void* parameter)
	{
		close();

#ifdef WIN32
		mThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)startAddress, parameter, 0, &mId);
		SetThreadPriority(mThread, THREAD_PRIORITY_TIME_CRITICAL);
#else
		int		  res ; 
		pthread_t		threadId ;
		//pthread_attr_t	attr ;

		//pthread_attr_init(&attr);
		//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    //分离 -- 线程结束时自己释放资源   
		//res = pthread_create (&threadId, &attr, (PTHREAD_START_ROUTINE)startAddress, parameter);

		//分离, 导致后面 用pthread_join 出时， 导致scanf 出错。
		res = pthread_create(&threadId, NULL, (PTHREAD_START_ROUTINE)startAddress, parameter);
		log_d ("res = %d threadId = %d\n", res, threadId);

		mId = threadId ;
#endif
		return mId;
	}


	unsigned int Thread::open(DWORD id)
	{
		close();

#ifdef WIN32
		mThread = OpenThread(0, FALSE, id);
		mId = id;
#else
		mId = id;
#endif
		return mId;
	}


	void Thread::close()
	{
#ifdef WIN32
		if (mThread != NULL)
		{
			CloseHandle(mThread);
			mThread = NULL;
		}
#endif

//		if (mMsgQueue)
//		{
//			delete mMsgQueue;
//			mMsgQueue = NULL;
//		}
	}


	void Thread::exit(DWORD exitCode)
	{
#ifdef WIN32
		if (mId == GetCurrentThreadId ())
		{
			ExitThread(exitCode);
		}
		else
		{
			TerminateThread(mThread, exitCode);
		}
#else
		int retVal;

		if ((pthread_t)mId == pthread_self())
			pthread_exit(&retVal);
		else
		{
			log_e("android bionic not implemented  'pthread_cancel' ");
			//pthread_cancel((pthread_t)mId);
		}
#endif
		mId = -1;
	}

	int Thread::join()
	{
#ifdef WIN32
		WaitForSingleObject(mThread,  INFINITE);
#else
		pthread_join(mId, NULL);
#endif
		mId = -1;
		return 0;
	}

	//#define SCHED_OTHER	0
	//#define SCHED_FIFO	1
	//#define SCHED_RR	2
	//E:\Ubuntu\bsp\kernel\linux-2.6.28\kernel\sched.c
	//static const int prio_to_weight[40]
	//分时 SCHED_OTHER (20 至 -19)
	const int conturnip_sched_ts_pri_map[] = 
	{5,		2,		1,	 0,		-1,		-2,   -5} ;
	//655	820	 1024	1277 	1586

	//实时 (1 - 99)
	const int conturnip_sched_rt_pri_map[] = 
	{1,		4,	 6,		8,		10,  12,  15} ;



	BOOL Thread::setPriority(int priority)
	{
#ifdef WIN32

		return SetThreadPriority(mThread, priority);
#else

		struct sched_param	thread_sched_param ;
		int			policy ;
		pthread_t	ptid ;
		int			ret  ;
		const int	*ppri_map ;
		int			ipri ;


		ptid = (pthread_t)mId;
		ret  = pthread_getschedparam(ptid, &policy, &thread_sched_param);
		if (ret != 0)
		{
			log_e ("pthread_getschedparam failed ptid=%d ", ptid);
			return FALSE ;
		}

		if (policy == SCHED_OTHER)
		{
			log_e ("thread (%d)'s policy is not realtime ", ptid);
			return FALSE ;
		}

		ppri_map = conturnip_sched_rt_pri_map ;
		ipri	 = ppri_map[3] ;

		switch (priority)
		{
		case THREAD_PRIORITY_IDLE:
			ipri = ppri_map[0] ;
			break ;

		case THREAD_PRIORITY_LOWEST:
			ipri = ppri_map[1] ;
			break ;

		case THREAD_PRIORITY_BELOW_NORMAL:
			ipri = ppri_map[2] ;
			break ;

		case THREAD_PRIORITY_NORMAL:
			ipri = ppri_map[3] ;
			break ;

		case THREAD_PRIORITY_ABOVE_NORMAL:
			ipri = ppri_map[4] ;
			break ;

		case THREAD_PRIORITY_HIGHEST:
			ipri = ppri_map[5] ;
			break ;

		case THREAD_PRIORITY_TIME_CRITICAL:
			ipri = ppri_map[6] ;
			break ;
		}

		thread_sched_param.sched_priority = ipri ;
		ret = pthread_setschedparam (ptid, policy, &thread_sched_param) ;
		if (ret != 0)
		{
			log_e ("pthread_setschedparam failed ptid=%d ", ptid) ;
			return FALSE ;
		}
		else
			return TRUE ;
#endif
	}


	int  Thread::getPriority()
	{
#ifdef WIN32
		return GetThreadPriority(mThread);
#else

		struct sched_param	thread_sched_param ;
		int			policy ;
		pthread_t	ptid ;
		int			ret  ;
		const int	*ppri_map ;
		int			iWndPriority ;
		int			iPriority ;


		ptid = (pthread_t)mId;
		ret = pthread_getschedparam (ptid, &policy, &thread_sched_param);
		if (ret != 0)
		{
			log_e ("pthread_getschedparam failed ptid=%d ", ptid);
			return THREAD_PRIORITY_ERROR_RETURN ;
		}

		if (policy == SCHED_OTHER)
		{
			log_e ("thread (%d)'s policy is not realtime ", ptid);
			return THREAD_PRIORITY_ERROR_RETURN ;
		}

		ppri_map	 = conturnip_sched_rt_pri_map;
		iWndPriority = THREAD_PRIORITY_ERROR_RETURN;

		iPriority = thread_sched_param.sched_priority;

		if (iPriority <= ppri_map[0])
			iWndPriority = THREAD_PRIORITY_IDLE ;
		else if (iPriority <= ppri_map[1])
			iWndPriority = THREAD_PRIORITY_LOWEST ;
		else if (iPriority <= ppri_map[2])
			iWndPriority = THREAD_PRIORITY_BELOW_NORMAL ;
		else if (iPriority <= ppri_map[3])
			iWndPriority = THREAD_PRIORITY_NORMAL ;
		else if (iPriority <= ppri_map[4])
			iWndPriority = THREAD_PRIORITY_ABOVE_NORMAL ;
		else if (iPriority <= ppri_map[5])
			iWndPriority = THREAD_PRIORITY_HIGHEST ;
		else //if (iPriority <= ppri_map[6])
			iWndPriority =  THREAD_PRIORITY_TIME_CRITICAL ;

		return iWndPriority ;
#endif
	}


	DWORD Thread::getId()
	{
		return mId;
	}


	Thread* Thread::getCurrent()
	{
		Thread *thread;

		thread = new Thread;

#ifdef WIN32
		thread->mThread = GetCurrentThread();
		thread->mId     = GetCurrentThreadId();
#else
		thread->mId  = (DWORD)pthread_self () ;
#endif
		return thread;
	}


	DWORD Thread::getCurrentId()
	{
		DWORD id;

#ifdef WIN32
		id     = GetCurrentThreadId();
#else
		id  = (DWORD)pthread_self () ;
#endif

		return id;
	}



	void Thread::sleep(DWORD milliseconds)
	{
#ifdef WIN32
/*
		static HANDLE hEvent = NULL;
		if (hEvent == NULL)
			hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		DWORD time ;
		DWORD endTime;

		endTime = time + milliseconds;
		while (time < endTime)
		{
			timeSetEvent(endTime - time, (LPTIMECALLBACK)hEvent, NULL, TIME_ONESHOT | TIME_CALLBACK_EVENT_PULSE);
			WaitForSingleObject(hEvent, time - endTime);
			time = timeGetTime();
		}
*/
		Sleep (milliseconds) ;
#else
		//经实测nanosleep, usleep 具有阻塞效果, 并会让出cpu, nanosleep(0), usleep(0) 至少延时一个tick 大约10ms 
		//select 会阻塞, 但会造成100% 占cpu, 相当循环一样, 不会让出cpu, select(0) 会立即返回, 但不会重新调度
		//所以选用nanosleep 
		//struct timeval tv ;
		struct timespec ts;

		//printf ("Sleep %d ", dwMilliseconds) ;
		/*
		if (dwMilliseconds == 0)
		{
		//不理想
		sched_yield () ;
		return ;
		}
		*/
		//if (dwMilliseconds == 0)
		//{
		//	schedule();
		//	return 0;
		//}
		ts.tv_sec  = milliseconds / 1000;
		ts.tv_nsec = (milliseconds % 1000) * 1000000;
		nanosleep(&ts, NULL) ;

		//printf ("Sleep %d End ", dwMilliseconds) ;
		//tv.tv_sec  = 0 ;
		//tv.tv_usec = dwMilliseconds * 1000 ;
		//select (0, NULL, NULL, NULL, &tv) ;
#endif
	}

#ifdef WIN32
	const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)
#endif


#ifdef WIN32
	__declspec(thread) char gThreadName[260] = "Win32 Thread"; 
#else
	//android 不支持TLS   http://blog.csdn.net/yahoozhuo/article/details/5467284 
	//强制使用 一些变量值突然改
	//__thread char gThreadName[260] = "linux thread"; 
#endif

	void Thread::setCurrentName(const char *name)
	{
#ifdef WIN32
		strcpy(gThreadName, name);
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = name;
		info.dwThreadID = GetCurrentThreadId();
		info.dwFlags = 0;

		__try
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}
#endif

	}



	//获取线程名称,获取的是函数static BOOL SetThreadName(LPCTSTR szName)设置名称。
	//DW(@TIB+0x24)  4644    线程Id 
	//(char*)(DW(@TIB+0x14)) 名称
	const char* Thread::getCurrentName(void)
	{
#ifdef WIN32
	//	char* pszName=NULL;
		return gThreadName;
#else
		return "linux thread";
#endif
#if 0
		__asm
		{
			mov eax, fs:[0x18]    //    Locate the caller's TIB
			mov eax, [eax+0x14]    //    Read the pvArbitary field in the TIB
			mov [pszName], eax    //    pszName = pTIB->pvArbitary
		}
		return pszName ? pszName : "Win32 Thread";
#endif
	}



	void Thread::print_sched(int pid)
	{
#ifndef WIN32
		struct sched_param	thread_sched_param ;
		int			policy ;
		pthread_t	ptid ;
		int			ret  ;


		log_i("Fibo MediaSerice logout_thread_sched SCHED_OTHER =%d", SCHED_OTHER);
		log_i("logout_thread_sched SCHED_FIFO  =%d", SCHED_FIFO);
		log_i("logout_thread_sched SCHED_RR    =%d", SCHED_RR);

		if (pid == -1)
			ptid = (pthread_t)pthread_self();
		else
			ptid = (pthread_t)pid;

		ret = pthread_getschedparam (ptid, &policy, &thread_sched_param);
		if (ret != 0)
		{
			log_i("print_thread_sched pthread_getschedparam failed ptid=%d ", (int)ptid);
			return ;
		}

		log_i("print_thread_sched policy    =%d", policy);
		log_i("print_thread_sched iPriority =%d", thread_sched_param.sched_priority);
#endif
	}


	void Thread::set_sched(int pid, int policy, int priority)
	{
#ifndef WIN32
		struct sched_param	thread_sched_param ;
		int			prevPolicy ;
		pthread_t	ptid ;
		int			ret  ;

		if (pid == -1)
			ptid = (pthread_t)pthread_self();
		else
			ptid = (pthread_t)pid;


		ret = pthread_getschedparam (ptid, &prevPolicy, &thread_sched_param);
		if (ret != 0)
		{
			log_i("set_thread_sched pthread_getschedparam failed ptid=%d ", (int)ptid);
			return;
		}

		thread_sched_param.sched_priority = priority; 
		ret = pthread_setschedparam(ptid, policy, &thread_sched_param); 
		if (ret != 0)
		{
			log_i("set_thread_sched pthread_setschedparam failed ptid=%d ", (int)ptid);
			return;
		}
#endif
	}

	Mutex Thread::mCPUfreqMutex;

	BOOL Thread::setCPUfreqPerformance(BOOL performance)
	{
		mCPUfreqMutex.lock();

		if (performance)
		{
		//	if (mCPUfreqPerformanceCount == 0)
			{
				setCPUfreqPolicy("performance");
			}
		//	mCPUfreqPerformanceCount++;
		}
		else
		{
		//	if (mCPUfreqPerformanceCount > 0)
			{
		//		mCPUfreqPerformanceCount--;
		//		if (mCPUfreqPerformanceCount == 0)
				{
					setCPUfreqPolicy("ondemand");
				}
			}
		}

		mCPUfreqMutex.unlock();

		return TRUE;
	}


	void Thread::setCPUfreqPolicy(const char* policy)
	{

		FILE *file;

		log_i("setCPUfreqPolicy  %s\n", policy); 

		file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "wt");
		if (file == NULL)
		{
			log_i("can't open /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor \n");
			return;
		}

		fwrite(policy, 1, strlen(policy), file);
		fclose(file);

		log_i("setCPUfreqPolicy end %s\n", policy); 
		return;

	}
}











