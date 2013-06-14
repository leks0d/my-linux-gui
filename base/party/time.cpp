#include "mango.h"

namespace mango
{

	Time::Time(void)
	{

	}


	Time::~Time(void)
	{

	}


	ULONGLONG Time::getMicrosecond()
	{
#ifdef WIN32
		DWORD		millisecond;
		ULONGLONG	microsecond;

		millisecond = timeGetTime();

		microsecond = millisecond;
		microsecond = microsecond * 1000;

		return microsecond;
#else
		struct timeval	tv;
		ULONGLONG		microsecond;

		gettimeofday(&tv, NULL);

		microsecond = (ULONGLONG)(tv.tv_sec) * 1000000 + tv.tv_usec;

		return microsecond;

#endif
	}
};
