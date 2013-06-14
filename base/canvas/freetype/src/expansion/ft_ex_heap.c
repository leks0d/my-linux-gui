/*++

Copyright (c) 2009 Wisky  

Module Name:

    vw_heap.c

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

#define P_MEM
#define V_MEM

#define HEAP_STRUCT_LEN	 sizeof(HEAP_ALLOC_STRUCT)
#define FootLoc(p)       (p+p->size-1)


/***************************************************************
* Routine Description:
*	  The function intializates memory.
*
* Parameter:
*     ppHead     -- pointer to pointer of memory header
*	  memblock   -- pointer to the lowed address of memory block. 	  
*	  size       -- bytes of memory bloack.
*
* Return Value:  
*	  None
* 
*  Remarks:
*	  You need to intializate memory before you call c_malloc or c_free.
*	      
****************************************************************/
void ft_ex_heap_meminit(PHEAP_ALLOC_STRUCT *ppHead, void *memblock, size_t size, PHEAP_ALLOC_PEAK pPeak)
{
	PHEAP_ALLOC_STRUCT p, f;

	*ppHead = (PHEAP_ALLOC_STRUCT)memblock;
	
	p         = *ppHead; 
	size      = size / HEAP_STRUCT_LEN;
	p->tag    = 0;
	p->size   = size - 1;
	p->link.llink  = p->rlink  = p;

	f		  = FootLoc (p);
	f->tag    = 0;
	f->link.uplink = p;

	f     += 1;
	f->tag = 1;

	if (pPeak)
	{
		pPeak->m_alloced = 0 ;
		pPeak->m_peak    = 0 ;
	}
}

/***************************************************************
* Routine Description:
*	  The function allocates memory blocks.
*
* Arithmetic:
*	  参考:《数据结构》(清华大学出版社) 193页 
*
* Parameter:
*    IN PHEAP_ALLOC_STRUCT ppHead     -- pointer to pointer of memory header
*	 IN size_t size					 -- Bytes to allocate
*    IN WORD   id					 -- flags of memory blocks
*
* Return Value:  
*	  The function returns a void pointer to the allocated space, or
* NULL if there is insufficient memory available. 
*
*  Remarks:
*    在调用函数前， 应用调用SysMemInit. 
****************************************************************/
void * ft_ex_heap_malloc (PHEAP_ALLOC_STRUCT *ppHead, size_t size, PHEAP_ALLOC_PEAK pPeak)
{
	PHEAP_ALLOC_STRUCT p, f;

	P_MEM ;

	size  = (size + HEAP_STRUCT_LEN - 1) / HEAP_STRUCT_LEN;
	size += 2;

	for (p = *ppHead ; p && p->size < size && p->rlink != *ppHead ;
		p = p->rlink);

	if (!p || p->size < size)
	{
		V_MEM ;
		return NULL;
	}


	f = FootLoc (p);

	*ppHead = p->rlink;
		
	if (p->size - size <= 1)
	{
		if (*ppHead == p)
			*ppHead = NULL;
		else
		{
			(*ppHead)->link.llink = p->link.llink ;
			p->link.llink->rlink         = *ppHead ;
		}
		p->tag = f->tag = 1;
	//	p->id  = id ;
	}
	else
	{
		f->tag    = 1;
		p->size  -= size;
		f         = FootLoc(p);
		f->tag    = 0;
		f->link.uplink = p;
		p         = f + 1;
		p->tag    = 1;
	//	p->id     = id ;
		p->size   = size;
	}

	V_MEM ;

	//GC_memset (p + 1, 0, size) ;
	if (pPeak)
	{
		pPeak->m_alloced += p->size ;
		pPeak->m_peak     = max (pPeak->m_peak, pPeak->m_alloced) ;
	}

	return p + 1;
}


/***************************************************************
* Routine Description:
*	  The function frees a memory blocks.
*
* Parameter:
*    IN PHEAP_ALLOC_STRUCT ppHead     -- pointer to pointer of memory header
*	 IN void *memblock				 -- pointer to previously allocated memory block
*               to be freed.    
*    IN WORD   id					 -- flags of memory blocks
*
* Return Value:  
*	  If the function succeed the return value is TRUE, or
* FALSE if it faild. 
*
*  Remarks:
*    
****************************************************************/
BOOL ft_ex_heap_free (PHEAP_ALLOC_STRUCT *ppHead, void *memblock, PHEAP_ALLOC_PEAK pPeak)
{
	PHEAP_ALLOC_STRUCT p, q, s, t, f;
	UINT              size;

	if (!memblock)
		return FALSE ;

	P_MEM ;

	p = (PHEAP_ALLOC_STRUCT)memblock;
	p--;
/*
	if (p->id != id)
	{
		V_MEM ;
		return FALSE;
	}
*/
	size = p->size;
	f    = FootLoc (p); 

	if (pPeak)
		pPeak->m_alloced -= size ;

	if ((p - 1)->tag == 1 && (p + size)->tag == 1)
	{
		p->tag    = 0;
		f->link.uplink = p;
		f->tag    = 0;

		if (!*ppHead)
			*ppHead = p->link.llink = p->rlink = p;
		else
		{
			q = (*ppHead)->link.llink;
			p->rlink = *ppHead;
			p->link.llink = q;
			q->rlink = (*ppHead)->link.llink = p;
			*ppHead = p;
		}

	}
	else if ((p - 1)->tag == 0 && (p + size)->tag == 1)
	{
		//
		// the left memory block is freed.
		//
		s = (p - 1)->link.uplink;
		s->size += size;
		f = p + size - 1;
		f->link.uplink = s;
		f->tag    = 0;
	}
	else if ((p - 1)->tag == 1 && (p + size)->tag == 0)
	{
		//
		// the right memory block is freed.
		//
		t      = p + size;

		if (*ppHead == t)
			*ppHead = p;

		p->tag = 0;

		p->link.llink        = t->link.llink;
		t->link.llink->rlink = p;

		p->rlink        = t->rlink;
		t->rlink->link.llink = p;

		p->size        += t->size;

		FootLoc(t)->link.uplink = p;
	}
	else if ((p - 1)->tag == 0 && (p + size)->tag == 0)
	{
		//
		// the right and left memory blocks are freed.
		//
		s = (p - 1)->link.uplink;
		t = p + size;

		if (*ppHead == t)
			*ppHead = s;


		s->size += size + t->size;

		t->link.llink->rlink = t->rlink;
		t->rlink->link.llink = t->link.llink;

		FootLoc(t)->link.uplink = s;
	}
	else
	{
		V_MEM ;
		return FALSE;
	}

	V_MEM ;
	return TRUE;
}

//空闲内存大小
size_t ft_ex_heap_free_size (PHEAP_ALLOC_STRUCT pHead)
{
	PHEAP_ALLOC_STRUCT p ;
	size_t size = 0 ;

	P_MEM ;

	size  = (size + HEAP_STRUCT_LEN - 1) / HEAP_STRUCT_LEN;
	size += 2;

	for (p = pHead ; p && p->size < size && p->rlink != pHead ;
		p = p->rlink)
	{
		size += p->size ;
	}

	V_MEM ;

	return (size * HEAP_STRUCT_LEN) ;
}

