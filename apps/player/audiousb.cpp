#include "mango.h"
#include "mango/thread.h"
#include <android/log.h> 
#include <sys/system_properties.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <poll.h>
#include <sys/un.h>  
#define LOG_TAG "AudioUSB"

#if 1
#define logi(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define logi(...)
#endif
	int gRun = 1;		
	//volatile
	typedef struct tagRingBufferContext
	{
		unsigned int mSize;
		unsigned int mIn;
		unsigned int mOut;

	} RingBufferContext;


	class RingBuffer
	{
	public:
		RingBuffer();
		~RingBuffer();


		void setBufferSize(unsigned int size);
		void clear();

		unsigned int getFreeBuffer(unsigned char **buffer1, unsigned int *len1, \
			unsigned char **buffer2, unsigned int *len2);

		unsigned int put(unsigned int len);

		unsigned int getDataBuffer(unsigned char **buffer1, unsigned int *len1, \
			unsigned char **buffer2, unsigned int *len2);

		unsigned int get(unsigned int len);

		bool isEmpty();

		unsigned int getDataLen();

		//		void waitFreeSpace();

	private:

		unsigned int mSize;
		unsigned int mIn;
		unsigned int mOut;
		unsigned char*		mBuffer;

	};





	//#define BUFFER_SIZE_IS_2_POWER
	#define FRAME_BYTES       2

	RingBuffer::RingBuffer()
	{
		mIn   = 0;
		mOut  = 0;
		mSize = 0;
		mBuffer  = NULL;
	}


	RingBuffer::~RingBuffer()
	{
		if (mBuffer == NULL)
			free(mBuffer);

		mBuffer = NULL;
	}



	void RingBuffer::setBufferSize(unsigned int size)
	{
		log_i("RingBuffer::setBufferSize size =%d, mBuffer=0x%x", size,  mBuffer);

		if (mBuffer == NULL)
			free(mBuffer);

		mBuffer = (unsigned char*)malloc(size);


		mIn  = 0;
		mOut = 0;
		mSize = size;

		log_i("RingBuffer::setBufferSize size =%d, mBuffer=0x%x", size,  mBuffer);
	}


	void RingBuffer::clear()
	{
		mIn  = 0;
		mOut = 0;
	}


	unsigned int RingBuffer::getFreeBuffer(unsigned char **buffer1, unsigned int *len1, \
		unsigned char **buffer2, unsigned int *len2)
	{

		unsigned int l;
		unsigned int len;

		len = mSize - mIn + mOut; 

		/* first put the data starting from mContext->mIn to buffer end */ 
#ifdef BUFFER_SIZE_IS_2_POWER
		l = min(len, mContext->mSize - (mContext->mIn  & (mContext->mSize - 1))); 
		*buffer1 = mBuffer + (mContext->mIn & (mContext->mSize - 1)); 
#else
		l = min(len, mSize - (mIn % mSize)); 
		*buffer1 = mBuffer + (mIn % mSize); 
#endif
		*len1    = l;

		/* then put the rest (if any) at the beginning of the buffer */ 
		*buffer2 = mBuffer;
		*len2    = len - l;

		//_ASSERT(*len1 % FRAME_BYTES == 0);
		//_ASSERT(*len2 % FRAME_BYTES == 0);

		return len; 
	}


	unsigned int RingBuffer::put(unsigned int len)
	{
		mIn += len; 

		//	_ASSERT(len % FRAME_BYTES == 0);

		return len;
	}


	unsigned int RingBuffer::getDataBuffer(unsigned char **buffer1, unsigned int *len1, \
		unsigned char **buffer2, unsigned int *len2)
	{
		unsigned int l;
		unsigned int len;

		len = mIn - mOut; 

		/* first get the data from mContext->mOut until the end of the buffer */ 
#ifdef BUFFER_SIZE_IS_2_POWER
		l = min(len, mContext->mSize - (mContext->mOut & (mContext->mSize - 1))); 
		*buffer1 = mBuffer + (mContext->mOut & (mContext->mSize - 1)) ;
#else
		l = min(len, mSize - (mOut % mSize)); 
		*buffer1 = mBuffer + (mOut % mSize) ;
#endif


		*len1    = l;

		/* then get the rest (if any) from the beginning of the buffer */ 
		*buffer2 = mBuffer;
		*len2    = len - l;

		//		_ASSERT(*len1 % FRAME_BYTES == 0);
		//		_ASSERT(*len2 % FRAME_BYTES == 0);

		return len; 
	}


	unsigned int RingBuffer::get(unsigned int len)
	{
		mOut += len; 

		//		_ASSERT(len % FRAME_BYTES == 0);
		//		pthread_cond_broadcast(&mCond);

		return len; 
	}


	bool RingBuffer::isEmpty()
	{
		return (mOut == mIn);
	}

#define USB_FUNCTION_PATH 	"/sys/class/android_usb/android0/functions"
#define USB_STATE_PATH 	"/sys/class/android_usb/android0/state"
#define USB_FUNCTION_HIFI "hifi\n"
#define USB_CONNECT_STAT "DISCONNECTED\n"

	int isHifiMode(){
		int fd = -1;
		int result = 0;
		char buf[20] = {0};
		
		fd = open(USB_FUNCTION_PATH, O_RDONLY);
		if(fd<=0){
				LOGI("open %s fail",USB_FUNCTION_PATH);
				return 0;
		}
		read(fd,buf,20);
		close(fd);
		
		if(strcmp(buf,USB_FUNCTION_HIFI)==0){
			
					fd = open(USB_STATE_PATH, O_RDONLY);
					if(fd<=0){
							LOGI("open %s fail",USB_STATE_PATH);
							return 0;
					}
					read(fd,buf,20);
					close(fd);
					
					if(strcmp(buf,USB_CONNECT_STAT)){
						result = 1;
					}
					
					LOGI("isHifiMode buf=%s result=%d",buf,result);
		}else{
				result = 0;
		}
		
		return result;
	}
int sendMyMsg(const char *msg){
    int connect_fd;
    int ret;
    char snd_buf[1024];
    int i;
    static struct sockaddr_un srv_addr;
//creat unix socket
    connect_fd=socket(AF_UNIX,SOCK_STREAM,0);
    if(connect_fd<0)  
    {
        perror("cannot create communication socket");
        return 0;
    }
    srv_addr.sun_family=AF_UNIX;  
    strcpy(srv_addr.sun_path,"UNIX_domain");  
//connect server  
    ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));  
    if(ret==-1)  
    {
        LOGI("connect err:%s",strerror(errno));
        close(connect_fd);
        return 0;
    }
    memset(snd_buf,0,1024);
    strcpy(snd_buf,msg);
//send info server
   	write(connect_fd,snd_buf,sizeof(snd_buf));
    close(connect_fd);
    return 1;
}

void sendAudioMsg(int rate,int bit){
	int ret;
	char buf[1024];
	
	sprintf(buf,"usbAudio msg:Rate=%d,Bit=%d",rate,bit);
	
	ret = sendMyMsg(buf);
	
	if(ret)
		LOGI("send audioMsg buf=%s",buf);
}

	RingBuffer  gHiFiRing;


	int WriteRingBuffer(void *data, int dataSize)
	{
		int writeBytes = 0;
		

		unsigned char *buffer1 ;
		unsigned char *buffer2 ;
		unsigned int  len1, len2, size;
		unsigned char *dataPos = (unsigned char *)data;
		int	count = dataSize;
		
		logi("WriteRingBuffer %s",dataPos);
		
		while (count > 0)  
		{
			gHiFiRing.getFreeBuffer(&buffer1, &len1, &buffer2, &len2);

			if (len1 > 0)
			{
				size = min(len1, count);
				memcpy(buffer1, dataPos, size);
				gHiFiRing.put(size);
			}
			else if (len2 > 0)
			{
				size = min (len2, count) ;
				memcpy(buffer2, dataPos, size);
				gHiFiRing.put(size);
			}
			else
			{
				size = 0 ;
#ifdef WIN32
				Sleep(1);
#else
				usleep(1000);
#endif
			}
			dataPos += size;
			count   -= size;
		} 

		return dataSize;
	}



	int ReadRingBuffer(void* dataBuffer, int bufferSize)
	{
		unsigned char*	buffer[2] ;
		unsigned int	dataLen[2];
		unsigned char*	raw;
		unsigned int    rawLen;

		unsigned int	len;
		unsigned char*	out;
		int				outFreeBytes;
		int				i;


		out	= (unsigned char*)dataBuffer;
		outFreeBytes = bufferSize;

		//while (outFreeBytes > 0)
		do
		{
			gHiFiRing.getDataBuffer(&buffer[0], &dataLen[0], &buffer[1], &dataLen[1]);
			if ((dataLen[0] == 0) && (dataLen[1] == 0))
			{
				break;
			}

			for (i = 0 ; i < 2; i++)
			{
				raw	   = (unsigned char*)buffer[i];
				rawLen = dataLen[i]; 

				len = min((int)rawLen, outFreeBytes);
				memcpy(out, raw, len);

				out			 += len;
				outFreeBytes -= len;

				gHiFiRing.get(len);
			}

		} while(0);

		return bufferSize - outFreeBytes;
	}



	
	int ReadRingBuffer2(void* dataBuffer, int bufferSize)
	{
		unsigned char*	buffer[2] ;
		unsigned int	dataLen[2];
		unsigned char*	raw;
		unsigned int    rawLen;

		unsigned int	len;
		unsigned char*	out;
		int				outFreeBytes;
		int				i;


		out	= (unsigned char*)dataBuffer;
		outFreeBytes = bufferSize;

		while (outFreeBytes > 0)
		{
			gHiFiRing.getDataBuffer(&buffer[0], &dataLen[0], &buffer[1], &dataLen[1]);
			if ((dataLen[0] == 0) && (dataLen[1] == 0))
			{
				usleep(1000);
			}

			for (i = 0 ; i < 2; i++)
			{
				raw	   = (unsigned char*)buffer[i];
				rawLen = dataLen[i]; 

				len = min((int)rawLen, outFreeBytes);
				memcpy(out, raw, len);

				out			 += len;
				outFreeBytes -= len;

				gHiFiRing.get(len);
			}

		};// while(0);

		return bufferSize - outFreeBytes;
	}




	int gReadingTouchCount = 0;
	int gReadingCount = 0;

	unsigned int USBHiFiReading(void *parameter)
	{
		FILE *fp = NULL;
		int  readlen;
		unsigned char *buffer[512];

		log_i("USBHiFi::USBHiFiReading");


		while(1)
		{
			if (fp == NULL //&& isHifiMode()
				)
				fp = fopen("/dev/android_hifi", "r");
			if (fp)
			{
				readlen = fread(buffer, 1, 16, fp);
				if (readlen > 0)
				{
					gReadingCount += readlen;
					WriteRingBuffer(buffer, readlen);
				}
			}
			else
			{
				logi("fopen /dev/android_hifi fail usleep ");
				usleep(1000 * 1000);
			}

		}

		return 0;
	}


	//ringBufferContext = (HifiRingBufferContext*)mRingBufferMemory;
	//ringBufferContext->mSize = HIFI_HIFI_BUFFER_SIZE;
	
	//mMixer.mHifiRingBuffer.set(ringBufferContext, ringBufferContext + 1);
	//mMixer.mHifiRingBuffer.clear();

	unsigned int USBConnectDetect(void *parameter)
	{
		while(1)
		{
			if(!isHifiMode()){
				char board[PROP_VALUE_MAX]={0};
				__system_property_get("init.svc.usbd",board);
				if(strcmp(board,"running")==0){
					gRun = 0;
					usleep(1000 * 1000);
					sendMyMsg("usbAudio stop");
					
				}
			}
			usleep(1000 * 1000);
		}
	}


extern "C"
{
	struct pcm;

	int pcm_write(struct pcm *pcm, void *data, unsigned count);
	int pcm_read(struct pcm *pcm, void *data, unsigned count);
	int pcm_close(struct pcm *pcm);
	struct pcm *pcm_open(unsigned flags);
	struct pcm *pcm_open_hifi(unsigned flags, int channels, int rate, int sampleBits);
	int pcm_ready(struct pcm *pcm);
	unsigned int pcm_buffer_size(struct pcm *pcm);
};

#define HIFI_HIFI_BUFFER_SIZE	(1024 * 256 * 8) //192k 24bits(32bits)Ô¼ÑÓ³Ù 0.16s;  44.1khz 16bits Ô¼ÑÓ³Ù2.8s 
#define isHifiTrack(rate, bits) (rate > 4800 || bits > 16)

#define hifi_pcm_open  pcm_open_hifi
#define hifi_pcm_write pcm_write
#define hifi_pcm_close pcm_close
#define hifi_pcm_ready pcm_ready

#define HIFI_PACKET_SIZE  4096

typedef struct tagHIFI_PACKET
{
	unsigned short mSize;
	unsigned short mType;
	unsigned char  data[HIFI_PACKET_SIZE - 4];
} HIFI_PACKET ;

enum HIFI_PACKET_TYPE
{
	HIFI_PACKET_TYPE_HEADER = 0x0101,

	HIFI_PACKET_TYPE_DATA   = 0x0201,

	HIFI_PACKET_TYPE_PLAY   = 0x0301,
	HIFI_PACKET_TYPE_PAUSE  = 0x0302,
	HIFI_PACKET_TYPE_STOP   = 0x0303,
};



typedef struct tagHIFI_PACKET_HEADER
{
	unsigned short mSize;
	unsigned short mType;  // HIFI_PACKET_TYPE_HEADER
	int			   mChannels;
	int			   mSamplingRate;
	int			   mBitPerSample;
	
} HIFI_PACKET_HEADER ;

	
HIFI_PACKET_HEADER  gHiFiHeader;
int gPacketType = 0;
unsigned char gPacketData[128];
int gSendingRecCount = 0;
int gSendingCount = 0;




#define FORMAT_PCM 1
#define AUDIO_HW_OUT_PERIOD_MULT 8 // (8 * 128 = 1024 frames)
#define AUDIO_HW_OUT_PERIOD_CNT  4

#define PCM_OUT        0x00000000
#define PCM_IN         0x10000000

#define PCM_STEREO     0x00000000
#define PCM_MONO       0x01000000

#define PCM_44100HZ    0x00000000
#define PCM_48000HZ    0x00100000
#define PCM_8000HZ     0x00200000
#define PCM_RATE_MASK  0x00F00000

#define PCM_PERIOD_CNT_MIN 2
#define PCM_PERIOD_CNT_SHIFT 16
#define PCM_PERIOD_CNT_MASK (0xF << PCM_PERIOD_CNT_SHIFT)
#define PCM_PERIOD_SZ_MIN 128
#define PCM_PERIOD_SZ_SHIFT 12
#define PCM_PERIOD_SZ_MASK (0xF << PCM_PERIOD_SZ_SHIFT)




	unsigned int USBHiFiSending(void *parameter)
	{
		struct pcm*		mHiFiOut;
		HIFI_PACKET* packet = (HIFI_PACKET *)malloc(sizeof(HIFI_PACKET));


		unsigned flags = PCM_OUT;
		flags |= PCM_STEREO;
		flags |= (AUDIO_HW_OUT_PERIOD_MULT - 1) << PCM_PERIOD_SZ_SHIFT;
		flags |= (AUDIO_HW_OUT_PERIOD_CNT - PCM_PERIOD_CNT_MIN) << PCM_PERIOD_CNT_SHIFT;

		
		while(1)
		{

			ReadRingBuffer2(packet, HIFI_PACKET_SIZE);
			gSendingRecCount += HIFI_PACKET_SIZE;
			memcpy(gPacketData, packet, 128);

			if (packet->mType != HIFI_PACKET_TYPE_HEADER)
				continue;

			gPacketType = packet->mType;
			memcpy(&gHiFiHeader, packet, sizeof(HIFI_PACKET_HEADER));

			mHiFiOut = hifi_pcm_open(flags, gHiFiHeader.mChannels, gHiFiHeader.mSamplingRate, gHiFiHeader.mBitPerSample);

			if (!hifi_pcm_ready(mHiFiOut)) 
			{
			//	logn_e("can't open hifi device");
				hifi_pcm_close(mHiFiOut);
				continue;
			}


			while(1)
			{
				ReadRingBuffer2(packet, HIFI_PACKET_SIZE);
				gSendingRecCount += HIFI_PACKET_SIZE;
				gSendingCount += HIFI_PACKET_SIZE;
				memcpy(gPacketData, packet, 128);

				gPacketType = packet->mType;

				if (packet->mType == HIFI_PACKET_TYPE_STOP)
					break;
				
				if (packet->mType == HIFI_PACKET_TYPE_DATA)
					pcm_write(mHiFiOut, packet->data, packet->mSize - 4);
			}

			hifi_pcm_close(mHiFiOut);
			mHiFiOut = NULL;
		}

		return 0;
	} 
typedef void* (*PTHREAD_START_ROUTINE)(void*);
int main (int argc, char* argv[])
{
		int n = 10;
		pthread_t		readThreadId ;
		pthread_t		sendThreadId ;
		
		gHiFiRing.setBufferSize(1024 * 4);
		gHiFiRing.clear();
	
		pthread_create(&readThreadId, NULL, (PTHREAD_START_ROUTINE)USBHiFiReading, (void*)(&n));
		//pthread_join(readThreadId,NULL);
		
		pthread_create(&sendThreadId, NULL, (PTHREAD_START_ROUTINE)USBHiFiSending, (void*)(&n));
		//pthread_join(sendThreadId,NULL);
		
		//pthread_create(&sendThreadId, NULL, (PTHREAD_START_ROUTINE)USBConnectDetect, (void*)(&n));
		
		LOGI("usbhifi init end-.");
		while(1){
			usleep(1000 * 1000);
			gReadingTouchCount++;
		}
		
		return 0;
}