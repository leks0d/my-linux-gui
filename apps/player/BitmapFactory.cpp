#include "player.h"


namespace mango
{
	bool BitmapFactory::decodeFile(MSkBitmap* mMSkBitmap,char* path,int w,int h){
		if( (path == NULL) || (strcmp(path,"(null)") == 0)){
			mMSkBitmap->release();
		}else{
			SkBitmap skBitmap,*pskBitmap;
			SkCanvas *skCanvas;
			
		    pskBitmap = new SkBitmap();
		    pskBitmap->setConfig(SkBitmap::kARGB_8888_Config,w,h);
		    pskBitmap->allocPixels();//����λͼ��ռ�ռ�
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
		}		
	}


	void Environment::space_info(char *path,int& toatl,int& avail,int& free)
	{
		struct statfs sfs;

		memset(&sfs,0,sizeof(struct statfs));
		statfs(path,&sfs);

		log_i("sfs.f_blocks=%lu,sfs.f_bsize=0x%x",sfs.f_blocks,sfs.f_bsize);
		toatl = sfs.f_blocks * sfs.f_bsize/1024;
		avail = sfs.f_bavail * sfs.f_bsize/1024;
		free = sfs.f_bfree * sfs.f_bsize/1024;
		log_i("toatl=%d,avail=%d,free=%d",toatl,avail,free);
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

		ret = fwrite(cmd_all,strlen(cmd_data),1,file);
		
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
			return ;
		}

		ret = fwrite(cmd_update,strlen(cmd_update),1,file);
		
		if(ret == -1){
			log_i("recovery write fail.");
		}else
			log_i("recovery write sucess.");
		
		fclose(file);
		
		system(reboot);
	}

};
