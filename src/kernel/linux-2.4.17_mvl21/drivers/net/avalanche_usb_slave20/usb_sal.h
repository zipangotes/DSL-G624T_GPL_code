

 /* TODO: include header file which contains type definitions */
 
#ifndef __USB_SAL_H
#define __USB_SAL_H

/*
 * Define your OS here. Everything else will be automatically taken
 * care of by SAL implementation.
 */

#undef    SAL_BIOS
#undef    SAL_NUCLEUS
#undef   SAL_VXWORKS
#define    SAL_LINUX
#undef    SAL_WINCE


#include "psp_types.h"
/** Note: usb-types.h must always be the first header file 
 * to be included as it defines the target OS for compilation 
 **/



#ifdef SAL_VXWORKS

#include <memLib.h>
#include <stdlib.h>
#include <vxWorks.h>
#include <semLib.h>
#include <msgQLib.h>
#include <timers.h>
#include <intLib.h>
#include <taskLib.h>
#include <string.h>

#include "sysApi.h"

/** EVENT_Q_.. macros must be defined for every OS porting */

#endif


#ifdef SAL_LINUX
#include <linux/config.h>
#include <linux/slab.h>

#endif



/* TODO : macros for flags and parameter options like NO_WAIT...   */
/* TODO : TIMER_VAL structure,make sure GLOBAL/STATIC are in usb-types.h*/


typedef void *SAL_MUTEX, *SAL_MSGQ,*SAL_TIMER,*SAL_SEM,
             *SAL_EVENT,*SAL_INT_HANDLER,*SAL_TIMER_HANDLER,
             *SAL_THREAD,*SAL_THREAD_HANDLER;


typedef struct timerval_t
  {
      unsigned long secs;
      unsigned long nanosecs;
  }SAL_TIMER_VAL;
              
typedef struct _SAL_THREAD_ATTR
{
    char         name[256];
    unsigned int priority;
    unsigned int os_options;
    unsigned int stacksize;
    void *ext;
} SAL_THREAD_ATTR;
              
/*-----------------------------------------------------------------------
 * MACROS RELATED TO FLAGS AND OPTIONS IN SERVCICE ROUTINES
 *----------------------------------------------------------------------*/
 
#define SAL_MUTEX_LOCKED  1
#define SAL_MUTEX_UNLOCKED 0 

#define SAL_SEM_FULL       0
#define SAL_SEM_EMPTY      1

#define SAL_EVENT_FULL     1
#define SAL_EVENT_EMPTY    0   /*event has not occured yet,event_wait blocks*/

#define SAL_EVENT_Q_FIFO      0
#define SAL_EVENT_Q_PRIORITY  1


#ifndef GLOBAL
#define GLOBAL 
#endif 

/*-----------------------------------------------------------------------
 * MEMORY MANAGEMENT RELATED SERVICES
 *----------------------------------------------------------------------*/

GLOBAL void *SAL_malloc(UINT32 size);
GLOBAL void *SAL_calloc(UINT32 nelements, UINT32 size);
GLOBAL void  SAL_free(void * ptr);
GLOBAL void *SAL_realloc(void *ptr,UINT32 size,UINT32 flags);
GLOBAL void *SAL_memalign(UINT32 alignment,UINT32 obj_size,UINT32 num_objs,UINT32 flags);
GLOBAL void SAL_memset(void *p, UINT8 c, UINT32 ); 
 
 
/*-----------------------------------------------------------------------
 * MUTEX RELATED SERVICES 
 *----------------------------------------------------------------------*/

GLOBAL SAL_MUTEX SAL_mutex_create(UINT32 mutexoptions,
                                              UINT32 initState);
GLOBAL INT32 SAL_mutex_lock(SAL_MUTEX  mutex,UINT32 timeout); 
GLOBAL INT32 SAL_mutex_unlock(SAL_MUTEX  mutex);
GLOBAL INT32 SAL_mutex_destroy(SAL_MUTEX  mutex);
 
 

/*-----------------------------------------------------------------------
 * SEMAPHORE RELATED SERVICES 
 *----------------------------------------------------------------------*/
GLOBAL SAL_SEM SAL_sem_create(UINT32 semoptions,UINT32 initState);
GLOBAL INT32 SAL_sem_down(SAL_SEM  sem,UINT32 timeout); 
GLOBAL INT32 SAL_sem_up(SAL_SEM  sem);
GLOBAL INT32 SAL_sem_destroy(SAL_SEM  sem);
 
 
 
/*-----------------------------------------------------------------------
 * MESSAGE QUEUE RELATED SERVICES 
 *----------------------------------------------------------------------*/

GLOBAL SAL_MSGQ SAL_msgQ_create(INT32 maxmsgs, 
                                            INT32 msgMaxLength,
                                            INT32 options);
GLOBAL INT32 SAL_msgQ_send ( SAL_MSGQ msgQid,
                                  UCHAR *  buffer, UINT32 nBytes, 
                                  INT32 timeout,INT32 priority);   
GLOBAL INT32 SAL_msgQ_rcv(SAL_MSGQ msgQid,
                               UCHAR * buffer, UINT32 nBytes, INT32 timeout);
GLOBAL INT32 SAL_msgQ_destroy(SAL_MSGQ msgQid);
 
 
 
/*-----------------------------------------------------------------------
 * EVENT HANDLING SERVICES 
 *----------------------------------------------------------------------*/

GLOBAL SAL_EVENT SAL_event_create(UINT32 options, UINT32 initState);
GLOBAL INT32 SAL_event_wait(SAL_EVENT event,UINT32 timeout);
GLOBAL INT32 SAL_event_post(SAL_EVENT event,UINT32 options);
GLOBAL INT32 SAL_event_destroy(SAL_EVENT event);
 
 
 
/*-----------------------------------------------------------------------
 * TIMER RELATED SERVICES
 *----------------------------------------------------------------------*/

GLOBAL SAL_TIMER  SAL_timer_create(UINT32 options);
 /* args is passed to the handler when the timer expires */
GLOBAL INT32 SAL_timer_connect(SAL_TIMER timer, 
                                   SAL_TIMER_HANDLER  handler, void *args);
GLOBAL INT32 SALimerDestroy(SAL_TIMER timer);
GLOBAL INT32 SAL_timer_settime(SAL_TIMER timer,
                                   SAL_TIMER_VAL newvalue,
                                   SAL_TIMER_VAL * oldvalue);
GLOBAL INT32 SAL_timer_cancel(SAL_TIMER timer);                                  
 
 
 
/*-----------------------------------------------------------------------+
 * INTERRUPT HANDLING SERVICES
 *----------------------------------------------------------------------*/
GLOBAL BOOL SAL_int_context(void);
GLOBAL INT32 SAL_int_connect(UINT32 irq,SAL_INT_HANDLER isr,void * arg,
                                const char * dev_name);
 /* The current implementation does not support interrupt sharing  */
GLOBAL INT32 SAL_int_disconnect(UINT32 irq);
GLOBAL INT32 SAL_int_enable(UINT32 irq);
GLOBAL INT32 SAL_int_disable(UINT32 irq);
GLOBAL INT32 SAL_int_on(UINT32* flags);
GLOBAL INT32 SAL_int_off(UINT32* flags);



/*-----------------------------------------------------------------------+
 * THREAD RELATED SERVICES
 *----------------------------------------------------------------------*/
GLOBAL INT32  SAL_thread_create(SAL_THREAD * thread,
		                    SAL_THREAD_ATTR *attr,
                                    SAL_THREAD_HANDLER thandler,
                                    void *args);
GLOBAL void  SAL_thread_exit(int exitcode);
GLOBAL SAL_THREAD SAL_thread_self(void);
                                    


/*-----------------------------------------------------------------------+
 * Software caching for homogenous objects
 *----------------------------------------------------------------------*/

/*** TODO: define routines for cache creation,object alloc,object dealloc
 ***       cache destroy here .***/

#endif  /*end of ifndef __USB_SAL_H conditional block */


