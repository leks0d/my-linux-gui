#include "mango.h"

namespace mango 
{

	Process::Process(void)
	{

	}


	Process::~Process(void)
	{

	}


	int Process::getCurrentId(void)
	{
#ifdef WIN32
		return GetCurrentProcessId();
#else
		return getpid();
#endif
	}


	int Process::open(int id) 
	{
#ifdef WIN32
		mObj = OpenProcess (PROCESS_ALL_ACCESS, FALSE, id) ;
#else
		mId = id;
#endif
		return MANGO_SUCCESS;
	}

	int getCurrent(void)
	{
#ifdef WIN32
		mObj = GetCurrentProcess();
		return MANGO_SUCCESS;
#else
		mId = getpid();
		return MANGO_SUCCESS;
#endif
	}

	void Process::exit(int code)
	{
#ifdef WIN32
		ExitProcess (code) ;
#else
		exit (code) ;
#endif
	}


#define WINEXEC_ARGC_MAX			5
#define WINEXEC_ARGV_BUF_CHARS		(260 + MAX_PATH)

	int Process::exec(char* cmdLine, int cmdShow)
	{
		int		iCmd, iBuf ;
		int		rc, ret ;
		char	szBuf[WINEXEC_ARGV_BUF_CHARS] ;
		char	*lpstrExecFileName ;
		char	*argv[WINEXEC_ARGC_MAX] ; 
		int		argc = 0 ;

		//分析command line
		iCmd	= 0 ;
		iBuf	= 0 ;
		argv[0] = cmdLine ;

		while (1)
		{
			while (lpCmdLine[iCmd] == ' ')
				iCmd++ ;

			if (!lpCmdLine[iCmd])
				break ;

			argv[argc] = szBuf + iBuf ;

			//第二个参数， 文件名中可能有空格
			while(cmdLine[iCmd] && (argc || (cmdLine[iCmd] != ' ')) && (iBuf < WINEXEC_ARGV_BUF_CHARS - 2))
			{
				szBuf[iBuf] = lpCmdLine[iCmd] ;
				iBuf++ ;
				iCmd++ ;
			}
			szBuf[iBuf] = '\0' ;
			iBuf++ ;

			argc++ ;
			if (argc > WINEXEC_ARGC_MAX - 2)
				break ;
		}

		lpstrExecFileName = argv[0] ;
		argv[argc] = NULL ;

		log ("exe file name '%s'\n", lpstrExecFileName);

#ifdef WIN32
		return WinExec(cmdLine, cmdShow) ;
#else
		rc = fork () ;
		if (rc == 0)
		{
			//child process
			//setpgid (0, 0) ;  //Change the parent grp ID to 0
			log ("argv[0]='%s'\n", argv[0]) ;

			ret = execv (lpstrExecFileName, argv) ; // there are other flavours of exec available,
			if(ret == -1)
			{
				log_e (" execv(%s) \n", lpstrExecFileName) ;
				exit (0) ;
			}
		}
		else if (rc == -1)
		{
			log_e("function failed !\n");
			return 0 ;
		}

		return 32 + rc ;
#endif
	}
}



