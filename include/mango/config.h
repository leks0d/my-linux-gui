
#ifndef _MANGO_CONFIG_H
#define _MANGO_CONFIG_H

namespace mango
{
#define  SCREEN_CX   320
#define  SCREEN_CY   240
#define  SCREEN_BITSPERPXIEL  32
#define  SCREEN_BUFFER_BYTES  (SCREEN_CX * SCREEN_CY * SCREEN_BITSPERPXIEL / 8)


//#define MANGO_ROOT  "/sdcard/mango/"
#define MANGO_ROOT  "/system/mango/"
}

#ifdef WIN32
#define SEMAPHORE_BY_MUTEX_CONDITION
#endif


#endif