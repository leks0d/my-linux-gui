
#ifndef _MANGO_STRING_H

namespace mango
{
	class String 
	{
	public:
		String(void);
		virtual ~String(void);

	public:
	static int lstrcmp (LPCTSTR lpString1, LPCTSTR lpString2);
	static int lstrcmpi (LPCTSTR lpString1,  LPCTSTR lpString2);
	static int lstrcmpin (LPCTSTR lpString1,  LPCTSTR lpString2, int iCount);
	static LPTSTR  copy (LPTSTR lpString1, LPCTSTR lpString2);
	static LPTSTR  lstrcpyn (LPTSTR lpString1, LPCTSTR lpString2, int iMaxLength);
	static LPTSTR  lstrcat(LPTSTR lpString1, LPCTSTR lpString2);
	static int  lstrlen (LPCTSTR lpString);

	static TCHAR * litot( int value, TCHAR *string, int radix);
	static int lttoi(const TCHAR *str);
	static TCHAR * litotime(int value, TCHAR *string);

	public:

	};
}




#endif