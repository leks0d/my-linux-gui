#include "mango.h"

namespace mango 
{

	Semaphore::Semaphore(void)
	{
#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mCount = 0;
#elif defined(WIN32)
		int count = 0;

		mSemaphore = CreateSemaphore(NULL, 0, MAXSHORT, NULL);
		::InitializeCriticalSection(&mCsection);
		mCount = count ;
#else
		mSemaphore = NULL;
		sem_init(&mNoNameSemaphore, 0, 0);
		mSemaphore = &mNoNameSemaphore;
#endif
	}


	Semaphore::Semaphore(LPCTSTR lpName, int initialCount)
	{
#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		if (lpName)
		{
			//没有实现
			_ASSERT(0);
		}
		mCount = initialCount;
#elif defined(WIN32)
		int count = initialCount;

		mSemaphore = CreateSemaphore(NULL, initialCount, MAXSHORT, lpName);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseSemaphore(mSemaphore, 1, (LPLONG)&count);
			WaitForSingleObject(mSemaphore, INFINITE);
		}

		::InitializeCriticalSection(&mCsection);
		mCount = count ;
#else
		if (lpName)
		{
			mSemaphore = sem_open ((char *)lpName, O_CREAT, 0x666, initialCount);
			log_e("android bionic not implemented %d 'sem_open'", 2);
		}
		else
		{
			mSemaphore = NULL;
			sem_init(&mNoNameSemaphore, 0, initialCount);
			mSemaphore = &mNoNameSemaphore;
		}
#endif
	}


	Semaphore::~Semaphore(void)
	{
#ifdef SEMAPHORE_BY_MUTEX_CONDITION

#elif defined(WIN32)
		if (mSemaphore)
		{
			CloseHandle (mSemaphore);
			::DeleteCriticalSection(&mCsection);
		}
		mSemaphore = NULL;	
#else
		if (mSemaphore)
			sem_destroy(mSemaphore);
		mSemaphore = NULL;	
#endif
	}


	int Semaphore::wait()
	{
		int value;
#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mMutex.lock();
		while(mCount == 0)
		{
			mCondition.wait(&mMutex);
		}
		mCount--;
		value = mCount;
		mMutex.unlock();

#elif defined(WIN32)
		
		_ASSERT(mSemaphore);
		value = WaitForSingleObject(mSemaphore, INFINITE);
		::EnterCriticalSection(&mCsection);
		mCount--;
		::LeaveCriticalSection(&mCsection);
#else
		_ASSERT(mSemaphore);
		value = sem_wait(mSemaphore);
#endif
		return value;
	}


	int Semaphore::trywait()
	{
		int value;
#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mMutex.lock();
		if (mCount > 0)
		{
			mCount--;
			value = 0;
		}
		else
			value = 1 ;

		mMutex.unlock();

		return value;

#elif defined(WIN32)
		_ASSERT(mSemaphore);
		::EnterCriticalSection(&mCsection);
		value = WaitForSingleObject(mSemaphore, 0);
		if (value == WAIT_TIMEOUT)
		{
			::LeaveCriticalSection(&mCsection);
			return 1;
		}

		mCount--;
		::LeaveCriticalSection(&mCsection);

#else
		_ASSERT(mSemaphore);
		value = sem_trywait(mSemaphore);
		if (value != 0)
			return 1;
#endif
		return 0;
	}


	int Semaphore::post()
	{
		int count;

#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mMutex.lock();
		mCount++;
		count = mCount;
		mCondition.signal();
		mMutex.unlock();

#elif defined(WIN32)
		_ASSERT(mSemaphore);
		::EnterCriticalSection(&mCsection);
		ReleaseSemaphore(mSemaphore, 1, (LPLONG)&count);
		mCount++;
		count = mCount;
		::LeaveCriticalSection(&mCsection);
#else
		_ASSERT(mSemaphore);
		count = sem_post (mSemaphore);
#endif

		return count;
	}


	int Semaphore::getValue()
	{
		int count;

#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mMutex.lock();
		count = mCount;
		mMutex.unlock();
#elif defined(WIN32)
		_ASSERT(mSemaphore);
		::EnterCriticalSection(&mCsection);
		count = mCount;
		::LeaveCriticalSection(&mCsection);
#else
		_ASSERT(mSemaphore);
		sem_getvalue(mSemaphore, &count);
#endif

		return count;
	}


	void Semaphore::reset()
	{
		int value;
#ifdef SEMAPHORE_BY_MUTEX_CONDITION
		mMutex.lock();
		mCount = 0;
		value  = mCount;
		mMutex.unlock();
#elif defined(WIN32)
		_ASSERT(mSemaphore);
		::EnterCriticalSection(&mCsection);

		do
		{
			value = WaitForSingleObject(mSemaphore, 0);

			if (value == WAIT_OBJECT_0)
				mCount--;

		} while (value == WAIT_OBJECT_0);

		_ASSERT(mCount == 0);
		::LeaveCriticalSection(&mCsection);

#else
		_ASSERT(mSemaphore);
		do
		{
			value = sem_trywait(mSemaphore);
		} while (value == 0);
#endif
	}

}

