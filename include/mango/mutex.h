#ifndef _MANGO_MUTEX_H
#define _MANGO_MUTEX_H

namespace mango 
{
	class EXPORT_CLASS Mutex;

	class Mutex : public Object
	{
	public:
		Mutex(void);
		Mutex(TCHAR* name);
		virtual ~Mutex(void);

		friend class Condition;

#ifdef WIN32
	private:
		HANDLE	mMutex;
#else
	public:
		pthread_mutex_t mMutex;
#endif

	public:
		void unlock();
		void lock();
	};
}

#endif