PROGS     = utelnetd

prefix	= ${TI_FILESYSTEM}
INSTDIR   = $(prefix)/sbin/
INSTMODE  = 0755
INSTOWNER = root
INSTGROUP = root

OBJS = utelnetd.o

all: $(PROGS)

CFLAGS   += -DSHELLPATH=\"$(run_prefix)/bin/login\"

ifneq ("","$(DEBUG)")
CFLAGS   += -DDEBUG
endif

$(PROGS): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: install
install: $(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

.PHONY: clean
clean:
	rm -f $(PROGS) *.o core
