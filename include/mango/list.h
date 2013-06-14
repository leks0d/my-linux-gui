
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

		//���ӵ�β�� 
		void add(TYPE* item);

		//���ӵ�ͷ�� -- GetFirst  �����ҵ���
		void addHead(TYPE* item);

		//ɾ��ָ����
		void del(TYPE* item);

		//����item ����
		int getCount () ;

		//��һ��
		TYPE* getFirst();

		//β��
		TYPE* getTail();

		//��һ��
		TYPE* getNext(TYPE* item, int cycle);

		//ǰһ��
		TYPE* getPrev(TYPE* item, int cycle);

		//�Ƶ�pos->next λ�� 
		void  move(TYPE* item, TYPE* pos);

		//�Ƶ�pos->prev λ�� 
		void  moveTail(TYPE* item, TYPE* pos);

		//����˳��, ���node
		TYPE* getNode (int order);

		//����node, ���
		int getOrder (TYPE* item);

		//����
		void sortItems (LISTITEMCOMPARE compare, void* param) ;

		void deleteAllItems();
		void deleteItem(TYPE* item);

		void freeAllItems();
		void freeItem(TYPE *item);

	public:
		LIST_HEAD	mHead;  //����

	protected:
		int			mCount;	//item ����

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


	//���ӵ�β�� 
	template<typename TYPE> void List<TYPE>::add(TYPE* item)
	{
		list_add_tail(&(item->mList), &mHead);
		mCount++ ;
	}


	//���ӵ�ͷ�� -- GetFirst  �����ҵ���
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


	//�Ƶ�pos->next λ�� 
	template<typename TYPE> void List<TYPE>::move(TYPE* item, TYPE* pos)
	{
		__list_del(item->mList.prev, item->mList.next);
		list_add(&(item->mList), &(pos->mList));
	}


	//�Ƶ�pos->prev λ�� 
	template<typename TYPE> void List<TYPE>::moveTail(TYPE* item, TYPE* pos)
	{
		__list_del(item->mList.prev, item->mList.next);
		list_add_tail(&(item->mList), &(pos->mList));
	}


	//����˳��, ���node
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


	//����node, ���
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


	//���ַ��ŶӲ���
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

			//������ǰ��
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

		//Ϊ��Ԫ���ó��ռ�
		for (i = queueItems; i > mid; i--)
			itemQueue[i] = itemQueue[i - 1];

		itemQueue[mid] = item;

		return TRUE;
	}


	//����
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


		//��������
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