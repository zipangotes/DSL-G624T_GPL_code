/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains various support functions
 *
 *  HISTORY:
 *   Date    Modifier      Ver    Notes
 *   01Oct03 Denis         1.00   Original Version
 *****************************************************************************/

#include "cpswhalcommon_support.h"

size_t cpswHalCommonstrlen(const char *s)
  {
  int l = 0;

  while (*s++) l++;
  return(l);
  }

char *cpswHalCommonstrcpy(char *s1, const char *s2)
  {
  char *s = s1;

  while((*s1++=*s2++));
  return(s);
  }

void *cpswHalCommonmemset(void *s, int c, size_t n)
  {
  unsigned char *ucp=s,uc=c;

  while(n--)
    *ucp++=uc;
  return(s);
  }

unsigned long cpswHalCommonstrtoul(const char *str, char **endptr, int base)
  {
  char *s;
  int neg,c,error,any;
  unsigned int acc;

  s=(char *)str;
  while((*s)&&((*s==' ')||(*s=='\t'))) s++;  /*Skip white space              */
  if (*s=='-')
    {neg=1; s++;}
   else
    {neg=0; if (*s=='+') s++;}
  if (((base==0)||(base==16))&&(*s=='0')&&((s[1]&0x5f)=='X'))
    {base=16; s+=2;}
  if (base==0)
    base=(*s=='0')?8:10;
  c=*s;
  error=0;
  acc=0;
  any=0;

  while((*s)&&(error==0))
    {
    c=*s;
    if (c>'9')
      {
      c&=0x5f;
      if ((c<'A')||(c>'F'))
        error++;
      }
     else
      if (c<'0') error++;
    c-='0';
    if (c>9) c-=7;
    if ((c<base)&&(error==0))
      {acc*=base; acc+=c; any++;}
     else
      error++;
    s++;
    }
  s--;
  if (neg) acc = -acc;
  if (any)
    {
    if (endptr) *endptr=s;
    return(acc);
    }
   else
    {
    if (endptr) *endptr=(char *)str;
    return(0);
    }
  }

void cpswHalCommonGpioFunctional(int base, int bit)
  {
  bit32u GpioEnr = base + 0xC;
  /*  To make functional, set to zero  */
  *(volatile bit32u *)(GpioEnr) &= ~(1 << bit);
  }

/*
 *
 *     R A N D O M  N U M B E R  S U P P O R T
 *
 *
 */
#define COMMON_RANDOM_MAX        0xFFFF
static unsigned long RandomSeed = 1;

int cpswHalCommonRandomRange(int min, int max)
  {
  int iTmp;

  iTmp =  cpswHalCommonRandom();
  iTmp %= ((max-min)+1);
  iTmp += min;
  return(iTmp);
  }

int cpswHalCommonRandom(void)
  {
  RandomSeed = RandomSeed * 1103515245 + 12345;
  return ((bit32u) (RandomSeed/65536) % (COMMON_RANDOM_MAX + 1));
  }

void cpswHalCommonSRandom(unsigned seed)
  {
  RandomSeed = seed;
  }


#ifdef __CPHAL_MEMTEST
static bit32u testpat[]={0x55555555,0xaaaaaaaa,0x33333333,0xcccccccc,
                         0x0f0f0f0f,0xf0f0f0f0,0x00ff00ff,0xff00ff00,
                         0x0000ffff,0xffff0000,0x00000000,0xffffffff,
                         0x00000000};

bit32u cpswHalCommonMemBasicDataTest(bit32u base)
  {
  bit32u *sp,exp,fnd,i,error;
  volatile bit32u *dp,*dp2;

  base&=0x1fffffff;
  base|=0xa0000000;
  base&=~3;
  dp=(volatile bit32u *)(base);
  dp2=(volatile bit32u *)(base+4);
  error=0;
  sp=testpat;
  for(i=0;i<(sizeof(testpat)/4);i++,sp++)
    {
    exp=*sp;
    *dp=exp;
    *dp2=i;
    fnd=*dp;
    error|=(exp^fnd);
    }
  return(error);
  }

bit32u cpswHalCommonMemAddressTest(bit32u base,bit32u size)
  {
  bit32u error,i,j,exp,fnd;
  volatile bit32u *dp;

  base&=~3;
  size&=~3;
  if (size<(16*1024))
    {
    base&=0x1fffffff;
    base|=0xa0000000;
    }
  error=0;
  for(i=0,j=base,dp=(volatile bit32u *)base;i<size;i+=4,j+=4,dp++)
    *dp=j;
  for(i=0,j=base,dp=(volatile bit32u *)base;i<size;i+=4,j+=4,dp++)
    {
    fnd=*dp;
    exp=j;
    error|=exp^fnd;
    }
  return(error);
  }

bit32u cpswHalCommonMemDataTest(bit32u base,bit32u size,int verbose)
  {
  bit32u error,exp,fnd,i,j,ci,*sp;
  volatile bit32u *dp;

  base&=~3;
  size&=~3;
  if (size<(16*1024))
    {
    base&=0x1fffffff;
    base|=0xa0000000;
    }
  error=0;
  for(j=0;(j<(sizeof(testpat)/4))&&(error==0)&&((j<2)||(verbose));j++)
    {
    for(i=0,ci=j,sp=&testpat[j],dp=(volatile bit32u *)base;i<size;i+=4,ci++)
      {
      if (ci>=(sizeof(testpat)/4))
        { ci=0; sp=testpat; }
      *dp++=*sp++;
      }
    for(i=0,ci=j,sp=&testpat[j],dp=(volatile bit32u *)base;i<size;i+=4,ci++)
      {
      if (ci>=(sizeof(testpat)/4))
        {ci=0;sp=testpat;}
      exp=*sp++;
      fnd=*dp++;
      error|=exp^fnd;
      }
    }
  return(error);
  }
#endif

#ifdef __CPHAL_FUTURE_USE
/* The following functions were used in previous HAL versions.  After the switch from
   strings to enums to represent keys, these functions were no longer used. */

unsigned long cpswHalCommonatoul(const char *str)
  {
  return(cpswHalCommonstrtoul (str, (char **) 0, 0));
  }

int cpswHalCommontolower(int c)
{
    if ((c >= 'a') && (c <= 'z'))
      {
       return c + 0x20;
      }
     else
       return c;
}

int cpswHalCommonstrcmpi(const char *s1, const char *s2)
  {
   for (; (cpswHalCommontolower(*s1) == cpswHalCommontolower(*s2)); s1++, s2++)
     if (*s1 == '\0')
        return(0);
   return(*s1 - *s2);
  }

char *cpswHalCommonstrstr(
    const char  *s1,
    const char  *s2)
{
    const char *s ;
    const char *t ;

    if (!s1 || !s2)
    return (0);

    if (!*s2)
    return ((char*)s1);

    for (; *s1; s1++)
    {
    if (*s1 == *s2)
        {
        t = s1;
        s = s2;
        for (; *t; s++, t++)
            {
        if (*t != *s)
            break;
        }
        if (!*s)
        return ((char*)s1);
    }
    }
    return (0);
}

char *cpswHalCommonstrchr(const char *s, int    c)
  {
  for (;; ++s)
    {
    if (*s == c)
      return((char *) s);
    if (!*s)
      return((char *) 0);
    }
  }

int cpswHalCommonstrcmp(const char  *s1,const char  *s2)
  {
  while (*s1 || *s2)
    if (*s1++ != *s2++)
      return(*--s1 - *--s2);
  return 0;
  }

void cpswHalCommoncatdnum(char *s,int num)
  {
  if (num>9)
    cpswHalCommoncatdnum(s,num/10);
  while(*s) s++;
  num=num%10;
  *s++=num+'0';
  *s=0;
  }

char *cpswHalCommonstrcat(char *s1, const char *s2)
  {
  register char *s = s1;

  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return s;
  }

#endif