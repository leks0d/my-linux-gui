
#ifndef _MANGO_TIME_H

namespace mango
{
	class Time : public Object
	{
	public:
		Time(void);
		virtual ~Time(void);

		//返回微秒计时, 考虑手机有可 40 至 50 不关机的情况， 需要 64 位计时.
		static ULONGLONG getMicrosecond();

		//返回毫秒计时
		static inline ULONGLONG getMillisecond()
		{
			return getMicrosecond() / 1000;
		}
	};
};


#endif