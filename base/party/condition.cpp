#include "mango.h"

namespace mango 
{

	Condition::Condition(void)
	{
#ifdef WIN32
		_gone       = 0;
		_blocked    = 0;
		_waiting    = 0;
		_gate       = ::CreateSemaphore(0, 1, 1, 0);
		_queue      = ::CreateSemaphore(0, 0, LONG_MAX, 0);
		_mutex      = ::CreateMutex(0, 0, 0);
#else
		pthread_cond_init(&mCond, NULL);
#endif
	}


	Condition::~Condition(void)
	{
#ifdef WIN32
		::CloseHandle(_gate);
		::CloseHandle(_queue);
		::CloseHandle(_mutex);
#else
		pthread_cond_destroy(&mCond);
#endif
	}


	int Condition::broadcast()
	{
#ifdef WIN32
		unsigned signals = 0;

		int res = 0;
		res = ::WaitForSingleObject(_mutex, INFINITE);
		_ASSERT(res == WAIT_OBJECT_0);

		if (_waiting != 0) // the m_gate is already closed
		{
			if (_blocked == 0)
			{
				res = ::ReleaseMutex(_mutex);
				_ASSERT(res);
				return true; // xupei
			}

			_waiting += (signals = _blocked);
			_blocked = 0;
		}
		else
		{
			res = ::WaitForSingleObject(_gate, INFINITE);
			_ASSERT(res == WAIT_OBJECT_0);
			if (_blocked > _gone)
			{
				if (_gone != 0)
				{
					_blocked -= _gone;
					_gone = 0;
				}
				signals = _waiting = _blocked;
				_blocked = 0;
			}
			else
			{
				res = ::ReleaseSemaphore(_gate, 1, 0);
				_ASSERT(res);
			}
		}

		res = ::ReleaseMutex(_mutex);
		_ASSERT(res);

		if (signals)
		{
			res = ::ReleaseSemaphore(_queue, signals, 0);
			_ASSERT(res);
		}
#else
		pthread_cond_broadcast(&mCond);
#endif
		return true;
	}


	int Condition::signal()
	{
#ifdef WIN32
		unsigned signals = 0;

		int res = 0;
		res = ::WaitForSingleObject(_mutex, INFINITE);
		_ASSERT(res == WAIT_OBJECT_0);

		if (_waiting != 0) // the m_gate is already closed
		{
			if (_blocked == 0)
			{
				res = ::ReleaseMutex(_mutex);
				_ASSERT(res);
				return true; // xupei
			}

			++_waiting;
			--_blocked;
			signals = 1;
		}
		else
		{
			res = ::WaitForSingleObject(_gate, INFINITE);
			_ASSERT(res == WAIT_OBJECT_0);
			if (_blocked > _gone)
			{
				if (_gone != 0)
				{
					_blocked -= _gone;
					_gone = 0;
				}
				signals = _waiting = 1;
				--_blocked;
			}
			else
			{
				res = ::ReleaseSemaphore(_gate, 1, 0);
				_ASSERT(res);
			}
		}

		res = ::ReleaseMutex(_mutex);
		_ASSERT(res);

		if (signals)
		{
			res = ::ReleaseSemaphore(_queue, signals, 0);
			_ASSERT(res);
		}
#else
		pthread_cond_signal(&mCond);
#endif
		return true;
	}


	int  Condition::wait(Mutex *mutex,  unsigned int milliseconds)
	{
#ifdef WIN32
		DWORD timeTest = GetTickCount();
		DWORD timeOut ;

		timeOut = GetTickCount() + milliseconds;

		::WaitForSingleObject(_gate, INFINITE);
		_blocked++;
		::ReleaseSemaphore(_gate, 1, 0);

		mutex->unlock();

		bool ret = false;
		unsigned int res = 0;

		for (;;)
		{
			//    int milliseconds;
			//     to_duration(timeout, milliseconds);

			res = ::WaitForSingleObject(_queue, milliseconds);
			_ASSERT(res != WAIT_FAILED && res != WAIT_ABANDONED);
			ret = (res == WAIT_OBJECT_0);

			if (res == WAIT_TIMEOUT)
			{
				if (GetTickCount() < timeOut)
					continue;

				// xtime cur;
				// moxu::xtime::now(cur);
				// if (xtime_cmp(timeout, cur) > 0)
				//      continue;
			}

			break;
		}

		unsigned was_waiting=0;
		unsigned was_gone=0;

		res = ::WaitForSingleObject(_mutex, INFINITE);
		_ASSERT(res == WAIT_OBJECT_0);
		was_waiting = _waiting;
		was_gone = _gone;
		if (was_waiting != 0)
		{
			if (!ret) // timeout
			{
				if (_blocked != 0)
					--_blocked;
				else
					++_gone; // count spurious wakeups
			}
			if (--_waiting == 0)
			{
				if (_blocked != 0)
				{
					res = ::ReleaseSemaphore(_gate, 1, 0); // open m_gate
					_ASSERT(res);
					was_waiting = 0;
				}
				else if (_gone != 0)
					_gone = 0;
			}
		}
		else if (++_gone == (UINT_MAX) / 2)
		{
			res = ::WaitForSingleObject(_gate, INFINITE);
			_ASSERT(res == WAIT_OBJECT_0);
			_blocked -= _gone;
			res = ::ReleaseSemaphore(_gate, 1, 0);
			_ASSERT(res);
			_gone = 0;
		}
		res = ::ReleaseMutex(_mutex);
		_ASSERT(res);

		if (was_waiting == 1)
		{
			for (; was_gone; --was_gone)
			{
				// better now than spurious later
				res = ::WaitForSingleObject(_queue, INFINITE);
				_ASSERT(res ==  WAIT_OBJECT_0);
			}
			res = ::ReleaseSemaphore(_gate, 1, 0);
			_ASSERT(res);
		}


		mutex->lock();

		timeTest = GetTickCount() - timeTest ;

		if (timeTest > 500)
		{
			//DebugBreak();
		}

		if (ret)
			return 0;
		else
			return 1;
#else

		int ret;

		if (milliseconds == INFINITE)
		{
			ret = pthread_cond_wait(&mCond, &mutex->mMutex);
		}
		else
		{


			struct timeval  now;
			struct timespec timeout;

			gettimeofday(&now, 0);
			
			timeout.tv_sec  = now.tv_sec + milliseconds / 1000;
			timeout.tv_nsec = now.tv_usec * 1000 + (milliseconds % 1000) * 1000000; //²»»á³¬¹ý 2Ãë
			if (timeout.tv_nsec > 1000000000)
			{
				timeout.tv_nsec -= 1000000000;
				timeout.tv_sec++;
			}

			ret = pthread_cond_timedwait(&mCond, &mutex->mMutex, &timeout);
		}

		return ret;
#endif
	}

}

