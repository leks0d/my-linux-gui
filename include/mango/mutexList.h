
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
		TYPE* getNext(TYPE* item, int cycle = false);

		//前一项
		TYPE* getPrev(TYPE* item, int cycle = false);

		//根据顺号, 获得node
		TYPE* getNode (int order);

		//根据node, 序号
		int getOrder (TYPE* item);

		//排序
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


	//增加到尾部 
	template<typename TYPE> inline void MutexList<TYPE>::add(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::add(item);
		mMutex.unlock();
	}


	//增加到头部 -- GetFirst  可以找到的
	template<typename TYPE> inline void MutexList<TYPE>::addHead(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::addHead(item);
		mMutex.unlock();
	}


	//删除指定项
	template<typename TYPE> inline void MutexList<TYPE>::del(TYPE* item)
	{
		mMutex.lock();
		List<TYPE>::del(item);
		mMutex.unlock();
	}


	//计算item 个数
	template<typename TYPE> inline int MutexList<TYPE>::getCount()
	{
		int count;
		mMutex.lock();
		count = List<TYPE>::getCount();
		mMutex.unlock();

		return count;
	}


	//第一项
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getFirst()
	{
		TYPE *obj = NULL ;

		mMutex.lock();
		obj = List<TYPE>::getFirst();
		mMutex.unlock();

		return obj ;
	}


	//尾项
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getTail()
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getTail();
		mMutex.unlock();

		return obj ;
	}


	//下一项
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getNext(TYPE* item, int cycle)
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getNext(item, cycle);
		mMutex.unlock();

		return obj;
	}


	//前一项
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getPrev(TYPE* item, int cycle)
	{
		TYPE *obj = NULL;

		mMutex.lock();
		obj = List<TYPE>::getPrev(item, cycle);
		mMutex.unlock();

		return obj;
	}


	//根据顺号, 获得node
	template<typename TYPE> inline TYPE* MutexList<TYPE>::getNode(int order)
	{
		TYPE *obj;

		mMutex.lock();
		obj = List<TYPE>::getNode(order);
		mMutex.unlock();

		return obj;
	}


	//根据node, 序号
	template<typename TYPE> inline int MutexList<TYPE>::getOrder(TYPE* item)
	{
		int	order = 0;

		mMutex.lock();
		order = List<TYPE>::getOrder(item);
		mMutex.unlock();

		return order;
	}


	//排序
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