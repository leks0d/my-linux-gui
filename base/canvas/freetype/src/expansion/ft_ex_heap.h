/*++

Copyright (c) 2009 Wisky  

Module Name:

    vw_heap.c

Abstract:
      

Environment:

    

Notes:

   Copyright (c) 2009 Wisky.  All Rights Reserved.


Revision History:

   2009-1-5 Created by Fibo Lu

--*/

#ifndef _VW_HEAP_H
#define _VW_HEAP_H

//�ڴ����ṹ
typedef struct tagHEAP_ALLOC_STRUCT
{
	union LINK_EX
	{
		struct tagHEAP_ALLOC_STRUCT *llink  ; //ͷ����, ָ��ǰ�����
		struct tagHEAP_ALLOC_STRUCT *uplink ; //�ײ���, ָ�򱾽��ͷ��

	} link ;

	WORD tag ;		//���־, 0:����, 1:ռ��, ͷ����β������
	UINT size ;		//ͷ����, ���С

	struct tagHEAP_ALLOC_STRUCT *rlink ; //ͷ����, ָ���̽��

} HEAP_ALLOC_STRUCT, *PHEAP_ALLOC_STRUCT ;


typedef struct tagHEAP_ALLOC_PEAK
{
	size_t m_alloced ;
	size_t m_peak ;

} HEAP_ALLOC_PEAK, *PHEAP_ALLOC_PEAK ;

//The function intializates memory.
void ft_ex_heap_meminit(PHEAP_ALLOC_STRUCT *ppHead, void *memblock, size_t size, PHEAP_ALLOC_PEAK pPeak) ;
//The function allocates memory blocks.
void * ft_ex_heap_malloc (PHEAP_ALLOC_STRUCT *ppHead, size_t size, PHEAP_ALLOC_PEAK pPeak) ;
//The function frees a memory blocks.
BOOL ft_ex_heap_free (PHEAP_ALLOC_STRUCT *ppHead, void *memblock, PHEAP_ALLOC_PEAK pPeak) ;
//�����ڴ��С
size_t ft_ex_heap_free_size (PHEAP_ALLOC_STRUCT pHead) ;

#endif