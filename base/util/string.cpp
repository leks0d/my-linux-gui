
#include "mango.h"

namespace mango
{
	String::String(void)
	{
	}

	String::~String(void)
	{

	}
	//Compares two character strings. The comparison is case-sensitive
	int String::lstrcmp (LPCTSTR lpString1, LPCTSTR lpString2)
	{
		while (*lpString1 &&  *lpString2)
		{
			if (*lpString1 != *lpString2)
				break ;

			lpString1++ ;
			lpString2++ ;
		}

		return (int)(*lpString1 - *lpString2) ;
	}


	//Compares two character strings. The comparison is not case-sensitive
	int String::lstrcmpi (LPCTSTR lpString1,  LPCTSTR lpString2)
	{
		TCHAR a1, a2 ;

		do
		{
			a1 = *lpString1 ;
			a2 = *lpString2 ;

			if (a1 >= 'A' && a1 <= 'Z')
				a1 = a1 + 'a' - 'A' ;

			if (a2 >= 'A' && a2 <= 'Z')
				a2 = a2 + 'a' - 'A' ;

			if (a1 != a2)
				break ;
			else if (a1 == 0 || a2 == 0)
				break ;

			lpString1++ ;
			lpString2++ ;

		} while (1) ;

		return (int)(a1 - a2) ;
	}



	//Compares two character strings. The comparison is not case-sensitive
	int String::lstrcmpin (LPCTSTR lpString1,  LPCTSTR lpString2, int iCount)
	{
		TCHAR a1 = 0, a2 = 0;

		while (iCount > 0)
		{
			iCount-- ;

			a1 = *lpString1 ;
			a2 = *lpString2 ;

			if (a1 >= 'A' && a1 <= 'Z')
				a1 = a1 + 'a' - 'A' ;

			if (a2 >= 'A' && a2 <= 'Z')
				a2 = a2 + 'a' - 'A' ;

			if (a1 != a2)
				break ;
			else if (a1 == 0 || a2 == 0)
				break ;

			lpString1++ ;
			lpString2++ ;

		} 

		return (int)(a1 - a2) ;
	}


	//This function copies a string to a buffer
	LPTSTR  String::copy (LPTSTR lpString1, LPCTSTR lpString2)
	{
		LPTSTR lptstr = lpString1 ;

		while (*lpString2)
		{
			*lpString1 = *lpString2 ;

			lpString1++ ;
			lpString2++ ;
		}

		*lpString1 = *lpString2 ;

		return lptstr ;
	}

	//This function copies a specified number of characters from 
	//a source string into a buffer.
	LPTSTR  String::lstrcpyn (LPTSTR lpString1, LPCTSTR lpString2, int iMaxLength)
	{
		LPTSTR lptstr = lpString1 ;

		while (*lpString2 && (iMaxLength > 1))
		{
			*lpString1 = *lpString2 ;

			lpString1++ ;
			lpString2++ ;
		}

		*lpString1 = *lpString2 ;

		return lptstr ;
	}


	//This function appends one string to another.
	LPTSTR  String::lstrcat(LPTSTR lpString1, LPCTSTR lpString2)
	{
		LPTSTR lptstr = lpString1 ;

		while (*lpString1)
		{
			lpString1++ ;
		}

		while (*lpString2)
		{
			*lpString1 = *lpString2 ;

			lpString1++ ;
			lpString2++ ;
		}

		*lpString1 = *lpString2 ;

		return lptstr ;
	}

	//This function returns the length 
	int  String::lstrlen (LPCTSTR lpString)
	{
		int iLength = 0 ;

		while (*lpString)
		{
			iLength++ ;
			lpString++ ;
		}

		return iLength ;
	}


	TCHAR * String::litot( int value, TCHAR *string, int radix)
	{
		BOOL	bNegative   ;
		int		iTChars  = 0 ;
		TCHAR	*pTChar = string ;
		TCHAR   tch ;

		if (value < 0)
		{
			bNegative = TRUE  ;
			value	  = 0 - value ;
			*pTChar++ = '-' ;
		}
		else
			bNegative = FALSE ;

		do
		{
			*pTChar++ = (value % radix) + '0' ;
			value	  = value / radix ;

			iTChars++ ;

		} while (value) ;

		*pTChar = '\0' ;
		pTChar -= iTChars ;

		while (iTChars > 1)
		{
			tch      = *pTChar ;
			*pTChar  = *(pTChar + iTChars - 1) ;
			*(pTChar + iTChars - 1) = tch ;

			iTChars -= 2 ;
			pTChar++ ;
		}

		return string ;
	}


	int String::lttoi(const TCHAR *str)
	{
		BOOL	bNegative = FALSE ;
		int		iValue = 0 ;

		if (*str == '-')
		{
			bNegative = TRUE ;
			str++ ;
		}

		while ((*str >= '0') && (*str <= '9'))
		{
			iValue *= 10 ;
			iValue += (*str - '0') ; 
			str++ ;
		}

		if (bNegative)
			iValue = 0 - iValue ;

		return iValue ;
	}


	TCHAR * String::litotime(int value, TCHAR *string)
	{
		int hour    ;
		int minuter ;
		int second  ;
		TCHAR	*ptchar = string ;

		hour    = value / 3600 ;
		value  -= hour * 3600 ;

		minuter = value / 60 ;
		value  -= minuter * 60 ;

		second  = value ;

		if (hour < 1000)
		{
			if (hour > 100)
			{
				*ptchar++ = hour / 100 + '0' ;
				hour = hour % 100 ;
			}
			*ptchar++ = hour / 10 + '0' ;
			*ptchar++ = hour % 10 + '0' ;
		}
		else
		{
			litot (hour, ptchar, 10) ;
			ptchar += lstrlen (ptchar) ;
		}

		*ptchar++ = ':' ;

		*ptchar++ = minuter / 10 + '0' ;
		*ptchar++ = minuter % 10 + '0' ;
		*ptchar++ = ':' ;

		*ptchar++ = second / 10 + '0' ;
		*ptchar++ = second % 10 + '0' ;
		*ptchar++ = '\0' ;

		return string ;
	}
};