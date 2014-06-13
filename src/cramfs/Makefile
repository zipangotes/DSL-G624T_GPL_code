CC = gcc
CFLAGS = -W -Wall -O2 -g -I./include

ZLIB = libztemp.a
ZLIB_VER=1.1.3

LDFLAGS = -L../../../tools/lib -lztemp

#TARGETS = mkcramfs cramfsck
TARGETS = mkcramfs

all: $(TARGETS)

mkcramfs: mkcramfs.o
	$(CC) $(CFALGS) -o $@ mkcramfs.o $(LDFLAGS)

cramfsck: cramfsck.o
	$(CC) $(CFALGS) -o $@ cramfsck.o $(LDFLAGS)

clean:
	rm -f $(TARGETS)
	rm -f *.o *~

.PHONY: all clean
