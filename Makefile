TARGET_HOME = $(shell pwd)
KERNEL_DIR  = $(TARGET_HOME)/src/kernel/linux-2.4.17_mvl21
TOOLS_DIR   = $(TARGET_HOME)/tools
BUILD_HOME  = $(shell cd $(TARGET_HOME);cd ..;pwd)
FSSTAMP     = $(shell date +'%Y%m%d%H%M%S')

REL_NSP32 = 1

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

SHELL := $(CONFIG_SHELL)

HAVE_CONFIG := $(shell if [ -e $(TARGET_HOME)/.config ]; then echo 1; \
	 else echo 0; fi;)

ifeq ($(HAVE_CONFIG),1)
include .config
endif

ifeq ($(CONFIG_MIPS_AR7RD),y) 
BOARD=AR7RD
BOARD_LOWER=ar7rd
ifeq ($(CONFIG_MIPS_AR7RD_7300c),y)
BOARD_EXT=_7300c
endif
endif
ifeq ($(CONFIG_MIPS_AR7WRD),y)
BOARD=AR7WRD
BOARD_LOWER=ar7wrd
endif
ifeq ($(CONFIG_MIPS_AR7VWI),y)
BOARD=AR7VWI
BOARD_LOWER=ar7vwi
endif
ifeq ($(CONFIG_MIPS_AR7VW),y)
BOARD=AR7VW
BOARD_LOWER=ar7vw
endif
ifeq ($(CONFIG_MIPS_AR7WI),y)
BOARD=AR7WI
BOARD_LOWER=ar7wi
endif
ifeq ($(CONFIG_MIPS_AR7VW),y)
BOARD=AR7VW
BOARD_LOWER=ar7vw
endif
ifeq ($(CONFIG_TI_NSP),y)
DIST=NSP
endif
ifeq ($(CONFIG_TI_PSP),y)
DIST=PSP
endif
ifeq ($(CONFIG_NFS_CLIENT),y)
NFSCLIENT=y
endif
ifeq ($(CONFIG_MIPS_AR7W_1230),y)
WIRELESS1230=y
endif

ifeq ($(CONFIG_TI_DSL_ANNEXA),y)
ANNEX=A
else
ifeq ($(CONFIG_TI_DSL_ANNEXB),y)
ANNEX=B
else
ANNEX=
endif
endif

export BOARD DIST NFSCLIENT TARGET_HOME NSPTEMPPATH WIRELESS1230 ANNEX


#---------------------------------
# build components..
#---------------------------------
TI_COMPONENTS-y :=
TI_COMPONENTS-n :=
TI_BASE_SYSTEM-y :=
TI_BASE_SYSTEM-n :=
TI_LIBRARIES-y :=
TI_LIBRARIES-n :=
TI_BOOTLOADER-y :=
TI_BOOTLOADER-n :=

#---BootLoader--------------------
ifeq ($(CONFIG_MIPS_AVALANCHE_ADAM2),y)
	TI_BOOTLOADER-$(CONFIG_MIPS_AVALANCHE_ADAM2) += Adam2
else
	TI_BOOTLOADER-$(CONFIG_MIPS_AVALANCHE_PSPBOOT) += psp_boot
endif	

#----System Base------------------
TI_BASE_SYSTEM-y += basefilesystem

#-----Libraries-------------------
#TI_LIBRARIES-$(CONFIG_TI_UCLIBC) += uClibc  # Always Compiled in!
TI_LIBRARIES-$(CONFIG_TI_LIBATM) += libatm
TI_LIBRARIES-$(CONFIG_TI_ZLIB)   += zlib
TI_LIBRARIES-y += libtomcrypt libtommath

#----Applications-----------------
TI_COMPONENTS-$(CONFIG_TI_BRCTL) += brctl
TI_COMPONENTS-$(CONFIG_TI_DHCP_FORWARDER) += dhcp-forwarder
TI_COMPONENTS-$(CONFIG_TI_DPROXY) += dproxy
TI_COMPONENTS-$(CONFIG_TI_IPTABLES) += iptables
TI_COMPONENTS-$(CONFIG_TI_MTD) += mtd
TI_COMPONENTS-$(CONFIG_TI_NETKIT_ROUTED) += netkit-routed
TI_COMPONENTS-$(CONFIG_TI_NETKIT_ROUTEDV2) += netkit-routedv2
TI_COMPONENTS-$(CONFIG_TI_NETTOOLS) += nettools
TI_COMPONENTS-$(CONFIG_TI_PPP_PPPOE) += ppp-pppoe
TI_COMPONENTS-$(CONFIG_TI_THTTPD) += thttpd
TI_COMPONENTS-$(CONFIG_TI_UDHCP) += udhcp
#TI_COMPONENTS-$(CONFIG_TI_BR2684CTL) += br2684ctl
TI_COMPONENTS-$(CONFIG_TI_UTELNETD) += utelnetd
TI_COMPONENTS-$(CONFIG_TI_BUSYBOX) += busybox
TI_COMPONENTS-$(CONFIG_TI_UPGRADER) += upgrader
TI_COMPONENTS-$(CONFIG_TI_CRAMFS) += cramfs
TI_COMPONENTS-$(CONFIG_TI_DROPBEAR) += dropbear
TI_COMPONENTS-$(CONFIG_TI_SQUASHFS) += squashfs
TI_COMPONENTS-$(CONFIG_TI_CHECKSUM) += TI-chksum
TI_COMPONENTS-$(CONFIG_TI_DSL_FW) += TI_dsl_firmware
TI_COMPONENTS-$(CONFIG_TI_IGMP_PROXY) += igmp-proxy
TI_COMPONENTS-$(CONFIG_TI_UPNP) += upnp
TI_COMPONENTS-$(CONFIG_TI_HTML) += html
TI_COMPONENTS-$(CONFIG_TI_LEDAPP) += TI-ledapp
TI_COMPONENTS-$(CONFIG_TI_LEDCFG) += TI-ledcfg
TI_COMPONENTS-$(CONFIG_TI_IPROUTE2) += iproute2
TI_COMPONENTS-$(CONFIG_TI_FOXY) += foxy
TI_COMPONENTS-$(CONFIG_TI_WLAN) += wlan
TI_COMPONENTS-$(CONFIG_TI_BRIDGE_UTILS) += bridge-utils
TI_COMPONENTS-$(CONFIG_TI_CFGMGR) += cfgmgr
TI_COMPONENTS-$(CONFIG_TI_SNTP) += msntp
TI_COMPONENTS-$(CONFIG_TI_VOICEAPP) += voice_app
#
#components1: basefilesystem uClibc libatm cfgmgr brctl dhcp-forwarder dproxy iptables mtd netkit-routed nettools 
#components2: ppp-pppoe thttpd udhcp zlib br2684ctl utelnetd busybox upgrader cramfs dropbear squashfs TI-chksum TI_dsl_firmware 


#--------------------------------------------------------------
# Generic help
#--------------------------------------------------------------
ifeq ($(CONFIG_ADAM2_8M),y)
ADAM2_MEMSIZE=8M
endif	
ifeq ($(CONFIG_ADAM2_16M),y)
ADAM2_MEMSIZE=16M
endif
ifeq ($(CONFIG_ADAM2_EXTERNAL_PHY),y)
ADAM2_PHY=external
endif
ifeq ($(CONFIG_ADAM2_INTERNAL_PHY),y)
ADAM2_PHY=internal
endif
ifeq ($(CONFIG_MIPS_AR7_MARVELL),y)
EMACPHY=marvell
endif
ifeq ($(CONFIG_AVALANCHE_CPMAC_AUTO),y)
EMACPHY=auto
endif
ifeq ($(CONFIG_ADAM2_FTP),y)
A2_FTP=yes
else
A2_FTP=no
endif

all:
	@echo "*-----------------------------------------------------------------------------------------*"
	@echo "*                        -*  Texas Instruments Linux *-                                   *"
	@echo "*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*"
	@echo "*                                                                                         *"
	@echo "*    make setconfig            : Select a configuration file for a build                  *"
	@echo "*                                                                                         *"
	@echo "*    make oldconfig            : Generates configuration files from a .config file        *"
	@echo "*                                                                                         *"
	@echo "*    make xconfig              : Graphical configration (uses .config by default)         *"
	@echo "*                                                                                         *"
	@echo "*    make nsp                  : Build NSP (kernel/filesystem)                            *"
	@echo "*                                                                                         *"
	@echo "*    make kernel               : Build kernel only                                        *"
	@echo "*                                                                                         *"
	@echo "*    make boot                 : Build Adam2 or PSPBoot Bootloader                        *"
	@echo "*                                 (options pulled from config file)                       *"
	@echo "*                                                                                         *"
	@echo "*    make show_config          : Show currently defined configuration of system.          *"
	@echo "*                                (Note busybox will not display components until after    *"
	@echo "*                                 initial build)                                          *"
	@echo "*                                                                                         *"
	@echo "*    make show_targets         : Show Configured Application targets                      *"
	@echo "*                                                                                         *"
	@echo "*    make <app_name>           : Build an individual application                          *"
	@echo "*                                (use show_targets to list available app names list)      *"
	@echo "*                                                                                         *"
	@echo "*    make clean                : clean build (not configuration)                          *"
	@echo "*                                                                                         *"
	@echo "*    make dist_clean           : clean build and configuration files                      *"
	@echo "*                                                                                         *"
	@echo "*    make no_tools_build       : Build a distribution that doesn't require a toolchain    *"
	@echo "*                                                                                         *"
	@echo "*    make no_tools_build_clean : clean no tools build                                     *"
	@echo "*-----------------------------------------------------------------------------------------*"

show_config:
	@echo "*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*"
	@echo "*                          Configuration Information                             *"
	@echo "*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*"
	@echo "*-----------------------*"
	@echo "* Configured BOARD type *" 
	@echo "*-----------------------*"
	@echo " $(BOARD) PHY_SELECTION=$(EMACPHY)"
	@echo ""						   
	@echo "*---------------------*"
	@echo "* Bootloader options  *"
	@echo "*---------------------*"
	@echo "Memory size=$(ADAM2_MEMSIZE) FTPC=$(A2_FTP) EMAC=$(ADAM2_PHY)"
	@echo ""
	@echo "*--------------------------------*"
	@echo "* Configured Application targets *"
	@echo "*--------------------------------*"
	@echo "$(TI_COMPONENTS-y)"
	@echo ""
	@echo "*-----------------------*"
	@echo "* ConfigMan Components  *"
	@echo "*-----------------------*"
	@$(MAKE) -s -C src/cfgmgr/src/modules/ show_config
ifeq ($(CONFIG_TI_CFGMGR_CMCLI),y)
	echo "CLI"
endif
	@echo ""
	@echo "*--------------------*"
	@echo "* Busybox components *"
	@echo "*--------------------*"
	@$(MAKE) -s -C src/busybox show_config
	@echo""

show_targets:
	@echo "*--------------------------------*"
	@echo "* Configured Application targets *"
	@echo "*--------------------------------*"
	@echo "$(TI_COMPONENTS-y) $(TI_COMPONENTS-n) $(TI_COMPONENTS-)" 
	@echo ""


#---------------------------------
# Configuration [oldconfig, xconfig, menuconfig
#---------------------------------
HOSTCC = $(NATIVEPATH) gcc
HOSTCFLAGS = -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

export HOSTCC HOSTCFLAGS

oldconfig: setup_paths setup_configs
	$(CONFIG_SHELL) tools/scripts/Configure -d tools/configs/config.in 
	$(MAKE) setup_links

xconfig: setup_paths setup_configs
	$(MAKE) -C tools/scripts kconfig.tk
	wish -f tools/scripts/kconfig.tk
	$(MAKE) setup_links

#menuconfig: setup_paths
#	$(MAKE) -C tools/scripts/lxdialog all
#	export TI_TARGET_KERNEL_PATH="~/work/nsp_dev/src/kernel/linux-2.4.17_mvl21/" 
#	$(CONFIG_SHELL) tools/scripts/Menuconfig tools/configs/config.in

config: setup_paths setup_configs
	$(CONFIG_SHELL) tools/scripts/Configure tools/configs/config.in
	$(MAKE) setup_links

setconfig: 
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME)
	$(MAKE) setoldconfig
setconfig1:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 1
	$(MAKE) setoldconfig
setconfig2:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 2
	$(MAKE) setoldconfig
setconfig3:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 3
	$(MAKE) setoldconfig
setconfig4:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 4
	$(MAKE) setoldconfig
setconfig5:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 5
	$(MAKE) setoldconfig
setconfig6:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 6
	$(MAKE) setoldconfig
setconfig7:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 7
	$(MAKE) setoldconfig
setconfig8:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 8
	$(MAKE) setoldconfig
setconfig9:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 9
	$(MAKE) setoldconfig
setconfig10:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 10
	$(MAKE) setoldconfig
setconfig11:
	@$(TARGET_HOME)/tools/bin/setconfig $(TARGET_HOME) 11
	$(MAKE) setoldconfig

setoldconfig:
	@if [ ! -e $(TARGET_HOME)/.bad_setconfig ]; then \
		$(MAKE) oldconfig; \
	fi
	@rm -f $(TARGET_HOME)/.bad_setconfig;

config_check:
ifeq ($(HAVE_CONFIG),0)
	@echo "*-------------------------------------------------*"
	@echo "*  ERROR: No NSP Configuration file               *"
	@echo "*         Please use [config|menuconfig|xconfig]  *"
	@echo "*         or copy board config file to .config    *"
	@echo "*-------------------------------------------------*"
	@cat $(TARGET_HOME)/force_error 2> /dev/null
endif

oldconfig_fail:
	@echo "*-------------------------------------------------*"
	@echo "*  ERROR: config file has not been generated!     *"
	@echo "*         Please use [config|menuconfig|xconfig]  *"
	@echo "*-------------------------------------------------*"
	@cat $(TARGET_HOME)/force_error 2> /dev/null


#---------------------------------
#Does initial build setup
#---------------------------------
setup_paths:
	@echo $(BUILD_HOME) > .bh_temp.in
	@sed -e 's/\//\\\//g' <.bh_temp.in >.bh_temp	
	@echo $(TARGET_HOME) > .th_temp.in
	@sed -e 's/\//\\\//g' <.th_temp.in >.th_temp
	@sed -e "s|$(BUILD_HOME)\/||" <.th_temp.in >.ti_linux_home


#
# Generated Configuration files (setup dependencies)
#

#src/uClibc/extra/Configs/Config.nsp.mipsel: src/uClibc/extra/Configs/Config.nsp.mipsel.orig
#src/busybox/sysdeps/linux/nsp_config.in: src/busybox/sysdeps/linux/nsp_config.in.orig
#tools/configs/config.in: tools/configs/config.in.orig

#src/uClibc/extra/Configs/Config.nsp.mipsel.orig:
#	@rm -f src/uClibc/extra/Configs/Config.nsp.mipsel
#	@sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' \
#		<src/uClibc/extra/Configs/Config.nsp.mipsel.orig >src/uClibc/extra/Configs/Config.nsp.mipsel
#src/busybox/sysdeps/linux/nsp_config.in.orig:
#	@rm  -f src/busybox/sysdeps/linux/nsp_config.in
#	@sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' \
#		<src/busybox/sysdeps/linux/nsp_config.in.orig >src/busybox/sysdeps/linux/nsp_config.in
#tools/configs/config.in.orig:
#	@rm -f tools/configs/config.in
#	@sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' -e  's/@@LINUXVERSION@@/$(TI_kernel_version)/' \
#		<tools/configs/config.in.orig >tools/configs/config.in

#
#
#

setup_configs:
	@if [ ! -e $(TARGET_HOME)/src/uClibc/extra/Configs/Config.nsp.mipsel ]; then \
		sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' \
		<src/uClibc/extra/Configs/Config.nsp.mipsel.orig >src/uClibc/extra/Configs/Config.nsp.mipsel; \
	fi
	@if [ ! -e $(TARGET_HOME)/src/busybox/sysdeps/linux/nsp_config.in ]; then \
		sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' \
		<src/busybox/sysdeps/linux/nsp_config.in.orig >src/busybox/sysdeps/linux/nsp_config.in; \
	fi
	@if [ ! -e $(TARGET_HOME)/tools/configs/config.in ]; then \
		sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' -e  's/@@LINUXVERSION@@/$(TI_kernel_version)/' \
		<tools/configs/config.in.orig >tools/configs/config.in; \
	fi

setup_links:
# Setup auto configuration files
	@if [ ! -e $(TARGET_HOME)/.config ]; then \
	$(MAKE) config_check; \
	fi
	@if [ ! -e $(TARGET_HOME)/tools/configs/nsp_autoconf.h ]; then \
	$(MAKE) oldconfig_fail; \
	fi
# Setup the configuration links for the kernel, busybox and uClibc(JAH_TBD)
	ln -sf $(TARGET_HOME)/.config src/kernel/$(TI_kernel_version)/.config
	ln -sf $(TARGET_HOME)/.config src/busybox/.config
	ln -sf $(TARGET_HOME)/.config src/uClibc/.config
	ln -sf $(TARGET_HOME)/.config src/cfgmgr/.config

# Link the kernel configuration file
	ln -sf $(TARGET_HOME)/tools/configs/nsp_autoconf.h src/kernel/$(TI_kernel_version)/include/linux/nsp.h 

# insure that we have an autoconf.h in the kernel include directory..
	@if [ ! -e $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/linux/autoconf.h ]; then \
	$(MAKE) -C $(TARGET_HOME)/src/kernel/$(TI_kernel_version) oldconfig; \
	fi

# insure that we have an autoconf.h in the uClibc include directory 
	@if [ ! -e $(TARGET_HOME)/src/uClibc/include/autoconf.h ]; then \
	$(MAKE) -C $(TARGET_HOME)/src/uClibc oldconfig; \
	fi

# Has uClibc been compiled?
	@if [ ! -e $(TARGET_HOME)/tools/mipsel-linux-uclibc/bin/mipsel-uclibc-gcc ]; then \
	$(MAKE) uClibc; \
	fi
	@if [ ! -e $(TARGET_HOME)/filesystem/lib/libuClibc-0.9.19.so ]; then \
	$(MAKE) uClibc; \
	fi



.PHONY: setup_paths setup_configs setup_links



board_check:
ifeq	"$(BOARD)" ""
	@echo "*----------------------------------------------------------------*"
	@echo "*  No BOARD specified please specify BOARD=	                *"
	@echo "*  (Do you have a .config file?)                                 *"
	@echo "*----------------------------------------------------------------*"
	@cat $(TARGET_HOME)/force_error 2> /dev/null
endif

	echo $(BOARD) > .board
	echo $(CMFLAGS) > .cmflags

#-------------------------
# Board specific Makefiles
#-------------------------
setup_board: board_check
	ln -sf ./Builds/Makefile.$(DIST).$(BOARD)  Makefile.board 

#-----------------------------------------------
# Distribution Builds
#-----------------------------------------------
nsp: nsp_oldconfig_check setup_board setup_configs setup_paths setup_links basefilesystem TI-chksum TI_dsl_firmware kernel_build $(TI_BASE_SYSTEM-y) $(TI_LIBRARIES-y) $(TI_COMPONENTS-y) filesystem config_xml

nsp_oldconfig_check:
	@if [ ! -e $(TARGET_HOME)/tools/configs/nsp_autoconf.h ]; then \
	$(MAKE) oldconfig; \
	fi


psp: setup_board

#-----------------------------------------------
# Kernel build
#-----------------------------------------------
kernel: setup_paths  basefilesystem kernel_build TI_dsl_firmware TI-ledcfg
kernel_build: board_check 
	@if [ ! -e $(TARGET_HOME)/Makefile.board ]; then \
	$(MAKE) setup_board; \
	fi
	@if [ ! -e $(TARGET_HOME)/tools/bin/7zip ]; then \
	$(MAKE) 7zip; \
	fi
	@if [ ! -e $(TARGET_HOME)/tools/bin/tichksum ]; then \
	$(MAKE) TI-chksum; \
	fi
	$(MAKE) -C $(TARGET_HOME) -f Makefile.board build_kernel BOARD=$(BOARD) DIST=$(DIST) BOARD_EXT=$(BOARD_EXT)

#-----------------------------------------------
# Build version information
#-----------------------------------------------
pop_ver:
	@echo "BUILD=$(BUILD)" > $(TARGET_HOME)/filesystem/etc/versions
	@echo "VERSION=$(CONFIG_TI_NSP_VERSION)" >> $(TARGET_HOME)/filesystem/etc/versions
	@echo "BOARD=$(BOARD)" >> $(TARGET_HOME)/filesystem/etc/versions
	@echo "FSSTAMP=$(FSSTAMP)" >> $(TARGET_HOME)/filesystem/etc/versions
ifdef NSP_CM_LABEL
	@echo 'NSP_CM_LABEL=$(NSP_CM_LABEL)' >> $(TARGET_HOME)/filesystem/etc/versions
endif


#-----------------------------------------------
# Enable/disable digital signature option
#-----------------------------------------------
digsig_on:
	@cat /dev/null > $(TARGET_HOME)/filesystem/etc/dss

digsig_off:
	@rm -rf $(TARGET_HOME)/filesystem/etc/dss

#-----------------------------------------------
# filesystem build
#-----------------------------------------------
filesystem: pop_ver
	$(MAKE) -C $(TARGET_HOME) -f Makefile.board build_filesystem BOARD=$(BOARD) DIST=$(DIST) BOARD_EXT=$(BOARD_EXT)

#-----------------------------------------------
# config.xml build
#-----------------------------------------------
config_xml: mkconfig
	@echo $(CONFIG_TI_NSP_VERSION)
	@awk '{ if(NR==1) print "<config version=\"'$(CONFIG_TI_NSP_VERSION)'\" fsstamp=\"'$(FSSTAMP)'\">"; else print $0 }' \
		$(TARGET_HOME)/filesystem/etc/config.xml > $(TARGET_HOME)/config.xml
	$(MAKE) -C $(TARGET_HOME) -f Makefile.board build_config BOARD=$(BOARD) DIST=$(DIST) BOARD_EXT=$(BOARD_EXT)
	@rm -rf $(TARGET_HOME)/config.xml

#-----------------------------------------------
# "no tools"  build
#-----------------------------------------------
no_tools_build:
ifeq    "$(BOARD)" ""
	@echo "*--------------------------------------------------------------------------------*"
	@echo "*  No BOARD specified please specify BOARD=AR7RD|AR7VWI|AR7WI|AR7V|AR7VW               *"
	@echo "*--------------------------------------------------------------------------------*"
	@cat $(TARGET_HOME)/force_error 2> /dev/null
endif

ifeq ($(CONFIG_MIPS_AVALANCHE_ADAM2),y)
	@if [ ! -f adam2.$(BOARD).* ]; then \
        echo "*-------------------------------------------------------------------------*"; \
        echo "*  Please create a bootloader image before doing 'make no_tools_build'    *"; \
        echo "*-------------------------------------------------------------------------*"; \
        cat $(TARGET_HOME)/force_error 2> /dev/null; \
        fi
else
	@if [ ! -f psbl.$(BOARD)$(BOARD_EXT).* ]; then \
	echo "*-------------------------------------------------------------------------*"; \
	echo "*  Please create a bootloader image before doing 'make no_tools_build'    *"; \
	echo "*-------------------------------------------------------------------------*"; \
	cat $(TARGET_HOME)/force_error 2> /dev/null; \
	fi
endif

	@if [ -d no_tools ]; then \
	rm -rf no_tools; \
	fi
	mkdir no_tools
	mkdir no_tools/bin

	# temporary filesystem dir, so contents of /var can be deleted after var.tar creation
	cp -a filesystem filesystem_temp
	cd filesystem_temp; tar cvf var.tar var
	rm -rf filesystem_temp/var/*


	cp ram_zimage.${BOARD_LOWER}${BOARD_EXT}.nsp.squashfs.bin ram_zimage.ar7xxx.nsp.squashfs.bin

	cp ram_zimage.ar7xxx.nsp.squashfs.bin ram_zimage.ar7xxx.nsp.squashfs.upgrade.bin
	./tools/bin/tichksum ram_zimage.ar7xxx.nsp.squashfs.upgrade.bin

	# ANNEX=A
	cp $(TARGET_HOME)/ar0700mp.bin $(TARGET_HOME)/filesystem_temp/lib/modules/ar0700xx.bin
	./tools/bin/mksquashfs filesystem_temp nsp.annexA.squashfs.img
	chmod 644 nsp.annexA.squashfs.img
	cp nsp.annexA.squashfs.img nsp.annexA.squashfs.upgrade.img
	./tools/bin/tichksum nsp.annexA.squashfs.upgrade.img

	echo "*** Creating Annex A single image..."
	./tools/bin/mknspimg -o nsp.annexA.firmware.img -i ram_zimage.ar7xxx.nsp.squashfs.bin \
		-i nsp.annexA.squashfs.img -a 0 4096 -v 
	cp nsp.annexA.firmware.img nsp.annexA.firmware.upgrade.img
	./tools/bin/tichksum nsp.annexA.firmware.upgrade.img

	mv nsp.annexA.* no_tools/.

	mv ram_zimage.ar7xxx.nsp.squashfs.bin  no_tools/.
	mv ram_zimage.ar7xxx.nsp.squashfs.upgrade.bin  no_tools/.

	@awk '{ if(NR==1) print "<config version=\"'$(CONFIG_TI_NSP_VERSION)'\" fsstamp=\"'$(FSSTAMP)'\">"; else print $0 }' \
		$(TARGET_HOME)/filesystem_temp/etc/config.xml > $(TARGET_HOME)/config.xml
	./tools/bin/mkconfig -i $(TARGET_HOME)/config.xml -o config.ar7xxx.img
	mv config.ar7xxx.img no_tools/.

	cp ar0700* no_tools/bin/.
	cp tools/bin/tichksum no_tools/bin/.
#   Update NSP_VERSION variable in no_tools_build script with CONFIG_TI_NSP_VERSION 
#   on the fly

	@awk '{ if(NR==2) print "NSP_VERSION=\"'$(CONFIG_TI_NSP_VERSION)'\""; else print $0 }' \
	$(TARGET_HOME)/tools/bin/no_tools_build > $(TARGET_HOME)/no_tools/no_tools_build
	chmod 755 $(TARGET_HOME)/no_tools/no_tools_build
		
	cp tools/bin/new_tools_build_readme.txt no_tools/readme.txt
	cp tools/bin/mksquashfs no_tools/bin/.
	cp tools/bin/base64 no_tools/bin/.
	cp tools/bin/mknspimg no_tools/bin/.
	cp tools/bin/tikeygen no_tools/bin/.
	cp tools/bin/tisign no_tools/bin/.
	cp tools/bin/mkconfig no_tools/bin/.
	cp tools/bin/convert_voice_cfg.pl no_tools/bin/.
	cp tools/bin/xmlindent no_tools/bin/.
	cp -a filesystem no_tools/filesystem
	@if [ -f adam2.$(BOARD).* ]; then \
	cp adam2.$(BOARD).* no_tools/.; \
	fi
	@if [ -f psbl.$(BOARD).* ]; then \
	cp psbl.$(BOARD)$(BOARD_EXT).* no_tools/.; \
	fi

	tar cvzf no_tools_build_$(BOARD)$(BOARD_EXT).tgz no_tools
	rm -rf filesystem_temp
	rm -rf $(TARGET_HOME)/config.xml

nspfsdbg:
	@echo "**** Building nsp.dbg.img and nsp.dbg.upgrade.img *****"
	rm -f nsp.dbg.*
	cp -a filesystem filesystem_temp
	cd filesystem_temp; tar cvf var.tar var
	rm -rf filesystem_temp/var/*
	./tools/bin/mksquashfs filesystem_temp nsp.dbg.img
	chmod 644 nsp.dbg.img
	cp nsp.dbg.img nsp.dbg.upgrade.img
	./tools/bin/tichksum nsp.dbg.upgrade.img
	rm -rf filesystem_temp
	@echo "****** Built nsp.dbg.img and nsp.dbg.upgrade.img *******"

# start - standalone voice builds
# NOTE: make < dimtest | ggbasica | ggsvca > is only valid for release builds (not from development build area!)
dimtest:
	@if [ -f filesystem/usr/sbin/ggmgcp ]; then \
	rm filesystem/usr/sbin/ggmgcp; \
	fi
	@if [ -f filesystem/usr/sbin/ggbasica ]; then \
	rm filesystem/usr/sbin/ggbasica; \
	fi
	@if [ -f filesystem/usr/sbin/ggsvca ]; then \
	rm filesystem/usr/sbin/ggsvca; \
	fi
	@if [ -f filesystem/usr/sbin/ggsip ]; then \
	rm filesystem/usr/sbin/ggsip; \
	fi
	cp ./dimtest filesystem/usr/sbin
	@if [ -f nsp.$(BOARD_LOWER).squashfs.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.img nsp.$(BOARD).squashfs.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.img nsp.$(BOARD).firmware.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).squashfs.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.$(BOARD).squashfs.upgrade.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.$(BOARD).firmware.upgrade.img; \
	fi

	$(MAKE) filesystem
	mv nsp.$(BOARD_LOWER).squashfs.img nsp.squashfs.dimtest.img
	mv nsp.$(BOARD_LOWER).firmware.img nsp.firmware.dimtest.img
	mv nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.squashfs.dimtest.upgrade.img
	mv nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.firmware.dimtest.upgrade.img

ggbasica:
	@if [ -f filesystem/usr/sbin/ggmgcp ]; then \
	rm filesystem/usr/sbin/ggmgcp; \
	fi
	@if [ -f filesystem/usr/sbin/dimtest ]; then \
	rm filesystem/usr/sbin/dimtest; \
	fi
	@if [ -f filesystem/usr/sbin/ggsvca ]; then \
	rm filesystem/usr/sbin/ggsvca; \
	fi
	@if [ -f filesystem/usr/sbin/ggsip ]; then \
	rm filesystem/usr/sbin/ggsip; \
	fi
	cp ./ggbasica filesystem/usr/sbin
	@if [ -f nsp.$(BOARD_LOWER).squashfs.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.img nsp.$(BOARD).squashfs.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.img nsp.$(BOARD).firmware.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).squashfs.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.$(BOARD).squashfs.upgrade.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.$(BOARD).firmware.upgrade.img; \
	fi
                                                                                  
	$(MAKE) filesystem
	mv nsp.$(BOARD_LOWER).squashfs.img nsp.squashfs.ggbasica.img
	mv nsp.$(BOARD_LOWER).firmware.img nsp.firmware.ggbasica.img
	mv nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.squashfs.ggbasica.upgrade.img
	mv nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.firmware.ggbasica.upgrade.img

ggsvca:
	@if [ -f filesystem/usr/sbin/ggmgcp ]; then \
	rm filesystem/usr/sbin/ggmgcp; \
	fi
	@if [ -f filesystem/usr/sbin/ggbasica ]; then \
	rm filesystem/usr/sbin/ggbasica; \
	fi
	@if [ -f filesystem/usr/sbin/dimtest ]; then \
	rm filesystem/usr/sbin/dimtest; \
	fi
	@if [ -f filesystem/usr/sbin/ggsip ]; then \
	rm filesystem/usr/sbin/ggsip; \
	fi
	cp ./ggsvca filesystem/usr/sbin

	@if [ -f nsp.$(BOARD_LOWER).squashfs.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.img nsp.$(BOARD).squashfs.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.img nsp.$(BOARD).firmware.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).squashfs.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.$(BOARD).squashfs.upgrade.img; \
	fi
	@if [ -f nsp.$(BOARD_LOWER).firmware.upgrade.img ]; then \
	cp nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.$(BOARD).firmware.upgrade.img; \
	fi
                                                                                  
	$(MAKE) filesystem
	mv nsp.$(BOARD_LOWER).squashfs.img nsp.squashfs.ggsvca.img
	mv nsp.$(BOARD_LOWER).firmware.img nsp.firmware.ggsvca.img
	mv nsp.$(BOARD_LOWER).squashfs.upgrade.img nsp.squashfs.ggsvca.upgrade.img
	mv nsp.$(BOARD_LOWER).firmware.upgrade.img nsp.firmware.ggsvca.upgrade.img


# end - standalone voice builds
.PHONY: dimtest ggbasica ggsvca

#----------------------------------------------
# Do not change the order of the next 3 lines..
#----------------------------------------------
export TI_build_home = $(shell cat .bh_temp)
export TI_linux_home = $(shell cat .ti_linux_home)
include $(TARGET_HOME)/TI_Build.make

#----------------------------------------------
# Application targets
#----------------------------------------------
basefilesystem: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/basefilesystem -f Makefile.flat

uClibc:
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
#	$(MAKE) -C $(TARGET_HOME)/src/uClibc -f Makefile.flat clean
	ln -sf $(TARGET_HOME)/tools/configs/nsp_autoconf.h $(TARGET_HOME)/src/uClibc/include/nsp.h 
	$(MAKE) -C $(TARGET_HOME)/src/uClibc -f Makefile.flat
#	ln -sf $(TARGET_HOME)/tools/configs/nsp_autoconf.h tools/mipsel-linux-uclibc/include/nsp.h 


libatm: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/libatm -f Makefile.flat

libtomcrypt: TI_header
	@echo "*--------------------------------------------------"
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/libtomcrypt-1.0 -f Makefile.flat

libtommath: TI_header
	@echo "*--------------------------------------------------"
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/libtommath-1.0 -f Makefile.flat    

libdmalloc: TI_header
	@echo "*--------------------------------------------------"
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/libdmalloc -f Makefile.flat    
  
7zip: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/7zip -f Makefile.flat

cfgmgr: TI_header atmlib_check zlib_shared
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	DEVAREA=$(TARGET_HOME)/src/cfgmgr/src/cmcli $(MAKE) -C $(TARGET_HOME)/src/cfgmgr -f Makefile.flat BOARD=$(BOARD)

html: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/html -f Makefile.flat

brctl: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/bridge-utils -f Makefile.flat

dhcp-forwarder: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/dhcp-forwarder -f Makefile.flat

dproxy: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/dproxy-nexgen -f Makefile.flat

iptables: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/iptables -f Makefile.flat

mtd: TI_header zlib
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/mtd -f Makefile.flat

netkit-routed: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/netkit-routed -f Makefile.flat

netkit-routedv2: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/netkit-routedv2 -f Makefile.flat

nettools: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/nettools -f Makefile.flat

ppp-pppoe: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/ppp-pppoe -f Makefile.flat

thttpd: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/thttpd -f Makefile.flat

udhcp: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/udhcp -f Makefile.flat

zlib: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/zlib -f Makefile.flat

zlib_shared: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/zlib -f Makefile.shared

br2684ctl: TI_header atmlib_check
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/br2684ctl -f Makefile.flat

iproute2: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/iproute2 -f Makefile.flat

utelnetd: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/utelnetd -f Makefile.flat

busybox: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	if [ ! -e $(TARGET_HOME)/src/busybox/sysdeps/linux/nsp_config.in ]; then \
		sed -e  's/@@TARGET_HOME@@/$(shell cat $(TARGET_HOME)/.th_temp)/' \
		<src/busybox/sysdeps/linux/nsp_config.in.orig >src/busybox/sysdeps/linux/nsp_config.in; \
	fi
	$(MAKE) -C $(TARGET_HOME)/src/busybox -f Makefile.flat native_build
	$(MAKE) -C $(TARGET_HOME)/src/busybox -f Makefile.flat build install

upgrader: TI_header
	@echo "*--------------------------------------------------"
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/upgrader -f Makefile.flat

cramfs: zlib
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/cramfs -f Makefile.flat

squashfs: zlib
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/squashfs -f Makefile.flat

TI-chksum: 
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/TI-chksum -f Makefile.flat

TI_dsl_firmware:
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"

	$(MAKE) -C $(TARGET_HOME)/src/TI_dsl_firmware REL=$(BOARD) -f Makefile.flat
	$(MAKE) -C $(TARGET_HOME) -f Makefile.board build_dsl_annex 

mkconfig: zlib
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/cfgmgr/src/mkconfig -f Makefile.flat

boot: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
ifeq ($(CONFIG_MIPS_AVALANCHE_ADAM2),y)
	$(MAKE) -C $(TARGET_HOME)/src/Adam2/Src -f Makefile.flat BOARD_EXT=$(BOARD_EXT) TARGET_HOME=$(TARGET_HOME)
else
	$(MAKE) -C $(TARGET_HOME)/src/psp_boot -f Makefile.flat  BOARD_EXT=$(BOARD_EXT) TARGET_HOME=$(TARGET_HOME)
endif

TI-ledapp: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/TI-ledapp -f Makefile.flat 

TI-ledcfg: TI_header
	@echo "*--------------------------------------------------"
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/TI-ledcfg -f Makefile.flat

dropbear: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/dropbear -f Makefile.flat


igmp-proxy: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/igmp-proxy -f Makefile.flat

upnp: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/upnp -f Makefile.flat

foxy: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/foxy -f Makefile.flat

wlan: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/wlan -f Makefile.flat

bridge-utils: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/bridge-utils -f Makefile.flat

msntp: TI_header
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/msntp -f Makefile.flat

voice_app: TI_header cfgmgr
	@echo "*--------------------------------------------------" 
	@echo "*  TI Linux Distribution                           "
	@echo "*  Build date: $(shell date -R) :        "
	@echo "*  Building: $@                                    "
	@echo "*--------------------------------------------------"
	$(MAKE) -C $(TARGET_HOME)/src/voice_app -f Makefile.flat

#-------------------------------------------------
# Quick dependency checks
#-------------------------------------------------
TI_header:
	@if [ ! -e $(TARGET_HOME)/.bh_temp ]; then \
	$(MAKE) setup_paths; \
	fi
	@if [ ! -e $(TARGET_HOME)/.ti_linux_home ]; then \
	$(MAKE) setup_paths; \
	fi
	@if [ ! -e $(TARGET_HOME)/.config ]; then \
	$(MAKE) config_check; \
	fi
	@if [ ! -e $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/linux/nsp.h ]; then \
	$(MAKE) oldconfig; \
	fi
	@if [ ! -e $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/linux/autoconf.h ]; then \
	$(MAKE) -C $(TARGET_HOME)/src/kernel/$(TI_kernel_version) oldconfig; \
	fi
#	@if [ ! -e $(TARGET_HOME)/Makefile.board ]; then \
#	$(MAKE) setup_board; \
#	fi
	@if [ ! -e $(TARGET_HOME)/tools/mipsel-linux-uclibc/bin/mipsel-uclibc-gcc ]; then \
	$(MAKE) uClibc; \
	fi

#
# Make sure nsp_autoconf.h exists
#

atmlib_check:
	@if [ ! -e $(TARGET_HOME)/tools/mipsel-linux-uClibc/include/atm.h ]; then \
	$(MAKE) libatm; \
	fi

kernel_check:
	@if [ ! -e $(TARGET_HOME)/Makefile.board ]; then \
	$(MAKE) kernel_build BOARD=$(BOARD) DIST=$(DIST) BOARD_EXT=$(BOARD_EXT); \
	fi 

.PHONY: basefilesystem cfgmgr brctl dhcp-forwarder dproxy iptables mtd netkit-routed nettools 
.PHONY: ppp-pppoe thttpd udhcp zlib zlib_shared utelnetd busybox upgrader cramfs TI-chksum squashfs
#.PHONY: ppp-pppoe thttpd udhcp zlib zlib_shared br2684ctl utelnetd busybox upgrader cramfs TI-chksum squashfs
.PHONY: TI_dsl_firmware boot uClibc TI-ledcfg libatm TI_header atmlib_check dropbear igmp-proxy
.PHONY: upnp no_config filesystem iproute2 msntp voice_app


#-------------------------------------------------
#  Clean up ..
#-------------------------------------------------

clean: deep_clean

deep_clean: uClibc_clean libatm_clean cfgmgr_clean brctl_clean dhcp-forwarder_clean dproxy_clean \
iptables_clean mtd_clean nettools_clean thttpd_clean zlib_clean mkconfig_clean br2684ctl_clean \
utelnetd_clean busybox_clean upgrader_clean cramfs_clean TI-chksum_clean TI_dsl_firmware_clean \
kernel_clean boot_clean Build_environment images_clean  dropbear_clean igmp-proxy_clean upnp_clean \
7zip_clean iproute2_clean wlan_clean msntp_clean libtomcrypt_clean libtommath_clean voice_app_clean \
foxy_clean netkit-routedv2_clean squashfs_clean udhcp_clean TI-ledcfg_clean no_tools_build_clean

dist_clean: deep_clean kernel_distclean uClibc_distclean dhcp-forwarder_distclean ppp-pppoe_distclean \
netkit-routedv2_distclean thttpd_distclean foxy_distclean upnp_distclean config_clean

kernel_clean:
	$(MAKE) -C $(TARGET_HOME)/src/kernel/$(TI_kernel_version) clean
	@if [ -e $(TARGET_HOME)/Makefile.board ]; then \
		$(MAKE) -f Makefile.board $(BOARD)_$(DIST)_kernel_clean BOARD=$(BOARD) DIST=$(DIST) BOARD_EXT=$(BOARD_EXT); \
	fi
	rm -f $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/linux/autoconf.h
	rm -f $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/asm
	rm -f $(TARGET_HOME)/.did_dep


uClibc_clean:
	$(MAKE) -C $(TARGET_HOME)/src/uClibc -f Makefile.flat clean
libatm_clean:
	$(MAKE) -C $(TARGET_HOME)/src/libatm -f Makefile.flat clean
cfgmgr_clean:
	DEVAREA=$(TARGET_HOME)/src/cfgmgr/src/cmcli $(MAKE) -C $(TARGET_HOME)/src/cfgmgr -f Makefile.flat clean
brctl_clean:
	$(MAKE) -C $(TARGET_HOME)/src/bridge-utils -f Makefile.flat clean
dhcp-forwarder_clean:
	$(MAKE) -C $(TARGET_HOME)/src/dhcp-forwarder -f Makefile.flat clean
dproxy_clean:
	$(MAKE) -C $(TARGET_HOME)/src/dproxy-nexgen -f Makefile.flat clean
iptables_clean:
	$(MAKE) -C $(TARGET_HOME)/src/iptables -f Makefile.flat clean
mtd_clean:
	$(MAKE) -C $(TARGET_HOME)/src/mtd -f Makefile.flat clean
netkit-routed_clean:
	$(MAKE) -C $(TARGET_HOME)/src/netkit-routed -f Makefile.flat clean
netkit-routedv2_clean:
	$(MAKE) -C $(TARGET_HOME)/src/netkit-routedv2 -f Makefile.flat clean
nettools_clean:
	$(MAKE) -C $(TARGET_HOME)/src/nettools -f Makefile.flat clean
ppp-pppoe_clean:
	$(MAKE) -C $(TARGET_HOME)/src/ppp-pppoe -f Makefile.flat clean
thttpd_clean:
	$(MAKE) -C $(TARGET_HOME)/src/thttpd -f Makefile.flat clean
zlib_clean:
	$(MAKE) -C $(TARGET_HOME)/src/zlib -f Makefile.flat clean
mkconfig_clean:
	$(MAKE) -C $(TARGET_HOME)/src/cfgmgr/src/mkconfig -f Makefile.flat clean
br2684ctl_clean:
	$(MAKE) -C $(TARGET_HOME)/src/br2684ctl -f Makefile.flat clean
iproute2_clean:
	$(MAKE) -C $(TARGET_HOME)/src/iproute2 -f Makefile.flat clean
utelnetd_clean:
	$(MAKE) -C $(TARGET_HOME)/src/utelnetd -f Makefile.flat clean
busybox_clean:
	$(MAKE) -C $(TARGET_HOME)/src/busybox -f Makefile.flat clean
foxy_clean:
	$(MAKE) -C $(TARGET_HOME)/src/foxy -f Makefile.flat clean
upgrader_clean:
	$(MAKE) -C $(TARGET_HOME)/src/upgrader -f Makefile.flat clean
cramfs_clean:
	$(MAKE) -C $(TARGET_HOME)/src/cramfs -f Makefile.flat clean
squashfs_clean:
	$(MAKE) -C $(TARGET_HOME)/src/squashfs -f Makefile.flat clean
TI-chksum_clean:
	$(MAKE) -C $(TARGET_HOME)/src/TI-chksum -f Makefile.flat clean
TI_dsl_firmware_clean:
	$(MAKE) -C $(TARGET_HOME)/src/TI_dsl_firmware -f Makefile.flat clean
boot_clean:
ifeq ($(CONFIG_MIPS_AVALANCHE_ADAM2),y)
	$(MAKE) -C $(TARGET_HOME)/src/Adam2/Src -f Makefile.flat BOARD_EXT=$(BOARD_EXT) clean
	rm -f $(TARGET_HOME)/adam2.*.bin
else
	$(MAKE) -C $(TARGET_HOME)/src/psp_boot -f Makefile.flat BOARD_EXT=$(BOARD_EXT) clean
	rm -f $(TARGET_HOME)/psbl.*.bin
endif
TI-ledapp_clean:
	$(MAKE) -C $(TARGET_HOME)/src/TI-ledapp -f Makefile.flat clean
TI-ledcfg_clean:
	$(MAKE) -C $(TARGET_HOME)/src/TI-ledcfg -f Makefile.flat clean
dropbear_clean:
	$(MAKE) -C $(TARGET_HOME)/src/dropbear -f Makefile.flat clean
igmp-proxy_clean:
	$(MAKE) -C $(TARGET_HOME)/src/igmp-proxy -f Makefile.flat clean
upnp_clean:
	$(MAKE) -C $(TARGET_HOME)/src/upnp -f Makefile.flat clean	
7zip_clean:
	$(MAKE) -C $(TARGET_HOME)/src/7zip -f Makefile.flat clean

wlan_clean:
	@if [ -e $(TARGET_HOME)/src/wlan/Makefile.flat ]; then \
	$(MAKE) -C $(TARGET_HOME)/src/wlan -f Makefile.flat clean; \
	fi

msntp_clean:
	$(MAKE) -C $(TARGET_HOME)/src/msntp -f Makefile.flat clean

voice_app_clean:
	@if [ -e $(TARGET_HOME)/src/voice_app/Makefile.flat ]; then \
	$(MAKE) -C $(TARGET_HOME)/src/voice_app -f Makefile.flat clean; \
	fi

udhcp_clean:
	$(MAKE) -C $(TARGET_HOME)/src/udhcp -f Makefile.flat clean

libtomcrypt_clean:
	$(MAKE) -C $(TARGET_HOME)/src/libtomcrypt-1.0 -f Makefile.flat clean
libtommath_clean:
	$(MAKE) -C $(TARGET_HOME)/src/libtommath-1.0 -f Makefile.flat clean  
libdmalloc_clean:
	$(MAKE) -C $(TARGET_HOME)/src/libdmalloc -f Makefile.flat clean  

Build_environment: tools_clean
	rm -f $(TARGET_HOME)/.th_temp*
	rm -f $(TARGET_HOME)/.ti_linux_home
	rm -f $(TARGET_HOME)/.cmflags
	rm -f $(TARGET_HOME)/.bh_temp*
	rm -f $(TARGET_HOME)/.board
	@if [ -e $(TARGET_HOME)/Makefile.board ]; then \
		rm -f $(TARGET_HOME)/Makefile.board; \
	fi
	rm -rf $(TARGET_HOME)/filesystem

tools_clean:
	rm -f  $(TARGET_HOME)/.tmpconfig*
	rm -f  $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/include/linux/nsp.h
	rm -f  $(TARGET_HOME)/src/busybox/sysdeps/linux/nsp_config.in
	rm -f  $(TARGET_HOME)/src/uClibc/include/nsp.h
	rm -f  $(TARGET_HOME)/src/uClibc/extra/Configs/Config.nsp.mipsel
	rm -f  $(TARGET_HOME)/tools/bin/7zip 
	rm -f  $(TARGET_HOME)/tools/bin/erase
	rm -f  $(TARGET_HOME)/tools/bin/eraseall
	rm -f  $(TARGET_HOME)/tools/bin/lock
	rm -f  $(TARGET_HOME)/tools/bin/mkfs.jffs2
	rm -f  $(TARGET_HOME)/tools/bin/unlock

config_clean:
	-rm -f  $(TARGET_HOME)/.config
	-rm -f  $(TARGET_HOME)/.config.old
	-rm -f  $(TARGET_HOME)/tools/configs/config.in
	-rm -rf $(TARGET_HOME)/tools/configs/nsp_autoconf.h
	-rm -f  $(TARGET_HOME)/src/kernel/$(TI_kernel_version)/.config
	-rm -f  $(TARGET_HOME)/src/busybox/.config
	-rm -f  $(TARGET_HOME)/src/busybox/.tmpconfig
	-rm -f $(TARGET_HOME)/src/uClibc/.config
	-rm -f $(TARGET_HOME)/src/cfgmgr/.config

no_tools_build_clean:
	@if [ -d no_tools ]; then \
	rm -rf no_tools; \
	fi
	@if [ -d filesystem_temp ]; then \
	rm -rf filesystem_temp; \
	fi
	rm -f no_tools_build_$(BOARD)$(BOARD_EXT).tgz

images_clean:
	rm -f $(TARGET_HOME)/ram_zimage_pad*	

kernel_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/kernel/$(TI_kernel_version) distclean

uClibc_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/uClibc -f Makefile.flat distclean
	
dhcp-forwarder_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/dhcp-forwarder -f Makefile.flat distclean

ppp-pppoe_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/ppp-pppoe -f Makefile.flat distclean

netkit-routedv2_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/netkit-routedv2 -f Makefile.flat distclean

thttpd_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/thttpd -f Makefile.flat distclean

foxy_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/foxy -f Makefile.flat distclean

upnp_distclean:
	$(MAKE) -C $(TARGET_HOME)/src/upnp -f Makefile.flat distclean

.PHONY: pop_ver clean deep_clean cfgmgr_clean brctl_clean dhcp-forwarder_clean dproxy_clean iptables_clean mtd_clean netkit-routed_clean nettools_clean config_xml
.PHONY: ppp-pppoe_clean thttpd_clean udhcp_clean zlib_clean mkconfig_clean br2684ctl_clean utelnetd_clean busybox_clean upgrader_clean cramfs_clean TI-chksum_clean
.PHONY: TI_dsl_firmware_clean boot_clean uClibc_clean TI-ledcfg_clean netkit-routedv2_distclean
.PHONY: Build_environment images_clean kernel_clean squashfs_clean dropbear_clean igmp-proxy_clean upnp_clean 7zip_clean iproute2_clean
.PHONY: wlan_clean msntp_clean voice_app_clean uClibc_distclean kernel_distclean dhcp-forwarder_distclean
.PHONY: ppp-pppoe_distclean thttpd_distclean  foxy_distclean no_tools_build_clean upnp_distclean
