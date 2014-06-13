# ledapp makefile

prefix=$(TI_FILESYSTEM)
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this for extra output and to compile with debugging symbols
#DEBUG=1

# Uncomment this to output messages to syslog, otherwise, messages go to stdout
CFLAGS += -Os

#CROSS_COMPILE=arm-uclibc-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
INSTALL = install

VER := 0.1.1


EXEC1 = ledapp
OBJS1 = ledapp.o

BOOT_PROGRAMS =
DAEMONS =
COMMANDS = ledapp

ifdef SYSLOG
CFLAGS += -DSYSLOG
endif

CFLAGS += -v -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"'

ifdef DEBUG
CFLAGS += -g -DDEBUG
else
CFLAGS += -Os -fomit-frame-pointer
STRIP=-s
endif

all: $(EXEC1)

$(OBJS1) : *.h Makefile
$(EXEC1) : Makefile

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC1): $(OBJS1)
	$(LD) $(LDFLAGS) $(OBJS1) -o $(EXEC1)


install: all

	$(INSTALL) $(STRIP) $(COMMANDS) $(USRBINDIR)

clean:
	-rm -f ledapp *.o core




