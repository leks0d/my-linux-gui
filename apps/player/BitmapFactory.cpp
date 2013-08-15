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
		}		
	}


	void Environment::space_info(char *path,int& toatl,int& avail,int& free)
	{
		struct statfs sfs;
		
		statfs(path,&sfs);

		log_i("sfs.f_blocks=%lu,sfs.f_bsize=%u",sfs.f_blocks,sfs.f_bsize);
		toatl = sfs.f_blocks * 8;
		avail = sfs.f_bavail * 8;
		free = sfs.f_bfree * 8;
		log_i("toatl=%d,avail=%d,free=%d",toatl,avail,free);
	}

	void Environment::memSizeToStr(int size, char *space){
		float sizeG,sizeK;
		sizeK = size;
		sizeG = sizeK/1024/1024;
		sprintf(space,"%.2fG",sizeG);
	}


};
