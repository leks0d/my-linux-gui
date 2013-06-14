
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

		do
		{

			if (mFile == NULL || id < mHeader.m_dwStringStartItem )
				break;

			id  = (id - mHeader.m_dwStringStartItem) * (mHeader.m_dwLanguages) ;
			id += mHeader.m_dwStringStartItem ;
			id += 0; //gsys_pUserSessionObj->m_wLanguage ; //mHeader.m_dwLanguages ;

			if (id >= mHeader.m_dwItems)
				break ;

			dwOffset = mHeader.m_dwOffsetOfItem + id * sizeof (RESOURCEDATA_ITEM);
			fseek(mFile, dwOffset, SEEK_SET);
			fread(&Item, 1, sizeof (RESOURCEDATA_ITEM), mFile);

			if (Item.m_dwType != (DWORD)RT_STRING)
				break ;

			iResTChars = Item.m_dwBytes / sizeof (TCHAR) ;
			if (bufferSize < iResTChars)
				break ;

			fseek (mFile, Item.m_dwOffset + mHeader.m_dwOffsetOfData, SEEK_SET);
			fread (buffer, 1, Item.m_dwBytes, mFile) ;

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

