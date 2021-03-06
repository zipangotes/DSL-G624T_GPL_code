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
 * FILE PURPOSE:    Header for shell.c
 *******************************************************************************
 * FILE NAME:       shell.h
 *
 * DESCRIPTION:     Header for shell.c
 *
 ******************************************************************************/

#ifndef	_SHELL_H_
#define	_SHELL_H_

void fix_vector_for_linux(void);
void AppCopyVectors(void);
void init_env(void);
void ShellAddPathToFileName(char *OutBuf,char *InBuf,char *Path);
unsigned int dm(int argc,char *argv[]);
int BadAddress(unsigned int adr);
int boot(void);
int fmt_extrn_dev_str(void);

#ifdef _STANDALONE
#include <psbl/stddef.h>
#include <psbl/stdio.h>
#else
#include "stddef.h"
#endif

#define SH_CMD_LEN      150
#define SH_ARGC_MAX     10
#define SH_PROMPT_SZ    24
#define PATH_COUNT_MAX  3
#define PRIV_ROOT  TRUE
#define PRIV_USER  FALSE

typedef struct SH_PATH {
	char     nm[50];
	char     sz;
} SH_PATH;

typedef struct sh_CB {
	char   cmd[SH_CMD_LEN];
	int    argc;
	char  *argv[SH_ARGC_MAX];
	char   path_count;
	SH_PATH  *path;
	char   prompt[SH_PROMPT_SZ + 1];
	unsigned char   priv_state;
} sh_CB;

typedef struct PSBL_REC_t {
    unsigned int psbl_size;
    unsigned int env_base;
    unsigned int env_size;
    unsigned int ffs_base;
    unsigned int ffs_size;
    struct sh_CB sh_cb;
    unsigned int ffs2_base;
    unsigned int ffs2_size;
}PSBL_REC;

typedef struct sh_cmd {
	const char      *name;
	FUNC_PTR        app_entry_pt;
	unsigned char   privilige;
	const char      *help;
} sh_cmd;

typedef enum fs_dev {
	e_Flash = 0,
	e_Sio,
} fs_dev;

typedef struct memwindow {
	unsigned int  base;
	unsigned int    end;
} memwindow;

void shell(void);
void sh_init(void);

#endif /* _SHELL_H_ */
