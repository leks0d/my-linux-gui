
#ifndef _MANGO_PARTY_H
#define _MANGO_PARTY_H

namespace mango
{

	class Party : public Object
	{
	public:
		Party(void);
		virtual ~Party(void);

	public:
		int getLastError(void);
	
	public:
		virtual int initialize();
		virtual	int messageLoop();
		virtual	int dispatchMessage(Message& msg);
		virtual int processMessage(Message& msg);

		virtual int wakeLock(void);
		virtual int wakeUnlock(void);


	public:
		class Session*		mSession;
		class SessionLocal* mSessionLocal; 
		class ViewZAxis*    mViewZAxis;

	public:
		MessageQueue*  mMessageQueue;
	};

}

#endif