
namespace mango
{
	class KernelMsgGet{
			int mFd;
			
			Thread mDetectThread;
			UseEventInterface *mPlayerEventInterface;
		public:
			CString mAudioBuf;
			
			KernelMsgGet();
			~KernelMsgGet();
			int SocketInit();
			void DetectRun();
			static unsigned int DetectLoop(void* p);
			void setPlayerEventInterface(UseEventInterface *inter){mPlayerEventInterface=inter;}
	};

};
