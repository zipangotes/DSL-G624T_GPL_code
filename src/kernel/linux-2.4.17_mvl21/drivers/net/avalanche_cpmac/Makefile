# File: drivers/net/avalanche_cpmac/Makefile
#
# Makefile for the Linux network (CPMAC) device drivers.
#
# DDA
#EXTRA_CFLAGS        += -DCPMAC_DDA_DEBUG
#EXTRA_CFLAGS        += -DPAL_INCLUDE_OSMEM -DPAL_INCLUDE_OSPROTECT 
EXTRA_CFLAGS        += -DCPMAC_CACHE_WRITEBACK_MODE 
EXTRA_CFLAGS        += -DCPMAC_USE_TASKLET_MODE 
#EXTRA_CFLAGS        += -DCPMAC_DDA_CACHE_INVALIDATE_FIX


# DDC
#EXTRA_CFLAGS        += -DCPMAC_DDC_DEBUG
#EXTRA_CFLAGS        += -DCPMAC_DDC_USE_ASSEMBLY
EXTRA_CFLAGS        += -DCPMAC_DDC_MIPS_OPTIMIZED

# Common
#EXTRA_CFLAGS        += -DCPMAC_MULTIFRAGMENT
EXTRA_CFLAGS        += -DCPMAC_MULTIPACKET_TX_COMPLETE_NOTIFY 
EXTRA_CFLAGS        += -DCPMAC_MULTIPACKET_RX_COMPLETE_NOTIFY 

#EXTRA_CFLAGS += -DCPMAC_CAPTURE_PERF_STATS


# Configuration
# Note: For testing we are use ENV variable 
#EXTRA_CFLAGS        += -DCPMAC_USE_CONFIG_SERVICE
EXTRA_CFLAGS        += -DCPMAC_USE_ENV

#INCLUDE PATH FOR MIB SPECIFIC HEADER FILES

EXTRA_CFLAGS        += -I$(TOPDIR)/include/asm/avalanche/generic



O_TARGET := avalanche_cpmac.o


list-multi := avalanche_cpmac.o
obj-$(CONFIG_MIPS_AVALANCHE_CPMAC) := avalanche_cpmac.o
#export-objs :=  DDA_switchLinux.o

#avalanche_cpmac-objs += cpmacNetLx.o ddc_cpmac.o ddc_cpmacTxRx.o  cpmacNetLxTxRx.o cpswhalcommon_miimdio.o  DDA_switchLinux.o CSL_switch.o CSL_marvell6063.o DDC_switch.o
avalanche_cpmac-objs += cpmacNetLx.o ddc_cpmac.o ddc_cpmacTxRx.o  cpmacNetLxTxRx.o cpswhalcommon_miimdio.o
ifeq ($(CONFIG_MIPS_EXTERNAL_SWITCH),y)
avalanche_cpmac-objs += dda_switch.o csl_switch.o ddc_switch.o
endif
ifeq ($(CONFIG_MIPS_AVALANCHE_MARVELL_6063),y)
avalanche_cpmac-objs += csl_marvell6063.o
endif

#avalanche_cpmac-objs += cpmacNetLx.o ddc_cpmac.o cpmacPalLx.o cpmacNetLxTxRx.o ddc_cpmacTxRx.o cpswhalcommon_miimdio.o

include $(TOPDIR)/Rules.make


avalanche_cpmac.o:        $(avalanche_cpmac-objs)
	$(LD) -r -o $@ $(avalanche_cpmac-objs)



clean:
	rm -f core *.o *.a *.s
