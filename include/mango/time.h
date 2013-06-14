
#ifndef _MANGO_TIME_H

namespace mango
{
	class Time : public Object
	{
	public:
		Time(void);
		virtual ~Time(void);

		//����΢���ʱ, �����ֻ��п� 40 �� 50 ���ػ�������� ��Ҫ 64 λ��ʱ.
		static ULONGLONG getMicrosecond();

		//���غ����ʱ
		static inline ULONGLONG getMillisecond()
		{
			return getMicrosecond() / 1000;
		}
	};
};


#endif