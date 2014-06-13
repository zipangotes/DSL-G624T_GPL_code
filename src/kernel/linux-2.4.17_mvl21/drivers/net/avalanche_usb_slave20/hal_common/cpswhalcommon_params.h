/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE: CPSWHALCOMMON_PARAMS.H
 *
 *  DESCRIPTION:
 *      Params include file
 *
 *  HISTORY:
 *      Date      Modifier  Ver    Notes
 ****************************************************************************/
#ifndef _CPSWHALCOMMON_PARAMS_H
#define _CPSWHALCOMMON_PARAMS_H
/* new file in RC 3.02 */

/**
@defgroup Common_Statistics Statistics

A broad array of module statistics is available.  Statistics values are accessed
through the provided macros (described below).  There are 5 different levels
of statistics, each of which correspond to a unique set of data.  Furthermore,
certain statistics data is indexed by using a channel number and Tx queue number.

Each module will individually define the content of each statistic level.  Please
refer the module-specific appendix for information about specific stats.

The caller requests statistics information by using the provided macros:
- halGetStats(lvl, value)
- halGetChStats(lvl, ch, value)
- halGetChQStats(lvl, ch, queue, value)

NOTE: Each macro requires OsDev (the OS device structure, containing OsFunc), HalFunc (the CPHAL
functions structure), and HalDev (the CPHAL private structure) to be defined in the current scope.
'Lvl' is the desired stats level to return.  If the stats group requires 'ch' and/or 'queue',
there are seperate macros defined which accept those as parameters.  The caller must supply
an int * in the 'value' field.  The HAL will allocate memory to store the returned data,
which MUST be freed by the OS when the OS no longer needs to returned data.

Code Examples:
@code
unsigned int *StatsData;

# Get Level 0 stats for Channel 1
halGetChStats(0, 1, &StatsData);

# Get Level 2 stats for Channel 0, Queue 0
halGetChQStats(2, 0, 0, &StatsData);

# Get Level 4 stats
halGetStats(4, &StatsData);
@endcode

The information returned through the Value parameter is an
array of pointers to strings.  The pointers are arranged in pairs.
The first pointer is a pointer to a name string for a particular statistic.
The next pointer is a pointer to a string containing the representation of
the integer statistic value corresponding to the first pointer.  This is followed
by another pair of pointers, and so on, until a NULL pointer is encountered.  The
following is example code for processing the statistics data.  Note that the OS
is responsible for freeing the memory passed back through the Value parameter.

The following provides sample code for printing out statistics value after
a call to one of the provided macros.

@code
unsigned int *StatsData;

# Get Level 0 stats for Channel 1
halGetChStats(0, 1, &StatsData);

# output Statistics data
PrintStats(StatsData);

...

void PrintStats(unsigned int *StatsPtr)
  {
   while(*StatsPtr)
     {
      printf("%20s:",  (char *)*StatsPtr);
      StatsPtr++;
      printf("%11s\n", (char *)*StatsPtr);
      StatsPtr++;
     }
   MySioFlush();

   # the upper layer is responsible for freeing stats info
   free(StatsPtr);
  }
@endcode

*/


/*
 * Parameter Table
 *
 * Field breakdown of Type:
 *  00:00  Initialize    (if set, param will be initialized to default value)
 *  01:01  PassThrough   (set if parameter has no storage - function call only)
 *  02:02  Writable      (if set, param is writable)
 *  03:03  WriteAfterOpen (if writable, can you write it after open?)
 *  04:04  Clearable     (if set, param can be cleared)
 *  05:05  IsEnv         (if 1, param is an OS environment variable)
 *  06:06  IsBaseAddress (if 1, the parameter is a base address,
 *                        should be converted to virtnocache)
 *  07:09  Group         (8 groups)
 *  10:10  IsString      (binary(0) or char string(1))
 *  11:16  Field Width   (UNUSED) (up to 63 bit width)
 *  17:21  Field Offset  (UNUSED) (up to 31 bit offset/shift)
 *  22:25  Substructure  (16 max; 0-HalDev, 1-ChData, 2-Stats, etc.)
 *  26:28  Unused
 *  29:30  Index         (0: No Index, 1: Channel #, 2: Ch and Queue #)
 *  31:31  ChDirection   (0: Tx, 1: Rx)
 */

#include "cpswhal.h"
#include "cpswhalcommon_support.h"

#ifndef offsetof
  /* offset of field m in a struct s */
  #define offsetof(s,m)   (size_t)( (char *)&(((s *)0)->m) - (char *)0 )
#endif

/* Parameter defines */
#define DO_INIT             (1  <<  0)

/* Define this if parameter has no storage - only the specified ParamFunc will be called */
#define PASS_THROUGH        (1  <<  1)

/* Permissions */
#define WRITABLE            (1  <<  2)
#define WR_AFTER_OPEN       (1  <<  3)
#define WRITEALL            (WRITABLE | WR_AFTER_OPEN)
#define CLEARABLE           (1  <<  4)

/* Define this if the parameter is an OS environment variable */
#define ISENV               (1  <<  5)

/* Define this if the parameter represents a base address */
#define ISBASE              (1  <<  6)

/* Stat Groups */
#define GROUP_SHIFT         7
#define GROUP_MASK          (0x7 <<  GROUP_SHIFT)
#define GROUP1              (1   <<  GROUP_SHIFT)
#define GROUP2              (2   <<  GROUP_SHIFT)
#define GROUP3              (3   <<  GROUP_SHIFT)
#define GROUP4              (4   <<  GROUP_SHIFT)
#define GROUP5              (5   <<  GROUP_SHIFT)
#define GROUP6              (6   <<  GROUP_SHIFT)
#define GROUP7              (7   <<  GROUP_SHIFT)

/* Define this if the parameter is a string */
#define ISSTRING            (1  << 10)

/* Width and offset (currently unused) */
#define WIDTH(n)            (n  << 11)
#define OFFSET(n)           (n  << 17)

/* Substructures */
#define MAX_SUB_STRUCT      16
#define SUBSTRUCT_SHIFT  22
#define SUBSTRUCT_MASK   (0xf  <<  SUBSTRUCT_SHIFT)
#define SUBSTRUCT(n)     (n    <<  SUBSTRUCT_SHIFT)  /* to be used in params tables */

/* Params code should not need these!  It should work independent of the content of a substructure
#define HAL_DEV             (0  <<  22)
#define CH_DATA             (1  <<  22)
#define STATS               (2  <<  22)
#define LOOSE               (15 <<  22)
*/

/* if the parameter is an array indexed by channel, define CH_INDEX.
   If the parameter is an array indexed by ch and queue, define CH_Q_INDEX. */
#define INDEX_SHIFT         29
#define INDEX_MASK          (0x3 << INDEX_SHIFT)
#define CH_INDEX            (1   << INDEX_SHIFT)
#define CH_Q_INDEX          (2   << INDEX_SHIFT)

/* to determine channel direction */
#define CH_DIRECTION_SHIFT  31
#define TX                  (0 << CH_DIRECTION_SHIFT)
#define RX                  (1 << CH_DIRECTION_SHIFT)
#define CH_DIRECTION(type)  ((type >> CH_DIRECTION_SHIFT) & 1)

/*
 *  Params Table:
 *
 *  Once the developer has identified the complete list of parameters to expose, each
 *  parameter must be characterized in an array of DEVICE_PARAMS structures.  The array of
 *  parameter structures should be statically defined and initialized only once.  Parameters
 *  can be either software values maintained by the HAL or they can represent harware registers
 *  or portions of hardware registers.
 *
 *  The first entry (pszKey) is a pointer to a string representing the name of the parameter.  It is recommended to define
 *  all parameter names as extern const char*, so that callers may use the externs when accessing
 *  the various API's which utilize the parameter table.
 *
 *  The second entry (Type) defines multiple characteristics of the given parameter.  #Defines are provided in
 *  the cpswhalcommon_params.h file to simplify this task.  Type is broken into multiple bit-fields, so the developer
 *  usually must OR together multiple elements into the Type field.  Following is a breakdown of possible
 *  Type elements:
 *  a) DO_INIT:  Define this if you want the parameter to be initialized to the "Default" value in the structure.  If
 *               you do not use DO_INIT, the value associated with the parameter will be unmodified during initialization.
 *  b) WRITABLE, WR_AFTER_OPEN:  These two elements control if and when the parameter may be written.  If the parameter is
 *               not a read-only parameter, you should use WRITABLE.  If the parameter can be written after the device is
 *               open, you should use WR_AFTER_OPEN.
 *  c) Substructures:  The params code allows for parameters to exist in up to 4 substructures.  You must define each
 *               parameter to be in a substructure.  The current defines are HAL_DEV (if the parameter is in the HAL_DEVICE
 *               structure, which is common), CH_DATA (if the parameter is in the HalDev->ChInfo structure), STATS (if the
 *               parameter exists in a statistics structure.  Please see the documentation for SUB_STRUCT which is related to this.
 *  d) Statistics Group:  You can optionally assign any parameter to a statistics "group".  This allows easy access to the
 *               values of related variables which can be retrieved by group.  Up to 7 statistics groups can be defined, using
 *               the #defines GROUP1, GROUP2, up to GROUP7.
 *  e) ISSTRING: Define this if the parameter's value is best represented in string form.
 *  f) ISBASE:   Define this if the parameter represents a base address.  This allows the params code to ensure the value is
 *               stored as a virtual, uncached address.
 *  g) Indices (CH_INDEX, CH_Q_INDEX):  Define CH_INDEX if the parameter can be indexed by channel only.  Define CH_Q_INDEX if
 *               the parameter can be indexed by both a channel and a queue number.
 *  h) Channel Direction (TX, RX): Define TX for channel parameters which are TX only.  Define RX for channel parameters which are
 *               RX only.
 */

/* The string 'Name' is only included in the params table if we are running in debug mode.
   It is only used for the CommonParamsDump() function. */
#ifdef __CPHAL_DEBUG
 #define PARM(Key,Name,Type,Offset,Default,Lo,Hi,Func) {Key,Name,Type,Offset,Default,Lo,Hi,Func}
#else
 #define PARM(Key,Name,Type,Offset,Default,Lo,Hi,Func) {Key,Type,Offset,Default,Lo,Hi,Func}
#endif

typedef struct _device_params_s
{
   int                 Key;
#ifdef __CPHAL_DEBUG
   const char          *pszName;
#endif
   const  unsigned int Type;
   size_t              Offset;
   const  unsigned int Default;
   const  unsigned int LowValue;
   const  unsigned int HighValue;
   int    (*ParamFunc)(HAL_DEVICE *HalDev, HAL_CONTROL_KEY Key, HAL_CONTROL_ACTION Action, void *Value);
}DEVICE_PARAMS;

typedef struct
{
   void   *sub_struct;
   unsigned int sub_struct_size;
   unsigned int sub_index_size1;
   unsigned int sub_index_size2;
   int (*IsOpen)(HAL_DEVICE *Dev, int p1, int p2);
}SUB_STRUCT;

int cpswHalCommonParamsControl(HAL_DEVICE *HalDev,OS_FUNCTIONS *OsFunc,const DEVICE_PARAMS *pp,SUB_STRUCT *SubStruct, int Key, int Action, void *Value);
int cpswHalCommonParamsDump(HAL_DEVICE *HalDev,OS_FUNCTIONS *OsFunc,const DEVICE_PARAMS *pp, SUB_STRUCT *SubStruct);
int cpswHalCommonParamsCheckAllParams(HAL_DEVICE *HalDev,const DEVICE_PARAMS *DeviceParams, SUB_STRUCT *SubStruct);
int cpswHalCommonParamsInit(HAL_DEVICE *HalDev,const DEVICE_PARAMS *DeviceParams,SUB_STRUCT *SubStruct);
int cpswHalCommonParseValue(const char *invalue, bit32u *value, int index);
bit32u cpswHalCommonParamsValueSet(HAL_CONTROL_ACTION Action, void *ParmValue);
void cpswHalCommonParseAction(int Action, int *Ch, int *Queue, int *ChValid, int *QueueValid, int *MaskedAction);
#endif
