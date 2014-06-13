/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains all the global constructs for the CPSW HAL
 *
 *  HISTORY:
 *   Date      Modifier      Ver    Notes
 *  01Oct03   Greg/Denis   1.00    Original Version
 *****************************************************************************/

/* Needed for the cpswhal.h include being done */

typedef void OS_DEVICE;
typedef void OS_SENDINFO;
typedef void OS_RECEIVEINFO;
typedef void OS_SETUP;

#include "cpswhalcommon_params.h"
#include "cpswhalcommon_support.h"

/*
 *  Checks a parameter in the parameter table to see if we are allowed
 *  to write it, given the current state of the system and the
 *  permissions related to the parameter.  Returns TRUE if writable,
 *  FALSE if not.
 */

static int IsWritable(HAL_DEVICE *HalDev, const DEVICE_PARAMS *DeviceParams, SUB_STRUCT *SubStruct, int StructIndex, int Ch)
{
    /* determine if permissible to write */
    if(!(DeviceParams->Type & WRITABLE))
      return(FALSE);   /* param not writable */

    /*  Parameter is writable, if can write after open, then done */
    if(DeviceParams->Type & WR_AFTER_OPEN)
      return(TRUE);    /* writable, even with Module Open*/

    /* Parameter is writable but not after open, check if module Open */
    if(SubStruct[StructIndex].IsOpen(HalDev, Ch, CH_DIRECTION(DeviceParams->Type)))
      return (FALSE);  /* Module is open, cannot update parameter */
    else
      return(TRUE);    /* Module is not open, can update parameter */
}

/* NOTE: Should this be in support rather than params? */
/* function looks for a decimal integer, maximum 19 digits in length */
int cpswHalCommonParseValue(const char *invalue, bit32u *value, int index)
  {
   char *p;
   char ValueFound[20];

   p = &ValueFound[0];

   while(*invalue)
     {
       if(*invalue != ';')
         {
         *p++ = *invalue++;
         }
       else
         {
           index--;
           if(index==0)
             {
               break;  /* Value Found */
             }
           else
             {
              p = &ValueFound[0]; /* not value wanted, reset pointer */
              invalue++;
             }
         }
     } /* while loop */

   if(index>1)
     return(1);  /* search terminated before delimiter found for requested index*/

   *p = '\0';  /* null terminate found value */

   *value = cpswHalCommonstrtoul(&ValueFound[0], 0, 10); /* convert value */
   return(0);
  }

/* This function parses the Ch and Queue information out of the Action parameter.
   If the channel is invalid, -1 is returned in the integer pointed to by index1.
   If the queue is invalid, -1 is returned in the integer pointed to by index2.
   The action is returned by itself in integer pointed to by index3. */
void cpswHalCommonParseAction(int Action,
                       int *Ch,
                       int *Queue,
                       int *ChValid,
                       int *QueueValid,
                       int *MaskedAction)
  {
   *ChValid = INVALID_CH;
   *QueueValid = INVALID_Q;
   if (Action & CH_VALID)
     {
      *Ch = (Action >> CH_FIELD_SHIFT) & CH_FIELD_MASK;
      *ChValid = VALID_CH;
      if (Action & Q_VALID)
        {
     *Queue = (Action >> Q_FIELD_SHIFT) & Q_FIELD_MASK;
         *QueueValid = VALID_Q;
        }
     }
   *MaskedAction = (Action & ACTION_FIELD_MASK);
  }

/**
 *  @ingroup Params_Functions
 *

 *  @param  HalDev        Device handle.
 *  @param  OsFunc        Os function block pointer.
 *  @param  DeviceParams  Pointer to the device params table.
 *  @param  SubStruct     Pointer to substructure definitions for this module.
 *  @param  Key           Pointer to Key, representing which parameter to access.
 *  @param  Action        Indicates the action to perform on the parameter (haGETbyREF, haSETbyREF, haSETbyVAL)
 *  @param  Value         (Void *) Output parameter for GET actions, input parameter for SET actions.
 *
 *
 *  @return EC_NO_ERRORS
 *
 */
int cpswHalCommonParamsControl(HAL_DEVICE *HalDev,OS_FUNCTIONS *OsFunc,const DEVICE_PARAMS *DeviceParams,SUB_STRUCT *SubStruct, int Key, int Action, void *Value)
  {
  void *stbase;
  int StructIndex, Ch=0, Queue=0,ChValid,QValid,loops,i;
  int sub_struct_size, sub_index_size1, sub_index_size2;
  int rc, MaskedAction;

  /* Default retrun code to not finding the Key */

  rc = EC_VAL_KEY_NOT_FOUND;

#ifdef __CPHAL_DEBUG
  if (Key == hkDumpParams)
    {
     cpswHalCommonParamsDump(HalDev,OsFunc,DeviceParams,SubStruct);
     return(0);
    }
#endif

  cpswHalCommonParseAction(Action, &Ch, &Queue, &ChValid, &QValid, &MaskedAction);

  if(Key == hkVersionCommon)
  {
      if(MaskedAction==haGETbyREF)
      {
          *((char **)Value) = (char *)Version_COMMON;
          return(0);
      }
      else
      {
          return(EC_FUNC_CONTROL|EC_VAL_INVALID_ACTION);
      }
  }

  /*  Loop to find Match */
  while(DeviceParams->Key != INVALID_KEY)
    {
    if (DeviceParams->Key == Key)
      {
       /* Match Found, if no variable assigned, break out of Find Match loop */
       if(DeviceParams->Type & PASS_THROUGH)
         break;
       StructIndex = (DeviceParams->Type&SUBSTRUCT_MASK)>>SUBSTRUCT_SHIFT;
       sub_struct_size = SubStruct[StructIndex].sub_struct_size;
       sub_index_size1 = SubStruct[StructIndex].sub_index_size1;
       sub_index_size2 = SubStruct[StructIndex].sub_index_size2;

       /* calculate address of first parameter to be accessed */
       stbase = SubStruct[StructIndex].sub_struct + ((bit32u)DeviceParams->Offset);
       ((bit32u)stbase) += (sub_struct_size * Ch * sub_index_size2) + (sub_struct_size * Queue);

       /* NOTE: All "Get" operations return a single value, Ch and Queue default to 0 */
       if(MaskedAction==haGETMax)
       {
          *(bit32u*)Value = DeviceParams->HighValue;
          rc = 0;
          break;
       }
       if(MaskedAction==haGETMin)
       {
          *(bit32u*)Value = DeviceParams->LowValue;
          rc = 0;
          break;
       }
       if(MaskedAction==haGETbyREF)
         {
         if(DeviceParams->Type & ISSTRING)
           {
           *((char **)Value)=*(char **)(((bit32u)stbase));
           }
         else
           {
           *((bit32u*)Value)=*((bit32u*)(((bit32u)stbase)));
           }
         rc = 0;

         break;
         }

       if (MaskedAction == haCLEAR)
         {
          rc = 0; break;
         }

       if ((MaskedAction==haSETbyVAL) || (MaskedAction==haSETbyREF) || (MaskedAction==haSETbySTR))
         {
          /* calculate the number of parameters to be accessed */
          if (ChValid == VALID_CH)
            {
             if (QValid == VALID_Q)
               loops = 1; /* caller specified both indices */
              else
               loops = sub_index_size2; /* caller specified first index, so # is size of second index */
            }
           else
            {
             loops = sub_index_size1 * sub_index_size2; /* caller specified nothing, so set everything */
            }

          /* if this is a SET, we must find out the current state of the substruct so that we can determine, based on parm table rules, whether or not the SET is currently allowable */
          if (!(IsWritable(HalDev, DeviceParams, SubStruct, StructIndex, Ch)))
            {
#ifdef __CPHAL_DEBUG
             OsFunc->Printf("Not writable, Key : %s\n", DeviceParams->pszName);
#endif
             return(EC_FUNC_CONTROL|EC_VAL_PARAM_NOT_WRITABLE);
            }

          for (i=0; i<loops; i++)
            {
             if ((MaskedAction==haSETbyVAL)&&((DeviceParams->Type&ISSTRING)==0))
               {
                if (!( ( DeviceParams->LowValue<=((bit32u)Value) ) &&( DeviceParams->HighValue>=((bit32u)Value) ) ) )
                  {
#ifdef __CPHAL_DEBUG
                   OsFunc->Printf("SET Out of Range, Key: %s\n", DeviceParams->pszName);
#endif
                   return(EC_VAL_OUT_OF_RANGE);
                  }

                if ((DeviceParams->Type&ISBASE) != 0)
                  {
                   /* if this is a base address, assure that we convert to virtual, uncached */
                   *((bit32u*)(((bit32u)stbase)))=(bit32u)PhysToVirtNoCache((bit32u)Value);
                  }
                 else
                  {
                   *((bit32u*)(((bit32u)stbase)))=(bit32u)Value;
                  }
                rc = 0;
               }

             if ((MaskedAction==haSETbyREF)&&((DeviceParams->Type&ISSTRING)==0))
               {
                if(!( ( DeviceParams->LowValue<=(*(bit32u*)Value) ) &&( DeviceParams->HighValue>=(*(bit32u*)Value) ) ) )
                  {
#ifdef __CPHAL_DEBUG
                   OsFunc->Printf("SET Out of Range, Key : %s\n", DeviceParams->pszName);
#endif
                   return(EC_VAL_OUT_OF_RANGE);
                  }

                if((DeviceParams->Type&ISBASE) != 0)
                  {
                   /* if this is a base address, assure that we convert to virtual, uncached */
                   *((bit32u*)(((bit32u)stbase)))=(bit32u)PhysToVirtNoCache(*(bit32u*)Value);
                  }
                 else
                  {
                   *((bit32u*)(((bit32u)stbase)))=*(bit32u*)Value;
                  }
                rc = 0;
               } /* haSETP logic */

             if ((MaskedAction==haSETbyVAL)&&((DeviceParams->Type&ISSTRING)!=0))
               {
                /* SETbyVAL for strings uses a string copy */
                cpswHalCommonstrcpy( (char *)(((bit32u)stbase)), Value);
                rc = 0;
               }

             if ((MaskedAction==haSETbyREF)&&((DeviceParams->Type&ISSTRING)!=0))
               {
                /* SETbyREF for strings uses a pointer copy */
                *((char**)((bit32u)stbase))=(char *)Value;
                rc = 0;
               }
#ifdef __CPHAL_FUTURE_USE
             if (MaskedAction==haSETbySTR)
               {
                if((DeviceParams->Type&ISSTRING)!=0)
                  {
                   *((char**)((bit32u)stbase))=(char *)Value;
                  }
                 else
                  {
                   if ((DeviceParams->Type&ISBASE) != 0)
                     {
                      /* if this is a base address, assure that we convert to virtual, uncached */
                      *((bit32u*)(((bit32u)stbase)))=(bit32u)PhysToVirtNoCache((bit32u)cpswHalCommonatoul(Value));
                     }
                    else
                     {
                      *((bit32u*)(((bit32u)stbase)))=(bit32u)cpswHalCommonatoul(Value);
                     }
                  }
                rc = 0;
               }
#endif
             /* sub_struct_size must be defaulted to one so that integer arrays within HalDev and other
                non-indexed structures can be correctly accessed */

             stbase += sub_struct_size;
            } /* for */
          /* break out after the for loop */
          break;
         } /* if haSETbyVAL || haSETSTR */

       return(EC_VAL_INVALID_ACTION);
      } /* if key matches (or pass through) */
     DeviceParams++;
    } /* while */

   /* call special function if provided */
   if (DeviceParams->ParamFunc != 0)
     {
      rc = DeviceParams->ParamFunc(HalDev, Key, Action, Value);
     }

   return(rc);
  }

#ifdef __CPHAL_DEBUG
int cpswHalCommonParamsDump(HAL_DEVICE *HalDev,OS_FUNCTIONS *OsFunc,const DEVICE_PARAMS *DeviceParams, SUB_STRUCT *SubStruct)
  {
   int line=0, StructIndex,sub_struct_size,sub_index_size1,sub_index_size2,i,j;
   void *stbase;

   while(DeviceParams->Key != INVALID_KEY)
     {
      StructIndex = (DeviceParams->Type&SUBSTRUCT_MASK)>>SUBSTRUCT_SHIFT;
      stbase = SubStruct[StructIndex].sub_struct + ((bit32u)DeviceParams->Offset);
      sub_struct_size = SubStruct[StructIndex].sub_struct_size;
      sub_index_size1 = SubStruct[StructIndex].sub_index_size1;
      sub_index_size2 = SubStruct[StructIndex].sub_index_size2;

      for (i=0; i<sub_index_size1; i++)
        {
         for (j=0; j<sub_index_size2; j++, stbase += sub_struct_size)
           {
            /* skip this parameter if the integer value is 0 */
            if ((DeviceParams->Type&ISSTRING) == 0)
              if (*((bit32u*)(((bit32u)stbase))) == 0)
                 continue;

            if (sub_index_size1 > 1)
              {
               if (sub_index_size2 > 1)
                 OsFunc->Printf("%-18s[%2d][%2d] ",DeviceParams->pszName,i,j);
                else
                 OsFunc->Printf("%-18s[%2d]     ",DeviceParams->pszName,i);
              }
             else
              OsFunc->Printf("%-18s         ",DeviceParams->pszName);

            if (DeviceParams->Type&ISSTRING)
              OsFunc->Printf("%-10s",((char*)(((bit32u)stbase))));
             else
              OsFunc->Printf("0x%08x",*((bit32u*)(((bit32u)stbase))));
            if (line++)
              {
               OsFunc->Printf("\n");
               line=0;
              }
             else
              OsFunc->Printf("  ");
           }
        }
      DeviceParams++;
     }

   if (line)
     OsFunc->Printf("\n");

   return(0);
  }
#endif

int cpswHalCommonParamsCheckAllParams(HAL_DEVICE *HalDev,const DEVICE_PARAMS *DeviceParams, SUB_STRUCT *SubStruct)
  {
   bit32u val,hi,lo, StructIndex;
   void *stbase;

#ifdef __CPHAL_DEBUG
   int err=0;
#endif

   while(DeviceParams->Key != INVALID_KEY)
     {
      /* only check parameters which are not pass through */
      if (!(DeviceParams->Type & PASS_THROUGH))
        {
         /* probably need to add a check here to only check parameter values,
            not including statistics values or soft stats */
         if (DeviceParams->Type & WRITABLE)
           {
            StructIndex = (DeviceParams->Type&SUBSTRUCT_MASK)>>SUBSTRUCT_SHIFT;
            stbase = SubStruct[StructIndex].sub_struct;

            if (DeviceParams->Type&ISSTRING)
              val=cpswHalCommonstrlen(((char*)(((bit32u)stbase)+((bit32u)DeviceParams->Offset))));
             else
              val=*((bit32u*)(((bit32u)stbase)+((bit32u)DeviceParams->Offset)));
            hi=DeviceParams->HighValue;
            lo=DeviceParams->LowValue;

            if (!((val>=lo)&&(val<=hi)))
              {
#ifdef __CPHAL_DEBUG
           /* for debug purposes, we want to see all of the out of range params at once */
               err=1;
               printf("(CheckAllParams) Out of Range, Key : %s\n", DeviceParams->pszName);
               DeviceParams++;
               continue;
#endif
               return(EC_VAL_OUT_OF_RANGE);
              }
           }
        }
      DeviceParams++;
     }

#ifdef __CPHAL_DEBUG
   if (err == 1)
     return(EC_VAL_OUT_OF_RANGE);
#endif

   return(0);
  }

/* we don't need to pass HalDev if we include it in the structure array */
int cpswHalCommonParamsInit(HAL_DEVICE *HalDev,const DEVICE_PARAMS *DeviceParams, SUB_STRUCT *SubStruct)
  {
   void *stbase;
   int StructIndex,sub_index_size1,sub_index_size2,sub_struct_size,i,j;

   while(DeviceParams->Key != INVALID_KEY)
     {
      if (DeviceParams->Type&DO_INIT)
        {
         StructIndex = (DeviceParams->Type&SUBSTRUCT_MASK)>>SUBSTRUCT_SHIFT;
         stbase = SubStruct[StructIndex].sub_struct + ((bit32u)DeviceParams->Offset);
         sub_struct_size = SubStruct[StructIndex].sub_struct_size;
         sub_index_size1 = SubStruct[StructIndex].sub_index_size1;
         sub_index_size2 = SubStruct[StructIndex].sub_index_size2;

         for (i=0; i<sub_index_size1; i++)
           {
            for (j=0; j<sub_index_size2; j++)
              {
               if (DeviceParams->Type&ISSTRING)
                 {
                  if (cpswHalCommonstrlen((char *)DeviceParams->Default)<DeviceParams->HighValue)
                    *(char**)(stbase) = (char*)DeviceParams->Default;
                 }
               else
                  *((bit32u*)(((bit32u)stbase)))=DeviceParams->Default;

               /* only update the base if continuing this inner loop */
               if(j < (sub_index_size2 - 1))
                 ((bit32u)stbase) += sub_struct_size;
              }
            ((bit32u)stbase) += sub_struct_size;
           }
        }
      DeviceParams++;
     }

   return(0);
  }

/**
 *  @ingroup Params_Functions
 *
 *  @param  Action        Indicates the action to perform on the parameter (haGETbyREF, haSETbyREF, haSETbyVAL)
 *  @param  ParmValue         (Void *) Output parameter for GET actions, input parameter for SET actions.
 *
 *  This functions returns the 'value' of ParmValue is Action is Set
 *
 *  @return Value of ParmValue
 *
 */
bit32u cpswHalCommonParamsValueSet(HAL_CONTROL_ACTION Action, void *ParmValue)
{
    bit32u Value;
    switch(Action)
    {
        case haSETbyREF:
             Value = *(unsigned int *) ParmValue;
        break;
        case haSETbyVAL:
             Value = (bit32u) ParmValue;
        break;
        default:
             Value = 0xFFFFFFFF;
        break;
    }
    return(Value);
}
