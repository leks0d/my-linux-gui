
#include <mango.h>


namespace mango 
{
	List::List(void)
	{
		mHead = this;
		mNext = this;
		mPrev = this;
	}


	List::~List(void)
	{

	}

	bool List::isEmpty()
	{
		return mNext == this;
	}


	int List::count()
	{
		int	 count = 0 ;
		List *node;

		for(node = this; node->next, node != this; node = node->next) {
			count++ ;
		}

		return count ;
	}


	void List::_add(List *newnode, List *prev, List *next)
	{
		next->mPrev = newnode;
		newnode->mNext = next;
		newnode->mPrev = prev;
		prev->mNext = newnode;

		newnode->mHead = mHead;
	}


	void List::add(List* node)
	{
		_add(node, mHead, mHead->mNext);
	}


	void List::addTail(List* node)
	{
		_add(node, mHead->mPrev, mHead);
	}


	void List::del()
	{
		mNext->mPrev = mPrev;
		mPrev->mNext = mNext;
		mHead = this;
	}


	List *List::first()
	{
		if (mHead->mNext == mHead)
			return NULL;
		else
			return mHead->mNext;
	}


	List *List::tail()
	{
		if (mHead->mPrev == mHead)
			return NULL ;
		else
			return mHead->mPrev;
	}


	List* List::next(bool cycle)
	{
		List* node;
	
		if (isEmpty())
			return NULL ;

		node = this->mNext;
		if ((node == mHead) && cycle)
			node = mHead->mNext ;

		if (node == mHead)
			return NULL;
		else
			return node;
	}

	List* List::prec(bool cycle)
	{
		List* node;

		if (isEmpty())
			return NULL;

		node = this->mPrev;
		if ((node == mHead) && cycle)
			node = mHead->mPrev;

		if (node == mHead)
			return NULL;
		else
			return node;
	}


	List* List::node(int order)
	{
		List *node;

		for(node = mHead->next; node->next != mHead && order > 0; node = node->next) {
			order--;
		}

		if (order || node == mHead)
			return NULL;
		else
			return node;
	}

	
	int List::order()
	{
		int order = 0;
		List *node;

		for(node = mHead->next; node->next != mHead && node != this; node = node->next) {
			order++;
		}

		if (node == this)
			return order;
		else
			return -1;
	}

}