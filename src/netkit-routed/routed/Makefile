all: routed

include ../MCONFIG
include ../MRULES

OBJS=	af.o if.o input.o main.o output.o startup.o \
	tables.o timer.o trace.o inet.o

routed: $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

install: all
	install -s -m$(DAEMONMODE) routed $(INSTALLROOT)$(SBINDIR)
#	install -m$(MANMODE) routed.8 $(INSTALLROOT)$(MANDIR)/man8

clean:
	rm -f *.o routed

$(OBJS): defs.h trace.h interface.h table.h af.h pathnames.h ../version.h
