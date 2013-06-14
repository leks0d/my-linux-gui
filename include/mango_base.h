
#ifndef _MANGO_H
#define _MANGO_H

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

#ifdef WIN32
#include "windows.h"
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>

#endif


#include "mango/define.h"
#include "mango/error.h"
#include "mango/config.h"

#include "mango/object.h"

#include "mango/point.h"
#include "mango/rect.h"
#include "mango/list.h"
#include "mango/mutexList.h"

#include "mango/log.h"
#include "mango/condition.h"
#include "mango/mutex.h"
#include "mango/semaphore.h"

#include "mango/thread.h"
#include "mango/message.h"
#include "mango/party.h"

#include "mango/graphic.h"
#include "mango/canvas.h"

#include "mango/view.h"
#include "mango/viewZAxis.h"
#include "mango/session.h"



#endif 