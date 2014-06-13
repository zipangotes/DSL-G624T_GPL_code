/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/**************************************************************************
 * FILE PURPOSE :   VxWorks SAL for USB
 **************************************************************************
 * FILE NAME    :   usb_sal.c
 *
 * DESCRIPTION  :
 *
 *  (C) Copyright 2003, Texas Instruments, Inc.
 *************************************************************************/


#include "usb_sal.h"


#define STACK_SIZE 0x100  
/* TODO: fix stack size for tsakSpawn here*/
#define ERROR_FIX -1
/* TODO :fix error codes */


/**
 * @defgroup SAL_API SAL API
 *
 *
 * @{
 */


/*-----------------------------------------------------------------------
 * Memory management related routines
 *----------------------------------------------------------------------*/

 
 
/**
 *  SAL_malloc  - allocate memory of requested size
 *
 *  @param size  IN - an unsigned integer that specifies the size of 
 *                     requested memory block.
 *
 *  Return Value     - pointer to the memory block that is allocated.
 */

#ifdef SAL_VXWORKS
GLOBAL void * SAL_malloc(UINT32 size)
{
    return malloc(size); 
}

GLOBAL void SAL_memset(void *p, unsigned char c, unsigned int size)
{
    memset(p, c, size);
}

GLOBAL void  *SAL_calloc(unsigned int nelements, unsigned int size)
{
    return calloc(nelements, size);
}
#endif
#ifdef SAL_LINUX
GLOBAL void * SAL_malloc(UINT32 size)
{
    return kmalloc(size,GFP_KERNEL); 
}

GLOBAL void SAL_memset(void *p, unsigned char c, unsigned int size)
{
    memset(p, c, size);
}

GLOBAL void  *SAL_calloc(unsigned int nelements, unsigned int size)
{
    void * ptr=NULL;
	unsigned int total_size=nelements*size;

	ptr=kmalloc(total_size,GFP_KERNEL);
			if(ptr!=NULL)
			{
					memset(ptr,0,total_size);
			}
	return ptr;
	
}
#endif


/**
 *  SAL_free   - frees memory allocated through a prior call of 
 *                   SAL_malloc().
 *
 *  @param ptr IN - pointer to the previously allocated memory block
 *
 *  Return Value   -  None
 *
 */
#ifdef SAL_VXWORKS 
GLOBAL void SAL_free(void * ptr)
{
    free(ptr);
}
#endif
#ifdef SAL_LINUX 
GLOBAL void SAL_free(void * ptr)
{
    kfree(ptr);
}
#endif



/**
 *  SAL_realloc - reallocate a block of memory of new size and copy 
 *                    old contents to the new block.
 *
 *  @param  ptr IN - pointer to old block of memory
 *
 *  @param  size IN - size of the new block of memory to be allocated.
 *
 *  @param  flags IN - flags for allocation of memory.
 *
 *  Return Value      -  Pointer to the reallocated block of memory.
 *
 *  Algorithm         - use the realloc function provided as a part of memLib
 *
 */
#ifdef SAL_VXWORKS 
GLOBAL void *SAL_realloc(void *ptr,UINT32 size,UINT32 flags)
{
    return realloc(ptr,size);
}



/**
 *  SAL_memalign  - allocates aligned memory
 *
 *  @param alignment IN - power of 2 boundary that the allocation must 
 *                         be aligned with.
 *
 *  @param obj_size  IN - size of an individual object.
 *  
 *  @param num_objs  IN - number of objects to be allocated.
 *
 *  @param  flags IN - flags for allocation of memory.
 *
 *  Return Value         - Pointer to the allocated block of memory or
 *                         NULL if allocation fails.
 *
 *  Algorithm            -
 *
 */
 
GLOBAL void *SAL_memalign(UINT32 alignment,UINT32 obj_size,UINT32 num_objs,UINT32 flags)
{
    unsigned int temp;

    temp=alignment;
     if (alignment==0) return NULL ;  /*set error to EINVAL*/

     if ( (temp & (temp-1))!= 0) return NULL; 
     /* alignment parameter is not an integral power of 2 */

    return memalign(alignment,obj_size*num_objs);
}



/*-----------------------------------------------------------------------
 * Mutex related service routines
 *----------------------------------------------------------------------*/
 
 
 
/**
 *   SAL_mutex_create - create a mutex object for mutual exclusion
 *
 *   @param  mutexoptions IN - options for creation of the mutex
 *                              options indicate whether pended tasks are 
 *                              queued based on Priority/FIFO order.              
 *
 *   @param  initstate    IN - initial state of the mutex, default
 *                               would be UNLOCKED
 *
 *   Return Value             - Handle to the Mutex object created.Null
 *                              otherwise.
 *
 *   Algorithm                - Creates a mutual exclusion semaphore and locks
 *                              it if required. Note that the creation and
 *                              obtaining of the lock is not atomic.
 *
 */ 

GLOBAL SAL_MUTEX SAL_mutex_create(UINT32 mutexoptions, UINT32 initState)
{
    SAL_MUTEX mutex;
    
    mutex=semMCreate(mutexoptions);
    if( (initState==SAL_MUTEX_LOCKED) && (mutex!=NULL) )
      {
       /** Try to lock the sempahore with NO_WAIT option
        ** If locking fails ,(which should not happen as we create
        ** the mutex and try to lock it immediately) return
        ** error */

         if(  (semTake( (SEM_ID)mutex,NO_WAIT) ) == -1)
             {
              semDelete( (SEM_ID) mutex );  
              return  NULL;
              }
          
      }
    return mutex;  
} 



/**
 *  SAL_mutex_lock  -  obtain lock on a mutex variable
 *  
 *  @param mutex IN    - the mutex varaible on which the lock must be obtained
 *
 *  @param timeout IN  - the timeout interval(number of ticks) after which
 *                        lock attempt returns failure. Other values include
 *                        NO_WAIT,WAIT_FOREVER
 *
 *  Return Value        - Returns error on timeout/invalid argument
 *
 *  Algorithm           - 
 *
 */
 
 GLOBAL INT32 SAL_mutex_lock(SAL_MUTEX  mutex,UINT32 timeout)
 {
    return semTake( (SEM_ID)mutex,timeout);
         
 }
 
 
 
/**
 *  SAL_mutex_unlock  - unlocks a mutex variable
 *
 *  @param1 mutex IN  - the mutex variable which is being released
 *
 *  Return value      - error if object handle or the opeation is invalid
 *
	 *  Algorithm         -
 *
 */
 
 GLOBAL INT32 SAL_mutex_unlock(SAL_MUTEX  mutex)
 {
     return semGive( (SEM_ID)mutex );
 }
 
 
 
/**
 *   SAL_mutex_destroy - destroys/ deletes a previously created mutex
 *
 *   @param1 mutex IN      - handle to the mutex variable being destroyed.
 *   
 *   Return Value          - error if handle is invalid
 *
 *   Algorithm             -
 *
 */
 
 GLOBAL INT32 SAL_mutex_destroy(SAL_MUTEX  mutex)
 {
     return semDelete( (SEM_ID) mutex );
 }
 
 
 
/*-----------------------------------------------------------------------
 * Semaphore Related service routines
 *----------------------------------------------------------------------*/
 
 
 
/**
 *  SAL_sem_create   - creates a semaphore variable.
 *
 *  @param1  semoptions IN    - options for creation of the semaphore
 *                              options indicate whether pended tasks are 
 *                              queued based on Priority/FIFO order.              
 *
 *   @param2  initstate    IN - initial state of the semaphore, default
 *                               would be FULL
 *
 *   Return Value             - Handle to the Semaphore object created.Null
 *                              otherwise.
 *
 *   Algorithm                - Creates a semaphore and locks it if required. 
 *                              Note that the creation and obtaining of the
 *                              lock is not atomic.
 *   Note - Current porting of semaphore to Vxworks still uses the Mutual
 *          exclusion semaphore.
 */
 
 GLOBAL SAL_SEM SAL_sem_create(UINT32 semoptions,UINT32 initState)
 {
    SAL_SEM semaphore;
    
    semaphore=semMCreate(semoptions);
    if( (initState==SAL_SEM_EMPTY) && (semaphore!=NULL) )
      {
          /** Try to lock the sempahore with NO_WAIT option
        ** If locking fails ,(which should not happen as we create
        ** the semaphore and try to lock it immediately) return
        ** error */
	
         if(  (semTake( (SEM_ID)semaphore,NO_WAIT) ) == -1)
             {
              semDelete( (SEM_ID) semaphore ); 
              return  NULL;
              }

      }
    return semaphore;    
 }
 
 
 
/**
 *  SAL_sem_down   - decrements the semaphore counter; blocks if the
 *                        state was empty.
 *
 *  @param1 sem    IN  - handle to the semaphore
 *
 *  @param2 timeout IN - the timeout interval(number of ticks) after which
 *                       down attempt returns failure. Other values include
 *                       NO_WAIT,WAIT_FOREVER.
 *
 *  Return Value       -
 *
 *  Algorithm          -
 *
 */
 
 GLOBAL INT32 SAL_sem_down(SAL_SEM  sem,UINT32 timeout)
 {
     return semTake( (SEM_ID)sem , timeout);
 }
 
 
 
/**
 *  SAL_sem_up    - increments the semaphore counter
 *
 *  @param1 sem  IN   - handle to the semaphore
 *
 *  Return value      -
 *
 *  Algorithm         - 
 *
 */
 
 GLOBAL INT32 SAL_sem_up(SAL_SEM  sem)
 {
     return semGive( (SEM_ID) sem);
 }
 
 
 
/**
 *  SAL_sem_destroy  - destroys a previously created semaphore variable
 *
 *  @param1 sem  IN      - handle to the semaphore being destroyed.
 *
 *  Return Value         -
 *
 *  Algortihm            - 
 *
 */
 
GLOBAL INT32 SAL_sem_destroy(SAL_SEM  sem)
{
    return semDelete( (SEM_ID) sem );
}
 
 
 
/*-----------------------------------------------------------------------
 * Message Queue related routines
 *----------------------------------------------------------------------*/
 
 
 
/** 
 *  SAL_msgQ_create  - create a message queue for communication
 * 
 *  @param1 maxmsgs IN   - maximum number of messages that can be held in 
 *                         in the message queue.
 *
 *  @param2 msgMaxLength IN - maximum length of each message within the queue
 *
 *  @param3 options      IN - options for creation of the message queue. 
 *                            options indicate whether pended tasks are 
 *                            queued based on Priority/FIFO order.   
 *
 *  Return Value            - Handle to the message queue created;
 *                            NULL-in case of error
 *  Algorithm
 *
 */
 
GLOBAL SAL_MSGQ SAL_msgQ_create(INT32 maxmsgs, INT32 msgMaxLength,
                                          INT32 options)
{
    return msgQCreate(maxmsgs,msgMaxLength,options);
}
 


/**
 *  SAL_msgQ_send - sends a message to the message queue 
 *
 *  @param1 msgQid  IN - handle to message queue being addressed
 *
 *  @param2 buffer  IN - pointer to the buffer which holds the data to be sent
 *
 *  @param3 nBytes  IN - number of bytes of data to be sent
 *
 *  @param4 timeout IN - timeout duration after which the send operation
 *                       is aborted.
 *
 *  @param5 priority IN - priority of the message being sent .
 *                        can take values MSG_PRI_NORMAL,MSG_PRI_URGENT
 *
 *  Return Values       - Error if any of the arguments are invalid or 
 *                        on timeout.
 *
 *  Algorithm           -
 *
 */
 
GLOBAL INT32 SAL_msgQ_send ( SAL_MSGQ msgQid, UCHAR *  buffer,
                                 UINT32 nBytes, INT32 timeout,INT32 priority)
{
    return msgQSend ( (MSG_Q_ID) msgQid , buffer,nBytes,timeout,priority);
}                                 
 
 
 
/**
 *  SAL_msgQ_rcv    - receives a message from the message queue
 *
 *  @param1  msgQid  IN - handle to message queue being addressed.
 *
 *  @param2  buffer  IN - pointer to the receive buffer.
 *
 *  @param3  nBytes  IN - buffer length
 *
 *  @param4  timeout IN - timeout duration after which receive operation
 *                        is aborted if the queue is still empty.
 *
 *  Return values       - the number of bytes of data received if successful; 
 *                        error code otherwise.
 *
 *  Algorithm           - 
 *
 */
 
GLOBAL INT32 SAL_msgQ_rcv(SAL_MSGQ msgQid,UCHAR * buffer, 
                              UINT32 nBytes, INT32 timeout)
{
    return msgQReceive( (MSG_Q_ID) msgQid, buffer, nBytes, timeout);
}                              



/**
 *  SAL_msgQ_destroy  - destroys a previously created message queues
 *                          and frees up allocated resources.
 *
 *  @param1  msgQid  IN  - handle to message queue object being destroyed.
 *
 *  Return Value         -  Error if an invalid handle is passed
 *
 *  Algorithm            -
 *
 */
 
GLOBAL INT32 SAL_msgQ_destroy(SAL_MSGQ msgQid)
{
    return msgQDelete( (MSG_Q_ID) msgQid);
}
  
 

/*-----------------------------------------------------------------------
	 * Event handling routines
 *----------------------------------------------------------------------*/
 
 
 
/**
 *  SAL_event_create - Creates a event- wait/wakeup object
 *
 *  @param1 options  IN - options for creation of the event object. 
 *                        options indicate whether pended tasks are 
 *                        queued based on Priority/FIFO order.  ??????????
 *
 *  @param2 initState IN - initial state of event object. Defaults
 *                         to EVENT_EMPTY so that a succeding call
 *                         to event_wait is blocked until the corresponding
 *                         event is posted.
 *
 *  Return Value 
 *
 *  Algorithm            - Creates a binary semaphore with an initial state
 *                         of LOCKED(default behaviour). A task which now 
 *                         waits on this object is suspended until the 
 *                         corresponding event is posted by another task.
 *
 */
 
GLOBAL SAL_EVENT SAL_event_create(UINT32 options,UINT32 initState)
{
    SAL_EVENT event;
    
    /*
     * USB_EVENT_EMPTY - event has not occured yet, Call to event_wait blocks.
     * USB_EVENT_FULL  - event has occured already. Not of much use   
     */

    switch (options)
    {
	    case SAL_EVENT_Q_PRIORITY : 
		    options = SEM_Q_PRIORITY;
		    break;
	    case SAL_EVENT_Q_FIFO : 
		    options = SEM_Q_FIFO;
		    break;
	    default : 
		    break;
    }

    /* sets the default behaviour to FIFO ordering */
         
    if(initState==SAL_EVENT_FULL) 
	    event=semBCreate(options,SEM_FULL);
    else 
	    event =semBCreate(options,SEM_EMPTY);
    
    return event;
}



/**
 *  SAL_event_wait - wait for the event to occur
 *
 *  @param1 event IN- handle to the event object on which the task waits.
 *
 *  @param2 timeout IN - timeout interval(in number of ticks) after which the 
 *                    event wait operation is aborted with a suitable error
 *                    code.
 *
 *  Return Value    - error if any of the arguments are invalid or if timeout
 *
 *  Algorithm       - Though the event semaphore is created with event_empty 
 *                    state initially, its still possible that at some point 
 *                    of time event_post might have been called with no tasks
 *                    waiting on the event. This would set the semaphore
 *                    state to FULL after which a subsequent event_wait 
 *                    will return immediately for an event that actually
 *                    occured  prior to the call of event_wait. To alleviate
 *                    this problem, we put a dummy semTake with NO_WAIT which
 *                    will bring back the sem state to SEM_EMPTY.
 *
 */
 
GLOBAL INT32 SAL_event_wait(SAL_EVENT event,UINT32 timeout)
{
	/*
    semTake( (SEM_ID)event , NO_WAIT ); 
     ensures that sem state is SEM_EMPTY */

    return semTake( (SEM_ID)event , timeout);
}

 
 
/**
 *  SAL_event_post - signal that the event has occured
 *
 * @param1  event     - handle to the event object.
 *
 * @param2  options   - can indicate whether to wake up all tasks
 *                      waiting on the event.
 *
 * Return Value       -
 *
 * Algorithm
 *
 */
 
GLOBAL INT32 SAL_event_post(SAL_EVENT event,UINT32 options)
{
/* TODO - EVENT POST- handle multiple pended task wakeup- event broadcast*/

return  semGive( (SEM_ID) event );

} 



/** 
 *  SAL_event_destroy - destroy a previously allocated event object
 *
 *  @param1 event         - handle to the event object to be destroyed.
 *
 *  Return Value          - error if handle is invalid.
 *
 *  Algorithm             - 
 *
 */
 
GLOBAL INT32 SAL_event_destroy(SAL_EVENT event)
{
    return semDelete( (SEM_ID) event);
}

 
 
/*-----------------------------------------------------------------------
 * Timer Related routines
 *----------------------------------------------------------------------*/
 
 
 
/** 
 *  SAL_timer_create   -  create a timer object
 *
 *  @param1 options  IN    - timer creation options  **TODO**
 *
 *  Return Value           - Timer Object or NULL if creation fails
 *
 *  Algorithm 
 *
 */
 
GLOBAL SAL_TIMER  SAL_timer_create(UINT32 options)
{
    timer_t * timerobj = NULL;
    int returncode;
    
    returncode= timer_create(CLOCK_REALTIME,NULL,timerobj);
    /* uses realtime clock of time base, uses the SIGALRM signal by default*/
    
    if(returncode==0) 
		return ( (SAL_TIMER) (*timerobj) );
    else return NULL;
    
}



/**
 *  SAL_timer_connect  - connects a handler routine to the timer signal
 *
 *  @param1 timer IN       - handle to the timer object
 * 
 *  @param2 handler IN     - the handler routine which is hooked on to 
 *                           this timer object.
 *  @param3 args    IN     -   *** TODO:timer_connect args parameter ***
 *
 *  Return Value           - error if any of the arguments are invalid.
 *
 *  Algorithm              - 
 *
 */
 
GLOBAL INT32 SAL_timer_connect(SAL_TIMER timer, 
                                   SAL_TIMER_HANDLER handler, void *args)
{
    return timer_connect( (timer_t)timer, handler,(int)args);
    /** TODO:timer_connect::Handler type casting, define handler protoype  **/
}                                  



/**
 *  SAL_timer_destroy  - destroys previously created timer object 
 *
 *  @param1  timer IN      - handle to the timer object to be destroyed
 *
 *  Return Value           - Error if handle supplied is invalid
 *
 *  Algorithm              -
 *
 */
 
GLOBAL INT32 SAL_timer_destroy(SAL_TIMER timer)
{
    return timer_delete( (timer_t) timer);
}



/**
 *  SAL_timer_settime  - arms the timer. Time for next expiry can be set 
 *                           with this routine.
 *
 *  @param1 timer IN       - handle to the timer object being addressed
 *
 *  @param2 newvalue IN    - TIMER_VAL structure which holds the new expiry
 *                           time.
 *  @param3 oldvalue OUT   - pointer to the TIMER_VAL structure which now 
 *                           holds time left for expiry prior to this routine
 *                           being invoked.
 *
 *  Return Value           - 
 *
 *  Algorithm              -
 *
 *  Note                   - the current implementation for VxWorks does not
 *                           make use of periodic timers and absolute time.
 *                           Always value in USBIMER_VAL indicates
 *                           relative time only.
 */
 
GLOBAL INT32 SAL_timer_settime(SAL_TIMER timer, SAL_TIMER_VAL newvalue,
                                   SAL_TIMER_VAL * oldvalue)
{
    struct itimerspec oldval,newval;
    int ret;
    
    newval.it_interval.tv_sec=0; 
    newval.it_interval.tv_nsec=0;
    /* turns of periodic timer option */

    newval.it_value.tv_sec= (time_t) newvalue.secs;
    newval.it_value.tv_nsec= (long) newvalue.nanosecs; 
    /*  overflow ????? */
    
    ret=timer_settime( (timer_t) timer,CLOCK_REALTIME, &newval,&oldval);
    
    if(ret==-1)
       {
       /* settime failed ; Set error code and return error */
       return -1;
       }
    
    (*oldvalue).secs= oldval.it_value.tv_sec;
    (*oldvalue).nanosecs=oldval.it_value.tv_nsec;
    
    return 0;
    /* return status for OK */
}                                  
 


/**
 *  SAL_timer_cancel - cancels/disarms the timer
 *  
 *  @param1  timer       - handle to timer which must be disarmed
 *
 *  Return value         - error if handle is invalid.
 *
 *  Algortihm            -
 *
 */
 
GLOBAL INT32 SAL_timer_cancel(SAL_TIMER timer)
{

/** TODO : verify implementation in timer_cancel **/
    return timer_delete( (timer_t) timer);
}
 
                           
/*-----------------------------------------------------------------------
 * Interrupt handling routines
 *----------------------------------------------------------------------*/
 
 
 
/** Note : Implementation of interrupt handler registeration involves the
 *  Interrupt controller code. The current porting exercise, assumes that only
 *  hard vector interrupt pin is used. Registeration of interrupt handlers 
 *  for other interrupt sources is left as responsibility of Interrupt Controller
 *  code. Thus, handler registeration cannot use the "intConnect" kind of call
 *  from VxWorks ARCHLIB. For regestering interrupt handlers, we use the 
 *  Interrupt Controller provided routines.
 */
 

/**
 *  SAL_int_context - determines whether the routine was invoked 
 *                        from interrupt context
 *
 *  @param              - NONE
 *
 *  Return value        - returns a boolean value indicating whether in
 *                        interrupt context or not.
 *
 *  Algorithm           - 
 *
 */
 
GLOBAL BOOL SAL_int_context(void)
{
    /*** TODO : map to the SAL definition of BOOL */
    return intContext();
}
 


/**
 *  SAL_int_connect    - associates a interrupt handler with interrupt 
 *                           source channel
 *
 *  @param1  irq   IN      - the interrupt source channel number.
 *
 *  @param2  isr   IN      - the interrupt handler routine to be hooked
 *                           on to the interrupt source channel.
 *
 *  @param3  arg   IN      - arguments to the interrupt handler routine
 *
 *  @param4  dev_name IN   - dev_name string for informational purpose only
 *                            required??????
 *
 *  Return Value           -
 *
 *  Algorithm              -
 *
 */
 
GLOBAL INT32 SAL_int_connect(UINT32 irq,SAL_INT_HANDLER isr,void * arg,
                                const char * dev_name)
{
/** TODO:int_connect ***/
 sysHwIntConnect(irq,isr,arg); /** irq here defines the interrupt source */
 return 0;   

}           



/**
 *  SAL_int_disconnect  - disconnects the handler associated with the
 *                            specified source.
 *
 *  @param1  irq   IN       - interrupt source number
 *
 *  Return Value            - error on invalid source number argument
 *
 *  Algorithm               -
 *
 */

#if 0
GLOBAL INT32 SAL_int_disconnect(UINT32 irq)
{
    return  sysHwIntDisconnect(irq);  
    /* return status check */
    /* TODO: int_disconnect :: linking SAL function to BSP disconnect */
}
#endif


/**
 *  SAL_int_enable      - enable the interrupt source specified 
 *
 *  @param1  irq IN         - Interrupt source number
 *
 *  Return Value            - error on invalid source number argument
 *
 *  Algorithm               - The specified interrupt source is enabled by
 *                            writing into the corresponding register in 
 *                            Interrupt controller as specified in line-to-
 *                            channel mapping.
 *
 */

GLOBAL INT32 SAL_int_enable(UINT32 irq)
{
    return sysHwIntEnable(irq) ;
    /* return status ?????  */
    /* TODO: int_enable::linking SAL function to BSP IntEnable */
}



/**
 *  SAL_int_disable    - disables the interrupt source specified
 *
 *  @param1 irq IN         - Interrupt source number
 * 
 *  Return Value           - error on invalid source number argument
 *
 *  Algorithm              - The specified interrupt source is disabled by
 *                           writing into the corresponding register in 
 *                           Interrupt controller as specified in line-to-
 *                           channel mapping. 
 *
 */
 
GLOBAL INT32 SAL_int_disable(UINT32 irq)
{
    return sysHwIntDisable(irq);
    /* return status check ??????? */
    /* TODO: int_disable :: linking SAL function to BSP disable */
}
              
#endif /* SAL_VXWORKS */


/**
 *  SAL_int_off    - Switches off all interrupts
 *
 *  @param1 flags IN         - pointer to variable used to store the interrupt 
 *                             status
 * 
 *  Return Value           - error on invalid flags
 *
 *  Algorithm              - System-wide interrupts are disabled by using the 
 *                           underlying OS call. This is used for crtitical 
 *                           section protection.
 */
 
#ifdef SAL_VXWORKS 
GLOBAL INT32 SAL_int_off(UINT32* flags)
{
    *flags = intLock();
    return *flags;
}
#endif  
#ifdef SAL_LINUX 
GLOBAL INT32 SAL_int_off(UINT32* flags)
{
		save_and_cli((*flags));
		return (*flags);
}
#endif  

              

/**
 *  SAL_int_on    - Restores interrupt status as before a call to SAL_int_off
 *
 *  @param1 flags IN         - pointer to variable which has the saved interrupt 
 *                           status
 * 
 *  Return Value           - error on invalid flags
 *
 *  Algorithm              - System-wide interrupts are re-enabled. Only those
 *                           interrupts which were previously enabled will be 
 *                           re-enabled. This is used for crtitical 
 *                           section protection.
 *
 */

#ifdef SAL_VXWORKS
GLOBAL INT32 SAL_int_on(UINT32* flags)
{
    return intUnlock(*flags);
}
#endif
#ifdef SAL_LINUX
GLOBAL INT32 SAL_int_on(UINT32* flags)
{
    restore_flags((*flags));
	return 0;
}
#endif



/*-----------------------------------------------------------------------
 * Thread related routines
 *----------------------------------------------------------------------*/        
 
/**
 *  SAL_thread_create - creates a new task
 *
 *  @param1 thread out - Thread handle for the newly created task is returned
 *                       through this parameter.
 *
 *  @param2 thandler in - entry point to the new task function.
 *
 *  @param3 args in     - argument/arguments array to the new task.
 *
 *  @param4 numargs in  - number of arguments to the newly created task.
 *
 *  @param5 options  in - options for task creation.
 *
 *  Return value        -  
 *
 *  Algorithm           -
 *
 */

#ifdef SAL_VXWORKS
GLOBAL INT32  SAL_thread_create
(
 SAL_THREAD * thread,
 SAL_THREAD_ATTR  *opt,
 SAL_THREAD_HANDLER thandler,
 void *arg
)
{
    int  taskid;
    
    *thread = NULL;
    taskid=taskSpawn(opt->name,opt->priority,opt->os_options,opt->stacksize,thandler,(UINT32)arg, 0,0,0,0,0,0,0,0,0);

    if( taskid  == ERROR ) 
	return -1;

    *thread = (SAL_THREAD) taskid;

    return 0;
}             



/** 
 *  SAL_thread_exit - exit a task
 *
 *  @param1  code in    -exit code from the task which will be stored in TCB
 *
 *  Return Value        - None
 *
 *  Algorithm           -
 *
 */
 
GLOBAL void SAL_thread_exit(int code)
{
    exit(code);
}

 
 
/**
 *  SAL_thread_self - return the task id of the running task
 *
 *  @param              - None
 *
 *  Return Value        - handle to the running task
 *
 *  Algorithm           -
 *
 */
 
GLOBAL SAL_THREAD SAL_thread_self(void)
{
    int taskid;
    
    taskid = taskIdSelf();
    return ( (SAL_THREAD) taskid );
}

#endif /* SAL_VXWORKS */

/**
 * @}
 */




