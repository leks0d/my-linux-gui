
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

	static const int utf8_2h_mask = 0xE0;
	static const int utf8_2h_mask_ret = 0xC0;
	static const int utf8_3h_mask = 0xF0;
	static const int utf8_3h_mask_ret = 0xE0;	
	static const int utf8_l_mask = 0xC0;
	static const int utf8_l_mask_ret = 0x80;
	
#if 0	
	int Charset::isTextUtf8(char *s){
	
		int len = strlen(s);
		int i,count;
		int ret = 0;

		log_i("Charset::isTextUtf8 len=%d,%s",len,s);
		
		
		if(len>=2){
			count = len/2;
			for(i=0;i<count&&i<3;i++){
				log_i("s[%d]=0x%x,s[%d]=0x%x",i*2,s[i*2],i*2+1,s[i*2+1]);
				if((s[i*2]&utf8_2h_mask)==utf8_2h_mask_ret && (s[i*2+1]&utf8_l_mask)==utf8_l_mask_ret){
					ret = 1;
				}else{
					ret = 0;
					break;
				}
			}
			count = len/3;
			for(i=0;i<count&&i<2;i++){
				log_i("s[%d]=0x%x,s[%d]=0x%x,s[%d]=0x%x",i*3,s[i*3],i*3+1,s[i*3+1],i*3+2,s[i*3+2]);
				if((s[i*3]&utf8_3h_mask)==utf8_3h_mask_ret && 
					(s[i*3+1]&utf8_l_mask)==utf8_l_mask_ret && 
					(s[i*3+2]&utf8_l_mask)==utf8_l_mask_ret){
					ret = 1;
				}else{
					ret = 0;
					break;
				}
			}
			log_i("isTextUtf8 ret = %d",ret);
			return ret;
		}else
			return 0;
		
	}
#else
int Charset::isTextUtf8(const char * lpstrInputStream){   
    int   i;   
    int cOctets;  // octets to go in this UTF-8 encoded character   
    char chr;   
    int  bAllAscii = 1;   
  	int iLen;

	if(lpstrInputStream == NULL)
		return false;
		
  	iLen = strlen(lpstrInputStream);
    cOctets= 0;
	
	//log_i("Charset::isTextUtf8 len=%d,%s",iLen,lpstrInputStream);
	
    for( i=0; i < iLen; i++ ){
        chr= *(lpstrInputStream+i); 
		
  		//log_i("isTextUtf8 chr[%d]=x%x,cOctets=%d",i,chr,cOctets);
  
        if( (chr&0x80) != 0 ) bAllAscii = 0;   
  		
        if( cOctets == 0 ){
            //   
            // 7 bit ascii after 7 bit ascii is just fine.  Handle start of encoding case.   
            //   
            if(chr >= 0x80){
               //   
               // count of the leading 1 bits is the number of characters encoded   
               //   
               do {
               	  cOctets++; 
                  chr <<= 1;   
               }while( (chr&0x80) != 0);
  
               cOctets--;                       // count includes this character   
               if( cOctets == 0 ) return 0;  // must start with 11xxxxxx   
            }   
        }else{   
            // non-leading bytes must start as 10xxxxxx   
            if( (chr&0xC0) != 0x80){
                return 0;   
            }
            cOctets--;                           // processed another octet in encoding   
        } 
    } 

    //   
    // End of text.  Check for consistency.   
    //   
  
    if( cOctets > 0 ){   // anything left over at the end is an error   
        return 0;   
    }   
  
    if( bAllAscii ){     // Not utf-8 if all ascii.  Forces caller to use code pages for conversion   
        return 0;   
    }   
  
    return 1;   
}  
#endif	


//																				
//以减少代码为目标																
//																				
//																				
//页码表格式说明：																
//前128*2 (不包含0x00 -- 0x7f，大多数语言的编码都与ASCII 谦容) 或者256*2 个字节，
//为单字节编码对应的UNICODE 码值， 可以称作“前导码值区”；						
//后128*256*2 或者256*256*2 个字节, 为双字节编码对应的UNICODE 码值, 为“后码值区”
//																				
//查询方式：																	
//先依照单字节编码查“前码值区” 对应的UNICODE 码值, 如果为0xfffe, 则需结合后面的字节， 查询“后码值区”。
#define CODEPAGE_SPECIAL_UNICODE  0xfffe										
																		
//定义页码表 ID														
#define CODEPAGE_936		0  //936   (ANSI/OEM - 简体中文 GBK)
																		
//定义“前导码值区”字(二个字节)数												
#define NUMBER_OF_LEAD		256  //0x00 -- 0xff								
																				
//定义内码到 “后码值区”的偏移量												
#define SECOND_CODE_OFFSET(uiPri, uiSec)  ((NUMBER_OF_LEAD + 256 * uiPri + uiSec) * 2)

	int Charset::multiByteToWideChar(UINT CodePage, LPCSTR lpMultiByteStr, int cchMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
	{
		unsigned int	uiPrimary, uiSecond ;									
		unsigned short  uiCode ;		

		int				iCharNum = 0 ;	

		if (CodePage == CP_UTF8)
		{
			return utf8_mbstowcs (lpWideCharStr, (const BYTE*)lpMultiByteStr, cchWideChar) ;
		}

		for ( ; cchMultiByte > 0 && cchWideChar > 0 ; cchMultiByte--, cchWideChar--, iCharNum++)
		{

			uiPrimary = (unsigned int)(unsigned short)(unsigned char)(*lpMultiByteStr) ;

			uiCode = gSessionLocal.mLanguageCodePage[uiPrimary] ;
			if (uiCode == CODEPAGE_SPECIAL_UNICODE)									
			{																		
				if (cchMultiByte < 2)												
					break ;															

				uiSecond = (unsigned int)(unsigned short)(unsigned char)(*(lpMultiByteStr + 1)) ;

				*lpWideCharStr = gSessionLocal.mLanguageCodePage[SECOND_CODE_OFFSET(uiPrimary, uiSecond)/2] ;

				lpMultiByteStr += 2	;												
				lpWideCharStr++     ;												
				cchMultiByte--      ;	
			}																		
			else																	
			{																		
				*lpWideCharStr++ = (WCHAR)(unsigned char)(*lpMultiByteStr++) ;
			}																		
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

	int  Charset::WideCharToChar(char*s,WCHAR *d){
		int    i,count;
		
		count = strlen(s);
		for (i = 0 ; i < 80 && (*s) && count ; i++, count--) {
			d[i] =  *s ;
			s++ ;
		}
		d[i] = '\0' ;

		return 0;
	}

}

