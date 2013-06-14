#include "mango.h"

namespace mango
{

	Bitmap::Bitmap(void)
	{
		mBitsNeedFree = false;
		mBits = NULL;
	}


	Bitmap::Bitmap(int width, int height, int bitsPixel)
	{
		mBitsNeedFree = false;
		mBits = NULL;
		create(width, height, bitsPixel);
	}


	Bitmap::~Bitmap(void)
	{


	}


	bool Bitmap::create(int width, int height, int bitsPixel)
	{
		int	bytes ;

		if (width < 0 || height < 0)
			return false;

		release();

		mWidth  = width;
		mHeight	= height;

		mWidthBytes	= width * (bitsPixel / 8);
		mBitsPixel	= bitsPixel;

		bytes = width * (bitsPixel / 8) * height;

		if (bytes > 0) 
		{
			mBits = malloc(bytes) ;
			mBitsNeedFree = true;
		}
		else
			mBits = NULL ;

		return true;
	}


	void Bitmap::set(int width, int height, int bitsPixel, void *bits)
	{
		if (mBitsNeedFree && bits)
			safeFree(mBits);

		mWidth  = width;
		mHeight	= height;

		mWidthBytes	= width * (bitsPixel / 8);
		mBitsPixel	= bitsPixel;

		if (bits)
		{
			mBits = bits;
			mBitsNeedFree = false;
		}
	}


	void Bitmap::setBits(void* bits)
	{
		if (mBitsNeedFree)
			safeFree(mBits);

		mBitsNeedFree = false;
		mBits = bits;
	}

	bool Bitmap::release()
	{
		if (mBitsNeedFree)
			safeFree(mBits);

		return true;
	}
}
