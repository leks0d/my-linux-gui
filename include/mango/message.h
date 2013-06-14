#ifndef _MANGO_MESSAGE_H
#define _MANGO_MESSAGE_H

namespace mango 
{

	enum ViewMessageID
	{
		VM_NULL = 0,                         
		VM_CREATE,                       
		VM_DESTROY,                      

		VM_ACTIVATE,  
		VM_INACTIVATE,  

		VM_SETFOCUS,                     
		VM_KILLFOCUS,                    
		VM_ENABLE,                       
		VM_SETREDRAW,                    
		VM_SETTEXT,                      
		VM_GETTEXT,                      
		VM_GETTEXTLENGTH,                
		VM_PAINT,                        
		VM_CLOSE,                        
		VM_QUIT,                         
		VM_ERASEBKGND,                   
		VM_SYSCOLORCHANGE,               
		VM_SHOWWINDOW,                   
		VM_WININICHANGE,   


		VM_KEYDOWN, 
		VM_KEYUP,

		VM_TOUCHDOWN,
		VM_TOUCHUP,
		VM_TOUCHMOVE, 

		VM_COMMAND,
	} ;


	class EXPORT_CLASS Message; 

	class Message : public Object
	{
	public:
		Message(void);
		Message(class View* view, unsigned int id, unsigned int parameter1 = 0, unsigned int parameter2 = 0);
		virtual ~Message(void);

	public:
		class View*  mView;
		unsigned int mId; 
		unsigned int mParameter1; 
		unsigned int mParameter2; 
		unsigned int mTime;
		unsigned int mResult;

	public:

		unsigned int getId()   { return mId; };
		class View* getView()  { return mView; };
		unsigned int getParameter()  { return mParameter1; };
		unsigned int getParameter1() { return mParameter1; };
		unsigned int getParameter2() { return mParameter2; };
		unsigned int getTime()  { return mTime; };

		void setId(unsigned int id)		{ mId   = id; };
		void setView(class View* view)	{ mView = view; };
		void setParameter(unsigned int parameter)	{ mParameter1 = parameter; };
		void setParameter1(unsigned int parameter)	{ mParameter1 = parameter; };
		void setParameter2(unsigned int parameter)	{ mParameter2 = parameter; };
		void setTime(unsigned int time)	{ mTime = time; };

		Message operator=(Message &msg);

	protected:
		bool  mNeedDeleteSemaphore;
		class Semaphore* mSemaphore;

		LIST_HEAD  mList;

	public:
		friend class MessageQueue;
	};


#ifdef WIN32
	class EXPORT_CLASS MessageQueue; 
#endif

	class MessageQueue : public Object
	{
	public:
		MessageQueue(void);
		virtual ~MessageQueue(void);


	public:
		int send(class View* view, unsigned int id, unsigned int parameter1 = 0, unsigned int paramter2 = 0);
		int post(class View* view, unsigned int id, unsigned int parameter1 = 0, unsigned int paramter2 = 0);

		int send(Message& msg);
		int post(Message& msg);

		int get(Message& msg);
		int peek(Message& msg, bool remove = true);

		void setResult(int result);

		unsigned int mCustomerThreadId;

	private:
		void	  addTail(Message *msg);
		Message*  getFirst(bool remove);

		LIST_HEAD   mMessageHead;
		Mutex		mMutex;
		Semaphore	mMessageCountSemphore;

		Message*   mCurrentMsg;
		int        mCurrentResult;

	};

	extern MessageQueue gMessageQueue;

};

#endif