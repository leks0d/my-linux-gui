/*++

Copyright (c) 2005-2013  Wisky

Module Name:

    type

Abstract:
     
	

Environment:
    

Notes:

   Copyright (c) 2005-2011 Wisky.  All Rights Reserved.


Revision History:

  2010-12-09 Created by Fibo Lu

--*/

#ifndef _MANGO_TYPE_H
#define _MANGO_TYPE_H

#define UNICODE

#ifndef MANGO_VERSION
#define MANGO_VERSION 0x0101
#endif //! MANGO_VERSION

#ifndef BASETYPES
#define BASETYPES
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#endif  //!BASETYPES 


#define MAX_PATH          (260*3)

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif


typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL				*PBOOL;
typedef BOOL				*LPBOOL;
typedef BYTE				*PBYTE;
typedef BYTE				*LPBYTE;
typedef int					*PINT;
typedef int					*LPINT;
typedef WORD				*PWORD;
typedef WORD				*LPWORD;
typedef long				*LPLONG;
typedef DWORD				*PDWORD;
typedef DWORD				*LPDWORD;
typedef void				*LPVOID;
typedef const void			*LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef void *PVOID;

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#endif

#ifdef WIN32
#else	//LINUX--GCC
#define _WCHAR_T_DEFINED
typedef long long int __int64 ;
typedef unsigned long long ULONGLONG;
#endif

typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef const WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;

typedef const WCHAR *LPCWSTR, *PCWSTR;

//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef const CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef const CHAR *LPCSTR, *PCSTR;

//
// Neutral ANSI/UNICODE types and macros
//
#ifdef  UNICODE                     // r_winnt

#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif // !_TCHAR_DEFINED 


typedef LPWSTR LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR LPCTSTR;
typedef LPWSTR LP;
#define __TEXT(quote) L##quote      

#else   /* UNICODE */               // r_winnt

#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPCH LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR PCTSTR, LPCTSTR;
#define __TEXT(quote) quote         // r_winnt

#endif /* UNICODE */  

#define TEXT(quote) __TEXT(quote)   

typedef SHORT *PSHORT;  
typedef LONG *PLONG;    

typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

typedef HANDLE *PHANDLE;
typedef void *HKEY;

#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif

#ifdef __cplusplus
#ifdef WIN32
#define EXPORT /*extern "C"*/ __declspec (dllexport)
#else
#define EXPORT /*extern "C"*/
#endif
#else
#ifdef WIN32
#define EXPORT __declspec (dllexport)
#else
#define EXPORT
#endif
#endif


typedef char	CCHAR;          
typedef DWORD	LCID;         
typedef PDWORD	PLCID;       
typedef WORD	LANGID;      

#define MINCHAR     0x80        
#define MAXCHAR     0x7f        
#define MINSHORT    0x8000      
#define MAXSHORT    0x7fff      
#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  
#define MAXBYTE     0xff        
#define MAXWORD     0xffff      
#define MAXDWORD    0xffffffff  

#define INFINITE    0xffffffff


#ifndef EXPORT_CLASS
#ifdef WIN32
#ifdef _MANGO_EXPORTS
#define EXPORT_CLASS _declspec(dllexport)
#define EXPORT_API   _declspec(dllexport)
#else
#define EXPORT_CLASS _declspec(dllimport)
#define EXPORT_API   _declspec(dllimport)
#endif
#else
#define EXPORT_CLASS 
#define EXPORT_API   
#endif  
#endif 


#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */



typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef DWORD   COLORREF;
typedef DWORD   *LPCOLORREF;

typedef int LRESULT;

#endif // _PARTICLE_TYPE_H 