
#include <mango.h>

namespace mango
{

	Resource::Resource()
	{
		mFile = NULL;
	}


	Resource::~Resource()
	{

	}


	bool Resource::open(const char *path)
	{
		mFile = fopen(path, "rb");
		if (mFile == NULL) {
			log_e ("Can't open resource file %c \n", path);
			return false;
		}

		fread(&mHeader, 1, sizeof (RESOURCEDATA_HEADER), mFile);
		return true;
	}


	void Resource::close(void)
	{
		if (mFile)
			fclose(mFile);

		mFile = NULL;
	}


	int Resource::loadString(int id,  TCHAR* buffer,  int bufferSize, int language)
	{
		int	 iResTChars, iRtnTChars = 0 ;
		DWORD dwReadBytes ;
		DWORD dwOffset ;
		RESOURCEDATA_ITEM Item ;
		short* readbuf,i;

		do
		{
			if (mFile == NULL || id < mHeader.m_dwStringStartItem ||language >= mHeader.m_dwLanguages)
				break;

			id  = (id - mHeader.m_dwStringStartItem) * (mHeader.m_dwLanguages) ;
			id += mHeader.m_dwStringStartItem ;
			id += gSessionLocal.mLanguageId; //gsys_pUserSessionObj->m_wLanguage ; //mHeader.m_dwLanguages ;

			
			
			if (id >= mHeader.m_dwItems)
				break ;
			//log_i("Resource::loadString sizeof(RESOURCEDATA_HEADER)=0x%x",sizeof(RESOURCEDATA_HEADER));
			dwOffset = mHeader.m_dwOffsetOfItem + id * sizeof (RESOURCEDATA_ITEM);
			//log_i("Resource::loadString dwOffset=0x%x",dwOffset);
			fseek(mFile, dwOffset, SEEK_SET);
			fread(&Item, 1, sizeof (RESOURCEDATA_ITEM), mFile);

			if (Item.m_dwType != (DWORD)RT_STRING)
				break;

			iResTChars = Item.m_dwBytes / 2 ;
			if (bufferSize < iResTChars)
				break ;
			//log_i("Resource::loadString dwOffset=0x%x,len=%d",Item.m_dwOffset + mHeader.m_dwOffsetOfData,Item.m_dwBytes);

			readbuf = new short[255];
			fseek (mFile, Item.m_dwOffset + mHeader.m_dwOffsetOfData , SEEK_SET);
			fread (readbuf, 1, Item.m_dwBytes, mFile) ;

			for(i=0;i<Item.m_dwBytes/2;i++){
				buffer[i] = readbuf[i];
				buffer[i] &= 0x00ffff;
			}
			delete readbuf;
			buffer[iResTChars] = '\0' ;

			iRtnTChars = iResTChars ;

		}while (0) ;

		return iRtnTChars ;
	}


	int Resource::loatBitmap(int id,  PRESOURCEIMAGE_HEADER  pResImageHeader,  void* buffer, int bufferBytes)
	{
		DWORD	dwReadBytes ;
		DWORD	dwOffset ;
		RESOURCEDATA_ITEM		Item ;
		DWORD dwDataBytes = 0 ;

		do
		{

			if (mFile == NULL || id >= mHeader.m_dwItems)
				break ;

			dwOffset = mHeader.m_dwOffsetOfItem + id * sizeof (RESOURCEDATA_ITEM);
			fseek(mFile, dwOffset, SEEK_SET);
			fread (&Item, 1, sizeof (RESOURCEDATA_ITEM), mFile);

			if (Item.m_dwType != (DWORD)RT_BITMAP)
				break ;

			if (Item.m_dwBytes > (DWORD)bufferBytes)
				break ;

			fseek(mFile, Item.m_dwOffset + mHeader.m_dwOffsetOfData, SEEK_SET) ;
			fread(pResImageHeader, 1, sizeof (RESOURCEIMAGE_HEADER), mFile) ;
			//if (pResImageHeader->m_wBitCount != 32 && pResImageHeader->m_wBitCount != 32)
			//break ;

			fread(buffer, 1, Item.m_dwBytes - sizeof (RESOURCEIMAGE_HEADER), mFile);
			dwDataBytes = Item.m_dwBytes ;

		}while (0) ;

		return dwDataBytes ;
	}

}

