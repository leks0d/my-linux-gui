#pragma once
#include "object.h"

namespace mango 
{

	class Process :	public Object
	{
	public:
		Process(void);
		virtual ~Process(void);

		int getCurrentId(void);
		int open(int id);

		int  getCurrent(void);
		static void exit(int code);

		int exec(char* cmdLine, int cmdShow);


	private:
		void* mObj;
		int   mId;
	};

}
