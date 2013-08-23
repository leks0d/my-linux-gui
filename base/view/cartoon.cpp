
#include "mango.h"

namespace mango
{


	static const BYTE conGearshiftGreater[20] =  //
	{
		17,  18,  18,  19,  20,  21,  21,  22,
		23,  23,  24,  25,  25,  26,  26,  27,
		27,  28,  28,  29,
	} ; 

	static const BYTE conGearshiftLess[100] =  //
	{
		0,   1,   2,   2,   3,   3,   4,   4,
		4,   5,   5,   5,   5,   6,   6,   6,
		6,   7,   7,   7,   7,   7,   8,   8,
		8,   8,   8,   9,   9,   9,   9,   9,
		9,   9,  10,  10,  10,  10,  10,  10,
		10,  11,  11,  11,  11,  11,  11,  11,
		11,  12,  12,  12,  12,  12,  12,  12,
		12,  13,  13,  13,  13,  13,  13,  13,
		13,  13,  14,  14,  14,  14,  14,  14,
		14,  14,  14,  15,  15,  15,  15,  15,
		15,  15,  15,  15,  15,  15,  16,  16,
		16,  16,  16,  16,  16,  16,  16,  16,
		16,  17,  17,  17,
	} ;


	Cartoon::Cartoon(void)
	{

	}

	Cartoon::~Cartoon(void)
	{

	}


	int Cartoon::getGearshift (int destShift, int viewSize)
	{
		int     speed ;
		int		shift, maxShift;
		int		absDestShift;
		int		gearShift;

		maxShift  = viewSize / 3;
		gearShift = maxShift;

		absDestShift = abs(destShift) ;
		if (absDestShift >= gearShift * 3)
		{
			shift = absDestShift / 3;
			shift = min(maxShift, shift);
		}
		else if (absDestShift >= gearShift)
		{
			speed = absDestShift * 10 / gearShift - 10;
			shift = absDestShift * ((int)conGearshiftGreater[speed]) / 100;
		}
		else
		{
			speed = absDestShift * 100 / gearShift ;
			shift = absDestShift * ((int)conGearshiftLess[speed]) / 100;

			if (absDestShift && shift < 8)
			{
				shift = min (8, absDestShift);
			}
		}

		if (destShift < 0)
			shift = 0 - shift;

		return shift ;
	}



	//变速移动动画
	void Cartoon::gearshift(View* view, int destShift, int viewSize, PGEARSHIFTPROCESS  pfnGearshiftProcess, void * lParam)
	{
		//
		int     speed ;
		int		shift, maxShift ;
		int		absDestShift ;
		int		gearShift ;
		DWORD	dwTickCount = 0 ;
		DWORD   dwCurTickCount ;


		maxShift   = viewSize / 3 ;
		gearShift  = maxShift     ;


		while (destShift)
		{

			absDestShift = abs (destShift) ;

			if (absDestShift >= gearShift * 3)
			{
				shift = absDestShift / 3 ;
				shift = min (maxShift, shift) ;
			}
			else if (absDestShift >= gearShift)
			{
				speed = absDestShift * 10 / gearShift - 10 ;
				shift = absDestShift * ((int)conGearshiftGreater[speed]) / 100 ;
			}
			else
			{
				speed = absDestShift * 100 / gearShift ;
				shift = absDestShift * ((int)conGearshiftLess[speed]) / 100 ;

				if (absDestShift && shift < 5)
				{
					shift = min (5, absDestShift) ;
				}
			}

			if (destShift < 0)
				shift = 0 - shift  ;

			destShift -= shift ;

			if (!pfnGearshiftProcess (view, 0, shift, lParam))
				break ;

			dwCurTickCount = Time::getMillisecond() ;
			if  (dwCurTickCount  < dwTickCount + CARTOON_MS_PER_FRAME) 
				Thread::sleep(dwTickCount + CARTOON_MS_PER_FRAME - dwCurTickCount) ;

			dwTickCount = Time::getMillisecond() ;
		} ;

		while (Time::getMillisecond() - dwTickCount < CARTOON_MS_PER_FRAME) ;
		dwTickCount = Time::getMillisecond();
	}

};

