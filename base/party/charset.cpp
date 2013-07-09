
#include <mango.h>

namespace mango
{
	/*
	* Sample implementation from Unicode home page.
	* http://www.stonehand.com/unicode/standard/fss-utf.html
	*/
	struct utf8_table {
		int     cmask;
		int     cval;
		int     shift;
		long    lmask;
		long    lval;
	};

	static const struct utf8_table utf8_table[] =
	{
		{0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
		{0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
		{0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
		{0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
		{0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
		{0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
		{0,						       /* end of table    */}
	};


	int Charset::utf8_mbtowc(WCHAR *p, const BYTE *s, int n)
	{
		long l;
		int c0, c, nc;
		const struct utf8_table *t;

		nc = 0;
		c0 = *s;
		l = c0;
		for (t = utf8_table; t->cmask; t++) {
			nc++;
			if ((c0 & t->cmask) == t->cval) {
				l &= t->lmask;
				if (l < t->lval)
					return -1;
				*p = (WCHAR)l;
				return nc;
			}
			if (n <= nc)
				return -1;
			s++;
			c = (*s ^ 0x80) & 0xFF;
			if (c & 0xC0)
				return -1;
			l = (l << 6) | c;
		}
		return -1;
	}

	int Charset::utf8_mbstowcs(WCHAR *pwcs, const BYTE *s, int n)
	{
		WCHAR *op;
		const BYTE *ip;
		int size;

		op = pwcs;
		ip = s;
		while (*ip && n > 0) 
		{
			if (*ip & 0x80) 
			{
				size = utf8_mbtowc(op, ip, n);
				if (size == -1) 
				{
					/* Ignore character and move on */
					ip++;
					n--;
				} 
				else 
				{
					op++;
					ip += size;
					n -= size;
				}
			} 
			else 
			{
				*op++ = *ip++;
				n--;
			}
		}
		return (op - pwcs);
	}

	int Charset::utf8_wctomb(BYTE *s, wchar_t wc, int maxlen)
	{
		long l;
		int c, nc;
		const struct utf8_table *t;

		if (!s)
			return 0;

		l = wc;
		nc = 0;
		for (t = utf8_table; t->cmask && maxlen; t++, maxlen--) {
			nc++;
			if (l <= t->lmask) {
				c = t->shift;
				*s = (BYTE)(t->cval | (l >> c));
				while (c > 0) {
					c -= 6;
					s++;
					*s = 0x80 | ((l >> c) & 0x3F);
				}
				return nc;
			}
		}
		return -1;
	}

	int Charset::utf8_wcstombs(BYTE *s, const WCHAR *pwcs, int maxlen)
	{
		const WCHAR *ip;
		BYTE *op;
		int size;

		op = s;
		ip = pwcs;
		while (*ip && maxlen > 0) 
		{
			if (*ip > 0x7f) 
			{
				size = utf8_wctomb(op, *ip, maxlen);
				if (size == -1) 
				{
					/* Ignore character and move on */
					maxlen--;
				} 
				else 
				{
					op += size;
					maxlen -= size;
				}
			} 
			else 
			{
				*op++ = (BYTE) *ip;
			}

			ip++;
		}
		*op='\0';
		return (op - s);
	}



	Charset::Charset(void)
	{

	}
	

	Charset::~Charset(void)
	{

	}


	int Charset::multiByteToWideChar(UINT CodePage, LPCSTR lpMultiByteStr, int cchMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
	{
#if 0
		unsigned int	uiPrimary, uiSecond ;									
		unsigned short  uiCode ;		
#endif
		int				iCharNum = 0 ;	

		if (CodePage == CP_UTF8)
		{
			return utf8_mbstowcs (lpWideCharStr, (const BYTE*)lpMultiByteStr, cchWideChar) ;
		}

		for ( ; cchMultiByte > 0 && cchWideChar > 0 ; cchMultiByte--, cchWideChar--, iCharNum++)
		{
#if 0
			uiPrimary = (unsigned int)(unsigned short)(unsigned char)(*lpMultiByteStr) ;

			uiCode = gsys_pUserSessionObj->m_pwCodePage[uiPrimary] ;
			if (uiCode == CODEPAGE_SPECIAL_UNICODE)									
			{																		
				if (cchMultiByte < 2)												
					break ;															

				uiSecond = (unsigned int)(unsigned short)(unsigned char)(*(lpMultiByteStr + 1)) ;

				*lpWideCharStr = gsys_pUserSessionObj->m_pwCodePage[SECOND_CODE_OFFSET(uiPrimary, uiSecond)/2] ;

				lpMultiByteStr += 2	;												
				lpWideCharStr++     ;												
				cchMultiByte--      ;	
			}																		
			else																	
			{																		
				*lpWideCharStr++ = (WCHAR)(unsigned char)(*lpMultiByteStr++) ;
			}																		
#endif
		}																			

		return iCharNum ;														
	}	


	int Charset::wideCharToMultiByte(UINT CodePage, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cchMultiByte)
	{
		if (CodePage == CP_UTF8)
			return utf8_wcstombs ((BYTE *)lpMultiByteStr, lpWideCharStr, cchMultiByte) ;
		else
			return 0 ;
	}

}

