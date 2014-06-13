/*************************************************************************
 *    TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 * 
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains all the global constructs for the module : cpswhalcpusb
 *   This file is automatically created from cpswhalcpusb_global.c using parse_global version 1.00
 *
 *  HISTORY:
 *  Maintained in cpswhalcpusb_global.c 
 ***************************************************************************/
#ifndef _cpswhalcpusb_GLOBAL_H
#define _cpswhalcpusb_GLOBAL_H
/** \namespace CPUSB_Version
This documents version 02.00.00 of the CPUSB CPHAL.
*/

extern HAL_CONTROL_KEY hkVersion_CPUSB[];
extern HAL_CONTROL_KEY hkCpusbModuleName[];
extern HAL_CONTROL_KEY hkCpusb[];
extern HAL_CONTROL_KEY hkCpusbBase[];

/**
@defgroup CPUSB_Config_Params CPUSB Configuration Parameters

This section documents the CPUSB configuration parameters.  The OS may 
call the Control() function to perform various actions (see HAL_CONTROL_ACTION)
on these keys.  See the detailed documentation in the table for more
information about the usage of each key.

@{
*/

extern HAL_CONTROL_KEY hkOsSetup[];
/** @hideinitializer USB interrupt number. */
extern HAL_CONTROL_KEY hkUsbIntBit[];
/** @hideinitializer VBUS interrupt number. */
extern HAL_CONTROL_KEY hkVbusIntBit[];
/** @hideinitializer Remote Wakeup interrupt number. */
extern HAL_CONTROL_KEY hkRemWakeupIntBit[];

/** @hideinitializer USB Mode Select [HC_USB_MODE_DEFAULT,HC_USB_MODE_RNDIS] */
extern HAL_CONTROL_KEY hkModeSelect[];
/** @hideinitializer SRAM Latency Test.[TRUE,FALSE] */
extern HAL_CONTROL_KEY hkLatencyTest[];
/** @hideinitializer Specifies whether device is forced to Full Speed.[TRUE,FALSE] */
extern HAL_CONTROL_KEY hkFullSpeed[];
/** @hideinitializer USB Interrupt Mask. */
extern HAL_CONTROL_KEY hkUsbIntrMask[];
/** @hideinitializer Specifies whether device is big or little endian.[HC_BIG_ENDIAN,HC_LITTLE_ENDIAN] */
extern HAL_CONTROL_KEY hkChEndianess[];
/** @hideinitializer Specifies whether UCP is present or not.[TRUE,FALSE] */
extern HAL_CONTROL_KEY hkUcpPresent[];
/** @hideinitializer MIPS addressable UCP Register Base Address.*/
extern HAL_CONTROL_KEY hkUcpRegBase[];
/** @hideinitializer Mask to disable the UCP.*/
extern HAL_CONTROL_KEY hkUcpDisVal[];
/** @hideinitializer Mask to clear the UCP disable register.*/
extern HAL_CONTROL_KEY hkUcpDisClrMask[];
/** @hideinitializer MIPS addressable Clock Control Register Base Address.*/
extern HAL_CONTROL_KEY hkClkRegBase[];
/** @hideinitializer Mask to enable the clock for the USB device module.*/
extern HAL_CONTROL_KEY hkClkMask[];

/** @hideinitializer Specifies whether Remote Wakeup is present or not.[TRUE,FALSE] */
extern HAL_CONTROL_KEY hkRemWakeup[];
/** @hideinitializer MIPS addressable UCP Register Base Address.*/
extern HAL_CONTROL_KEY hkRemWakeupRegBase[];
/** @hideinitializer Specifies the mask for enabling the remote wakeup events.*/
extern HAL_CONTROL_KEY hkRemWakeupEventMask[];
/** @hideinitializer Specifies the mask to enable the remote wakeup interrupt*/
extern HAL_CONTROL_KEY hkRemWakeupIntrEn[];
/** @hideinitializer Specifies the mask to clear the active remote wakeup events*/
extern HAL_CONTROL_KEY hkRemWakeupEventClrMask[];
/** @hideinitializer Specifies whether Low Power mode is enabled.[TRUE,FALSE] */
extern HAL_CONTROL_KEY hkLowPwrMode[];
/** @hideinitializer MIPS addressable Low-Power Register Base Address.*/
extern HAL_CONTROL_KEY hkLowPwrRegBase[];
/** @hideinitializer Specifies the mask for enabling the low power mode.*/
extern HAL_CONTROL_KEY hkLowPwrModeMask[];

/* Parameters read by the OS */
/** @hideinitializer Returns the available buffer ram size. It is recommended that the os use this value to allocate endpoint buffer size.*/
extern HAL_CONTROL_KEY hkAvailBuffRamSize[];
/** @hideinitializer Specifies the USB device address. This can only be SETbyVAL and use device address 0 to clear the address*/
extern HAL_CONTROL_KEY hkUsbDevAddr[];
/** @hideinitializer Stalls the IN endpoint. The only valid actions are haSETbyVAL and haCLEAR. The value field represents the endpoint to be stalled.*/
extern HAL_CONTROL_KEY hkInStall[];
/** @hideinitializer Stalls the IN endpoint. The only valid actions are haSETbyVAL and haCLEAR. The value field represents the endpoint to be stalled.*/
extern HAL_CONTROL_KEY hkOutStall[];
/** @hideinitializer Writes the data direction of a control transaction to the USB control register. The only valid actions are haSETbyVAL. [HC_USB_DATADIR_OUT,HC_USB_DATADIR_IN]*/
extern HAL_CONTROL_KEY hkUsbDirCtrl[];
/** @hideinitializer Sets the USB device in the High Speed test mode specified in the value field. The only valid actions are haSETbyVAL.[HC_TEST_J_MODE,HC_TEST_K_MODE,HC_TEST_SE0_NAK_MODE,HC_TEST_PKT_MODE,HC_TEST_FORCE_EN_MODE]
*/
extern HAL_CONTROL_KEY hkUsbTestMode[];
/** @hideinitializer Set the USB hardware in remote wakeup mode. 0-Remote wakeup disabled, 1-Remote wakeup enabled. */
extern HAL_CONTROL_KEY hkUsbRemWakeup[];

/* Stats */
extern HAL_CONTROL_KEY hkWrongAckErrors[];
extern HAL_CONTROL_KEY hkRxErrors[];
extern HAL_CONTROL_KEY hkAckErrors[];
extern HAL_CONTROL_KEY hkNumSuspInt[];
extern HAL_CONTROL_KEY hkNumResrInt[];
extern HAL_CONTROL_KEY hkNumRstrInt[];
extern HAL_CONTROL_KEY hkNumSetupInt[];
extern HAL_CONTROL_KEY hkNumSetupOwInt[];
extern HAL_CONTROL_KEY hkNumIEP0Int[];
extern HAL_CONTROL_KEY hkNumVbusInt[];
extern HAL_CONTROL_KEY hkNumOEP0Int[];
extern HAL_CONTROL_KEY hkNumRxInt[];
extern HAL_CONTROL_KEY hkNumTxInt[];


/* 
Remove this - compile and test before deleting.
Register - USB INTR
HAL_CONTROL_KEY hkVbusConnect[]="VbusConn";
HAL_CONTROL_KEY hkVbusDisconnect[]="VbusDisconn";
HAL_CONTROL_KEY hkUsbSetupOW[]="SetupOwIntr";
HAL_CONTROL_KEY hkUsbResume[]="ResumeIntr";
HAL_CONTROL_KEY hkUsbReset[]="RstIntr";
HAL_CONTROL_KEY hkUsbSetup[]="SetupIntr";
HAL_CONTROL_KEY hkUsbIEP0Ack[]="IEP0Ack";
HAL_CONTROL_KEY hkUsbSuspend[]="SusrIntr";
HAL_CONTROL_KEY hkUsbSof[]="SofIntr";
HAL_CONTROL_KEY hkUsbOEP0Ack[]="OEP0Intr";
HAL_CONTROL_KEY hkSpeedSelect[]="SpeedSelect";
 @hideinitializer Specifies the USB VBUS state. 1- connect, 0-disconnected
HAL_CONTROL_KEY hkVbusState[]="VbusState";
@hideinitializer Specifies the mask for enabling CPPI channels
HAL_CONTROL_KEY hkChMask[]= "ChannelMask";
HAL_CONTROL_KEY hkInNakIE[]="InNakIE";
HAL_CONTROL_KEY hkOutNakIE[]="OutNakIE";  

 Control Endpoint Register Parameters
HAL_CONTROL_KEY hkINEPNak[]="INEPNak";
HAL_CONTROL_KEY hkOUTEPNak[]="OUTEPNak";
*/
/*@}*/
/**
@defgroup CPUSB_Channel_Config_Params Channel Configuration Parameters

This section documents the Channel configuration parameters.  The OS may call the halControl() function  with one of these keys. In order to access a particular channel, the OS should pass hkKey.Ch as the key, where hkKey is one of the keys from below and Ch is the channel number.<br>   
@{
*/
/* Channel Parameters*/
/** @hideinitializer CPPI Channel configuration parameter. Specifies the endpoint size for the IN endpoint.Use hkAvailBuffSize to make sure that the buffer size can be accomodated.*/
extern HAL_CONTROL_KEY hkINEpBuffSize[];
/** @hideinitializer Specifies the address of USB IN endpoint buffer X. This is an offset in the buffer RAM. */
extern HAL_CONTROL_KEY hkINEpBuffBaseAddrX[];
/** @hideinitializer Specifies the address of USB IN endpoint buffer Y. This is an offset in the buffer RAM. */
extern HAL_CONTROL_KEY hkINEpBuffBaseAddrY[];
/** @hideinitializer CPPI Channel configuration parameter. Specifies the endpoint size for the OUT endpoint. Use hkAvailBuffSize to make sure that the buffer size can be accomodated.*/
extern HAL_CONTROL_KEY hkOUTEpBuffSize[];
/** @hideinitializer Specifies the address of USB OUT endpoint buffer X. This is an offset in the buffer RAM. */
extern HAL_CONTROL_KEY hkOUTEpBuffBaseAddrX[];
/** @hideinitializer Specifies the address of USB OUT endpoint buffer Y. This is an offset in the buffer RAM. */
extern HAL_CONTROL_KEY hkOUTEpBuffBaseAddrY[];
/** @hideinitializer Specifies if double buffering is enabled. [TRUE,FALSE] */
extern HAL_CONTROL_KEY hkINEpDbuf[];
/** @hideinitializer Specifies if double buffering is enabled. [TRUE,FALSE] */
extern HAL_CONTROL_KEY hkOUTEpDbuf[];
/*@}*/

/**
@defgroup CPUSB_Control_Endpoint_Config_Params Control Endpoint Configuration Parameters 
The HAL provides a default configuration for the Control Endpoint 0. Control endpoint always has IN and OUT endpoints enabled. In the default configuration, the control endpoint buffer sizes are selected to be 64 bytes. Double buffering is not enabled for the endpoint buffers.The IN and OUT USB interrupts are enabled.<br>
This section documents the required control endpoint configuration parameters. The driver can apply a different configuration to the control endpoint using @c halControl(). Note : The base addresses for the control endpoint, for different configurations, is specified in a table in the USB2.0 module specification.<br>

@{
*/
/* Control Endpoint Parameters */
/** @hideinitializer Specifies the address of control endpoint buffers. Both IN and OUT endpoints have the same size.*/
extern HAL_CONTROL_KEY hkCtrlEPBuffSize[];
/** @hideinitializer Specifies if double buffering is enabled for IN and OUT control endpoint. [TRUE,FALSE] */
extern HAL_CONTROL_KEY hkCtrlEPDblBuff[];
/** @hideinitializer Specifies if USB IN ACK interrupt is enabled [TRUE,FALSE] */
extern HAL_CONTROL_KEY hkInUSBIE[];
/** @hideinitializer Specifies if USB OUT ACK interrupt is enabled [TRUE,FALSE] */
extern HAL_CONTROL_KEY hkOutUSBIE[];
/*@}*/
/**
@defgroup CPUSB_OS_Config_Params OS Configuration Parameters

This section documents the required OS configuration parameters.  The
HAL may call the osControl() function at any time with one of these
keys.  The required action is defined on a parameter by parameter
basis.

@{
*/

/*@}*/



#endif /*ends #ifndef _cpswhalcpusb_GLOBAL_H*/





