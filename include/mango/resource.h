
#ifndef _MANGO_RESOURCE_H
#define _MANGO_RESOURCE_H

namespace mango
{

#ifndef WIN32
#define MAKEINTRESOURCEA(i) (LPSTR)((DWORD)((WORD)(i)))
#define MAKEINTRESOURCEW(i) (LPWSTR)((DWORD)((WORD)(i)))
#ifdef UNICODE
#define MAKEINTRESOURCE  MAKEINTRESOURCEW
#else
#define MAKEINTRESOURCE  MAKEINTRESOURCEA
#endif // !UNICODE
#endif

/*
 * Predefined Resource Types
 */
#define RT_BITMAP           MAKEINTRESOURCE(2)
#define RT_STRING           MAKEINTRESOURCE(6)



	//资源数据头
	typedef struct tagRESOURCEDATA_HEADER
	{
		char	m_szFlag[4] ;
		DWORD	m_dwSize ;		//结构体字节数
		DWORD   m_dwItems ;		//资源条数
		DWORD	m_dwFileBytes ; //资源数据文件总长度

		DWORD   m_dwOffsetOfItem ; //条目偏移量
		DWORD   m_dwOffsetOfData ; //数据偏移量	

		DWORD	m_dwStringStartItem ; //字符串开始ITEM(标识符)值
		DWORD	m_dwLanguages ;   //字符串的语言数

	} RESOURCEDATA_HEADER, *PRESOURCEDATA_HEADER ;

	//资源标识
	#define RESOURCEDATA_FLAG_STR    "RESD"

	//资源数据条目
	typedef struct tagRESOURCEDATA_ITEM
	{
		DWORD  m_dwOffset ;  //相当资源文件头的偏移量
		DWORD  m_dwBytes ;	 //资源字节数, 字符串包括字符串结束符
		DWORD  m_dwType ;	 //资源类型	

	}RESOURCEDATA_ITEM, *PRESOURCEDATA_ITEM ;

	//资源类型 
	//#define RESOURCEDATA_NULL		0
	//#define RESOURCEDATA_STRING		1
	//#define RESOURCEDATA_IMAGE		2
	//#define RESOURCEDATA_DATA		3

	//为加快转换速度, 暂定最高条目数为10 万, 已完全足够了
	#define RESOURCEDATA_MAX_ITEMS   (100*1000)

	//为加快转换速度, 暂定最大资源字节数200MB, 已完全足够了
	#define RESOURCEDATA_MAX_BYTES   (200*1024*1024)

	#define RESOURCE_TEXT_MAX_CHARS		256


	typedef struct tagRESOURCEIMAGE_HEADER
	{
		int  m_iWidth  ;
		int  m_iHeight ;
		WORD m_wType   ;
		WORD m_wBitCount ;

	} RESOURCEIMAGE_HEADER, *PRESOURCEIMAGE_HEADER ;

	
	//图片资源类型
	#define RESOURCEIMAGE_NULL 0
	#define RESOURCEIMAGE_PNG  1

	#define RESOURCE_MAX_BYTES  (SCREEN_CX * SCREEN_CY * 4 + 32)

	class Resource
	{
	public:
		Resource();
		~Resource();

	public:
		bool open(const char *path);
		void close(void);


		int loadString(int id,  TCHAR* buffer,  int bufferSize, int language = 1);
		int loatBitmap(int id,  PRESOURCEIMAGE_HEADER  pResImageHeader,  void* buffer, int bufferBytes); 


	public:
		FILE*				mFile;
		RESOURCEDATA_HEADER	mHeader;
	};

}

#endif