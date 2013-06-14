/*++

Copyright (c) 2009 Wisky

Module Name:

    ft_ex_truetype.c

Abstract:
      

Environment:

    Turnip

Notes:

   Copyright (c) 2009 Wisky.  All Rights Reserved.


Revision History:

   Created by Fibo Lu

--*/
#include <ft2build.h>
#include "../../include/freetype/freetype.h"
#include "mango.h" 
typedef struct tagFT_EX_TRUETYPE
{
	FT_Library  m_library ;
	FT_Face     m_face[FT_EX_TRUETYPE_FONTS] ;

	BOOL		m_bHFontValid[FT_EX_TRUETYPE_FONTS] ;
	int			m_iFontSize[FT_EX_TRUETYPE_FONTS] ;
	int			m_load_flags[FT_EX_TRUETYPE_FONTS] ;

}FT_EX_TRUETYPE ;


FT_EX_TRUETYPE	 gft_ex_TrueType ;

const int conftTrueTypeToLoadFlag[] = 
{FT_RENDER_MODE_NORMAL, FT_RENDER_MODE_LCD, FT_RENDER_MODE_LCD_V} ;

const int conftTrueTypeToMode[]     = 
{FT_EX_TRUETYPE_MODE_NORMAL, FT_EX_TRUETYPE_MODE_NORMAL, \
FT_EX_TRUETYPE_MODE_NORMAL, FT_EX_TRUETYPE_MODE_LCD_H, FT_EX_TRUETYPE_MODE_LCD_V} ;


//初始化
BOOL ft_ex_TrueTypeInit (char **ppszFontFileName, int iCount)
{
	BOOL	  bSuccess = FALSE ;
	FT_Error  error ;
	int		  iFont ;	

	iCount  = min (FT_EX_TRUETYPE_FONTS, iCount) ;

	do
	{
 		error = FT_Init_FreeType( &(gft_ex_TrueType.m_library) );              /* initialize library */
		if (error)
			break ;

		for (iFont = 0 ; iFont < iCount; iFont++)
		{
			error = FT_New_Face( gft_ex_TrueType.m_library, ppszFontFileName[iFont], 0, &(gft_ex_TrueType.m_face[iFont])); /* create face object */
			if (error)
				break ;

			/* use 50pt at 100dpi */
			error = FT_Set_Char_Size( gft_ex_TrueType.m_face[iFont], 0, 16 * 64, 72, 72) ; /* set character size */

			//FT_EX_Set_Transform(gft_ex_TrueType.m_face[iFont], &matrix, NULL);
			gft_ex_TrueType.m_bHFontValid[iFont] = TRUE ;
			gft_ex_TrueType.m_iFontSize[iFont]   = 16 ;
			gft_ex_TrueType.m_load_flags[iFont]  = FT_LOAD_RENDER ; 
		}

		if (iFont != iCount)
			break ;
	
		bSuccess = TRUE ;
	
	} while (0) ;

	return bSuccess ;
}


//销毁
BOOL ft_ex_TrueTypeDestroy (int iFont)
{
	FT_Done_Face    (gft_ex_TrueType.m_face[iFont]) ;
	FT_Done_FreeType(gft_ex_TrueType.m_library) ;

	return TRUE ;
}


//获得字符的大小
BOOL ft_ex_TrueTypeGetCharSize (int iFont, WCHAR wchar, SIZE *psize)
{
	FT_Error  error ;
	int		  iMode ;

    error = FT_Load_Char (gft_ex_TrueType.m_face[iFont], wchar, gft_ex_TrueType.m_load_flags[iFont] );
    if ( error )
      return FALSE ;                 /* ignore errors */

	psize->cx = gft_ex_TrueType.m_face[iFont]->glyph->bitmap.width ;
	psize->cy = gft_ex_TrueType.m_face[iFont]->glyph->bitmap.rows ;

	iMode = FT_LOAD_TARGET_MODE (gft_ex_TrueType.m_load_flags[iFont]) ;

	if (iMode == FT_RENDER_MODE_LCD)
		psize->cx = psize->cx / 3 ;
	else if (iMode == FT_RENDER_MODE_LCD_V)
		psize->cy = psize->cy / 3 ;

	return TRUE ;
}




//获得字符的位图
BYTE *ft_ex_TrueTypeGetCharBmp (int iFont, WCHAR wchar, SIZE *psize, int *pdyExtra)
{
	FT_Error  error ;
	int		  iMode ;

    error = FT_Load_Char (gft_ex_TrueType.m_face[iFont], wchar, gft_ex_TrueType.m_load_flags[iFont] );
    if ( error )
      return NULL ;                 /* ignore errors */

	psize->cx = gft_ex_TrueType.m_face[iFont]->glyph->bitmap.width ;
	psize->cy = gft_ex_TrueType.m_face[iFont]->glyph->bitmap.rows ;

	*pdyExtra = gft_ex_TrueType.m_iFontSize[iFont] - gft_ex_TrueType.m_face[iFont]->glyph->bitmap_top ;

	iMode = FT_LOAD_TARGET_MODE (gft_ex_TrueType.m_load_flags[iFont]) ;

	if (iMode == FT_RENDER_MODE_LCD)
	{
		psize->cx = psize->cx / 3 ;
	}
	else if (iMode == FT_RENDER_MODE_LCD_V)
	{
		psize->cy = psize->cy / 3 ;
	}

	return gft_ex_TrueType.m_face[iFont]->glyph->bitmap.buffer ;
}


//从DC 设置字符大小
BOOL ft_ex_iTrueTypeSetSize (int iFont, int iPoints)
{
	FT_Error  error = 0 ;

	if (!gft_ex_TrueType.m_bHFontValid[iFont])
		return FALSE ;

	if (gft_ex_TrueType.m_iFontSize[iFont] != iPoints)
	{
		error = FT_Set_Char_Size( gft_ex_TrueType.m_face[iFont], 0, iPoints * 64, 72, 72) ; /* set character size */
		gft_ex_TrueType.m_iFontSize[iFont] = iPoints ;
	}

	if (error)
		return FALSE ;
	else
		return TRUE ;
}


//设置字符大小
BOOL ft_ex_TrueTypeSetSize (int iFont, int iPoints, BOOL bValid)
{
	FT_Error  error = 0 ;

	if (!bValid)
	{
		gft_ex_TrueType.m_bHFontValid[iFont] = TRUE ;
		return TRUE ;
	}

	gft_ex_TrueType.m_bHFontValid[iFont] = FALSE ;

	if (gft_ex_TrueType.m_iFontSize[iFont] != iPoints)
	{
		error = FT_Set_Char_Size( gft_ex_TrueType.m_face[iFont], 0, iPoints * 64, 72, 72) ; /* set character size */
		gft_ex_TrueType.m_iFontSize[iFont] = iPoints ;
	}

	if (error)
		return FALSE ;
	else
		return TRUE ;
}


//获得字符大小
int ft_ex_TrueTypeGetSize (int iFont)
{
	return gft_ex_TrueType.m_iFontSize[iFont] ;
}

//#define FT_EX_LOAD_TARGET_( x )      ( (FT_EX_Int32)( (x) & 15 ) << 16 )
//设字符呈递模式
int ft_ex_TrueTypeSetMode (int iFont, int iMode) 
{
	int iPrevMode ;

	iPrevMode = ft_ex_TrueTypeGetMode (iFont) ;
	if (iMode == iPrevMode)
		return iMode ;

	gft_ex_TrueType.m_load_flags[iFont] &= (~(FT_LOAD_TARGET_(15))) ;
	gft_ex_TrueType.m_load_flags[iFont] |= FT_LOAD_TARGET_ (conftTrueTypeToLoadFlag[iMode]) ;

	return iMode ;
}


//获得字符呈递模式
int ft_ex_TrueTypeGetMode (int iFont) 
{
	int iMode ;

	iMode = conftTrueTypeToMode[FT_LOAD_TARGET_MODE (gft_ex_TrueType.m_load_flags[iFont])] ;

	return iMode ;
}


//#define FILE void

FILE * ft_ex_fopen (const char *filename, const char *mode)
{
	return fopen(filename, mode);
#if 0
	if (filename[0] == 'a' || filename[0] == 'A')
	{
		SetMemoryFilePointer (&(gsys_UserSessionLocal.m_FontMemFile[0]), 0, FILE_BEGIN) ;
		return &(gsys_UserSessionLocal.m_FontMemFile[0]) ;
	}
	else if (filename[0] == 'u' || filename[0] == 'U')
	{
		SetMemoryFilePointer (&(gsys_UserSessionLocal.m_FontMemFile[1]), 0, FILE_BEGIN) ;
		return &(gsys_UserSessionLocal.m_FontMemFile[1]) ;
	}
	return NULL ;
#endif
}

size_t ft_ex_fread (void *buffer, size_t size, size_t count, FILE *stream )
{
	return fread(buffer, size, count, stream);
#if 0
	DWORD dwRead = 0 ;

	if ((stream == NULL) || (count == 0) || (size == 0))
		return 0 ;

	ReadMemoryFile ((PMEMORY_FILE)stream, buffer, size * count, &dwRead) ;

	return (dwRead / size) ;
#endif
}


int ft_ex_fseek (FILE *stream, long offset, int origin)
{
	return fseek(stream, offset, origin);
#if 0
	int   iRtn = 0 ;
	DWORD dwMoveMethod ;    

	if (stream == NULL)
		return 1 ;

	switch (origin)
	{
	case SEEK_SET:
		dwMoveMethod = FILE_BEGIN ;
		break ;

	case SEEK_CUR:
		dwMoveMethod = FILE_CURRENT ;
		break ;

	case SEEK_END:
		dwMoveMethod = FILE_END ;
		break ;

	default:
		iRtn = 1 ;
		break ;
	}

	if (iRtn == 0)
		SetMemoryFilePointer ((PMEMORY_FILE)stream, offset, dwMoveMethod) ;

	return iRtn ;
#endif
}

long ft_ex_ftell(FILE *stream )
{
	return ftell(stream);
#if 0
	if (stream == NULL)
		return 0 ;

	return (long)SetMemoryFilePointer ((PMEMORY_FILE)stream, 0, FILE_CURRENT) ;
#endif
}

int ft_ex_fclose (FILE *stream)
{
	return fclose(stream);
#if 0
	return 0 ;
#endif
}

