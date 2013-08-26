
#ifndef _MANGO_FILE_H
#define _MANGO_FILE_H

namespace mango
{

#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  


	class File : public Object
	{
	public:
		File(void);
		virtual ~File(void);

	public:
		bool findOpen(TCHAR* path);
		bool findClose();
		bool findNext(TCHAR* path, int *attribute);
		bool findNext(TCHAR* path, int *attribute,char *cName);
		static bool isDirect(char* file);
		static TCHAR* pathAddBackslash(TCHAR* path);
		static bool  pathMatchSpec(LPCTSTR pszFile, LPCTSTR pszSpec,DWORD dwFlags);
		static TCHAR* pathFindFileName(TCHAR* path);
		static bool pathRemoveFileSpec(TCHAR* path);

	public:

#ifdef WIN32
		HANDLE  mFind;
		WIN32_FIND_DATA  mFindData;
		int    mNextCount;
#else
	    DIR    *mDir;
		char   mPath[MAX_PATH];
#endif

	};

}

#endif