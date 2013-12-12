#include "player.h"


namespace mango
{
	bool BitmapFactory::genBitmapFromFile(MSkBitmap* mMSkBitmap,char* path,int w,int h){
		MSkBitmap mskBitmap;
		
		mskBitmap.createFile(path);
		
		if(mskBitmap.isVaild()){
			SkBitmap skBitmap,*pskBitmap;
			
			skBitmap.setConfig(SkBitmap::kARGB_8888_Config,mskBitmap.width,mskBitmap.height);
			skBitmap.setPixels(mskBitmap.mBits);

			pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();
		    memset((char *)pskBitmap->getPixels(),0,w*h*4);

			SkCanvas *skCanvas = new SkCanvas(*pskBitmap);
			SkIRect srcRect;
			SkRect dstRect;
				
			srcRect.set(0,0,skBitmap.width(),skBitmap.height());
			dstRect.set(0,0,w,h);

			skCanvas->drawBitmapRect(skBitmap,&srcRect,dstRect);

			skCanvas->restore();
			
			mMSkBitmap->createNormal((int *)pskBitmap->getPixels(),pskBitmap->width(),pskBitmap->height());
			delete pskBitmap;
		}
	}
	bool BitmapFactory::decodeFile(MSkBitmap* mMSkBitmap,char* path,int w,int h){
		if( (path == NULL) || (strcmp(path,"(null)") == 0)){
			mMSkBitmap->release();
		}else{
			int start,end;
			SkBitmap skBitmap,*pskBitmap = NULL;
			SkCanvas *skCanvas = NULL;

			start = Time::getMillisecond();
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();//分配位图所占空间
		    memset((char *)pskBitmap->getPixels(),0,w*h*4);
		    
			bool ret = SkImageDecoder::DecodeFile(path,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);			

			if(ret){
				log_i("width=%d,height=%d",skBitmap.width(),skBitmap.height());

				skCanvas = new SkCanvas(*pskBitmap);
				SkIRect srcRect;
				SkRect dstRect;
				
				srcRect.set(0,0,skBitmap.width(),skBitmap.height());
				PlayingView::CalculateSize(skBitmap.width(),skBitmap.height(),w,h,dstRect);

				skCanvas->drawBitmapRect(skBitmap,&srcRect,dstRect);

				skCanvas->restore();
				mMSkBitmap->create((int *)pskBitmap->getPixels(),pskBitmap->width(),pskBitmap->height());
			}else{
				log_i("DecodeFile fail path=%s\n",path);
				mMSkBitmap->release();
			}
			
			safeDelete(skCanvas);
			safeDelete(pskBitmap);

			end = Time::getMillisecond() - start;
			log_i("DecodeFile space time %dms",end);
		}		
	}

	bool BitmapFactory::decodeBuffer(MSkBitmap* mMSkBitmap,void* buf,int size,int w,int h){
		if( buf == NULL && size <= 0){
			mMSkBitmap->release();
		}else{
			//int start,end;
			SkBitmap skBitmap,*pskBitmap = NULL;
			SkCanvas *skCanvas = NULL;

			//start = Time::getMillisecond();
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();//分配位图所占空间
		    memset((char *)pskBitmap->getPixels(),0,w*h*4);
		    
			//bool ret = SkImageDecoder::DecodeFile(path,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);			
			bool ret = SkImageDecoder::DecodeMemory(buf,size,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);
			if(ret){
				log_i("width=%d,height=%d",skBitmap.width(),skBitmap.height());

				skCanvas = new SkCanvas(*pskBitmap);
				SkIRect srcRect;
				SkRect dstRect;
				
				srcRect.set(0,0,skBitmap.width(),skBitmap.height());
				PlayingView::CalculateSize(skBitmap.width(),skBitmap.height(),w,h,dstRect);

				skCanvas->drawBitmapRect(skBitmap,&srcRect,dstRect);

				skCanvas->restore();
				mMSkBitmap->create((int *)pskBitmap->getPixels(),pskBitmap->width(),pskBitmap->height());

			}else{
				mMSkBitmap->release();
			}

			safeDelete(skCanvas);
			safeDelete(pskBitmap);
			
			//end = Time::getMillisecond() - start;
			
			//log_i("DecodeFile space time %dms",end);
		}		
	}

	void Environment::space_info(const char *path,__u32& toatl,__u32& avail,__u32& free)
	{
		struct statfs sfs;
		long start;
		start = Time::getMillisecond();
		
		memset(&sfs,0,sizeof(struct statfs));
		statfs(path,&sfs);
	
		//log_i("sfs.f_blocks=%lu,sfs.f_bsize=0x%x",sfs.f_blocks,sfs.f_bsize);
		toatl = sfs.f_blocks * sfs.f_bsize/1024;
		avail = sfs.f_bavail * sfs.f_bsize/1024;
		free = sfs.f_bfree * sfs.f_bsize/1024;
		//log_i("toatl=%d,avail=%d,free=%d",toatl,avail,free);
		log_i("space_info space Time:%dms",Time::getMillisecond()-start);
	}
	__u32 Environment::getSdcardAvailSpace(){
		__u32 state,toatl,avail,free;
		
		Environment::space_info("/mnt/external_sd",toatl,avail,free);

		return avail;
	}

	int Environment::getSdcardCid(char *cid){
		FILE* file;
		char buffer[64]={0};
		size_t ret;
		
		file  = fopen("/sys/devices/platform/rk29_sdmmc.0/mmc_host/mmc0/mmc0:0002/cid", "rt");
		
		if (file == NULL) {
			memset(cid,0,64);
			memcpy(cid,"null",4);
			return -1;
		}
		
		ret = fread(buffer, 1, 64, file);

		if(ret>=33)
			buffer[32] = '\0';
		
		log_i("buffer='%s',ret=%ld",buffer,ret);
		
		memcpy(cid,buffer,64);
		
		fclose(file);
		
		return 1;
	}

	int  Environment::updateSDcard(){
		char buf[64];
		
		getSdcardCid(buf);
		
		return gSettingProvider.updateSDcard(getSdcardAvailSpace(),buf);
	}
	bool Environment::isSDcardChange(){
		Cursor cur;
		bool isChange = true;
		CString cId,sp,csp;
		char buf[64];
		
		gSettingProvider.queryCursor(SETTING_SCANSDCARDSTATE_ID,&cur);
		
		cur.getValueCstring(0,"value",sp);
		cur.getValueCstring(0,"name",cId);

		log_i("valeu=%s,name=%s",sp.string,cId.string);
		
		csp = getSdcardAvailSpace();
		getSdcardCid(buf);
		
		if(cId == buf && sp == csp.string){
			isChange = false;
		}
		log_i("isChange = %d",isChange);
		return isChange;
	}

	void Environment::memSizeToStr(int size, char *space){
		float sizeG,sizeK;
		sizeK = size;
		sizeG = sizeK/1024/1024;
		sprintf(space,"%.2fG",sizeG);
	}

	void Environment::recovery(){
		const char *COMMAND_FILE = "/cache/recovery/command";
		const char *cmd_data = "--wipe_data";
		const char *cmd_cache = "--wipe_cache";
		const char *cmd_all = "--wipe_all";
		const char *reboot = "reboot recovery";
		FILE* file;
		int ret;
		
		file  = fopen(COMMAND_FILE, "w");
		if (file == NULL) {
			log_e("fopen error:%s",COMMAND_FILE);
			return ;
		}

		ret = fwrite(cmd_data,strlen(cmd_data),1,file);
		
		if(ret == -1){
			log_i("recovery write fail.");
		}else
			log_i("recovery write sucess.");
		
		fclose(file);
		
		system(reboot);
	}
	void Environment::install(){
		const char *COMMAND_FILE = "/cache/recovery/command";
		const char *cmd_data = "--wipe_data";
		const char *cmd_cache = "--wipe_cache";
		const char *cmd_all = "--wipe_all";
		const char *cmd_update = "--update_rkimage=/mnt/sdcard/update.img";
		const char *reboot = "reboot recovery";
		FILE* file;
		int ret;
		
		file  = fopen(COMMAND_FILE, "w");
		if (file == NULL) {
			log_e("fopen error:%s",COMMAND_FILE);
			return;
		}

		ret = fwrite(cmd_update,strlen(cmd_update),1,file);
		
		if(ret == -1){
			log_i("recovery write fail.");
		}else
			log_i("recovery write sucess.");
		
		fclose(file);
		
		system(reboot);
	}
	void Environment::reboot(){
		const char *cmd = "reboot";
		system(cmd);
	}

	void Environment::openMute(){
		const char *cmd = "./system/bin/muteopen";
		system(cmd);
	}

	void Environment::sync(){
		const char *cmd = "./system/bin/sync";
		system(cmd);
	}
	void Environment::logcat(){
		Thread thread;
		thread.create(Environment::logcatRunnig,NULL);
	}
	unsigned int Environment::logcatRunnig(void *parameter){
		char cmd[100],file[100];
		int i = 0,ret;
		
		system("logcat -c");
		
		while(1){
			if(FileAttr::FileExist("/mnt/sdcard/.album_img")){
				break;
			}else{
				Thread::sleep(1000);
			}
		}
		
		while(1){
			sprintf(file,"/mnt/sdcard/audio_%d.log",i);
			if(!FileAttr::FileExist(file))
				break;
			i++;
		}
		
		sprintf(cmd,"./system/bin/logcat -v time > %s",file);
		
		ret = system(cmd);
		log_i("logcat end ret=%d",ret);
	}

	bool Environment::sdcardNeedScanner(){
		const char *bootfile = SDCARD_BOOT_PATH;
		bool ret = FileAttr::FileExist(bootfile);
		if(!ret)
			mkdir(bootfile,0777);
		return ret;
	}

	unsigned long Environment::get_file_size(const char *path)  
	{  
	    unsigned long filesize = -1;      
	    struct stat statbuff;  
	    if(path==NULL || stat(path, &statbuff) < 0){  
	        return filesize;  
	    }else{
	        filesize = statbuff.st_size;  
	    }
		log_i("filesize=%d,path=%s",filesize,path);
	    return filesize;
	}
	bool Environment::isSDcardExist(){
		const char *path = "/dev/block/mmcblk0p1";
		return FileAttr::FileExist(path);
	}
	void Environment::MD5(char* data,CString& out){
		MD5_CTX ctx;
		char *mddata;
		unsigned char md[16];
		char buf[33]={'\0'};
		char tmp[3]={'\0'};
		int i;
		
		if(data == NULL){
			mddata = "";
		}else{
			mddata = data;
		}
		
		MD5_Init(&ctx);
		MD5_Update(&ctx,mddata,strlen(mddata));
		MD5_Final(md,&ctx);
		
		for( i=0; i<16; i++ ){
			sprintf(tmp,"%02X",md[i]);
			strcat(buf,tmp);
		}
		out = buf;
	}
	void Environment::checkWallpaper(){
		if(FileAttr::FileExist(WALLPAPER_FILE)){
			BitmapFactory::decodeFile(&gWallpaperBitmap,WALLPAPER_FILE,320,240);
		}else{
			gWallpaperBitmap.release();
		}
	}
	void Environment::drawWallpaper(Canvas& canvas){
		if(gWallpaperBitmap.isVaild()){
			canvas.drawBitmap(gWallpaperBitmap.mBits,0,0,gWallpaperBitmap.width,gWallpaperBitmap.height);
		}else{
			canvas.drawImageResource(IDP_PLAYING_BACKGROUND,0,0,false);
		}
	}
};
