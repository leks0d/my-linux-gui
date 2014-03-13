
#include "player.h"
namespace mango
{
	uint32_t Uvcontert::getpossibleEncodings(const char *entry){
		uint8_t* src = (uint8_t *)entry;
		uint8_t uch;
		uint32_t tmpEncoding,encoding=0;
		bool noAsci = false;
		int ret = 0;
		
		if(!entry)
			return 0;
		
        while ((uch = *src++)) {
			if (uch & 0x80){
				noAsci = true;
				break;
			}
        }
		
		if(noAsci){
			if(possibleEncodings&&ponvertValues){
				tmpEncoding = possibleEncodings(entry);
				if(tmpEncoding != kEncodingAll && tmpEncoding != kEncodingNone){
					encoding = tmpEncoding;
				}
			}else
				log_i("-----possibleEncodings is null");
		}else
			encoding = kEncodingAll;
		
		return encoding;		
	}

Uvcontert::Uvcontert(){
	encode = kEncodingAll;
}

Uvcontert::~Uvcontert(){
	encode = 0;
}
void Uvcontert::addItem(const char * str){
	uint32_t tmp = getpossibleEncodings(str);
	entry.addString(str);
	encode = encode & tmp;
}
void Uvcontert::doContert(){
	uint32_t locale;
	int i;
	if(gSessionLocal.getLangId() == LANGID_SIMPLIFIED ||
		gSessionLocal.getLangId() == LANGID_TRADITIONAL){
		locale = kEncodingGBK;
	}else{
		locale = kEncodingShiftJIS;
	}
	for(i=0;i<entry.getCount();i++){
		char out[1024];
		CString item;
		
		entry.getCString(i,item);

		if(encode != kEncodingAll && encode != kEncodingNone)){
			if(encode & locale){
				ponvertValues(locale,item.string,out);
			}else if(encode & kEncodingShiftJIS){
				ponvertValues(kEncodingShiftJIS,item.string,out);
			}else if(encode & kEncodingGBK){
				ponvertValues(kEncodingGBK,item.string,out);
			}else{
				strcpy(out,item.string);
			}
		}else{
			strcpy(out,item.string);
		}
		
		result.addString(out);
	}
	
}
int Uvcontert::getItem(int pos,CString& str){
	return result.getCString(pos,str);
}

};


