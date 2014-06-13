# Path to parent kernel include files directory
#KERNEL_INCLUDE=/usr/src/linux/include
#LIBC_INCLUDE=/usr/include
KERNEL_INCLUDE=$(KERNEL_DIR)/include
LIBC_INCLUDE=$(LIBC_DIR)/include

DEFINES= -DRESOLVE_HOSTNAMES

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

#options if you compile with libc5, and without a bind>=4.9.4 libresolv
#LDLIBS=
#ADDLIB=inet_ntop.o inet_pton.o

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

ifeq ($(LIBC_INCLUDE)/socketbits.h,$(wildcard $(LIBC_INCLUDE)/socketbits.h))
  ifeq ($(LIBC_INCLUDE)/net/if_packet.h,$(wildcard $(LIBC_INCLUDE)/net/if_packet.h))
    GLIBCFIX=-I../include-glibc -include ../include-glibc/glibc-bugs.h
  endif
endif
ifeq ($(LIBC_INCLUDE)/bits/socket.h,$(wildcard $(LIBC_INCLUDE)/bits/socket.h))
  GLIBCFIX=-I../include-glibc -include ../include-glibc/glibc-bugs.h
endif

CC = gcc
CCOPTS = -D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall -Werror -g
CFLAGS = $(CCOPTS) $(GLIBCFIX) -I$(KERNEL_INCLUDE) -I../include $(DEFINES)

LDLIBS += -L../lib -lnetlink -lutil

#SUBDIRS=lib ip tc
SUBDIRS=lib tc

LIBNETLINK=../lib/libnetlink.a ../lib/libutil.a

all: check-kernel
	@set -e; \
	for i in $(SUBDIRS); \
	do $(MAKE) -C $$i; done

check-kernel:
ifeq ($(KERNEL_INCLUDE),)
	@echo "Please, set correct KERNEL_INCLUDE"; false
else
	@set -e; \
	if [ ! -r $(KERNEL_INCLUDE)/linux/autoconf.h ]; then \
		echo "Please, Compile the kernel first"; false; fi
endif

clean:
	for i in $(SUBDIRS) doc; \
	do $(MAKE) -C $$i clean; done

.EXPORT_ALL_VARIABLES:
