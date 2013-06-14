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


//���ֻ��ͬʱ֧����������
#define FT_EX_TRUETYPE_FONTS   3

//TrueType �ڴ��ʼ��
void ft_ex_meminit (void *pStartAddr, unsigned int uiBytes) ;

//TrueType  ��ÿ����ڴ��С, ���ڵ���
size_t ft_ex_freesize () ;

//TrueType ����ڴ�ʹ�÷�ֵ, ���ڵ���
size_t ft_ex_mempeak () ;

//��ע:  ֧��һ���Ƚϴ��truetye �����ļ�������Ҫ260KB �ڴ� 
//		 һ��������ĸ��Сtryetype�����ļ�������Ҫ130KB   
//�������ͨ������ft_ex_mempeak ��ѯ, 
//����ڴ���ٻ����, ������ʾ�ַ�, ���߿�ʼ����ʾ, �㾭����β���֮��, ������ʾ��		 

//��ʼ��
BOOL ft_ex_TrueTypeInit (char **ppszFontFileName, int iCount) ;

//����
BOOL ft_ex_TrueTypeDestroy (int iFont) ;

//����ַ��Ĵ�С
BOOL ft_ex_TrueTypeGetCharSize (int iFont, WCHAR wchar, SIZE *psize) ;

//����ַ���λͼ
BYTE *ft_ex_TrueTypeGetCharBmp (int iFont, WCHAR wchar, SIZE *psize, int *pdyExtra) ;

//��DC �����ַ���С
BOOL ft_ex_iTrueTypeSetSize (int iFont, int iPoints) ;

//�����ַ���С
BOOL ft_ex_TrueTypeSetSize (int iFont, int iPoints, BOOL bValid) ;

//����ַ���С
int ft_ex_TrueTypeGetSize (int iFont) ;

//���ַ��ʵ�ģʽ
int ft_ex_TrueTypeSetMode (int iFont, int iMode) ;

//����ַ��ʵ�ģʽ
int ft_ex_TrueTypeGetMode (int iFont) ;

#define FT_EX_TRUETYPE_MODE_NORMAL	0
#define FT_EX_TRUETYPE_MODE_LCD_H	1
#define FT_EX_TRUETYPE_MODE_LCD_V	2

//LCD ��RGB ���д���
#define FT_EX_TRUETYPE_LCD_RGB_RGB		0
#define FT_EX_TRUETYPE_LCD_RGB_RBG		1
#define FT_EX_TRUETYPE_LCD_RGB_GBR		2
#define FT_EX_TRUETYPE_LCD_RGB_GRB		3
#define FT_EX_TRUETYPE_LCD_RGB_BGR		4
#define FT_EX_TRUETYPE_LCD_RGB_BRG		5


//����
#define FT_EX_TRUETYPE_MODE		FT_EX_TRUETYPE_MODE_LCD_H

#define FT_EX_TRUETYPE_LCD_RGB		FT_EX_TRUETYPE_LCD_RGB_RGB

#endif