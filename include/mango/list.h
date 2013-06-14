
#ifndef _MANGO_LIST_H
#define _MANGO_LIST_H

#include "linux_list.h"

namespace mango
{
	typedef int (*LISTITEMCOMPARE)(void*, void*, void*);

	template<typename TYPE> class List
	{
	public:
		List();
		~List();

	public:

		//增加到尾部 
		void add(TYPE* item);

		//增加到头部 -- GetFirst  可以找到的
		void addHead(TYPE* item);

		//删除指定项
		void del(TYPE* item);

		//计算item 个数
		int getCount () ;

		//第一项
		TYPE* getFirst();

		//尾项
		TYPE* getTail();

		//下一项
		TYPE* getNext(TYPE* item, int cycle);

		//前一项
		TYPE* getPrev(TYPE* item, int cycle);

		//移到pos->next 位置 
		void  move(TYPE* item, TYPE* pos);

		//移到pos->prev 位置 
		void  moveTail(TYPE* item, TYPE* pos);

		//根据顺号, 获得node
		TYPE* getNode (int order);

		//根据node, 序号
		int getOrder (TYPE* item);

		//排序
		void sortItems (LISTITEMCOMPARE compare, void* param) ;

		void deleteAllItems();
		void deleteItem(TYPE* item);

		void freeAllItems();
		void freeItem(TYPE *item);

	public:
		LIST_HEAD	mHead;  //正序

	protected:
		int			mCount;	//item 总数

		int binarySortInsert(TYPE **itemQueue, int queueItems, TYPE *item, LISTITEMCOMPARE compare, void* param);

	} ;


	template<typename TYPE> List<TYPE>::List()
	{
		INIT_LIST_HEAD(&mHead);
		mCount = 0;
	}


	template<typename TYPE> List<TYPE>::~List()
	{

	}


	//增加到尾部 
	template<typename TYPE> void List<TYPE>::add(TYPE* item)
	{
		list_add_tail(&(item->mList), &mHead);
		mCount++ ;
	}


	//增加到头部 -- GetFirst  可以找到的
	template<typename TYPE> void List<TYPE>::addHead(TYPE* item)
	{
		list_add(&(item->mList), &mHead);
		mCount++ ;
	}


	template<typename TYPE> void List<TYPE>::del(TYPE* item)
	{
		list_del(&(item->mList));
		mCount-- ;
	}


	template<typename TYPE> int List<TYPE>::getCount()
	{
		int			count = 0;
		LIST_HEAD	*pos;

		list_for_each(pos, &mHead)
		{
			count++ ;
		}

		return count;
	}


	template<typename TYPE> TYPE* List<TYPE>::getFirst()
	{
		TYPE *obj = NULL ;

		if (mHead.next != &mHead)
			obj = list_entry(mHead.next, TYPE, mList) ;

		return obj ;
	}


	template<typename TYPE> TYPE* List<TYPE>::getTail()
	{
		TYPE *obj = NULL ;

		if (mHead.prev != &mHead)
			obj = list_entry(mHead.prev, TYPE, mList) ;

		return obj ;
	}


	template<typename TYPE> TYPE* List<TYPE>::getNext(TYPE* item, int cycle)
	{
		LIST_HEAD  *pos;
		TYPE	   *obj = NULL;

		if (list_empty (&mHead))
			return NULL;

		if (item == NULL)
			pos = mHead.next;
		else
		{
			pos = item->mList.next;
			if ((pos == &mHead) && cycle)
				pos = mHead.next;
		}

		if (pos != &mHead)
			obj = list_entry(pos, TYPE, mList);

		return obj;
	}


	template<typename TYPE> TYPE* List<TYPE>::getPrev(TYPE* item, int cycle)
	{
		LIST_HEAD	*pos;
		TYPE		*obj = NULL;

		if (list_empty (&mHead))
			return NULL ;

		if (item == NULL)
			pos = mHead.prev;
		else
		{
			pos = item->mList.prev;
			if ((pos == &mHead) && cycle)
				pos = mHead.prev;
		}

		if (pos != &mHead)
			obj = list_entry(pos, TYPE, mList);

		return obj;
	}


	//移到pos->next 位置 
	template<typename TYPE> void List<TYPE>::move(TYPE* item, TYPE* pos)
	{
		__list_del(item->mList.prev, item->mList.next);
		list_add(&(item->mList), &(pos->mList));
	}


	//移到pos->prev 位置 
	template<typename TYPE> void List<TYPE>::moveTail(TYPE* item, TYPE* pos)
	{
		__list_del(item->mList.prev, item->mList.next);
		list_add_tail(&(item->mList), &(pos->mList));
	}


	//根据顺号, 获得node
	template<typename TYPE> TYPE* List<TYPE>::getNode(int order)
	{
		int			count = 0;
		LIST_HEAD	*pos;
		TYPE	   *obj = NULL;

		list_for_each(pos, &mHead)
		{
			if (order <= 0)
			{
				obj = list_entry(pos, TYPE, mList);
				break;
			}
			order--;
		}

		return obj;
	}


	//根据node, 序号
	template<typename TYPE> int List<TYPE>::getOrder(TYPE* item)
	{
		int			order = 0;
		LIST_HEAD	*pos;
		TYPE		*obj;
		BOOL		bSuccess = FALSE;

		list_for_each(pos, &mHead)
		{
			obj = list_entry(pos, TYPE, mList); 
			if (obj == item)
			{
				bSuccess = TRUE;
				break ;
			}
			order++ ;
		}

		if (bSuccess)
			return order;
		else
			return -1;
	}


	//二分法排队插入
	template<typename TYPE> BOOL List<TYPE>::binarySortInsert (TYPE **itemQueue, int queueItems, TYPE* item, \
		LISTITEMCOMPARE compare, void* param)
	{
		int low, mid, high ;
		int result ;
		int i ;

		low  = 0;
		high =  queueItems - 1;

		while (1)
		{
			mid    = (low + high) / 2 ;
			result = compare ((void*)item, (void*)(itemQueue[mid]), param);

			//负数在前面
			if (result < 0)
				high = mid - 1;
			else if (result > 0)
				low  = mid + 1;
			else
			{
				mid++ ;
				break;
			}

			if (low > high)
			{
				if (result > 0)
					mid++ ;

				break;
			}
		}

		//为新元素让出空间
		for (i = queueItems; i > mid; i--)
			itemQueue[i] = itemQueue[i - 1];

		itemQueue[mid] = item;

		return TRUE;
	}


	//排序
	template<typename TYPE> void List<TYPE>::sortItems (LISTITEMCOMPARE compare, void* param)
	{
		int	 order, count;
		TYPE **itemQueue;
		TYPE *item;

		count = getCount();
		if (count <= 0)
			return;

		itemQueue = (TYPE **)malloc(sizeof(TYPE *) * count);
		if (itemQueue == NULL)
			return;

		item = getFirst();
		itemQueue[0] = item;

		for (order = 1; order < count; order++)
		{
			item = getNext(item, FALSE);
			binarySortInsert(itemQueue, order, item, compare, param);
		}


		//重新排列
		for (order = 0; order < count; order++)
		{
			item = itemQueue[order];
			list_del(&(item->mList));
			list_add_tail(&(item->mList), &(mHead));
		}

		safeFree(itemQueue);
	}



	template<typename TYPE> void List<TYPE>::deleteAllItems()
	{
		TYPE		*obj;

		while (obj = getFirst())
		{
			del (obj);
			delete obj;
		}
	}


	template<typename TYPE> void List<TYPE>::deleteItem(TYPE* item)
	{
		del(item);
		delete item;
	}


	template<typename TYPE> void List<TYPE>::freeAllItems()
	{
		TYPE		*obj;

		while (obj = getFirst())
		{
			del(obj);
			free(obj);
		}
	}


	template<typename TYPE> void List<TYPE>::freeItem(TYPE *item)
	{
		del(item);
		free(item);
	}

}




#endif