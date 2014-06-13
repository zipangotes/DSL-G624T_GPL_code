#ifndef ___CMDLINE_H___
#define	___CMDLINE_H___

/* ********* Library Configuration ********* */
typedef	struct CMDLINE_OPT
{
	int	min;					/* Minimum number of arguments this option takes */
	int	max;					/* Maximum number of arguments this option takes */
	int	flags;					/* Controlling flags (whether to accept or not, etc) */
} CMDLINE_OPT;

typedef	struct CMDLINE_CFG
{
	CMDLINE_OPT	opts[26];		/* Options 'a' through 'z' */
	CMDLINE_OPT	global;			/* Global option (outside 'a'..'z') */
} CMDLINE_CFG;
/* ******************************************** */

#define	CMDLINE_OPTFLAG_ALLOW	0x1			/* The option is allowed */
#define	CMDLINE_OPTFLAG_MANDAT	0x2			/* The option is mandatory */

extern	void	cmdline_print(char* argv[]);

extern	int		cmdline_configure(CMDLINE_CFG* p_cfg);
extern	int		cmdline_read(int argc, char* argv[]);

extern	void*	cmdline_getarg_list(char opt);
extern	int		cmdline_getarg_count(void* list);
extern	int		cmdline_getopt_count(char opt);
extern	int		cmdline_getarg(void* list, int num);

extern	char*	cmdline_error(int err);
#endif


