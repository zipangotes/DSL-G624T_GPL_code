# ledcfg makefile

prefix=$(TI_FILESYSTEM)
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this to output messages to syslog, otherwise, messages go to stdout
CFLAGS += -Os

#CROSS_COMPILE=arm-uclibc-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
INSTALL = install

VER := 0.1.1

EXEC1 = ledcfg
OBJS1 = ledcfg.o

BOOT_PROGRAMS =
DAEMONS =
COMMANDS = ledcfg

ifdef SYSLOG
CFLAGS += -DSYSLOG
endif

CFLAGS += -v -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"'

all: ledcfg

ledcfg: src/ledcfg.c

ledcfg.o:
	$(CC) -c $(CFLAGS) src/ledcfg.c -o ledcfg.o

$(EXEC1): $(OBJS1)
	$(LD) $(LDFLAGS) $(OBJS1) -o $(EXEC1)


install: all

	$(INSTALL) $(STRIP) $(COMMANDS) $(USRBINDIR)

clean:
	-rm -f ledcfg *.o core




