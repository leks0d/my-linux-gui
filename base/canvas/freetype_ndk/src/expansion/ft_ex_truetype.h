/*++

Copyright (c) 2009 Wisky

Module Name:

    ft_ex_truetype.c

Abstract:
      

Environment:

    Windows

Notes:

   Copyright (c) 2009 Wisky.  All Rights Reserved.


Revision History:

   Created by Fibo Lu

--*/

#ifndef _FT_EX_TRUETYPE_H
#define _FT_EX_TRUETYPE_H


//最多只能同时支持三种字体
#define FT_EX_TRUETYPE_FONTS   3

//TrueType 内存初始化
void ft_ex_meminit (void *pStartAddr, unsigned int uiBytes) ;

//TrueType  获得空闲内存大小, 用于调试
size_t ft_ex_freesize () ;

//TrueType 获得内存使用峰值, 用于调试
size_t ft_ex_mempeak () ;

//备注:  支持一个比较大的truetye 字体文件至少需要260KB 内存 
//		 一个仅含字母的小tryetype字体文件至少需要130KB   
//具体可以通过调用ft_ex_mempeak 查询, 
//如查内存过少会造成, 不能显示字符, 或者开始会显示, 便经过多次操作之后, 不再显示了		 

//初始化
BOOL ft_ex_TrueTypeInit (char **ppszFontFileName, int iCount) ;

//销毁
BOOL ft_ex_TrueTypeDestroy (int iFont) ;

//获得字符的大小
BOOL ft_ex_TrueTypeGetCharSize (int iFont, WCHAR wchar, SIZE *psize) ;

//获得字符的位图
BYTE *ft_ex_TrueTypeGetCharBmp (int iFont, WCHAR wchar, SIZE *psize, int *pdyExtra) ;

//从DC 设置字符大小
BOOL ft_ex_iTrueTypeSetSize (int iFont, int iPoints) ;

//设置字符大小
BOOL ft_ex_TrueTypeSetSize (int iFont, int iPoints, BOOL bValid) ;

//获得字符大小
int ft_ex_TrueTypeGetSize (int iFont) ;

//设字符呈递模式
int ft_ex_TrueTypeSetMode (int iFont, int iMode) ;

//获得字符呈递模式
int ft_ex_TrueTypeGetMode (int iFont) ;

#define FT_EX_TRUETYPE_MODE_NORMAL	0
#define FT_EX_TRUETYPE_MODE_LCD_H	1
#define FT_EX_TRUETYPE_MODE_LCD_V	2

//LCD 中RGB 排列次序
#define FT_EX_TRUETYPE_LCD_RGB_RGB		0
#define FT_EX_TRUETYPE_LCD_RGB_RBG		1
#define FT_EX_TRUETYPE_LCD_RGB_GBR		2
#define FT_EX_TRUETYPE_LCD_RGB_GRB		3
#define FT_EX_TRUETYPE_LCD_RGB_BGR		4
#define FT_EX_TRUETYPE_LCD_RGB_BRG		5


//配制
#define FT_EX_TRUETYPE_MODE		FT_EX_TRUETYPE_MODE_LCD_H

#define FT_EX_TRUETYPE_LCD_RGB		FT_EX_TRUETYPE_LCD_RGB_RGB

#endif