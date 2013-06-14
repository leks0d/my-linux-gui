
#ifndef _MANGO_SIZE_H

namespace mango
{
	class Size : public Object
	{
	public:
		Size(void);
		Size(int cx, int cy);
		virtual ~Size(void);

		void set(int cx, int cy);
		Size& operator =(const Size &size);

	public:
		int cx;
		int cy;
	};
}




#endif