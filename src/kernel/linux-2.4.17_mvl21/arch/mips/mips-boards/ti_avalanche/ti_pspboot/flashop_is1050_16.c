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
/*  sys_printf() is also unuasable whilest the flash in not in a read mode         */
/*                                                                             */
/*  And last but not least, all FWB... routines return TRUE if successfull and */
/*  FALSE if unsuccessfull                                                     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#ifdef _STANDALONE
#include <psbl/stddef.h>
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
int _FWBWriteBlock(void);
int _FlashOperationComplete(bit32u adr,bit16u dat,int timeoutloops);

#define MCONTENTS_OF(addr)      (*((volatile unsigned short *)(addr)))
#define MEM_READ16(addr)         (MCONTENTS_OF(addr))
#define MEM_WRITE16(addr,val)    (MCONTENTS_OF(addr) = (val))

#define FLASH_MAX_WAIT 125
#define MAXBUF 2

bit32u FWBBaseAdr,FWBBaseUnLockAdr;
int    FWBvalid;
int    FWBopen=FALSE;
char   FWBwdata[MAXBUF];


int FWBUnLock(bit32u adr,bit32 size)
  {

  return(TRUE);
  }

/*Returns TRUE for success and FALSE for failure                                  */

int FWBLock(bit32u from,bit32 size)
  {

  return(TRUE);
  }

void UnLock(bit32u adr)
  {

  FWBBaseUnLockAdr=adr;
  MEM_WRITE16(adr,0x0060);
  MEM_WRITE16(adr,0x00D0);
  MEM_WRITE16(adr,0x00FF);
  }

void Lock(void)
  {

  MEM_WRITE16(FWBBaseUnLockAdr,0x0060);
  MEM_WRITE16(FWBBaseUnLockAdr,0x0001);
  MEM_WRITE16(FWBBaseUnLockAdr,0x00FF);
  }

int FWBGetBlockSize(bit32u base)
  {
  int itmp;

  base&=0x1fffffff;
  base|=0xa0000000;
#ifdef _AVALANCHE_    /*EVM                                                  */
  if ((base>=CS3_BASE)&&(base<(CS3_BASE+CS3_SIZE)))
    {
    itmp=(128*1024);   /*Strata flash 8/16 bit                               */
    if (EMIF_ASYNC_CS3&0x2)
      itmp*=2;              /*Dual devices                                   */
    return(itmp);
    }
#endif
  if ((base>=CS0_BASE)&&(base<(CS0_BASE+CS0_SIZE)))
    {
    itmp=(64*1024);   /*Strata flash 8/16 bit                                */
    if (base>=((CS0_BASE+CS0_SIZE)-(64*1024)))  /* Top Boot Block            */
      itmp=(8*1024);
    if (EMIF_ASYNC_CS0&0x2)
      itmp*=2;              /*Dual devices                                   */
    return(itmp);
    }
  return(0);
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
    UnLock(i);
    MEM_WRITE16(i,0x0020);
    MEM_WRITE16(i,0x00d0);
    sts=_FlashOperationComplete(i,0xffff,FLASH_MAX_WAIT*5000000);
    Lock();
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
  int i;

  FWBopen=FALSE;
  if (FWBGetBlockSize(base)==0) 
    return(FALSE);
  for(i=0;i<MAXBUF;i++) 
    FWBwdata[i]=0xff;
  FWBBaseAdr=0xffffffff;
  FWBvalid=0;
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
  if((adr&(~(MAXBUF-1)))!=FWBBaseAdr)
    sts=FWBClose();
  if (FWBvalid==0)
    {
    for(i=0,isp=(bit16u *)(adr&~(MAXBUF-1)),idp=(bit16u *)FWBwdata;i<(MAXBUF/2);i++)
      *idp++=*isp++;
    }
  /*sys_printf("FWBWriteByte(%x,%x);\n",adr,cVal&0x0ff);                         */
  FWBBaseAdr=adr&(~(MAXBUF-1));
  i=adr&(MAXBUF-1);
  FWBvalid++;
  FWBwdata[i]=cVal;
  return(sts);
  }

int FWBClose(void)
  {
  int sts,i;

  if (!FWBopen) 
    return(FALSE);
  if (FWBvalid==0) 
    return(TRUE);
  for(i=0,sts=1;(sts)&&(i<5);i++)
    {
    sts=_FWBWriteBlock();
    if (sts)
      {
      MEM_WRITE16(FWBBaseAdr,0x00ff);
      MEM_WRITE16(FWBBaseAdr,0x0050);
      }
    }

  _FlashErrorOut("Program Block",sts);

  for(i=0;i<MAXBUF;i++) 
    FWBwdata[i]=0xff;
  FWBBaseAdr=0xffffffff;
  FWBvalid=0;
  return(sts==0);
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

/*Used to decode Flash error values                                          */

void _FlashErrorOut(char *str,int sts)
  {

  if (sts)
    sys_printf("Flash '%s' operation Failed: sts=%08x\n",str,sts);
  }

/*Returns 0 for success and !=0 for error                                    */

int _FWBWriteBlock(void)
  {
  bit16u *uip;
  int sts;

  UnLock(FWBBaseAdr);
  MEM_WRITE16(FWBBaseAdr,0x0040);
  uip=(bit16u *)FWBwdata;
  MEM_WRITE16(FWBBaseAdr,*uip);
  sts=_FlashOperationComplete(FWBBaseAdr,*uip,FLASH_MAX_WAIT*650);
  Lock();
  return(sts);
  }

/*Returns 0 for success and !=0 for failure                                  */

int _FlashOperationComplete(bit32u adr,bit16u dat,int timeoutloops)
  {
  bit32u sts,fnd;
  int icount;
  int err;

  icount=0;
  do
    {
    if (++icount>timeoutloops)
      {
      MEM_WRITE16(adr,0x00ff);    /*Reset to read mode                     */
      return(0xeeee0001);
      }
    sts=MEM_READ16(adr);
    if ((sts&0x0080)==0x0080)
      {
      err = (sts&0x003a);
      MEM_WRITE16(adr,0x00ff);    /*Reset to read mode                     */
      if (!err)
        {
        fnd=MEM_READ16(adr);
        if (fnd==dat) 
          return(0);
        }
       else
        MEM_WRITE16(adr,0x0050);  /*Reset error bits                       */
      return(sts);
      }
    }while(1);
  }


/* The following are for the Intel 28FXXXC3 Flash Chips */
#define FLASH_READ_CMD		0x00FF00FF
#define FLASH_READ_CFG_CMD	0x00900090

#ifdef EB
#define E28F160C3T		0x008988C2
#define E28F320C3T		0x008988C4
#define E28F640C3T		0x008988CC
#else
#define E28F160C3T		0x88C20089
#define E28F320C3T		0x88C40089
#define E28F640C3T		0x88CC0089
#endif

#define MCONTENTS_OF32(addr)      (*((volatile unsigned int *)(addr)))
#define MEM_READ32(addr)         (MCONTENTS_OF32(addr))
#define MEM_WRITE32(addr,val)    (MCONTENTS_OF32(addr) = (val))

/* This function returns back a 0 if failure - no valid flash detected */
int FWBGet_flash_type()
{
	bit32u	flash_id;
  
	/* Figure out if we have the right flash chips */
	MEM_WRITE32(CS0_BASE,FLASH_READ_CMD);
	MEM_WRITE32(CS0_BASE,FLASH_READ_CFG_CMD);
	flash_id = MEM_READ32(CS0_BASE);
	MEM_WRITE32(CS0_BASE,FLASH_READ_CMD);

	switch(flash_id)
	{
		case E28F160C3T: return 0x200000;
		case E28F320C3T: return 0x400000;
		case E28F640C3T: return 0x800000;
		default: return 0;
	}
}
