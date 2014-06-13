/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Using AIX */
/* #undef AIX */

/* lastlog file location */
/* #undef CONF_LASTLOG_FILE */

/* utmpx file location */
/* #undef CONF_UTMPX_FILE */

/* utmp file location */
/* #undef CONF_UTMP_FILE */

/* wtmpx file location */
/* #undef CONF_WTMPX_FILE */

/* wtmp file location */
/* #undef CONF_WTMP_FILE */

/* Disable use of lastlog() */
#define DISABLE_LASTLOG 

/* Disable use of login() */
#define DISABLE_LOGIN 

/* Disable use of pututline() */
#define DISABLE_PUTUTLINE 

/* Disable use of pututxline() */
#define DISABLE_PUTUTXLINE 

/* Using syslog */
/* #undef DISABLE_SYSLOG */

/* Disable use of utmp */
#define DISABLE_UTMP 

/* Disable use of utmpx */
#define DISABLE_UTMPX 

/* Disable use of wtmp */
#define DISABLE_WTMP 

/* Disable use of wtmpx */
#define DISABLE_WTMPX 

/* Use zlib */
#define DISABLE_ZLIB 

/* Define if you have the `clearenv' function. */
#define HAVE_CLEARENV 1

/* Define if you have the <crypt.h> header file. */
#define HAVE_CRYPT_H 1

/* Define if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define if you have the `endutent' function. */
#define HAVE_ENDUTENT 1

/* Define if you have the `endutxent' function. */
/* #undef HAVE_ENDUTXENT */

/* Define if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define if you have the `getspnam' function. */
#define HAVE_GETSPNAM 1

/* Define if you have the `getusershell' function. */
#define HAVE_GETUSERSHELL 1

/* Define if you have the `getutent' function. */
#define HAVE_GETUTENT 1

/* Define if you have the `getutid' function. */
#define HAVE_GETUTID 1

/* Define if you have the `getutline' function. */
#define HAVE_GETUTLINE 1

/* Define if you have the `getutxent' function. */
/* #undef HAVE_GETUTXENT */

/* Define if you have the `getutxid' function. */
/* #undef HAVE_GETUTXID */

/* Define if you have the `getutxline' function. */
/* #undef HAVE_GETUTXLINE */

/* Define if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the <ioctl.h> header file. */
/* #undef HAVE_IOCTL_H */

/* Define if you have the <lastlog.h> header file. */
#define HAVE_LASTLOG_H 1

/* Define if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define if you have the <libutil.h> header file. */
/* #undef HAVE_LIBUTIL_H */

/* Define if you have the `z' library (-lz). */
/* #undef HAVE_LIBZ */

/* Define if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Have login() */
#define HAVE_LOGIN 

/* Define if you have the `logout' function. */
#define HAVE_LOGOUT 1

/* Define if you have the `logwtmp' function. */
#define HAVE_LOGWTMP 1

/* Define if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define if you have the `openpty' function. */
#define HAVE_OPENPTY 1

/* Define if you have the <paths.h> header file. */
#define HAVE_PATHS_H 1

/* Define if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define if you have the `pututline' function. */
#define HAVE_PUTUTLINE 1

/* Define if you have the `pututxline' function. */
/* #undef HAVE_PUTUTXLINE */

/* Define if you have the `select' function. */
#define HAVE_SELECT 1

/* Define if you have the `setutent' function. */
#define HAVE_SETUTENT 1

/* Define if you have the `setutxent' function. */
/* #undef HAVE_SETUTXENT */

/* Define if you have the <shadow.h> header file. */
#define HAVE_SHADOW_H 1

/* Define if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define if the system has the type `socklen_t'. */
#define HAVE_SOCKLEN_T 1

/* Define if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if you have the `strlcat' function. */
#define HAVE_STRLCAT 1

/* Define if you have the `strlcpy' function. */
#define HAVE_STRLCPY 1

/* Define if the system has the type `struct sockaddr_storage'. */
/* #undef HAVE_STRUCT_SOCKADDR_STORAGE */

/* Define if `ut_addr' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_ADDR */

/* Define if `ut_addr_v6' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_ADDR_V6 */

/* Define if `ut_host' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_HOST */

/* Define if `ut_id' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_ID */

/* Define if `ut_syslen' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_SYSLEN */

/* Define if `ut_time' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_TIME */

/* Define if `ut_tv' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_TV */

/* Define if `ut_type' is member of `struct utmpx'. */
/* #undef HAVE_STRUCT_UTMPX_UT_TYPE */

/* Define if `ut_addr' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_ADDR 1

/* Define if `ut_addr_v6' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_ADDR_V6 1

/* Define if `ut_exit' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_EXIT */

/* Define if `ut_host' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_HOST 1

/* Define if `ut_id' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_ID 1

/* Define if `ut_pid' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_PID 1

/* Define if `ut_time' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_TIME 1

/* Define if `ut_tv' is member of `struct utmp'. */
/* #undef HAVE_STRUCT_UTMP_UT_TV */

/* Define if `ut_type' is member of `struct utmp'. */
#define HAVE_STRUCT_UTMP_UT_TYPE 1

/* Define if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/stropts.h> header file. */
/* #undef HAVE_SYS_STROPTS_H */

/* Define if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define if the system has the type `uint16_t'. */
#define HAVE_UINT16_T 1

/* Define if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if you have the `updwtmp' function. */
#define HAVE_UPDWTMP 1

/* Define if you have the <util.h> header file. */
/* #undef HAVE_UTIL_H */

/* Define if you have the `utmpname' function. */
#define HAVE_UTMPNAME 1

/* Define if you have the `utmpxname' function. */
/* #undef HAVE_UTMPXNAME */

/* Define if you have the <utmpx.h> header file. */
/* #undef HAVE_UTMPX_H */

/* Define if you have the <utmp.h> header file. */
#define HAVE_UTMP_H 1

/* Define if the system has the type `u_int16_t'. */
#define HAVE_U_INT16_T 1

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to the type of arg 1 for `select'. */
#define SELECT_TYPE_ARG1 int

/* Define to the type of args 2, 3 and 4 for `select'. */
#define SELECT_TYPE_ARG234 (fd_set *)

/* Define to the type of arg 5 for `select'. */
#define SELECT_TYPE_ARG5 (struct timeval *)

/* Define if the `setpgrp' function takes no argument. */
#define SETPGRP_VOID 

/* Define if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Use /dev/ptmx */
/* #undef USE_DEV_PTMX */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */
