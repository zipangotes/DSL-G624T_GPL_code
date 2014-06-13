/*+----------------------------------------------------------------------------------+**
**|                            ****                                                  |**
**|                            ****                                                  |**
**|                            ******o***                                            |**
**|                      ********_///_****                                           |**
**|                      ***** /_//_/ ****                                           |**
**|                       ** ** (__/ ****                                            |**
**|                           *********                                              |**
**|                            ****                                                  |**
**|                            ***                                                   |**
**|                                                                                  |**
**|         Copyright (c) 1998-2004 Texas Instruments Incorporated                   |**
**|                        ALL RIGHTS RESERVED                                       |**
**|                                                                                  |**
**| Permission is hereby granted to licensees of Texas Instruments                   |**
**| Incorporated (TI) products to use this computer program for the sole             |**
**| purpose of implementing a licensee product based on TI products.                 |**
**| No other rights to reproduce, use, or disseminate this computer                  |**
**| program, whether in part or in whole, are granted.                               |**
**|                                                                                  |**
**| TI makes no representation or warranties with respect to the                     |**
**| performance of this computer program, and specifically disclaims                 |**
**| any responsibility for any damages, special or consequential,                    |**
**| connected with the use of this program.                                          |**
**|                                                                                  |**
**+----------------------------------------------------------------------------------+*/
#include "nsp_upg.h"
#include "cmdline.h"

/* **************************************************************************
	Command line configuration.
***************************************************************************** */
CMDLINE_CFG	cmd_line_cfg =
{
	{
		/*	MIN		MAX		FLAGS								OPTION	*/
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-a' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-b' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-c' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-d' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-e' */
		{	1,		1,	(CMDLINE_OPTFLAG_ALLOW | CMDLINE_OPTFLAG_MANDAT) },	/* '-f' <filename> */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-g' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-h' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-i  */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-j' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-k' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-l' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-m' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-n' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-o  */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-p' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-q' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-r' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-s' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-t' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-u' */
		{	0,		0,	CMDLINE_OPTFLAG_ALLOW },			/* '-v' VERBOSE/NON-VERBOSE */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-w' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-x' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* '-y' */
		{	0,		0,	!CMDLINE_OPTFLAG_ALLOW }			/* '-z' */
	},
	{	0,		0,	!CMDLINE_OPTFLAG_ALLOW },			/* global arguments */
};

/* **************************************************************************
	Help Screen
***************************************************************************** */
static void print_help(void)
{
	static char* help[]=
	{
		"upgrade 1.0, Texas Instruments, 2004",
		"Command line syntax:",
		"        upgrade -f <filename>",
		"Example:",
		"        upgrade -f filename",
		"NOTE: the <filename> file must be located in /var directory."
	};

	int i,n;
	n=sizeof(help)/sizeof(char*);
	for(i=0;i<n;i++){printf("%s\n",help[i]);}
}

/* **************************************************************************
	int nsp_upg_printf_close(void)

	This application starts the NSP upgrade procedure. When called with no
	arguments, the application prints the command line syntax and exits.

	Command line syntax:
		upgrade -f <filename>

	Arguments:
		argc		- number of arguments
		argv		- array of arguments
		envp		- array of environment variables
	Returns:
		NSP_UPG_ERR_OK		- success
		NSP_UPG_ERR_ERROR	- failure
	Errors:

***************************************************************************** */
int main(int argc,char* argv[],char* envp[])
{
 int r;char *err,*name;

 /* deal with the command line */
 cmdline_configure(&cmd_line_cfg);r=cmdline_read(argc, argv);
 if(r!=0){err=cmdline_error(r);printf("%s\n",err);print_help();return(NSP_UPG_ERR_ERROR);}
 name=argv[cmdline_getarg(cmdline_getarg_list('f'),0)];

 /* initialize the upgrader application */
 r=nsp_upg_cmd_init();NSP_UPG_ERR_RETURN(r,-1);

 /* post to the queue what we want to do */
 r=nsp_upg_cmd_flash(name);NSP_UPG_ERR_RETURN(r,-1);
 r=nsp_upg_cmd_setmtds(name,NSP_UPG_MTD_SBLOCK);NSP_UPG_ERR_RETURN(r,-1);
 r=nsp_upg_cmd_reboot();NSP_UPG_ERR_RETURN(r,-1);

 /* start the upgrade procedure */
 r=nsp_upg_cmd_start();NSP_UPG_ERR_RETURN(r,-1);
 return(NSP_UPG_ERR_OK);
}




