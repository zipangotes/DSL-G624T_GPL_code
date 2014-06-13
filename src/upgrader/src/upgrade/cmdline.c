#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"

/* ********* Library internal data ********* */
#define	CMDLINE_TRUE			1
#define	CMDLINE_FALSE			0

typedef	enum CMDLINE_ERR
{
	CMDLINE_ERR_OK		= 0,	/* No Error (OK) */
	CMDLINE_ERR_ERROR	= -1,	/* Unspecified error */
	CMDLINE_ERR_INVKEY	= -3,	/* Invalid option key */
	CMDLINE_ERR_MANYARG	= -4,	/* Too many arguments */
	CMDLINE_ERR_FEWARG	= -5,	/* Too few arguments */
	CMDLINE_ERR_ILLOPT	= -6,	/* Option not allowed (illegal option) */
	CMDLINE_ERR_NOMEM	= -7,	/* No memory */
	CMDLINE_ERR_OPTMIS	= -8	/* A mandatory option is missing */
} CMDLINE_ERR;

/* Argument list */
typedef	struct CMDLINE_ARG
{
	int				index;		/* Index of the argument in the command line */
	struct CMDLINE_ARG*	p_next;	/* Next node in the linked list */
} CMDLINE_ARG;

/* Master control block for an option */
typedef struct CMDLINE_ARGS
{
	int				argc;		/* Total count of arguments found */
	int				optc;		/* Total count of options found */
	CMDLINE_ARG*	list;		/* Argument list */
} CMDLINE_ARGS;

/* Master control block for all found arguments */
typedef	struct CMDLINE_DATA
{
	CMDLINE_ARGS	opt_args[26];	/* Array of MCBs for each option ('a' through 'z') */
	CMDLINE_ARGS	glb_args;		/* Global arguments */
	int				parsed;			/* Internal flag to prevent client calls if library is not initialized */
} CMDLINE_DATA;

/* ********* Local Data ********* */
static CMDLINE_CFG cmdline_cfg;
static CMDLINE_DATA cmdline_data;

char*	cmdline_errmsg = "CMDLINE ERROR";

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
void* cmdline_getarg_list(char opt)
{
	int index = (opt - 'a');

	/* Check the validity of the index */
	if((index < 0) || (index > 25))
	{
		/* ERROR: Wrong option */
		return NULL;
	}

	/* Return a pointer to the ARGS control structure */
	return((void*)(&cmdline_data.opt_args[index]));
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
int cmdline_getarg_count(void* list)
{
	CMDLINE_ARGS*	p_args = (CMDLINE_ARGS*)list;

	/* Return number of arguments for this option */
	return(p_args->argc);
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
int cmdline_getopt_count(char opt)
{
	int				index;

	/* Calculate index value */
	index = opt - 'a';
	if(index < 0 || index > 25) return -1;

	/* Return number of arguments for this option */
	return(cmdline_data.opt_args[index].optc);
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
int cmdline_getarg(void* list, int num)
{
	int i;
	CMDLINE_ARGS*	p_args = (CMDLINE_ARGS*)list;
	CMDLINE_ARG*	p_arg;

	/* Search the 'num' argument in the list for this option */
	for(i=0,p_arg=p_args->list; (p_arg!=NULL) && (i<p_args->argc); i++, p_arg=p_arg->p_next)
	{
		/* if num matches i, we found it */
		if(i==num) return(p_arg->index);
	}
	/* We did not find the specified argument or the list was empty */
	return -1;
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
int cmdline_configure(CMDLINE_CFG* p_cfg)
{
	/* reset global data */
	memset(&cmdline_cfg,0,sizeof(cmdline_cfg));
	memset(&cmdline_data,0,sizeof(cmdline_data));

	/* Copy the user's config structure */
	cmdline_cfg = *p_cfg;
	return 0;
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
char* cmdline_error(int err)
{
	/* TODO: implement a table of error messages */
	return(cmdline_errmsg);
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
static void cmdline_print_args(CMDLINE_ARGS* p_arglist, char* argv[])
{
	CMDLINE_ARG*	p_arg;

	printf("   Number of times option was specified: %d\n", p_arglist->optc);
	printf("   Number of Arguments:                  %d\n", p_arglist->argc);

	if(p_arglist->argc > 0)
	{
		printf("   Argument List: ");

		for(p_arg=p_arglist->list; p_arg != NULL; p_arg=p_arg->p_next)
			printf("%s ", argv[p_arg->index]);
	}

	printf("\n");
}

/* ***************************************************************
* Print all found command line options and their arguments
****************************************************************** */
void cmdline_print(char* argv[])
{
	int i;

	/* Check if the command line was parsed */
	if(cmdline_data.parsed != CMDLINE_TRUE)
	{
		printf("The command line has not been parsed yet.\n");
		return;
	}

	/* Print out option arguments */
	for( i = 0; i < 26; i++ )
	{
		/* Check if the option was specified */
		if(cmdline_data.opt_args[i].optc !=0 )
		{
			/* Print out option name and arguments */
			printf("Option: -%c\n", (char)('a'+i));
			cmdline_print_args(&(cmdline_data.opt_args[i]), argv);
		}
	}

	/* Print out global arguments */
	printf("Global arguments:\n");
	cmdline_print_args(&(cmdline_data.glb_args), argv);
}

/* ***************************************************************
* Print configuration
****************************************************************** */
void cmdline_print_cfg(void)
{

}

static void cmdline_argadd(CMDLINE_ARGS* p_arglist, CMDLINE_ARG* p_arg)
{
	CMDLINE_ARG*	p_list;
	CMDLINE_ARG*	p_prev=NULL;

	/* See if we had anything in the list */
	if(p_arglist->argc == 0)
	{
		/* Link the argument in */
		p_arglist->list = p_arg;
	}
	else
	{
		/* Find the tail of the list */
		for(p_list=p_arglist->list; p_list != NULL; p_list=p_list->p_next)
			p_prev = p_list;

		/* Link the argument in */
		p_prev->p_next=p_arg;
	}

	/* Keep track of arg number */
	p_arglist->argc++;
}

/* ***************************************************************
* cmdline_read()
* Read and parse command line arguments
****************************************************************** */
int cmdline_read(int argc, char* argv[])
{
	int i, option=0;

	/* Process every command line argument in argv[] array */
	for( i = 1; i < argc; i++ )
	{
		/* Does the argument start with a dash? */
		if( *argv[i] == '-' )
		{
			/* The argument must be two characters: a dash, and a letter */
			if( strlen(argv[i]) != 2 )
			{
				/* ERROR: option syntax (needs to be a dash and one letter) */
				return(CMDLINE_ERR_ERROR);
			}

			/* Check validity of the option key ('a' through 'z') */
			if( ((*(argv[i] + 1)) < 'a') || ((*(argv[i] + 1)) > 'z') )
			{
				/* ERROR: option sysntax (invalid option key) */
				return(CMDLINE_ERR_INVKEY);
			}

			/* Calculate the option index */
			option = (*(argv[i] + 1)) - 'a';
			if((option < 0) || (option > 25)) return(CMDLINE_ERR_INVKEY);

			/* Check to see if the option is allowed */
			if( cmdline_cfg.opts[option].flags & CMDLINE_OPTFLAG_ALLOW )
			{
				/* Option allowed. */
				cmdline_data.opt_args[option].optc++;
				continue;
			}
			else
			{
				/* ERROR: Option is not allowed */
				return(CMDLINE_ERR_ILLOPT);
			}
		}
		else
		{
			/* Read the arguments for the option */
			CMDLINE_ARG*	p_arg;

			/* Allocate space for the argument node */
			p_arg = (CMDLINE_ARG*)calloc(1,sizeof(CMDLINE_ARG));
			if( p_arg== NULL )
			{
				/* ERROR: Can't allocate memory for the argument index */
				return(CMDLINE_ERR_NOMEM);
			}

			/* Initialize the argument */
			p_arg->index	= i;
			p_arg->p_next	= NULL;

			/* Check if we can add to the list of arguments for this option */
			if( (option < 0)																/* Do we have to add to the global list? */
				|| (cmdline_data.opt_args[option].argc == cmdline_cfg.opts[option].max)		/* Did we reach MAX arguments? */
				)
			{
				/* This option does not require arguments. Keep the argument in the global list. */
				cmdline_argadd(&(cmdline_data.glb_args), p_arg);
				continue;
			}
			else
			{
				/* See if the current count has reached max for this option */
				if( cmdline_data.opt_args[option].argc == cmdline_cfg.opts[option].max )
				{
					/* ERROR: too many arguments for an option */
					return(CMDLINE_ERR_MANYARG);
				}
				else
				{
					/* Link the argument to the arg list of the option */
					cmdline_argadd(&(cmdline_data.opt_args[option]), p_arg);
					continue;
				}
			}
		}
	}

	/* ****** We read the complete command line. See if what we collected matches the configuration ******* */

	/* Check every collected option against its configuration */
	for( i=0; i < 26; i++ )
	{
		/* Check if this option was allowed */
		if(cmdline_cfg.opts[i].flags & CMDLINE_OPTFLAG_ALLOW)
		{
			/* See if it was mandatory */
			if(cmdline_cfg.opts[i].flags & CMDLINE_OPTFLAG_MANDAT)
			{
				/* Check if we really collected this option on the command line. */
				if(cmdline_data.opt_args[i].optc == 0)
				{
					/* ERROR: a missing mandatory option */
					return(CMDLINE_ERR_OPTMIS);
				}
				else
				{
					/* Option was there. Check how many args we got for it. */
					if(cmdline_data.opt_args[i].argc < cmdline_cfg.opts[i].min)
					{
						/* ERROR: too few arguments for an option */
						return(CMDLINE_ERR_FEWARG);
					}
					else
					{
						/* This mandatory option was proper. */
						continue;
					}
				}
			}
			else	/* This is non-mandatory option: */
			{
				/* Check if the option was specified on the command line */
				if(cmdline_data.opt_args[i].optc == 0)
				{
					/* option wasn't specified, go to the next */
					continue;
				}
				else
				{
					/* Option was there. Check how many args we collected for it. */
					if(cmdline_data.opt_args[i].argc < cmdline_cfg.opts[i].min)
					{
						/* ERROR: too few arguments for a non-mandatory option */
						return(CMDLINE_ERR_FEWARG);
					}
					else
					{
						/* This non-mandatory option was proper. */
						continue;
					}
				}
			}
		}
		else	/* Option was not allowed. */
		{
			/* We should not get here as the non-allowed options should have been
			trapped eariler. */
		}
	}

	/* Command line was proper as far as the number of options and their arguments */
	cmdline_data.parsed = CMDLINE_TRUE;
	return(CMDLINE_ERR_OK);
}

