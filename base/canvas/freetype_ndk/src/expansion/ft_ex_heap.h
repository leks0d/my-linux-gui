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

//内存分配结构
typedef struct tagHEAP_ALLOC_STRUCT
{
	union LINK_EX
	{
		struct tagHEAP_ALLOC_STRUCT *llink  ; //头部域, 指向前驱结点
		struct tagHEAP_ALLOC_STRUCT *uplink ; //底部域, 指向本结点头部

	} link ;

	WORD tag ;		//块标志, 0:空闲, 1:占用, 头部和尾部均有
	UINT size ;		//头部域, 块大小

	struct tagHEAP_ALLOC_STRUCT *rlink ; //头部域, 指向后继结点

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
//空闲内存大小
size_t ft_ex_heap_free_size (PHEAP_ALLOC_STRUCT pHead) ;

#endif