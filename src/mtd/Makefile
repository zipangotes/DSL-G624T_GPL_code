# $Id: Makefile,v 1.1.1.1 2003/11/25 22:15:11 jharrell Exp $
#

TARGETS = jffs kernel
#TARGETS = ffs2 kernel
SUBDIRS = util # boot
LINUXDIR = $(KERNEL_DIR)

all:	
	make -C $(LINUXDIR) SUBDIRS=$(shell pwd)/kernel modules
	for a in $(SUBDIRS); do \
		make -C $$a; \
	done

jffs:
	make -C $(LINUXDIR) SUBDIRS=$(shell pwd)/fs/jffs modules

jffs2:
	make -C $(LINUXDIR) SUBDIRS=$(shell pwd)/fs/jffs2 modules

ffs2:
	make -C $(LINUXDIR) SUBDIRS=$(shell pwd)/fs/ffs2 modules

util:
	make -C util

boot:
	make -C boot

clean:
	find kernel/ -name '*.[oa]' -type f | xargs rm -f
	find fs/ -name '*.[oa]' -type f | xargs rm -f
	make -C util clean
	make -C boot clean
