
#include <mango.h>

namespace mango
{

	File::File(void)
	{
#ifdef WIN32
		mFind = INVALID_HANDLE_VALUE;
#else
		mDir = NULL;;
#endif
	}


	File::~File(void)
	{
		findClose();
	}

	bool File::findOpen(TCHAR* path)
	{
#ifdef WIN32
		TCHAR buffer[MAX_PATH];

		lstrcpy(buffer, path);
		lstrcat(buffer, TEXT ("\\*.*")) ;

		mNextCount = 0;
		mFind = FindFirstFile (buffer, &mFindData); 
		if (mFind != INVALID_HANDLE_VALUE)
			return true;
		else
			return false;
#else
		TCHAR  tcPath[MAX_PATH];
		char   szPath[MAX_PATH];
		int attribute;
		int bytes;


		bytes = Charset::wideCharToMultiByte(CP_UTF8, path, String::lstrlen(path), szPath, MAX_PATH);
		if (bytes >= 0)
			szPath[bytes] = '\0';


		log_i ("Actual path = %s\n", szPath) ;
		mDir = opendir (szPath) ;
		if (mDir == NULL)
		{
			log_e ("Can't open dir %s \n", szPath) ;
			return false;
		}
		strcpy (mPath, szPath) ;

		if (!findNext(tcPath, &attribute))
		{
			log_e ("Can't find files in dir(%s) \n", szPath) ;
			closedir (mDir) ;
			mDir = NULL; 
			return false ;
		}

		return true;
#endif
	}


	bool File::findClose()
	{
#ifdef WIN32
		if (mFind != INVALID_HANDLE_VALUE)
			FindClose(mFind);

		mFind = INVALID_HANDLE_VALUE;
#else
		if (mDir)
			closedir (mDir) ;

		mDir = NULL;;
#endif
		return true;
	}


	bool File::findNext(TCHAR* path, int *attribute)
	{
#ifdef WIN32

		if (mNextCount == 0)
			mNextCount = 1;
		else if(!FindNextFile (mFind, &mFindData))
			return false;

		lstrcpy (path, mFindData.cFileName);
		*attribute = mFindData.dwFileAttributes;
		return true;
#else
		struct   dirent   *s_dir;   
		char   szFile[MAX_PATH] ;
		struct stat64 statbuf ;
		int charCount;

		s_dir = readdir(mDir) ;
		if (s_dir == NULL)
			return false;

		//log_i ("name %s \n", s_dir->d_name) ;

		strcpy(szFile, mPath) ;
		strcat(szFile, "/") ;
		strcat(szFile, s_dir->d_name) ;

		//log_i ("fulll name %s \n", s_dir->d_name) ;

		if (stat64 (szFile, &statbuf) == -1)
		{
			log_e("Get stat Error:%s\n",  s_dir->d_name) ;
			return false;
		}


		charCount = Charset::multiByteToWideChar(CP_UTF8, s_dir->d_name, strlen(s_dir->d_name), path, MAX_PATH);
		if (charCount >= 0)
			path[charCount] = '\0';

		if (S_ISDIR(statbuf.st_mode))
			*attribute = FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_NORMAL ;
		else
			*attribute = FILE_ATTRIBUTE_NORMAL ;

		return true;
#endif
	}

		bool File::findNext(TCHAR* path, int *attribute,char *cName)
		{
			struct	 dirent   *s_dir;	
			char   szFile[MAX_PATH] ;
			struct stat64 statbuf ;
			int charCount;
	
			s_dir = readdir(mDir) ;
			if (s_dir == NULL)
				return false;
	
			//log_i ("name %s \n", s_dir->d_name) ;
	
			strcpy(szFile, mPath) ;
			strcat(szFile, "/") ;
			strcat(szFile, s_dir->d_name) ;

			strcpy(cName,s_dir->d_name);
			//log_i ("fulll name %s \n", s_dir->d_name) ;
	
			if (stat64 (szFile, &statbuf) == -1)
			{
				log_e("Get stat Error:%s\n",  s_dir->d_name) ;
				return false;
			}
	
			charCount = Charset::multiByteToWideChar(CP_UTF8, s_dir->d_name, strlen(s_dir->d_name), path, MAX_PATH);
			if (charCount >= 0)
				path[charCount] = '\0';
	
			if (S_ISDIR(statbuf.st_mode))
				*attribute = FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_NORMAL ;
			else
				*attribute = FILE_ATTRIBUTE_NORMAL ;

			return true;
		}
	bool File::isDirect(char* file){
		struct stat64 statbuf ;
		
		if (stat64 (file, &statbuf) == -1)
		{
			log_e("Get stat Error:%s\n",file) ;
			return false;
		}
		//log_i("statbuf.st_mode = 0x%x,file:%s",file);
		return S_ISDIR(statbuf.st_mode);
	}


	TCHAR* File::pathAddBackslash(TCHAR* path)
	{
		int i ;

		i = String::lstrlen (path) -1 ;
		i = max (i, 0) ;

		if (path[i] != PATH_BACKSLASH)
		{
			path[i + 1] = PATH_BACKSLASH ;
			path[i + 2] = '\0' ;
		}
		return path;
	}


	//判断文件名, 是否符合指定类型
	bool File::pathMatchSpec(LPCTSTR pszFile, LPCTSTR pszSpec,DWORD dwFlags)
	{
		bool bMatch = false ;
		int iCharsOfSpec = 0 ;
		int iCharsOfFile = 0 ;
		LPCTSTR  ptName    ; 
		LPCTSTR  ptPostfix ;
		BOOL	 bExistPostFix = FALSE ; 
		TCHAR a1, a2 ;

		ptName    = pszFile + String::lstrlen (pszFile) ;

		//找到后缀
		ptPostfix = ptName ;
		while (ptName >  pszFile)
		{
			ptName-- ;
			if (*ptName == '.' && (!bExistPostFix))
			{
				ptPostfix     = ptName + 1 ;
				bExistPostFix = TRUE ;
			}
			else if (*ptName == PATH_BACKSLASH)
				break ;
		}

		ptName  = pszFile ;

		//匹配, 后缀
		while (*pszSpec)
		{
			if (*pszSpec != '.')
			{
				pszSpec++ ;
				continue  ;
			}
			else if (*pszSpec == '.')
			{
				pszSpec++ ;
				ptName = ptPostfix ;

				if (*pszSpec == '*')
				{
					bMatch = TRUE ;
					break ;
				}

				while ((*pszSpec) && (*pszSpec != ';') && (*ptName))
				{
					a1 = *pszSpec ;
					a2 = *ptName ;

					if (a1 >= 'A' && a1 <= 'Z')
						a1 = a1 + 'a' - 'A' ;

					if (a2 >= 'A' && a2 <= 'Z')
						a2 = a2 + 'a' - 'A' ;

					if (a1 != a2)
						break ;

					pszSpec++ ;
					ptName++ ;
				}

				if (((*pszSpec == '\0') || (*pszSpec == ';')) && (*ptName == '\0'))
				{
					bMatch = true ;
					break ;
				}
			}
		}

		return bMatch ;
	}


	TCHAR* File::pathFindFileName(TCHAR* path)
	{
		int i ;

		i = String::lstrlen (path) ;
		if (i == 0)
			return (TCHAR*)path ;

		path += (i - 1) ;
		for ( ; i > 0 ; i--, path--) {
			if (*path == PATH_BACKSLASH && *(path + 1) != '\0')	{
				path++ ;
				break ;
			}
		}
		return (TCHAR*)path ;
	}
	

	bool File::pathRemoveFileSpec (TCHAR* path)
	{
		TCHAR* file;

		file =  pathFindFileName (path) ;
		if (file > path) {
			*(file - 1) = '\0' ;
			return true;
		}
		else
			return false ;
	}

}

