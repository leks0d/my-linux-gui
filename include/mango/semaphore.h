#ifndef _MANGO_SEMAPHORE_H
#define _MANGO_SEMAPHORE_H

namespace mango 
{
	class EXPORT_CLASS Semaphore; 

	class Semaphore : public Object
	{
	public:
		Semaphore(void);
		Semaphore(LPCTSTR lpName, int initialCount = 0);
		virtual ~Semaphore(void);

	public:
		int  getValue();

		int  wait();
		int  trywait();
		int  post();

		int  release() { return post();};
		void reset();

		void up()	 { release(); }
		void down()	 { wait(); }

	protected:

#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		int			mCount;
		Mutex		mMutex;
		Condition	mCondition;

#elif defined(LINUX_THREAD)
		void  *mSemaphore;	
#elif defined(WIN32)
		HANDLE			  mSemaphore;
		CRITICAL_SECTION  mCsection;
		int				  mCount;
#else
		sem_t  mNoNameSemaphore;
		sem_t  *mSemaphore;	
#endif

	} ;

}

#endif