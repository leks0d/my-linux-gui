#include "mango.h"

namespace mango
{
	Message::Message(void)
	{
		mSemaphore = NULL;
	}


	Message::Message(class View* view, unsigned int id, unsigned int parameter1, unsigned int parameter2)
	{
		mView = view;
		mId	  = id; 
		mParameter1	= parameter1; 
		mParameter2	= parameter2; 
		mTime = 0; 

		mSemaphore = NULL;
	}

	Message::~Message(void)
	{

	}


	Message Message::operator=(Message &msg)
	{
		this->mId	  = msg.mId;
		this->mView	  = msg.mView;
		this->mParameter1 = msg.mParameter1;
		this->mParameter2 = msg.mParameter2;
		this->mTime   = msg.mTime;

		this->mNeedDeleteSemaphore = false;
		this->mSemaphore = msg.mSemaphore;

		return *this; 
	}


	MessageQueue::MessageQueue(void)
		: mMessageCountSemphore(NULL, 0)
	{
		INIT_LIST_HEAD(&mMessageHead);

		mCurrentMsg = NULL;
	}


	MessageQueue::~MessageQueue(void)
	{

	}

		
	int MessageQueue::send(class View* view, unsigned int id, unsigned int parameter1, unsigned int paramter2)
	{
		Message msg;

		msg.setView(view);
		msg.setId(id);
		msg.setParameter1(parameter1);
		msg.setParameter2(paramter2);
		
		return send(msg);
	}
	
	int MessageQueue::post(class View* view, unsigned int id, unsigned int parameter1, unsigned int paramter2)
	{
		Message msg;

		msg.setView(view);
		msg.setId(id);
		msg.setParameter1(parameter1);
		msg.setParameter2(paramter2);
		
		return post(msg);
	}


	int MessageQueue::send(Message& msg)
	{
		int result;
		Message *item ;

		//send 调用者线程 不能 与 消息处理位于同一线程， 否则会死锁
		_ASSERT(Thread::getCurrentId() != mCustomerThreadId);

		item  = new Message();
		*item = msg;

		item->mSemaphore = new Semaphore();

		addTail(item);
		mMessageCountSemphore.up();

		item->mSemaphore->wait();
		result = item->mResult;

		delete item->mSemaphore;
		delete item;

		return result;
	}


	int MessageQueue::post(Message& msg)
	{
		Message *item;
		
		item = new Message();
		*item  = msg;

		addTail(item);
		mMessageCountSemphore.up();

		return 0;
	}


	int MessageQueue::get(Message& msg)
	{
		Message *item ;

		if (mCurrentMsg != NULL) {
			mCurrentMsg->mResult = mCurrentResult;

			if (mCurrentMsg->mSemaphore)
				mCurrentMsg->mSemaphore->release();

			mCurrentMsg = NULL;
		}

		mMessageCountSemphore.wait();

		item = getFirst(true);
		_ASSERT(item);

		msg.setId(item->getId());
		msg.setView(item->getView());
		msg.setParameter1(item->getParameter1());
		msg.setParameter2(item->getParameter2());
		msg.setTime(item->getTime());

		if (item->mSemaphore == NULL) {
			safeDelete(item);
		} else {
			mCurrentMsg = item;
		}

		return 1;
	}


	int MessageQueue::peek(Message& msg, bool remove)
	{
		return 0;
	}


	void MessageQueue::addTail(Message *msg)
	{
		mMutex.lock();
		list_add_tail(&(msg->mList), &(mMessageHead));
		mMutex.unlock();
	}


	Message*  MessageQueue::getFirst(bool remove)
	{
		Message* msg;

		mMutex.lock();
		
		if (list_empty(&mMessageHead)) {
			msg = NULL;
		} else {
			msg = list_entry(mMessageHead.next, Message, mList); 
			if (remove) {
				list_del(&msg->mList);
			}
		}
		
		mMutex.unlock();
		
		return msg;
	}


	void MessageQueue::setResult(int result)
	{
		mCurrentResult = result;
	}






	MessageQueue gMessageQueue;
}
