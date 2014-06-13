/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: CPSWHALCOMMON_STDDEF.H
 *
 *  DESCRIPTION:
 *      CPHAL Only include - OS-defined 'typedefs'
 *
 *  HISTORY:
 *      Date      Modifier              Notes
 *      01Oct03   Michael Hanrahan
 *      14Nov03   Michael Hanrahan      Now only contains OS typedefs
 *****************************************************************************/
#ifndef _CPSWHALCOMMON_STDDEF_H
#define _CPSWHALCOMMON_STDDEF_H

#include "cpswhalcommon_defs.h"

  #ifndef _INC_ADAM2

    #ifndef size_t
      #define size_t unsigned int
    #endif /* size_t check */

    #ifndef TRUE
      #define TRUE (1==1)
    #endif

    #ifndef FALSE
      #define FALSE !(TRUE)
    #endif

    #ifndef NULL
      #define NULL 0
    #endif

  #endif  /* ADAM2 check */
#endif    /* _CPSWHALCOMMON_STDDEF_H */