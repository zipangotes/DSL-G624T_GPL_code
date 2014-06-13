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
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  Now that you've gotten here you are probably serious about understanding   */
/*  the way Adam2 uses the flash. All flash operations are contained in this   */
/*  module. There are 7 interfaces to this module. FWBGetBlockSize, FWBErase,  */
/*  FWBOpen, FWBWriteByte, and FWBClose, FWBUnLock, FWBLock.                   */
/*                                                                             */
/*  FWBGetBlockSize(base);       - returns the Flash Block size for the        */
/*                                 requested region so that all users of the   */
/*                                 flash understand the blocking factor.       */
/*                                                                             */
/*  FWBErase(base,size,verbose); - Erases from base address of size in blocks. */
/*                                                                             */
/*  FWBOpen(Base);               - Will always be called prior to FWBWriteByte,*/
/*                                 and is responsible for unlocking the region */
/*                                 and preparing for flash writes.             */
/*                                                                             */
/*  FWBWriteByte(address,data);  - Is called to write data to the flash        */
/*                                 address, remember that a FWBClose will be   */
/*                                 called prior to any use of the written data.*/
/*                                                                             */
/*  FWBClose()                   - Will complete any pending flash writes to a */
/*                                 region, and lock the flash.                 */
/*                                                                             */
/*  FWBUnLock(base,size)         - Unlocks the blocks specified                */
/*                                                                             */
/*  FWBLock(base,size)           - Lock the blocks specified                   */
/*                                                                             */
/*  The use of FWBOpen, FWBWriteByte, and FWBClose allows for efficient block  */
/*  writes. Remember it is possible to only get One FWBWriteByte in-between a  */
/*  FWBOpen and a FWBClose.                                                    */
/*                                                                             */
/*  During the Boot process the FWBGetBlockSize is called. This block can be   */
/*  called at any time, and returns the size of the flash blocks for the       */
/*  requested region. It is possible to have different size flash blocks in    */
/*  different memory regions.                                                  */
/*                                                                             */
/*  Any time the Kernel wishes to erase a block of flash, it will call the     */
/*  FWBErase routine. The routine is assumed to erase in blocks. That is       */
/*  FWBErase(CS0_BASE,1,0); will erase the block at CS0_BASE. The same can be  */
/*  said for FWBErase(CS0_BASE,FWBGetBlockSize(CS0_BASE),0); So that           */
/*  FWBErase(CS0_BASE,FWBGetBlockSize(CS0_BASE)+1,0); and                      */
/*  FWBErase(CS0_BASE+1,FWBGetBlockSize(CS0_BASE),0); actually erases 2 blocks.*/
/*                                                                             */
/*  It is important to not that the FWBXxxxx routines must always leave the    */
/*  flash in a read mode. Failure to do this will result in a system hang.     */
/*  printf() is also unuasable whilest the flash in not in a read mode         */
/*                                                                             */
/*  And last but not least, all FWB... routines return TRUE if successfull and */
/*  FALSE if unsuccessfull                                                     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#ifdef _STANDALONE
#include <psbl/stddef.h>
#include <psbl/ffs.h>
#include <psbl/stdio.h>
#include <psbl/flashop.h>
#include <psbl/hw.h>
#else 
#include "platform.h"
#include "stddef.h"
#include "flashop.h"
#include "hw.h"
#endif

void _FlashErrorOut(char *str,int sts);
int _FlashOperationComplete(bit32u adr,bit16u dat,int timeoutloops);

#define MCONTENTS_OF(addr)      (*((volatile unsigned int *)(addr)))
#define MEM_READ(addr)         (MCONTENTS_OF(addr))
#define MEM_WRITE(addr,val)    (MCONTENTS_OF(addr) = (val))

#define MEM_WRITE16(addr,val)  ((*((volatile unsigned short *)(addr)))=(val))
#define MEM_READ16(addr)       (*((volatile unsigned short *)(addr)))

#define FLASH_MAX_WAIT 125


bit32u global_addr = 0;
bit16u global_data = 0;

int    FWBopen=FALSE;
char   FWBwdata[2];
 
int FWBGetBlockSize(bit32u base)
  {
  int itmp;

  base&=0x1fffffff;
  base|=0xa0000000;
  if ((base>=CS0_BASE)&&(base<(CS0_BASE+CS0_SIZE)))
    {
    base&=0x03ffffff;

    if (base<(64*1024)) 
      itmp=(8*1024);
    else
      itmp=(64*1024);

    if (EMIF_ASYNC_CS0&0x2)
      itmp*=2;              /*Dual devices                                   */
    return(itmp);
    }
  return(0);
  }

/* This function returns back the next block/sector in flash */
bit32u FWBGetNextBlock(bit32u base)
{
	bit32u current_blk_sz;
	bit32u new_blk;

  	base&=0x1fffffff;
  	base|=0xa0000000;

	current_blk_sz = FWBGetBlockSize(base);
	
	if(current_blk_sz == 0)
	{
		return(0);
	}

	base &= ~(current_blk_sz-1);		
	new_blk = base + current_blk_sz;

	return(new_blk);
}

int FWBErase(bit32u base,int size,int verbose)
  {
  bit32u i,sts,blocksize,from,to;

  FWBopen=FALSE;

  if (size==0)
    return(0);
  size--;

  from=base;
  blocksize=FWBGetBlockSize(from);
  if (blocksize==0)
    {
    sys_printf("Invalid flash address %x\n",from);
    return(0);
    }
  from&=~(blocksize-1);

  to=base+size;
  blocksize=FWBGetBlockSize(to);
  if (blocksize==0)
    {
    sys_printf("invalid flash address %x\n",to);
    return(0);
    }
  to  |= (blocksize-1);

  if (verbose) sys_printf("FlashEraseBlock(%08x,%08x);\n",from,to);
  for(i=from,sts=0;(i<to)&&(!sts);)
    {
    MEM_WRITE16(0xb0000000+(0x555<<1),0x00aa);
    MEM_WRITE16(0xb0000000+(0x2aa<<1),0x0055);
    MEM_WRITE16(0xb0000000+(0x555<<1),0x0080);
    MEM_WRITE16(0xb0000000+(0x555<<1),0x00aa);
    MEM_WRITE16(0xb0000000+(0x2aa<<1),0x0055);
    MEM_WRITE16(i,0x0030);

    sts=_FlashOperationComplete(i,0xffff,FLASH_MAX_WAIT*5000000);
    _FlashErrorOut("Erase Block",sts);
    if (verbose) sys_printf(".");
    blocksize=FWBGetBlockSize(i);
    i+=blocksize;
    if (blocksize==0)
      {
      if (verbose) sys_printf("invalid flash address %x\n",i);
      return(0);
      }
    }
  return(sts==0);
  }

int FWBOpen(bit32u base)
  {

  FWBopen=FALSE;
  if (FWBGetBlockSize(base)==0)
    return(FALSE);
  FWBopen=TRUE;
  return(TRUE);
  }

int FWBWriteByte(bit32u adr, bit8 cVal)
  {
  int sts,i;
  bit16u *isp,*idp;

  if (!FWBopen)
    return(FALSE);
  sts=TRUE;
  isp=(bit16u *)(adr&~1);
  idp=(bit16u *)FWBwdata;
  *idp=*isp;
  i=adr&1;
  FWBwdata[i]=cVal;
  MEM_WRITE16(0xb0000000+(0x555<<1),0x00aa);
  MEM_WRITE16(0xb0000000+(0x2aa<<1),0x0055);
  MEM_WRITE16(0xb0000000+(0x555<<1),0x00a0);
  MEM_WRITE16(isp,*idp);
  sts=_FlashOperationComplete(adr&~1,*idp,FLASH_MAX_WAIT*5000);
  
  if( sts )
  {
     global_addr = ( bit32u )isp;
     global_data = *idp;

    _FlashErrorOut("Write Byte",sts);
  }  
  return(sts==0);
  }

int FWBClose(void)
  {

  if (!FWBopen)
    return(FALSE);
  FWBopen=FALSE;
  return(TRUE);
  }

/*Used to decode Flash error values                                          */

void _FlashErrorOut(char *str,int sts)
  {

  if (sts)
    sys_printf("Flash '%s' operation Failed: sts=%08x\n",str,sts);
  }

/*Returns 0 for success and !=0 for failure                                  */

int _FlashOperationComplete(bit32u adr,bit16u dat,int timeoutloops)
  {
  bit16u fnd,fnd2;
  int icount;
  int err;

  icount=0;
  fnd2=MEM_READ16(adr);
  do
    {
    if (++icount>timeoutloops)
      {
      MEM_WRITE16(adr,0x00f0);    /*Reset to read mode                     */
      return(0xeeee0001);
      }
    fnd=fnd2;
    fnd2=MEM_READ16(adr);
    if (fnd2==dat) 
      return(0);
    if ((dat&0x80)!=(fnd2&0x80))
      {
      if (fnd2&0x20)
        {
        err=fnd2&0x20;
        fnd2=MEM_READ16(adr);
        if (fnd2==dat) 
          return(0);
        MEM_WRITE16(adr,0x00f0);    /*Reset to read mode                     */
        return(err);
        }
      }
    }while(1);
  }

int FWBUnLock(bit32u adr,bit32 size)
  {
  return(TRUE);
  }

/*Returns TRUE for success and FALSE for failure                                  */

int FWBLock(bit32u from,bit32 size)
  {
  return(TRUE);
  }

