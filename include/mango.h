
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
#else
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>   
#include <sys/stat.h>
#include <dlfcn.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h>

#include "mango/type.h"
#endif


//Truetype font header
#ifdef __cplusplus
extern "C" {
#endif

#include "../base/canvas/freetype/src/expansion/ft_ex_truetype.h"

#ifdef __cplusplus
}
#endif

#if __cplusplus
#include "mango/define.h"
#include "mango/error.h"
#include "mango/config.h"

#include "mango/object.h"

#include "mango/point.h"
#include "mango/size.h"
#include "mango/rect.h"
#include "mango/list.h"
#include "mango/string.h"
#include "mango/time.h"

#include "mango/log.h"
#include "mango/condition.h"
#include "mango/mutex.h"
#include "mango/semaphore.h"

#include "mango/mutexList.h"

#include "mango/thread.h"
#include "mango/message.h"
#include "mango/party.h"

#include "mango/charset.h"
#include "mango/file.h"
#include "mango/resource.h"
#include "mango/graphic.h"
#include "mango/canvas.h"

#include "mango/view.h"
#include "mango/viewZAxis.h"
#include "mango/session.h"

#include "mango/controls.h"
#include "mango/gestureDetector.h"

#endif


#endif 