
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



	//��Դ����ͷ
	typedef struct tagRESOURCEDATA_HEADER
	{
		char	m_szFlag[4] ;
		DWORD	m_dwSize ;		//�ṹ���ֽ���
		DWORD   m_dwItems ;		//��Դ����
		DWORD	m_dwFileBytes ; //��Դ�����ļ��ܳ���

		DWORD   m_dwOffsetOfItem ; //��Ŀƫ����
		DWORD   m_dwOffsetOfData ; //����ƫ����	

		DWORD	m_dwStringStartItem ; //�ַ�����ʼITEM(��ʶ��)ֵ
		DWORD	m_dwLanguages ;   //�ַ�����������

	} RESOURCEDATA_HEADER, *PRESOURCEDATA_HEADER ;

	//��Դ��ʶ
	#define RESOURCEDATA_FLAG_STR    "RESD"

	//��Դ������Ŀ
	typedef struct tagRESOURCEDATA_ITEM
	{
		DWORD  m_dwOffset ;  //�൱��Դ�ļ�ͷ��ƫ����
		DWORD  m_dwBytes ;	 //��Դ�ֽ���, �ַ��������ַ���������
		DWORD  m_dwType ;	 //��Դ����	

	}RESOURCEDATA_ITEM, *PRESOURCEDATA_ITEM ;

	//��Դ���� 
	//#define RESOURCEDATA_NULL		0
	//#define RESOURCEDATA_STRING		1
	//#define RESOURCEDATA_IMAGE		2
	//#define RESOURCEDATA_DATA		3

	//Ϊ�ӿ�ת���ٶ�, �ݶ������Ŀ��Ϊ10 ��, ����ȫ�㹻��
	#define RESOURCEDATA_MAX_ITEMS   (100*1000)

	//Ϊ�ӿ�ת���ٶ�, �ݶ������Դ�ֽ���200MB, ����ȫ�㹻��
	#define RESOURCEDATA_MAX_BYTES   (200*1024*1024)

	#define RESOURCE_TEXT_MAX_CHARS		256


	typedef struct tagRESOURCEIMAGE_HEADER
	{
		int  m_iWidth  ;
		int  m_iHeight ;
		WORD m_wType   ;
		WORD m_wBitCount ;

	} RESOURCEIMAGE_HEADER, *PRESOURCEIMAGE_HEADER ;

	
	//ͼƬ��Դ����
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