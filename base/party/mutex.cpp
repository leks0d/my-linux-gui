#include "mango.h"

namespace mango 
{
	Mutex::Mutex(void)
	{
#ifdef WIN32
		mMutex = CreateMutex(NULL, FALSE, NULL);
#else
		pthread_mutex_init(&mMutex, NULL);
#endif
	}


	Mutex::Mutex(TCHAR* name)
	{
#ifdef WIN32
		mMutex = CreateMutex(NULL, FALSE, name);
#else
		pthread_mutex_init(&mMutex, NULL);
#endif
	}


	Mutex::~Mutex(void)
	{
#ifdef WIN32
		if (mMutex != NULL)
		{
			CloseHandle(mMutex);
			mMutex = NULL;
		}
#else
		pthread_mutex_destroy(&mMutex);
#endif
	}


	void Mutex::unlock()
	{
#ifdef WIN32
		ReleaseMutex(mMutex);
#else
		pthread_mutex_unlock(&mMutex);
#endif
	}


	void Mutex::lock()
	{
#ifdef WIN32
		WaitForSingleObject(mMutex, INFINITE);
#else
		pthread_mutex_lock(&mMutex);
#endif
	}
}

