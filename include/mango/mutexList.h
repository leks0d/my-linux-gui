
#ifndef _MANGO_MUTEXLIST_H
#define _MANGO_MUTEXLIST_H


namespace mango
{
	template<typename TYPE> class MutexList : List<TYPE>
	{
	public:
		MutexList();
		~MutexList();

	protected:
		class Mutex  mMutex;

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
		TYPE* getNext(TYPE* item, int cycle = false);

		//ǰһ��
		TYPE* getPrev(TYPE* item, int cycle = false);

		//����˳��, ���node
		TYPE* getNode (int order);

		//����node, ���
		int getOrder (TYPE* item);

		//����
		void sortItems (LISTITEMCOMPARE compare, void* param);

		void deleteAllItems();
		void deleteItem(TYPE* item);

		void freeAllItems();
		void freeItem(TYPE *item);

	} ;


	template<typename TYPE> MutexList<TYPE>::MutexList()
	{
		//	mSemaphore.create(NULL, 1);
	}


	template<typename TYPE> MutexList<TYPE>::~MutexList()
	{

	}


	//���ӵ�β�� 
	template<typename TYPE> inline void MutexList<TYPE>::add(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::add(item);
		mMutex.unlock();
	}


	//���ӵ�ͷ�� -- GetFirst  �����ҵ���
	template<typename TYPE> inline void MutexList<TYPE>::addHead(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::addHead(item);
		mMutex.unlock();
	}


	//ɾ��ָ����
	template<typename TYPE> inline void MutexList<TYPE>::del(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::del(item);
		mMutex.unlock();
	}


	//����item ����
	template<typename TYPE> inline int MutexList<TYPE>::getCount()
	{
		int count;
		mMutex.lock();
		count = List<TYPE>::getCount();
		mMutex.unlock();

		return count;
	}


	//��һ��
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getFirst()
	{
		TYPE *obj = NULL ;

		mMutex.lock();
		obj = List<TYPE>::getFirst();
		mMutex.unlock();

		return obj ;
	}


	//β��
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getTail()
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getTail();
		mMutex.unlock();

		return obj ;
	}


	//��һ��
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getNext(TYPE* item, int cycle)
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getNext(item, cycle);
		mMutex.unlock();

		return obj;
	}


	//ǰһ��
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getPrev(TYPE* item, int cycle)
	{
		TYPE *obj = NULL;

		mMutex.lock();
		obj = List<TYPE>::getPrev(item, cycle);
		mMutex.unlock();

		return obj;
	}


	//����˳��, ���node
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getNode(int order)
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getNode(order);
		mMutex.unlock();

		return obj;
	}


	//����node, ���
	template<typename TYPE> inline int MutexList<TYPE>::getOrder(TYPE* item)
	{
		int	order = 0;

		mMutex.lock();
		order = List<TYPE>::getOrder(item);
		mMutex.unlock();

		return order;
	}


	//����
	template<typename TYPE> inline void MutexList<TYPE>::sortItems (LISTITEMCOMPARE compare, void* param)
	{
		mMutex.lock();
		List<TYPE>::sortItems(compare, param);
		mMutex.unlock();
	}


	template<typename TYPE> inline void MutexList<TYPE>::deleteAllItems()
	{
		mMutex.lock();
		List<TYPE>::deleteAllItems();
		mMutex.unlock();
	}


	template<typename TYPE> inline void MutexList<TYPE>::deleteItem(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::deleteItem(item);
		mMutex.unlock();
	}


	template<typename TYPE> inline void MutexList<TYPE>::freeAllItems()
	{
		mMutex.lock();
		List<TYPE>::freeAllItems();
		mMutex.unlock();
	}


	template<typename TYPE> inline void MutexList<TYPE>::freeItem(TYPE *item)
	{
		mMutex.lock();
		List<TYPE>::freeItem(item);
		mMutex.unlock();
	}
}

#endif