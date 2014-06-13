.PHONY: dummy

#TI_build_home = $(shell cat .bh_temp)
#TI_linux_home = $(shell cat .ti_linux_home)

#-----------------------------------------
# Release specific Information
#-----------------------------------------

TI_release_version =  3.1
TI_kernel_version  =  linux-2.4.17_mvl21

#-----------------------------------------
# build specific defines (global)
#-----------------------------------------

TI_tools_path  = $(TARGET_HOME)/tools
TI_kernel_path = $(TARGET_HOME)/src/kernel


#-----------------------------------------
# filesystem specific defines
#-----------------------------------------
#TI_base = $(TI_build_home)/$(TI_linux_home)
TI_base = $(TARGET_HOME)
#TI_filesystem_path = $(TI_build_home)/$(TI_linux_home)/filesystem
TI_filesystem_path = $(TARGET_HOME)/filesystem

TI_bindir = 	/filesystem/usr/bin
TI_sbindir =	/filesystem/usr/sbin
TI_etcdir =	/filesystem/etc
TI_datadir =	/filesystem/share
TI_libdir =	/filesystem/usr/lib
TI_mandir =	/filesystem/share/man
TI_infodir =	/filesystem/share/info
TI_includedir =	/filesystem/usr/include
TI_docdir =     /filesystem/share/doc
TI_rsbindir =	/filesystem/sbin
TI_rusrdir =    /filesystem/usr
TI_rlibdir =	/filesystem/lib
TI_wwwdir =     /filesystem/usr/www
TI_wwwsafedir = /filesystem/usr/www_safe
TI_cgidir =     /filesystem/usr/www/cgi-bin
TI_htmldir =    /filesystem/usr/www/html
TI_htmlsafedir = /filesystem/usr/www_safe/html
TI_imagedir =   /filesystem/usr/www/html/defs/style5
TI_vardir =	/filesystem/var
TI_vartmpdir =  /filesystem/var/tmp
TI_varflashdir= /filesystem/var/flash
TI_vardevdir=   /filesystem/var/dev
TI_varprocdir=  /filesystem/var/proc

#-----------------------------------------
# Tools specific defines
#----------------------------------------
TI_uclibc_base = $(TI_tools_path)/mipsel-linux-uclibc
TI_uclibc_path = $(TI_uclibc_base)/bin/
TI_uclibc_include = $(TI_uclibc_base)/include
TI_uclibc_library = $(TI_uclibc_base)/lib
TI_Adam2_path = $(TI_tools_path)/Adam2

#------------------------------------------
# Environment specific sets
#------------------------------------------
#CROSS_COMPILE_PREFIX = $(TI_uclibc_base)
#TEMPPATH = $(PATH)
#uclibcPATH = $(TI_uclibc_path)
#export PATH := $(uclibcPATH):$(TEMPPATH)

export NATIVEPATH := PATH=$(shell echo -n $(PATH))
export PATH := $(TI_uclibc_path):$(shell echo -n $(PATH))

CMFLAGS = $(shell cat $(TI_base)/.cmflags)
