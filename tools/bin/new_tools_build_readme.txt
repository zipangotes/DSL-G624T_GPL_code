                   No Tools Build
                   ==============

Description
===========
No tools build is meant for customers not having Monta-vista tools chain to 
build the NSP image.Typical application is to allow such customers to allow 
changing of the web pages and to recreate the NSP image with these changes.

no_tools_build package (is present in the nsp release package from CM as 
no_tools_build_<BOARD>_<Bootloader>_<Ethernet option>.tgz)  is created 
using the filesystem, kernel and bootloaderimages created from the regular 
builds (from nsp directory). Here is the typical sequence to build it from 
nsp folder

make dist_clean
make setconfig -> select board, ethernet type, voice option etc
make nsp
make boot
make no_tools_build

This will create the tgz file, no_tools_build_<board>.tgz. CM rename it to 
include board type , boot loader type and ethernet option type in the filename. 
One would find this under nsp directory of the software release package. Also 
copied as part of the binary image package.These images are the same as the 
one build using monta-vista tool. This provides a way to build the filesystem 
image without the monta-vista tool. 

The no_tools_build pakage include following 

bin - All scripts required for the no_tools_build

nsp single images
------------------
nsp.annexB.firmware.img  
nsp.annexB.firmware.upgrade.img
nsp.annexA.firmware.img
nsp.annexA.firmware.upgrade.img

where annexA is nsp with DSL datapump and  annexB is nsp with ISDN datapump.

nsp filesystem upgrade images
---------------------------
nsp.annexA.squashfs.upgrade.img
nsp.annexB.squashfs.upgrade.img

Kernel images
--------------------- 
ram_zimage.ar7xxx.nsp.squashfs.bin
ram_zimage.ar7xxx.nsp.squashfs.upgrade.bin

config image
------------
config.ar7xxx.img

bootloader image
----------------
psbl.<BOARD>.bin 
or
adam2.<BOARD>.bin

no_tools_build - script to build nsp image using this package.

Steps to follow to build from no_tools_build package
----------------------------------------------------
untar the package under a linux directory.
cd no_tools

to re-build the images, run the following script

>./no_tools_build

