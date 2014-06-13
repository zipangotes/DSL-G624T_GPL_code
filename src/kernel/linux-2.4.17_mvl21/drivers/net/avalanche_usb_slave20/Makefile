# File: drivers/net/avalanche_usb_slave20/Makefile
#
# Makefile for the Linux network V2USB slave 2.0  device drivers.
#

O_TARGET := avalanche_usb_slave20.o

EXTRA_CFLAGS += -Ihal_usb20 -Ihal_common -Idrv_core -I.

list-multi := avalanche_usb_slave20.o

obj-$(CONFIG_MIPS_AVALANCHE_USB_SLAVE20) := avalanche_usb_slave20.o


# USB 2.0 HAL  objs
avalanche_usb_slave20-objs += hal_usb20/cpswhalcpusb_global.o\
							  hal_usb20/cpswhalcpusb_cppi.o\
							  hal_usb20/cpswhalcpusb.o \
           					  hal_usb20/cpswhalcpusb_isr.o\
                              hal_usb20/cpswhalcpusb_params.o

# HAL common objects
avalanche_usb_slave20-objs += hal_common/cpswhalcommon_global.o\
		                      hal_common/cpswhalcommon_params.o\
                              hal_common/cpswhalcommon_support.o



# Driver Core objs
avalanche_usb_slave20-objs += drv_core/usb20_lib.o\
							  drv_core/usb20_rndis.o\
							  drv_core/usb20_drvrndis.o\
                    		  drv_core/usb20_drvapi.o\
							  drv_core/usb20_state.o\
                              drv_core/usb20_drv.o\
							  drv_core/usb20_drvhal.o

# Driver Linux Adaptation obs
avalanche_usb_slave20-objs += usb20_config.o\
					          usb20_os_linux.o\
					          usb20_RndisEnd_Lx.o \
                              usb_sal.o 

include $(TOPDIR)/Rules.make


avalanche_usb_slave20.o:        $(avalanche_usb_slave20-objs)
	$(LD) -r -o $@ $(avalanche_usb_slave20-objs)



clean:
	rm -rf core *.o *.a *.s
