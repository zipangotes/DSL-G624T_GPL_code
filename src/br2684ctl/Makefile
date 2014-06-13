PREFIX=${TI_FILESYSTEM}

all: br2684ctl

br2684ctl: br2684ctl.c
	gcc -latm -o br2684ctl br2684ctl.c 
	strip br2684ctl

install: br2684ctl
	cp br2684ctl $(PREFIX)/usr/sbin/

clean:
	rm -rf br2684ctl *.o
