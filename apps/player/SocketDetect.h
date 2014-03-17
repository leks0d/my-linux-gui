
namespace mango
{
	class SocketDetect{
			int mFd;
			
			Thread mDetectThread;
			UseEventInterface *mPlayerEventInterface;
		public:
			CString mAudioBuf;
			
			SocketDetect();
			~SocketDetect();
			int SocketInit();
			void DetectRun();
			static unsigned int DetectLoop(void* p);
			void setPlayerEventInterface(UseEventInterface *inter){mPlayerEventInterface=inter;}
	};

};
