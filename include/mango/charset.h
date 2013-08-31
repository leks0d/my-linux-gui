
#ifndef _MANGO_CHARSET_H
#define _MANGO_CHARSET_H

namespace mango
{

//
//  Code Page Default Values.
//
#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

//”Ô—‘

#define LANGID_SIMPLIFIED  0x0000
#define LANGID_ENGLISH     0x0001
#define LANGID_TRADITIONAL 0x0002
#define LANGID_JAPANESE    0x0003

	class Charset : public Object
	{
	public:
		Charset(void);
		virtual ~Charset(void);

	public:
		static int utf8_mbtowc(WCHAR *p, const BYTE *s, int n);
		static int utf8_mbstowcs(WCHAR *pwcs, const BYTE *s, int n);
		static int utf8_wctomb(BYTE *s, wchar_t wc, int maxlen);
		static int utf8_wcstombs(BYTE *s, const WCHAR *pwcs, int maxlen);
		static int isTextUtf8(const char *s);
		static int WideCharToChar(char*s,WCHAR *d);
		static int multiByteToWideChar(UINT CodePage, LPCSTR lpMultiByteStr, int cchMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
		static int wideCharToMultiByte(UINT CodePage, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cchMultiByte);
	};
}

#endif