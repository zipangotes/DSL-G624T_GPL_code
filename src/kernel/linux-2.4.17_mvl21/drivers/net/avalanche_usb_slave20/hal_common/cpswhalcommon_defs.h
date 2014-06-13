/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: CPSWHALCOMMON_DEFS.H
 *
 *  DESCRIPTION:
 *      CPHAL typedefs needed for OS application
 *
 *  HISTORY:
 *      Date      Modifier              Ver    Notes
 *      14Nov03   Michael Hanrahan      1.00   from stddef.h
 *****************************************************************************/
#ifndef _CPSWHALCOMMON_DEFS_H
#define _CPSWHALCOMMON_DEFS_H

#ifndef _INC_ADAM2

    typedef char           bit8;
    typedef short          bit16;
    typedef int            bit32;

    typedef unsigned char  bit8u;
    typedef unsigned short bit16u;
    typedef unsigned int   bit32u;

#endif  /* _INC_ADAM2  */

/*  HAL Data Structure definitions for CPHAL users */

#ifndef _CPSW_HAL
  typedef void HAL_DEVICE;
  typedef void HAL_RECEIVEINFO;
#endif

/*
 *
 * Extension structure typedefs 
 * Structures CPHAL module specific
 *
 */ 

typedef struct hal_functions_ext_s  HAL_FUNCTIONS_EXT;
typedef struct channel_info_ext_s   CHANNEL_INFO_EXT;

/*
 *
 *  MDIO definitions
 *
*/

#define SNWAY_AUTOMDIX       (1<<16)   /* Bit 16 and above not used by MII register */
#define SNWAY_FD1000         (1<<13)
#define SNWAY_HD1000         (1<<12)
#define SNWAY_NOPHY          (1<<10)
#define SNWAY_LPBK           (1<<9)
#define SNWAY_FD100          (1<<8)
#define SNWAY_HD100          (1<<7)
#define SNWAY_FD10           (1<<6)
#define SNWAY_HD10           (1<<5)
#define SNWAY_AUTO           (1<<0)
#define SNWAY_AUTOALL        (SNWAY_AUTO|SNWAY_FD100|SNWAY_FD10|SNWAY_HD100|SNWAY_HD10|SNWAY_AUTOMDIX)

#define DIRECTION_TX 0
#define DIRECTION_RX 1

/* Channel Teardown Defines */

#define PARTIAL_TEARDOWN  0
#define TX_TEARDOWN       1
#define RX_TEARDOWN       (1<<1)
#define FULL_TEARDOWN     (1<<2)
#define BLOCKING_TEARDOWN (1<<3)

/* The following macros define the method for overloading Ch and Queue information into the 
   Action field of the Control() API */
#define INVALID_KEY                  -1
#define INVALID_CH                   -1
#define INVALID_Q                    -1
#define VALID_CH                     1
#define VALID_Q                      1
#define CH_VALID_BIT                 31
#define Q_VALID_BIT                  30
#define CH_FIELD_SHIFT               16
#define CH_FIELD_MASK                0x3FFF
#define Q_FIELD_SHIFT                8
#define Q_FIELD_MASK                 0xFF
#define CH_VALID                     (1<<CH_VALID_BIT)
#define Q_VALID                      (1<<Q_VALID_BIT)
#define ACTION_FIELD_MASK            0xFF
#define ACTION_CH(action,ch)         (CH_VALID|(ch<<CH_FIELD_SHIFT)|action)
#define ACTION_CH_Q(action,ch,q)     (CH_VALID|Q_VALID|(ch<<CH_FIELD_SHIFT)|(q<<Q_FIELD_SHIFT)|action)

/**
 *  @ingroup shared_data
 *  These are the actions defined for use with the @c Control() HAL function.
 *  Not all actions are applicable to all @p HAL_CONTROL_KEYs.  Refer to the
 *  module specific documentation in the appendices to determine which actions
 *  may be used to access a given @p HAL_CONTROL_KEY.
 */
enum HAL_CONTROL_ACTION
  {
   haCLEAR=0,      /**< Clears the value of the key. (See specific Key for allowable use) */
   haGETbyREF,     /**< Gets the value of the key by reference. */
   haGETDefault,   /**< Returns the Default for the key by reference */
   haGETMax,       /**< Returns the Maximum Range for the key by reference */
   haGETMin,       /**< Returns the Minimum Range for the key by reference */
   haNULL,         /**< No action defined (used when a key does not represent data, but is used as a signal). */
   haSETbyREF,     /**< Sets the value of the key by reference. */
   haSETbyVAL,     /**< Sets the value of the key by value. */
   haSETbySTR      /**< @internal */
  };
typedef enum HAL_CONTROL_ACTION HAL_CONTROL_ACTION;

typedef int HAL_CONTROL_KEY;

/**
 *  @ingroup shared_data
 *  This is the fragment list structure.  Each fragment list entry contains a
 *  length and a data buffer.
 */
typedef struct
{
   bit32u   len;    /**< Length of the fragment in bytes (lower 16 bits are valid).  For SOP, upper 16 bits is the buffer offset. */
   void     *data;  /**< Pointer to fragment data */
   void     *OsInfo; /**< Pointer to OS defined data */
}FRAGLIST;


/*
 *
 *   P U S H  /  P U L L  M A C R O S
 *
*/

/* All the parameter access macros are based upon the following base macro. For the macros
   which utilize Ch and Queue information, that information is overloaded into the action
   parameter.  The macros ACTION_CH and ACTION_CH_Q define this behavior, and they are part
   of the common module, implemented in cpswhalcommon_defs.h. */
#define halIoctl(action, key, value) HalFunc->Control(HalDev,key,action,value)

/**
@defgroup Parameter_Macros Useful Macros for Parameter Access

The macros defined here may be used by the OS to access HAL
parameters (HAL_CONTROL_KEYs) in a simple fashion.  NOTE: Each of these
macros requires certain local variables to be in scope.  All of the
macros require the following two variables: 'HAL_FUNCTIONS *HalFunc', and
'HAL_DEVICE *HalDev'.
@{
*/

/** @hideinitializer For HAL_CONTROL_KEY 'key', gets the 'value' by reference.  Caller must
    provide a pointer to the expected type in 'value'.  Equivalent to the haGETbyREF action. */
#define halPullRef(key,value)   cpswhalGet(HalDev, HalFunc, key, value)
/** @hideinitializer For HAL_CONTROL_KEY 'key', sets the 'value' by reference.  Caller must
    provide a pointer to the data in 'value'.  Equivalent to the haSETbyREF action. */
#define halPushRef(key,value)   halIoctl(haSETbyREF,   key, value)
/** @hideinitializer For HAL_CONTROL_KEY 'key', sets the 'value' by passing the actual value.
    Equivalent to the haSETbyVAL action. */
#define halPushValue(key,value) cpswhalSet(HalDev, HalFunc, key, value)

/* The following macros allow the OS to easily set/get a Channel or Channel/Queue parameter value. */

/** @hideinitializer For HAL_CONTROL_KEY 'key' and (int) channel 'ch', sets the 'value' by passing the actual value.
    Equivalent to the haSETbyVAL action. */
#define halPushValueCh(key,value,ch) halIoctl(ACTION_CH(haSETbyVAL,ch), key, (void*)value)

/** @hideinitializer For HAL_CONTROL_KEY 'key' and (int) channel 'ch', sets the 'value' by reference.  Caller must
    provide a pointer to the data in 'value'.  Equivalent to the haSETbyREF action. */
#define halPushRefCh(key,value,ch) halIoctl(ACTION_CH(haSETbyREF,ch), key, value)

/** @hideinitializer For HAL_CONTROL_KEY 'key' and (int) channel 'ch', gets the 'value' by reference.  Caller must
    provide a pointer to the expected type in 'value'.  Equivalent to the haGETbyREF action. */
#define halPullRefCh(key,value,ch) halIoctl(ACTION_CH(haGETbyREF,ch), key, value)

/** @hideinitializer For HAL_CONTROL_KEY 'key', (int) channel 'ch', and (int) queue 'q', sets the 'value' by passing the actual value.
    Equivalent to the haSETbyVAL action. */
#define halPushValueChQ(key,value,ch,q) halIoctl(ACTION_CH_Q(haSETbyVAL,ch,q), key, (void *)value)

/** @hideinitializer For HAL_CONTROL_KEY 'key', (int) channel 'ch', and (int) queue 'q', sets the 'value' by reference.  Caller must
    provide a pointer to the data in 'value'.  Equivalent to the haSETbyREF action. */
#define halPushRefChQ(key,value,ch,q) halIoctl(ACTION_CH_Q(haSETbyREF,ch,q), key, value)

/** @hideinitializer For HAL_CONTROL_KEY 'key', (int) channel 'ch', and (int) queue 'q', gets the 'value' by reference.  Caller must
    provide a pointer to the expected type in 'value'.  Equivalent to the haGETbyREF action. */
#define halPullRefChQ(key,value,ch,q) halIoctl(ACTION_CH_Q(haGETbyREF,ch,q), key, value)

/** @} */

#define osIoctl(action, key, value) OsFunc->Control(OsDev,key,action,value)
#define osPullRef(key,value)        osIoctl(haGETbyREF, key, value)
#define osPushRef(key,value)        osIoctl(haSETbyREF, key, value)
#define osPushValue(key,value)      osIoctl(haSETbyVAL, key, (void*)(value))


#endif /* _CPSWHALCOMMON_DEFS_H */
