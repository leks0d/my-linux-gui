
#ifndef _MANGO_DEFINE_H
#define _MANGO_DEFINE_H


#ifndef EXPORT_CLASS
#ifdef WIN32
#ifdef  MANGO_EXPORTS  
#define EXPORT_CLASS //_declspec(dllexport)
#define EXPORT_API   //_declspec(dllexport)
#else
#define EXPORT_CLASS //_declspec(dllimport)
#define EXPORT_API   //_declspec(dllimport)
#endif
#else
#define EXPORT_CLASS 
#define EXPORT_API   
#endif
#endif


#ifndef safeFree
#define safeFree(x)		\
    if (NULL != x)      \
    {                   \
        free(x) ;		\
        x = NULL;       \
    }
#endif

#ifndef safeDelete
#define safeDelete(x)		\
    if (NULL != x)      \
    {                   \
        delete x;		\
        x = NULL;       \
    }
#endif


#ifndef arrSize		/* 取得数组的元素个数 */
#define arrSize(a)	(sizeof(a)/sizeof(a[0]))
#endif


#ifndef WIN32
//FIBO #define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#endif

//FIBO
#define RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))

#define ARGB(a,r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))

#define COLOR_ORANGE RGB(255,149,0)
#define COLOR_GRAY RGB(180,180,180)
#define COLOR_WHITE RGB(255,255,255)
#define COLOR_BLACK RGB(0,0,0)


#ifdef WIN32
#define PATH_BACKSLASH  '\\'
#else
#define PATH_BACKSLASH '/'
#endif

#endif
