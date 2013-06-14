/*++

Copyright (c) 2009 Wisky  

Module Name:

    sys_memory.c

Abstract:
      

Environment:

    Turnip

Notes:

   Copyright (c) 2009 Wisky.  All Rights Reserved.


Revision History:

   2009-1-5 Created by Fibo Lu

--*/

#include "mango.h" 
#include "ft_ex_heap.h"


//系统内存
PHEAP_ALLOC_STRUCT	gft_ex_pHeap ;
HEAP_ALLOC_PEAK		gft_ex_pHeapPeak ;


//内存初始化
void ft_ex_meminit (void *pStartAddr, unsigned int uiBytes)
{
	size_t size ;

	size = sizeof (HEAP_ALLOC_STRUCT) ;
	size = uiBytes  ;
	ft_ex_heap_meminit (&gft_ex_pHeap, pStartAddr, size, &gft_ex_pHeapPeak) ;

}


//分配内存
void *ft_ex_malloc (size_t size)
{
	return ft_ex_heap_malloc (&gft_ex_pHeap, size, &gft_ex_pHeapPeak) ;
}



//释放内存
void ft_ex_free (void *memblock)
{
	ft_ex_heap_free (&gft_ex_pHeap, memblock, &gft_ex_pHeapPeak) ;
}


//重新分配
void *ft_ex_realloc( void *memblock, size_t size )
{
	if (memblock)
		ft_ex_free (memblock) ;

	return ft_ex_malloc (size) ;

}

//获得空闲内存大小
size_t ft_ex_memfreesize ()
{
	return ft_ex_heap_free_size (gft_ex_pHeap) ;
}


//获得内存使用峰值
size_t ft_ex_mempeak ()
{
	return gft_ex_pHeapPeak.m_peak * sizeof (HEAP_ALLOC_STRUCT) ;
}