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

/*******************************************************************************
 * FILE PURPOSE:    Environment variables routines.
 *******************************************************************************
 * FILE NAME:       env.c
 *
 * DESCRIPTION:     Environment variables routines.
 *
 ******************************************************************************/

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#include <psbl/stdio.h>
#include <psbl/env.h>
#include <psbl/flashop.h>
#include <psbl/debug.h>
#include <psbl/errno.h>
#include <psbl/sysdefs.h>
#include <psbl/heapmgr.h>
#include <psbl/build_ops.h>
#include <psbl/string.h>
#include "shell.h"
#else
#include "platform.h"    
#include "env.h"         
#include "flashop.h"     
#include "debug.h"       
#include "errno.h"       
#include "shell.h" 
#include "sysconf.h"
#endif

#define MAX_ENV_ENTRY 			(block_size/FLASH_ENV_ENTRY_SIZE)

#ifndef _STANDALONE
unsigned int strlen(const char *str);
int strcmp(const char *A, const char *B);
char *strcpy(char *DST, const char *SRC);
#endif

typedef enum ENV_VARS {
	env_vars_start = 0,
	CPUFREQ,
	MEMSZ,
	FLASHSZ,
	MODETTY0,
	MODETTY1,
	PROMPT,
	BOOTCFG,
	HWA_0,
#if !defined (AVALANCHE) || defined(TNETC401B)     
	HWA_1,
#endif        
#if !defined(TNETV1020_BOARD)        
	HWA_RNDIS,
#endif    
#if defined (TNETD73XX_BOARD)    
	HWA_3,
#endif    
	IPA,
	IPA_SVR,
	BLINE_MAC0,
#if !defined (AVALANCHE) || defined(TNETC401B)         
	BLINE_MAC1,
#endif    
#if !defined(TNETV1020_BOARD)        
	BLINE_RNDIS,
#endif    
#if defined (TNETD73XX_BOARD)    
	BLINE_ATM,
#endif
#if !defined(TNETV1020_BOARD)            
	USB_PID,
	USB_VID,
	USB_EPPOLLI,
#endif    
  IPA_GATEWAY,
  SUBNET_MASK,
#if defined (TNETV1050_BOARD)    
    BLINE_ESWITCH,
#endif    
#if !defined(TNETV1020_BOARD)            
    USB_SERIAL,
    HWA_HRNDIS,      /* Host (PC) side RNDIS address */
#endif    
    REMOTE_USER,
    REMOTE_PASS,
    REMOTE_DIR,
    SYSFREQ,
    LINK_TIMEOUT,
#ifndef AVALANCHE     /* Avalanche boards use only one mac port */
    MAC_PORT,
#endif    
    PATH,
    HOSTNAME,
#ifdef WLAN
	 HW_REV_MAJOR,
	 HW_REV_MINOR,
	 HW_PATCH,
	 SW_PATCH,
	 SERIAL_NUMBER,
#endif
    TFTPCFG,
#if defined (TNETV1050_BOARD)    
    HWA_ESWITCH,
#endif
    BUILD_OPS,
    TFTP_FO_FNAME,
    TFTP_FO_PORTS,
    CONSOLE_STATE,
    MIPSFREQ,
	/*
	 * Add new env variables here.
	 * NOTE: New environment variables should always be placed at the end, ie 
	 *       just before env_vars_end.
	 */
	
    env_vars_end
} ENV_VARS;

typedef struct ENVDESC {
	ENV_VARS   idx;
	const char      *nm;
    char      *alias;
} ENVDESC;

#define ENVSTR(x)         #x
#define _ENV_ENTRY(x)  {x, ENVSTR(x), 0}
ENVDESC env_ns[] = {
	_ENV_ENTRY(env_vars_start), /* start. */
	_ENV_ENTRY(CPUFREQ),
	_ENV_ENTRY(MEMSZ),
	_ENV_ENTRY(FLASHSZ),
	_ENV_ENTRY(MODETTY0),
	_ENV_ENTRY(MODETTY1),
	_ENV_ENTRY(PROMPT),
	_ENV_ENTRY(BOOTCFG),
	_ENV_ENTRY(HWA_0),
#if !defined (AVALANCHE) || defined(TNETC401B)    
	_ENV_ENTRY(HWA_1),
#endif
#if !defined(TNETV1020_BOARD)        
	_ENV_ENTRY(HWA_RNDIS),
#endif    
#if defined (TNETD73XX_BOARD)    
	_ENV_ENTRY(HWA_3),
#endif    
	_ENV_ENTRY(IPA),
	_ENV_ENTRY(IPA_SVR),
	_ENV_ENTRY(IPA_GATEWAY),
	_ENV_ENTRY(SUBNET_MASK),
	_ENV_ENTRY(BLINE_MAC0),
#if !defined (AVALANCHE) || defined(TNETC401B)    
	_ENV_ENTRY(BLINE_MAC1),
#endif
#if !defined(TNETV1020_BOARD)    
	_ENV_ENTRY(BLINE_RNDIS),
#endif
#if defined (TNETD73XX_BOARD)    
	_ENV_ENTRY(BLINE_ATM),
#endif
#if !defined(TNETV1020_BOARD)            
	_ENV_ENTRY(USB_PID),
	_ENV_ENTRY(USB_VID),
	_ENV_ENTRY(USB_EPPOLLI),
#endif
#if defined (TNETV1050_BOARD)    
	_ENV_ENTRY(BLINE_ESWITCH),
#endif
#if !defined(TNETV1020_BOARD)            
	_ENV_ENTRY(USB_SERIAL),
	_ENV_ENTRY(HWA_HRNDIS),
#endif    
    _ENV_ENTRY(REMOTE_USER),
    _ENV_ENTRY(REMOTE_PASS),
    _ENV_ENTRY(REMOTE_DIR),
    _ENV_ENTRY(SYSFREQ),
    _ENV_ENTRY(LINK_TIMEOUT),
#ifndef AVALANCHE       /* Avalanche boards use only one mac port */
    _ENV_ENTRY(MAC_PORT),
#endif    
    _ENV_ENTRY(PATH),
    _ENV_ENTRY(HOSTNAME),
#ifdef WLAN
    _ENV_ENTRY(HW_REV_MAJOR),
    _ENV_ENTRY(HW_REV_MINOR),
    _ENV_ENTRY(HW_PATCH),
    _ENV_ENTRY(SW_PATCH),
    _ENV_ENTRY(SERIAL_NUMBER),
#endif
    _ENV_ENTRY(TFTPCFG),
#if defined (TNETV1050_BOARD)    
    _ENV_ENTRY(HWA_ESWITCH),    
#endif
    _ENV_ENTRY(BUILD_OPS),
    _ENV_ENTRY(TFTP_FO_FNAME),
    _ENV_ENTRY(TFTP_FO_PORTS),
    _ENV_ENTRY(CONSOLE_STATE),
    _ENV_ENTRY(MIPSFREQ),   
	/*
	 * Add new entries below this.
	 */
    /* Adam2 environment name alias. Temporary. */
    {IPA,     "my_ipaddress"},
    {CPUFREQ, "cpufrequency"},    
    {SYSFREQ, "sysfrequency"}, 
    {HWA_0,   "maca"},
#ifndef AVALANCHE    
    {HWA_1,   "macb"},
#endif
    {MEMSZ,   "memsize"},
    
	_ENV_ENTRY(env_vars_end) /* delimiter. */
};

static unsigned int env_size;
static unsigned int env_base;

/* TODO: remove this */
t_env_var env_vars[10];

static const char envVersion[] = { "TIENV0.8" };  /* string size should be <= sizeof(ENV_VAR) */

static const char envErrReadOnly[] = { "Env: %s is read-only.\n" };

#define ENV_CELL_SIZE           16

/* control field decode */
#define ENV_GARBAGE_BIT			0x01    /* Env is garbage if this bit is off */
#define ENV_DYNAMIC_BIT			0x02    /* Env is dynamic if this bit is off */
#define ENV_PERM_BIT            0x04    /* Env is a backup of a permanent value */

typedef struct ENV_VAR_t {
    unsigned char   varNum;
    unsigned char   ctrl;
    unsigned short  chksum;
    unsigned char   numCells;
    unsigned char   data[ENV_CELL_SIZE - 5];    /* The data section starts 
                                                 * here, continues for
                                                 * numCells.
                                                 */
}ENV_VAR;

#ifdef _STANDALONE

#ifdef PERMANENT_VARIABLES

/* Structure for permanent variables linked list */
typedef struct PERM_ENV_VAR_t 
{
  const char *name;
  const char *value;
  struct PERM_ENV_VAR_t *next;
    
}PERM_ENV_VAR;

/* Head of the permanent variables linked list (RAM copy) */
static PERM_ENV_VAR *perm_env_list_head = NULL;

/* Current index of the permanent variables pointer (in the flash).
   It is being increased for each write to the flash */
static unsigned int perm_cur_index = 0;

#define MAX_PERM_VAR_SIZE     ( 640 )   /* Size of permanent variables space in bytes */

/* Permanent variables space, must be initialized to all 0xFFs */
static const unsigned char perm_env_ptr[ MAX_PERM_VAR_SIZE ] = 
{ 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  
};  

#endif /* PERMANENT_VARIABLES */

#endif

static unsigned int MaxEnvVarsNum;       /* Holds count for the maximum 
                                          * number of environment variable
                                          * that can be set. 
                                          */
static PSBL_REC* psbl_rec =  (PSBL_REC*)0x94000300;

/* Internal functions */
int IsReadOnlyVar( const char* env_nm );
static int IsPreDefinedVar(const char* var); /* Checks for variable in
                                              * pre-defined list. 
                                              * Returns Index if found - else 0.
                                              * Please note:
                                              * ZERO is illegal index.
                                              */
#ifdef _STANDALONE
#define enter_critical_section()
#define exit_critical_section()
#else
void enter_critical_section(void);
void exit_critical_section(void);
#endif

static ENVDESC* GetEnvDescForEnvVar(const char* var);
static int __sys_setenv( const char *env_nm, const char *env_val, unsigned char perm );
#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES
static int __sys_setpermenv( const char *env_nm, const char *env_val);
static int __sys_ispermenv( const char *env_nm );
static const char* __sys_getpermenvval( unsigned  index );
static const char* __sys_getpermenvname( unsigned int index );
static void __sys_initpermenv( void );
int sys_setpermenv(const char *env_nm, const char *env_val);
#endif
#endif

/* Internal macros */
#define IsEnvGarbage(var)       (((var)->ctrl & ENV_GARBAGE_BIT) == 0)
#define IsEnvDynamic(var)       (((var)->ctrl & ENV_DYNAMIC_BIT) == 0)
#define IsEnvPerm(var)          (((var)->ctrl & ENV_PERM_BIT) == 0)
#define EnvGetNextBlock(var)    ((ENV_VAR*)( (char*)(var) + (var)->numCells * ENV_CELL_SIZE))

static int EnvMakeGarbage(ENV_VAR* pVar)
{
    int status;
   
    enter_critical_section();
    
    status = FWBOpen((int)&((pVar)->ctrl));
    FWBWriteByte((int)&((pVar)->ctrl), (pVar)->ctrl & ~ENV_GARBAGE_BIT);
    FWBClose();

    exit_critical_section();

    return (status == 0);
}

static char* GetEnvBaseAndSize(unsigned int* size)
{
	*size = env_size;

#ifdef ENV_SPACE_SIZE
    if(*size > ENV_SPACE_SIZE){
        *size = ENV_SPACE_SIZE;
    }    
#endif
	return( (char *) env_base);	
}

#if ENV_DEBUG
static int GetChecksum(ENV_VAR* var) {
    unsigned short chksum = 0;
    unsigned int tmp, i;

    chksum = var->varNum + var->numCells;
    
    tmp = strlen(var->data);
    if(IsEnvDynamic(var)) {            
        tmp += strlen(var->data + tmp + 1) + 1;
    }

    for(i = 0; i < tmp; i++) {
        chksum += var->data[i]; 
    }
    
    return chksum;
}
#endif

/* returns the variable number if pre-defined, else return 0 */
static int IsPreDefinedVar(const char* var)
{
    ENVDESC* env;
    
    if((env = GetEnvDescForEnvVar(var)) != NULL) return env->idx;
      
    return 0;  
}

static ENVDESC* GetEnvDescForEnvVar(const char* var)
{
    int ii;        
    /* go over the list of pre-defined environment variables */
	for (ii = env_vars_start; env_ns[ii].idx != env_vars_end; ii++){   
        /* check if the env variable is listed */            
		if (strcmp(env_ns[ii].nm, var) == 0) 
        {
            return  &env_ns[ii];
        }

        /* if an alias is present, check if the alias matches
         * the description
         */
        if(env_ns[ii].alias != NULL) {
    		if (strcmp(env_ns[ii].alias, var) == 0)
            {
                return &env_ns[ii];
			}			
        }
    }
    return NULL;
}

static char* GetPreDefinedVarName(int index) {
    int ii;
        
    if ( index >= env_vars_end || index <= env_vars_start) {
        return NULL;
    }
    for(ii = env_vars_start; env_ns[ii].idx != env_vars_end; ii++) {
        if(env_ns[ii].idx == index) {
            if(env_ns[ii].alias != NULL) {
                return env_ns[ii].alias;
            } else {                        
            return ( char *)env_ns[ii].nm;
        }
    }
    }
    return NULL;
}

/* Gives the nth non-garbage environment block. Indexed starting ZERO */
static ENV_VAR* GetEnvBlockByNumber(int index) {
    
    ENV_VAR* pVar;
    int count = 0; 
    unsigned int end_address, size;
    
    pVar = (ENV_VAR*)GetEnvBaseAndSize(&size);
    end_address = ( int ) pVar + size;
    
    /* skip first block */
    pVar++;

    enter_critical_section();
    
    for(;( ( int )pVar < end_address ) && pVar->varNum!=0xFF; pVar = EnvGetNextBlock(pVar)) {
        if(!IsEnvGarbage(pVar)) {
            if(count == index){
                exit_critical_section();                    
                return pVar; 
            }
            else count++;
        }
    }    

    exit_critical_section();
    
    return NULL;
}
/*
 * Gets the name and value from a given environment block. Also checks the 
 * checksum while doing so. If it finds that the checksum is invalid, it 
 * marks the environment as garbage, and returns NULL for the name and value.
 */
static void GetNameAndValueFromEnvVar(ENV_VAR* pVar, char** ppName, 
                                                            char** ppValue)
{
    unsigned short chksum = 0;
    int i;
        
    enter_critical_section();
    chksum += (pVar->varNum + pVar->ctrl + pVar->numCells);        
            
    if(IsEnvDynamic(pVar)) {
        *ppName  = pVar->data;
        *ppValue = pVar->data + strlen(pVar->data) + 1;
    } else {
        *ppName  = GetPreDefinedVarName(pVar->varNum);
        *ppValue = pVar->data;                
    }        

    for(i = 0; i < strlen(*ppValue); i++) {
        chksum += (unsigned char) (*ppValue)[i];
    }

    if(IsEnvDynamic(pVar)) {
        for(i = 0; i < strlen(*ppName); i++) {
            chksum += (unsigned char) (*ppName)[i];
        }
    }    
    
    chksum += pVar->chksum;

    chksum  = ~(chksum);
    
    /* bad checksum */
    if(chksum != 0) {
        *ppName = *ppValue = NULL;            
        EnvMakeGarbage(pVar);
    }                
    
    exit_critical_section();

    return;
}

/* returns the non-garbage block corresponding to the asked var. */
static ENV_VAR* GetEnvBlockByName(const char *var) {
        
    ENV_VAR* pVar;
    int index = IsPreDefinedVar(var);
    int i;
    

    for(i = 0; i < MaxEnvVarsNum; i++) {
            
        if( !(pVar = GetEnvBlockByNumber(i)) ) return NULL;
    

        enter_critical_section();
        
        if(index) { /* Pre-defined environment variables */
            if(pVar->varNum == index) {
                exit_critical_section();
                return pVar;           
            }
        } else {    /* Dynamic environment variables */
            if(!strcmp(var, pVar->data)) {
                exit_critical_section();
                return pVar;
            }
        }        

        exit_critical_section();
    }
    return NULL;
}

static int FormatEnvBlock(void) 
{
    unsigned int size, i;
    unsigned char* pFlash = GetEnvBaseAndSize(&size);
   
#ifdef ENV_SPACE_SIZE
    char *pExtraSpace;
    
    if(!(pExtraSpace = _malloc(env_size - size))) {
        return SBL_EFAILURE;
    }
    
    memset(pExtraSpace, 0xFF, env_size - size);
    memcpy(pExtraSpace, (char*)env_base + ENV_SPACE_SIZE, 
                    env_size - size);    
#endif    
   
    enter_critical_section();

    /* If Erase returns 0 => Flash has gone bad. Return error */
    if(FWBErase((unsigned int)pFlash, size, 0) == 0) {
        exit_critical_section();
        return SBL_EFAILURE;
    }
            
    FWBOpen((int)pFlash);
        
	for (i = 0; i <= strlen(envVersion) ;i++) {
	    FWBWriteByte( (int)(pFlash++), envVersion[i]);
    }

#ifdef ENV_SPACE_SIZE
    pFlash = (char*)env_base + ENV_SPACE_SIZE;
	for (i = 0; i < env_size - size ;i++) {
	    FWBWriteByte( (int)(pFlash++), pExtraSpace[i]);
    }
    _free(pExtraSpace); 
#endif    
    
	FWBClose();	

    exit_critical_section();

    return SBL_SUCCESS;
}

int EnvInit(void)
{
#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES
    unsigned int index = 0;
    char *env_name;
#endif
#endif
    static char after_init = 0;
    
    if( after_init )
      return SBL_SUCCESS;
    else
      after_init = 1;  

    env_size = psbl_rec->env_size;
    env_base = psbl_rec->env_base;

    MaxEnvVarsNum = (env_size)/(ENV_CELL_SIZE) - 1; /* Ignore the header */
    
    if(strcmp(( char *)env_base, envVersion) != 0) 
    {
        FormatEnvBlock();            
    } 
#if 0
    else 
    {
        /* TODO: Visit each environment variable and calculate the checksum. 
         * If incorrect, mark as garbage
         */   
    }
#endif

#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES

    /* Initialize permanent variables section */
    __sys_initpermenv();
    
    /* Hai: Copy variables from permanent to temporary storage */
    while( ( env_name = ( char *) __sys_getpermenvname( index ) ) != NULL )
    {
      if( __sys_setenv( env_name, __sys_getpermenvval( index ), TRUE ) == SBL_ERESCRUNCH )
      {
        sys_printf( "\nUnable to copy variable %s", env_name );
      }
      
      index++;
    }
    
    if( index == 0 )
    {       
      /* No permanent variables were found, check if PSPBoot was upgraded. if so,
         copy the backed up permanent variables back to their place */
      for(index = 0; index < MaxEnvVarsNum; index++) 
      {
        ENV_VAR* pVar;
        char *pName, *pValue;
        
        if( !(pVar = GetEnvBlockByNumber( index )) )
          break;

        GetNameAndValueFromEnvVar(pVar, &pName, &pValue);        

        if( !pName )
          continue;

        if( IsEnvPerm( pVar ) )
        {
          /* This is a permanent variable */
          sys_setpermenv( ( char *)pName, ( char *)pValue );
        }
      }    
    }  
#endif
    if( sys_getenv( "BUILD_OPS" ) == 0 )
    {
      unsigned int flag;
      char buf[ 16 ];
      
      flag =
#ifdef FFS
      BLDOPS_FFS |
#endif

#ifdef INCLUDE_LZMA
      BLDOPS_LZMA |
#endif

#ifdef INCLUDE_7ZIP
      BLDOPS_7ZIP |
#endif

#ifdef INCLUDE_GZIP
      BLDOPS_GZIP |
#endif

#ifdef FTP
      BLDOPS_FTP |
#endif

#ifdef TFTP
      BLDOPS_TFTP |
#endif

#ifdef DUAL_IMAGE_SUPPORT
      BLDOPS_DUAL_IMAGE |
#endif

#ifdef TFTP_FAILOVER_SUPPROT
      BLDOPS_TFTP_FAILOVER |
#endif

#ifdef DHCP_SUPPORT
      BLDOPS_DHCP |
#endif

#ifdef FTP_SERVER_SUPPORT
      BLDOPS_PCAPP |
#endif

#ifdef TIBINARY_SUPPORT
      BLDOPS_TI_BINARY |
#endif

#ifdef ELF_SUPPORT
      BLDOPS_ELF |
#endif
      0;
      
      sys_sprintf( buf, "0x%x", flag );
      
      sys_setenv( "BUILD_OPS", buf );
    }
#endif
    
    return SBL_SUCCESS;        
}

/********************************************************************************/


#ifdef _STANDALONE

#ifdef PERMANENT_VARIABLES
int sys_setpermenv(const char *env_nm, const char *env_val)
{   
    if( ( !env_nm ) || ( !env_val ) )
     return SBL_EFAILURE;

#ifndef _STANDALONE
    if( IsReadOnlyVar( env_nm ) )
    {
	    sys_printf(envErrReadOnly, env_nm);
		return SBL_SUCCESS;       
	}
#endif	
    return __sys_setpermenv(env_nm, env_val);
}

static void __sys_initpermenv( void )
{
  PERM_ENV_VAR *var, *last_var = NULL;
  unsigned char *perm_ptr = (unsigned char *)perm_env_ptr;
  int len;
  
  perm_env_list_head = NULL;
    
  /* Section ends either with NULL or 0xFF (unflashed area) */
  while( ( *perm_ptr != 0xFF ) && ( *perm_ptr != 0 ) && ( perm_cur_index < MAX_PERM_VAR_SIZE ) )
  {   
     /* Allocate memory for new node */
     var = _malloc( sizeof( PERM_ENV_VAR ) );
     
     if ( !var )
     {
        sys_printf( "Permanent variables memory allocation failed!\n" );
        break;
     }

     /* Assign variable name */
     var->name = ( const char *)perm_ptr;

     /* Calculate length of name */
     len = strlen( perm_ptr ) + 1;
     
     /* Validate the length. May be invalid */
     if ( len > MAX_PERM_VAR_SIZE )
     {
        /* This is probably garbage */
        _free( ( void *)var );
        
        /* Stop the flash parsing */
        return;
     }
     
     perm_cur_index += len;
     
     /* Increment pointer to next enrty */
     perm_ptr += len;

     /* Assign variable value */
     var->value = ( const char *)perm_ptr;
     
     /* Calculate length of value */
     len = strlen( perm_ptr ) + 1;

     /* Validate the length. May be invalid */
     if ( len > MAX_PERM_VAR_SIZE )
     {
        /* This is probably garbage */
        _free( ( void *)var );
        
        /* Stop the flash parsing */
        return;
     }

     perm_cur_index += len;

     /* Increment pointer to next enrty */
     perm_ptr += len;
     
     var->next = NULL;
     
     /* In case it is not the first iteration, initialize the next
        field of the previous node */
     if ( last_var )
        last_var->next = var;
     else
        /* Initialize the head of the list */
        perm_env_list_head = var;

     /* Store last variable */
     last_var = var;
  }      
}     

static PERM_ENV_VAR *__sys_retrieveperm( unsigned int index )
{
  PERM_ENV_VAR *var = perm_env_list_head;
  unsigned int i = 0;
  
  if( !perm_env_list_head )
     return NULL;
  
  /* Go through linked list */
  while ( i++ < index )
  {
     if( var->next )
        var = var->next;
     else
        /* No such index */
        return NULL;   
  }

  return (PERM_ENV_VAR *)var;
}

static const char* __sys_getpermenvval( unsigned int index )
{
  PERM_ENV_VAR *var = __sys_retrieveperm( index );
  
  /* If no such index, return NULL */
  if( !var )
     return NULL;
    
  /* Return variable's name */
  return var->value;      
}

static const char* __sys_getpermenvname( unsigned int index )
{
  PERM_ENV_VAR *var = __sys_retrieveperm( index );
  
  /* If no such index, return NULL */
  if( !var )
     return NULL;
    
  /* Return variable's name */
  return var->name;      
}

static int __sys_ispermenv( const char *env_nm )
{
    PERM_ENV_VAR *var = perm_env_list_head;

    if( !perm_env_list_head )
       return FALSE;
   
    /* Go through linked list */
    do
    {
        /* Compare names. If found, it is permanent */
        if ( strcmp( env_nm, var->name ) == 0 )
           return TRUE;
    }
    while ( ( var = var->next ) != NULL );
    
    return FALSE;
}

static int __sys_setpermenv( const char *env_nm, const char *env_val )
{
    PERM_ENV_VAR *new_var;
    unsigned int pVar;
    unsigned int i, len;
    
    /* Check for pre-existance of the variable */
    if( __sys_ispermenv(env_nm) )
    {
      sys_printf("Env: Variable %s already defined\n", env_nm );    
      return SBL_ERESCRUNCH;
    }   

    /* Calculate length of new variable (including NULLs) */
    len = strlen( env_nm ) + strlen( env_val ) + 2;
    
    /* Check if there is enough space */
    if( perm_cur_index + len > MAX_PERM_VAR_SIZE )
    {
      sys_printf("Env: No space for another permanent variable: %s\n", env_nm );       
      return SBL_ERESCRUNCH;
    }  
  
    /* Allocate memory for new node in the linked list */
    new_var = _malloc( sizeof( PERM_ENV_VAR ) );   
    
    if ( !new_var )
    {
        sys_printf("Env: No space for another permanent variable: %s\n", env_nm );       
        return SBL_ERESCRUNCH;
    }  
    
#if ENV_DEBUG
    sys_printf("New PERMANENT Environment to be written at index %d\n", nextslot );
#endif
      
    enter_critical_section();

    /* Prepare a new permanent variable cell */ 
    /* Initialize flash address */
    pVar = ( unsigned int )KSEG1( perm_env_ptr + perm_cur_index );
    
    /* Open flash for writing */
    if( !FWBOpen( pVar ) )
    {
#if ENV_DEBUG
      sys_printf("Cannot open flash!\n" );
#endif
      _free( (void *)new_var );
      return SBL_EFAILURE;       
    }

    /* initialize name field */
    new_var->name = ( const char *) pVar;
    
    /* Write the permanent variable name to the flash (including NULL) */
    for(i = 0; i <= strlen( env_nm ); i++, perm_cur_index++, pVar++ )
    {
      if( !FWBWriteByte( pVar, env_nm[ i ]) )
      {
#if ENV_DEBUG
         sys_printf("Cannot write byte %d of variable %s!\n", i, env_nm );
#endif        
         FWBClose();
         _free( (void *)new_var );
         return SBL_EFAILURE;
      }
    }

    /* initialize value field */
    new_var->value = ( const char *) KSEG1( perm_env_ptr + perm_cur_index );
    
    /* Write the permanent variable value to the flash (including NULL) */
    for(i = 0; i <= strlen( env_val ); i++, perm_cur_index++, pVar++ )
    {
      if( !FWBWriteByte( pVar, env_val[ i ]) )
      {
#if ENV_DEBUG
         sys_printf("Cannot write byte %d of variable %s!\n", i + strlen( env_nm ), env_nm );
#endif        
         FWBClose();
         _free( (void *)new_var );
         return SBL_EFAILURE;
      }
    }
    
    /* Close flash */
    if( !FWBClose() )
    {
#if ENV_DEBUG
      sys_printf("Cannot close flash!\n" );
#endif
      _free( (void *)new_var );
      return SBL_EFAILURE;       
    }
    exit_critical_section();

    /* Add the node to the linked list */
    if ( !perm_env_list_head )
    {
        new_var->next = NULL;
        
        /* This is the new head */
        perm_env_list_head = new_var;
    }
    else
    {
        /* This is the next node on the list */
        new_var->next = ( PERM_ENV_VAR *)perm_env_list_head;
        perm_env_list_head = new_var;
    }
   
    /* Backup the new variable */
    return __sys_setenv( env_nm, env_val, TRUE );
}
#endif
#endif
int sys_setenv(const char *env_nm, const char *env_val) 
{   
    if( ( !env_nm ) || ( !env_val ) )
     return SBL_EFAILURE;

    /*
     * CPUFREQ and SYSFREQ should not be modified 
     */
#ifndef _STANDALONE
    if( IsReadOnlyVar( env_nm ) )
    {
	    sys_printf(envErrReadOnly, env_nm);
		return SBL_SUCCESS;       
	}
#endif
    return __sys_setenv(env_nm, env_val, FALSE);
}
        
/* Returns:
 * SBL_SUCCESS: successfully configured.
 * SBL_ERESCRUNCH: env block is exhausted, env set failed.
 */
static int __sys_setenv( const char *env_nm, const char *env_val, unsigned char perm )
{
    ENV_VAR *pVar, new, *pBase, *pOldEnv = NULL;
    unsigned int size, i, newsize, end_address;
    char *pTmp;
#if (AUTO_DEFRAG_ENVIRONMENT == TRUE)
    int IsGarbagePresent = FALSE;
#endif
#if ENV_DEBUG
    sys_printf( "Setting var %s to %s, perm = %d\n", env_nm, env_val, perm );
#endif    
    /* Check for pre-existance of the variable */
    if((pTmp = sys_getenv( (char *)env_nm))) 
    {
        /* Env Exists. See if the value to be set is same as old one */
        if(!strcmp(pTmp, env_val)) {
            return SBL_SUCCESS;
        }
        /* Env Exists but is a different value. Make old one garbage */ 
        pOldEnv = GetEnvBlockByName( (char *)env_nm);
#if (AUTO_DEFRAG_ENVIRONMENT == TRUE)
        IsGarbagePresent = TRUE;
#endif

        if( IsEnvPerm( pOldEnv ) )
        {
          sys_printf(envErrReadOnly, env_nm);
	      return SBL_SUCCESS;       
        }  
    }

    pBase = pVar = (ENV_VAR*)GetEnvBaseAndSize(&size);
    end_address = ( unsigned int ) pVar + size;

    /* skip first block */
    pVar++;    
   
    enter_critical_section();
    
    /* Go to the end of Available Flash space */
    for( ;( ( int )pVar < end_address ) && pVar->varNum!=0xFF; pVar = EnvGetNextBlock(pVar)) {
#if (AUTO_DEFRAG_ENVIRONMENT == TRUE)
        if(IsEnvGarbage(pVar)) {
            IsGarbagePresent = TRUE;
        }
#endif        
    }

    exit_critical_section();
    
#if ENV_DEBUG
    sys_printf("New Environment to be written at %x", pVar);
#endif
    
    memset((char*)&new, 0xFF, sizeof(new));
    new.chksum = 0;                
       
    if(!(new.varNum = IsPreDefinedVar(env_nm))) {       
        /* Dynamic variable */
        new.ctrl &= ~(ENV_DYNAMIC_BIT);
    }
    
#ifdef PERMANENT_VARIABLES
    if( perm )
      new.ctrl &= ~( ENV_PERM_BIT );
#endif
    
    new.chksum += (new.varNum + new.ctrl);
   
    newsize = sizeof(ENV_VAR) - sizeof(new.data) + strlen(env_val) + 1;
   
    enter_critical_section();
    
    if(IsEnvDynamic(&new)) {
        newsize  += strlen(env_nm) + 1;
        for(i=0; i < strlen(env_nm); i++) {
            new.chksum += (unsigned char) env_nm[i];
        }
    }

    exit_critical_section();
    
    new.numCells = ((newsize)/ENV_CELL_SIZE)+((newsize%ENV_CELL_SIZE)?1:0);
    new.chksum += new.numCells;
    
    for(i = 0; i <= strlen(env_val); i++) {
        new.chksum += (unsigned char) env_val[i];            
    }            
  
    new.chksum = ~(new.chksum);
    
    /* Check if enough space is available to store the env variable */
    if(((char*)pVar + (new.numCells*ENV_CELL_SIZE)) > ((char*)pBase + size)) {
#if (AUTO_DEFRAG_ENVIRONMENT == TRUE)
        if(IsGarbagePresent){
            if (sys_defragenv() == SBL_SUCCESS) {
                return __sys_setenv(env_nm, env_val, perm );
            }
        }
#endif        
        sys_printf("Error: Out of Environment space\n");
        return SBL_ERESCRUNCH;
    }
    
#if ENV_DEBUG   
    sys_printf("newsize = %d\n", newsize);
    sys_printf("new.numCells = %d\n", new.numCells);
#endif
   
    enter_critical_section();
    
    /* Write to flash */
    FWBOpen((int)pVar);
    
    for(i = 0; i < sizeof(ENV_VAR) - sizeof(new.data); i++) {
        FWBWriteByte(((int)pVar)++, ((char*)&new)[i]);
    }

    if(IsEnvDynamic(&new)) {
        for(i = 0; i <= strlen(env_nm); i++) {
            FWBWriteByte(((int)pVar)++, env_nm[i]);
        }
    }

    for(i = 0; i <= strlen(env_val); i++) {
        FWBWriteByte(((int)pVar)++, env_val[i]);
    }        
    
    FWBClose();
    
    exit_critical_section();
    
    if(pOldEnv) {
        EnvMakeGarbage(pOldEnv);
    }

    
    return SBL_SUCCESS;
}

int sys_unsetenv(const char *env_nm)
{
    ENV_VAR* pVar;        

    if( IsReadOnlyVar( env_nm ) )
      goto fail;
          
    if( !(pVar = GetEnvBlockByName(env_nm)) ) 
      return SBL_EFAILURE;
   
#ifdef PERMANENT_VARIABLES
    if( !IsEnvPerm( pVar ) )
#endif
    {   
      EnvMakeGarbage(pVar); 
      return SBL_SUCCESS;  
    }  

fail:    
    sys_printf(envErrReadOnly, env_nm);	
    return SBL_EFAILURE;       
 
}

void echo(int argc, char **argv)
{
    int ii;
    if (argc == 1) 
    {
        sh_printenv();
    } 
    else 
    {
        if (strcmp(argv[1], "envlist") == 0) 
        {
            sys_printf("Pre-defined Variable list:\n\n");                
            
            for ( ii = env_vars_start + 1; ii < env_vars_end; ii++) 
            {
                sys_printf("%-13s\n", GetPreDefinedVarName(ii));
            }
        }
        else 
            /* user gave some unsupported echo request */
            sh_printenv();
    }
}

#ifndef _STANDALONE
int FWBGet_flash_type(void);
int sys_initenv(void)
{
    static char after_init = 0;
    
    if( after_init )
      return TRUE;
    else
      after_init = 1;  

    env_size = psbl_rec->env_size;
    env_base = psbl_rec->env_base;

#if (defined(TNETV1050SDB) || defined(TNETV1050RDB))
    FWBGet_flash_type();
#endif
    
    MaxEnvVarsNum = (env_size)/(ENV_CELL_SIZE) - 1; /* Ignore the header */
    return TRUE;
}
#endif

void sh_printenv(void)
{
    ENV_VAR* pVar;
    int i;
    char *pName, *pValue;
    
        
    for(i = 0; i < MaxEnvVarsNum; i++) {

        if( !(pVar = GetEnvBlockByNumber(i)) ) goto out;
#if ENV_DEBUG
        sys_printf("%s %x\n", "sh_printenv", pVar);
#endif
        GetNameAndValueFromEnvVar(pVar, &pName, &pValue);
        if(pName == NULL) continue;
        sys_printf("\n%-13s\t%s", pName, pValue);
    }
out:    
    sys_printf("\n");
    return;        
}

char* sys_getenv(const char *var)
{
    ENV_VAR* pVar;
    char *pName, *pValue;

    if( !(pVar = GetEnvBlockByName(var)) ) return NULL;
#if ENV_DEBUG
    sys_printf("%s %x\n", "sys_getenv", pVar);
#endif
    
    GetNameAndValueFromEnvVar(pVar, &pName, &pValue);
    
    return pValue;
}

int sys_defragenv(void)
{
    char **ppRamStore = NULL;
    char *pName, *pValue;
#ifdef PERMANENT_VARIABLES
    char *IsPerm = NULL;
#endif
    unsigned int i;
    ENV_VAR* pVar;
    int defragFail=FALSE;

    if( !(ppRamStore = (char**)_malloc(sizeof(char*)*MaxEnvVarsNum)) ) {
        defragFail = TRUE;                
        goto defragerror;            
    }
#ifdef PERMANENT_VARIABLES
    if( !(IsPerm = (char*)_malloc( MaxEnvVarsNum )) ) {
        defragFail = TRUE;                
        goto defragerror;            
    }
#endif

    memset ((char*)ppRamStore, 0, sizeof(char*)*MaxEnvVarsNum);
#ifdef PERMANENT_VARIABLES
    memset (IsPerm, 0, MaxEnvVarsNum );
#endif
    for(i = 0; i < MaxEnvVarsNum; i++) {
            
        if( !(pVar = GetEnvBlockByNumber(i)) ) break;  

        GetNameAndValueFromEnvVar(pVar, &pName, &pValue);        
        
        if(pName == NULL) continue;
        
        if( !(ppRamStore[i] = _malloc(strlen(pName) + strlen(pValue) + 2)) ) {
            defragFail = TRUE;                
            goto defragerror;            
        }

        /* store name and value in RAM */
        memcpy((char*)ppRamStore[i], pName, strlen(pName) + 1);
        memcpy((char*)ppRamStore[i] + strlen(pName) + 1, 
                         pValue,strlen(pValue) + 1);
#ifdef PERMANENT_VARIABLES
        IsPerm[ i ] = IsEnvPerm( pVar );                 
#endif
    }
    
    if(FormatEnvBlock() != SBL_SUCCESS) {
        defragFail = TRUE;
    }
    
defragerror:
    if(ppRamStore) {
        for(i = 0; i < MaxEnvVarsNum; i++) {
            if(ppRamStore[i]) {
#ifdef PERMANENT_VARIABLES
                __sys_setenv(ppRamStore[i], 
                                ppRamStore[i]+strlen(ppRamStore[i]) + 1, IsPerm[ i ] );
#else
                __sys_setenv(ppRamStore[i], 
                                ppRamStore[i]+strlen(ppRamStore[i]) + 1, 0 );
#endif
                _free(ppRamStore[i]);
            }
        }
        _free(ppRamStore);
    }

#ifdef PERMANENT_VARIABLES
    if( IsPerm )
      _free(IsPerm);
#endif
    
    if(defragFail) {
        sys_printf("Out of memory. Defragment aborted.\n");
        return SBL_ERESCRUNCH;
    } 

    return SBL_SUCCESS;
}

int get_envstring(int index, char *buffer)
{        
    ENV_VAR* pVar;

    if( !(pVar = GetEnvBlockByNumber(index)) ) return 0;
            
#ifndef _STANDALONE /* OS context */
    return sprintf(buffer, "%-13s\t%s\n", 
                    sys_getivar(index), sys_getienv(index));
#else
    return sys_sprintf(buffer, "%-20s  %s\r\n", 
                    sys_getivar(index), sys_getienv(index));
#endif
}

char* sys_getienv(int var_num)
{
    ENV_VAR* pVar;
    char* pName, *pValue;
        
    if( !(pVar = GetEnvBlockByNumber(var_num)) ) return 0;
    
    GetNameAndValueFromEnvVar(pVar, &pName, &pValue);

    return pValue;
}

char* sys_getivar (int var_num) {
    ENV_VAR* pVar;
    char* pName, *pValue;
        
    if( !(pVar = GetEnvBlockByNumber(var_num)) ) return 0;
    
    GetNameAndValueFromEnvVar(pVar, &pName, &pValue);

    return pName;
}

int EnvAddAlias(const char* orig, char* alias)
{
    ENVDESC* env;
            
    if((env = GetEnvDescForEnvVar(orig)) != NULL)
    {
        env->alias = alias;
        return SBL_SUCCESS;
    }
    return SBL_EFAILURE;
}

char* GetResolvedEnvName(const char *envName)
{
    ENVDESC* env;

    if((env = GetEnvDescForEnvVar(envName)) != NULL)
    {
        if(env->alias != NULL) {
            return env->alias;
        } else {
            return ( char *)env->nm;
        }
    }
    return NULL;
}

int IsReadOnlyVar( const char* env_nm )
{
  if( (strcmp("CPUFREQ", env_nm) == 0) || 
      ( strcmp("SYSFREQ", env_nm) == 0 ) || (strcmp("BUILD_OPS", env_nm) == 0) 
      || (strcmp("MIPSFREQ", env_nm) == 0) )
    return 1;
    
  return 0;
}
