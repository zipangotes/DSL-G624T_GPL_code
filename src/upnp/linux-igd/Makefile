CC=gcc
INCLUDES= -I. -I./upnp
ifeq ($(DEBUG),1)
ULIB=/work/matarj/libupnp-1.2.1/upnp/bin/debug
else
ULIB=.
endif
LIBS= -lpthread -L$(ULIB) -L. -lupnp -lthreadutil -lixml


ifeq ($(DEBUG),1)
OPT = -ggdb
else
OPT = -Os
endif

CFLAGS += -Wall $(OPT)

APPS = upnpd

all: $(APPS)

#upnpd:  gate.o gateway.o sample_util.o ipcon.o portmap.o pmlist.o
upnpd:  gate.o gateway.o ipcon.o portmap.o pmlist.o
	$(CC)  $(CFLAGS) -Xlinker -rpath-link . gate.o gateway.o ipcon.o portmap.o pmlist.o $(LIBS) -o  $@ 
#	strip -s upnpd
	@echo "make $@ finished on `date`"

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o $(APPS) *.a

install: upnpd
	@install -d /etc/linuxigd
	@install etc/* /etc/linux-igd
	@install upnpd /usr/bin

