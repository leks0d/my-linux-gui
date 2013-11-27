#include "player.h"
#include <sys/socket.h>
#include <linux/netlink.h>
#include <cutils/sockets.h>
#include <poll.h>
#include <sys/un.h>   
#define UNIX_DOMAIN "UNIX_domain" 
#define UNIX_MSG_FLASH_UNMOUNT "Volume flash /mnt/sdcard state changed from 4 (Mounted) to 5 (Unmounting)"
#define UNIX_MSG_FLASH_MOUNT   "Volume flash /mnt/sdcard state changed from 3 (Checking) to 4 (Mounted)"
#define UNIX_MSG_SDCARD_UNMOUNT   "Volume sdcard /mnt/external_sd state changed from 1 (Idle-Unmounted) to 0 (No-Media)"
#define UNIX_MSG_SDCARD_MOUNT   "Volume sdcard /mnt/external_sd state changed from 3 (Checking) to 4 (Mounted)"
#define UNIX_MSG_SDCARD_START_UMOUNT "Volume sdcard /mnt/external_sd state changed from 4 (Mounted) to 5 (Unmounting)"
#define UNIX_MSG_SDCARD_SHARE		"Volume sdcard /mnt/external_sd state changed from 1 (Idle-Unmounted) to 7 (Shared-Unmounted)"
namespace mango
{
	const char HEADSET_STATE_PATH[]= "/sys/class/switch/h2w/state";
	const char  headName[]={"SWITCH_NAME=h2w"};
	const char  headState[]={"SWITCH_STATE="};
	char  sdcardState[]=SDCARD_BLOCK_PATH;
	SocketDetect::SocketDetect(void){
/********************开机检测SD card 状态，判断是否需要扫描*************************/		
		if(!FileAttr::FileExist(sdcardState)){
			Environment::sdcardNeedScanner();	//not SD card ,create file.
		}else{
			log_i("exist Sdcard ,don't create file.");	//exist Sdcard ,don't create file.
		}
/************如果开机时没有没有SD卡，创建boot文件，有则不创建************************/
	}

	SocketDetect::~SocketDetect(void){
	}
	
	unsigned int SocketDetect::DetectLoop(void* p){
		SocketDetect *socketDetect = (SocketDetect *)p;
		socketDetect->DetectRun();
	}
#if 0	
	int SocketDetect::SocketInit(){
    struct sockaddr_nl addr;
    int sz = 64*1024;
    int s;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = PF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;

    s = socket(PF_NETLINK,SOCK_DGRAM,NETLINK_KOBJECT_UEVENT);
    if(s < 0)
        return 0;
		log_i("NETLINK_KOBJECT_UEVENT = %d",NETLINK_KOBJECT_UEVENT);	
		
    setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));

    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(s);
        return 0;
    }

    mFd = s;
    
    mDetectThread.create(DetectLoop,this);
	
    return (mFd >= 0);		
	}
#else
	int SocketDetect::SocketInit(){
    socklen_t clt_addr_len;  
    int listen_fd;  
    int com_fd;  
    int ret;  
    int i;  
    static char recv_buf[1024];   
    int len;  
 
    struct sockaddr_un srv_addr;  
    listen_fd=socket(AF_UNIX,SOCK_STREAM,0);  
    if(listen_fd<0)  
    {  
        perror("cannot create communication socket");  
        return -1;  
    }    
    //set server addr_param  
    srv_addr.sun_family=AF_UNIX;
    strncpy(srv_addr.sun_path,UNIX_DOMAIN,sizeof(srv_addr.sun_path)-1);  
    unlink(UNIX_DOMAIN);
    //bind sockfd & addr
    ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));  
    if(ret==-1)
    { 
        perror("cannot bind server socket");  
        close(listen_fd);  
        unlink(UNIX_DOMAIN);  
        return -2;  
    }
    //listen sockfd   
    ret=listen(listen_fd,1);  
    if(ret==-1)
    {
        perror("cannot listen the client connect request");  
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return -3;
    }
    //have connect request use accept  
/*    len=sizeof(clt_addr);
    com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);  
    if(com_fd<0)  
    {  
        perror("cannot accept client connect request");  
        close(listen_fd);  
        unlink(UNIX_DOMAIN);  
        return -4;
    }
    //read and printf sent client info  
    log_i("/n=====info=====/n");  
    for(i=0;i<4;i++)  
    {
        memset(recv_buf,0,1024);  
        int num=read(com_fd,recv_buf,sizeof(recv_buf));  
        log_i("Message from client (%d)) :%s/n",num,recv_buf);    
    }*/
//    close(com_fd);  
//    close(listen_fd);  
//    unlink(UNIX_DOMAIN);  
//    return 1;  

	    mFd = listen_fd;
	    
	    mDetectThread.create(DetectLoop,this);
		
	    return (mFd >= 0);		
	}
#endif	
	
#if 0	
	void SocketDetect::DetectRun(void){
		char buffer[1024];
		int len;
		while (1){
			while (1) {
		      struct pollfd fds;
		      int nr;
		    
		      fds.fd = mFd;
		      fds.events = POLLIN;
		      fds.revents = 0;
		      nr = poll(&fds, 1, -1);
		     
		      if(nr > 0 && fds.revents == POLLIN) {
		        int count = recv(mFd, buffer, sizeof(buffer), 0);
		        if (count > 0) {
		        		len = count;
		           break;
		        } 
		      }
		    }
		    buffer[len] = '\0';
		    log_i("SocketDetect::DetectRun %s",buffer);
		}
	}
#else
void SocketDetect::DetectRun(void){
	char recv[1024];
	int len,com_fd;
	struct sockaddr_un clt_addr;
	while (1){
	    len = sizeof(clt_addr);
	    com_fd = accept(mFd,(struct sockaddr*)&clt_addr,&len);  
	    if(com_fd<0){
	        close(mFd);
	        unlink(UNIX_DOMAIN);
	        break;
	    }
	    //read and printf sent client info  
	    while(1){
         memset( recv, 0, sizeof(recv));
         len = read(com_fd, recv, sizeof(recv));
         if(len == 0){
            close( com_fd );
            log_i("close( com_fd )");
            break;
         }else if(len < 0){
            unlink(UNIX_DOMAIN);
            close(com_fd);
            close(mFd);
            break;
         }else{
         	int result = 0;
            log_i("Message from client:%s",recv);
			if(strcmp(recv,UNIX_MSG_SDCARD_MOUNT)==0){
				log_i("sdcard insert");
				result = SDCARD_MOUNT;
			}else if(strcmp(recv,UNIX_MSG_SDCARD_UNMOUNT)==0){
				log_i("sdcard eject");
				result = SDCARD_UNMOUNT;
			}else if(strcmp(recv,UNIX_MSG_FLASH_UNMOUNT)==0){
				log_i("usb open");
				result = FLASH_UNMOUNT;
			}else if(strcmp(recv,UNIX_MSG_FLASH_MOUNT)==0){
				log_i("usb close");
				result = FLASH_MOUNT;
			}else if(strcmp(recv,UNIX_MSG_SDCARD_START_UMOUNT)==0){
				log_i("sdcard start unmounting");
				result = SDCARD_START_UNMOUNT;
			}else if(strcmp(recv,UNIX_MSG_SDCARD_SHARE)==0){
				log_i("sdcard start share");
				result = SDCARD_SHARE_UNMOUNT;
			}else
				log_i("usb unknow");
			if(result > 0)
		   		gPowerManager->setPowerState(0);
			
			log_i("set power state POWER_STATE_ON");
			if(mPlayerEventInterface != NULL)
				mPlayerEventInterface->onKeyDispatch(result,VM_MEDIA,0);
         }
	    }
	}
	log_i("SocketDetect::DetectRun Exit");
}

#endif
};
