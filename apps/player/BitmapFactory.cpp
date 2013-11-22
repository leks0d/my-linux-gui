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
			SkBitmap skBitmap,*pskBitmap;
			SkCanvas *skCanvas;

			start = Time::getMillisecond();
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();//分配位图所占空间
		    memset((char *)pskBitmap->getPixels(),0,w*h*4);
		    
			bool ret = SkImageDecoder::DecodeFile(path,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);			

			if(ret){
				log_i("skBitmap->width()=%d,skBitmap->height()=%d",skBitmap.width(),skBitmap.height());

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
			end = Time::getMillisecond() - start;
			log_i("DecodeFile space time %dms",end);
		}		
	}

	bool BitmapFactory::decodeBuffer(MSkBitmap* mMSkBitmap,void* buf,int size,int w,int h){
		if( buf == NULL && size <= 0){
			mMSkBitmap->release();
		}else{
			int start,end;
			SkBitmap skBitmap,*pskBitmap;
			SkCanvas *skCanvas;

			start = Time::getMillisecond();
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();//分配位图所占空间
		    memset((char *)pskBitmap->getPixels(),0,w*h*4);
		    
			//bool ret = SkImageDecoder::DecodeFile(path,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);			
			bool ret = SkImageDecoder::DecodeMemory(buf,size,&skBitmap,SkBitmap::kARGB_8888_Config,SkImageDecoder::kDecodePixels_Mode);
			if(ret){
				log_i("skBitmap->width()=%d,skBitmap->height()=%d",skBitmap.width(),skBitmap.height());

				skCanvas = new SkCanvas(*pskBitmap);
				SkIRect srcRect;
				SkRect dstRect;
				
				srcRect.set(0,0,skBitmap.width(),skBitmap.height());
				PlayingView::CalculateSize(skBitmap.width(),skBitmap.height(),w,h,dstRect);

				skCanvas->drawBitmapRect(skBitmap,&srcRect,dstRect);

				skCanvas->restore();
				mMSkBitmap->create((int *)pskBitmap->getPixels(),pskBitmap->width(),pskBitmap->height());
			}else{
				//log_i("DecodeFile fail path=%s\n",path);
				mMSkBitmap->release();
			}
			end = Time::getMillisecond() - start;
			log_i("DecodeFile space time %dms",end);
		}		
	}

	void Environment::space_info(char *path,__u32& toatl,__u32& avail,__u32& free)
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
		char *COMMAND_FILE = "/cache/recovery/command";
		char *cmd_data = "--wipe_data";
		char *cmd_cache = "--wipe_cache";
		char *cmd_all = "--wipe_all";
		char *reboot = "reboot recovery";
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
		char *COMMAND_FILE = "/cache/recovery/command";
		char *cmd_data = "--wipe_data";
		char *cmd_cache = "--wipe_cache";
		char *cmd_all = "--wipe_all";
		char *cmd_update = "--update_rkimage=/mnt/sdcard/update.img";
		char *reboot = "reboot recovery";
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
		char *cmd = "reboot";
		system(cmd);
	}

	void Environment::openMute(){
		char *cmd = "./system/bin/muteopen";
		system(cmd);
	}

	void Environment::sync(){
		char *cmd = "./system/bin/sync";
		system(cmd);
	}

	bool Environment::sdcardNeedScanner(){
		char *bootfile = "/mnt/boot";
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
		char *path = "/dev/block/mmcblk0p1";
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
	
};
