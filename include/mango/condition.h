#ifndef _MANGO_CONDITION_H
#define _MANGO_CONDITION_H

namespace mango 
{
#ifdef WIN32
	class EXPORT_CLASS Condition; 
	class EXPORT_CLASS Mutex;
#else
	class Mutex;
#endif

	class Condition : public Object
	{
	public:
		Condition(void);
		virtual ~Condition(void);

	private:

#ifdef WIN32
		HANDLE _gate;
		HANDLE _queue;
		HANDLE _mutex;
		unsigned _gone;
		unsigned long _blocked;
		unsigned _waiting;
#else
	public:
		pthread_cond_t mCond;
#endif

	public:
		int broadcast();
		int signal();

		int  wait(Mutex *mutex,  unsigned int milliseconds = -1);
	};
}

#endif