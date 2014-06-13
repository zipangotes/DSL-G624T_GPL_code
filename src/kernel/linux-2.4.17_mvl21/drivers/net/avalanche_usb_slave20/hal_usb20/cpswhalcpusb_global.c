/******************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2003 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:
 *
 *  DESCRIPTION:
 *   This file contains all the global constructs for the CPUSB
 *
 *  HISTORY:
 *  08Jul03 Menaka Venkateswaran 1.00    Created
 *
 *****************************************************************************/
/** \namespace CPUSB_Version
This documents version 02.00.00 of the CPUSB CPHAL.
*/
#include <cpswhalcommon_stddef.h>

HAL_CONTROL_KEY hkVersion_CPUSB[]="CPUSB 02.00.05 "__DATE__" "__TIME__;
HAL_CONTROL_KEY hkCpusbModuleName[]="cpusb";
HAL_CONTROL_KEY hkCpusb[] = "cpusb";
HAL_CONTROL_KEY hkCpusbBase[] = "cpusbbase";

/**
@defgroup CPUSB_Config_Params CPUSB Configuration Parameters

This section documents the CPUSB configuration parameters.  The OS may 
call the Control() function to perform various actions (see HAL_CONTROL_ACTION)
on these keys.  See the detailed documentation in the table for more
information about the usage of each key.

@{
*/

HAL_CONTROL_KEY hkOsSetup[] = "ossetup";
/** @hideinitializer USB interrupt number. */
HAL_CONTROL_KEY hkUsbIntBit[]="UsbIntBit";
/** @hideinitializer VBUS interrupt number. */
HAL_CONTROL_KEY hkVbusIntBit[]="VbusIntBit";
/** @hideinitializer Remote Wakeup interrupt number. */
HAL_CONTROL_KEY hkRemWakeupIntBit[]="RemWkupIntBit";

/** @hideinitializer USB Mode Select [HC_USB_MODE_DEFAULT,HC_USB_MODE_RNDIS] */
HAL_CONTROL_KEY hkModeSelect[]="ModeSelect";
/** @hideinitializer SRAM Latency Test.[TRUE,FALSE] */
HAL_CONTROL_KEY hkLatencyTest[]= "LatencyTest";
/** @hideinitializer Specifies whether device is forced to Full Speed.[TRUE,FALSE] */
HAL_CONTROL_KEY hkFullSpeed[]="FullSpeed";
/** @hideinitializer USB Interrupt Mask. */
HAL_CONTROL_KEY hkUsbIntrMask[]= "UsbIntrMask";
/** @hideinitializer Specifies whether device is big or little endian.[HC_BIG_ENDIAN,HC_LITTLE_ENDIAN] */
HAL_CONTROL_KEY hkChEndianess[]= "ChEndianess";
/** @hideinitializer Specifies whether UCP is present or not.[TRUE,FALSE] */
HAL_CONTROL_KEY hkUcpPresent[]="UcpPresent";
/** @hideinitializer MIPS addressable UCP Register Base Address.*/
HAL_CONTROL_KEY hkUcpRegBase[]="UcpRegBase";
/** @hideinitializer Mask to disable the UCP.*/
HAL_CONTROL_KEY hkUcpDisVal[]="UcpDisVal";
/** @hideinitializer Mask to clear the UCP disable register.*/
HAL_CONTROL_KEY hkUcpDisClrMask[]="UcpClrMask";
/** @hideinitializer MIPS addressable Clock Control Register Base Address.*/
HAL_CONTROL_KEY hkClkRegBase[]="ClkRegBase";
/** @hideinitializer Mask to enable the clock for the USB device module.*/
HAL_CONTROL_KEY hkClkMask[]="ClkMask";

/** @hideinitializer Specifies whether Remote Wakeup is present or not.[TRUE,FALSE] */
HAL_CONTROL_KEY hkRemWakeup[]="RemWakeup";
/** @hideinitializer MIPS addressable UCP Register Base Address.*/
HAL_CONTROL_KEY hkRemWakeupRegBase[]="RemWakeupRegBase";
/** @hideinitializer Specifies the mask for enabling the remote wakeup events.*/
HAL_CONTROL_KEY hkRemWakeupEventMask[]="RemWkupEventMask";
/** @hideinitializer Specifies the mask to enable the remote wakeup interrupt*/
HAL_CONTROL_KEY hkRemWakeupIntrEn[]="RemWkupIntrEn";
/** @hideinitializer Specifies the mask to clear the active remote wakeup events*/
HAL_CONTROL_KEY hkRemWakeupEventClrMask[]="RemWkupClrMask";
/** @hideinitializer Specifies whether Low Power mode is enabled.[TRUE,FALSE] */
HAL_CONTROL_KEY hkLowPwrMode[]= "SerialSuspend";
/** @hideinitializer MIPS addressable Low-Power Register Base Address.*/
HAL_CONTROL_KEY hkLowPwrRegBase[]="LowPwrRegBase";
/** @hideinitializer Specifies the mask for enabling the low power mode.*/
HAL_CONTROL_KEY hkLowPwrModeMask[]="LowPwrModeMask";

/* Parameters read by the OS */
/** @hideinitializer Returns the available buffer ram size. It is recommended that the os use this value to allocate endpoint buffer size.*/
HAL_CONTROL_KEY hkAvailBuffRamSize[] = "BuffRam";
/** @hideinitializer Specifies the USB device address. This can only be SETbyVAL and use device address 0 to clear the address*/
HAL_CONTROL_KEY hkUsbDevAddr[]="UsbAddr";
/** @hideinitializer Stalls the IN endpoint. The only valid actions are haSETbyVAL and haCLEAR. The value field represents the endpoint to be stalled.*/
HAL_CONTROL_KEY hkInStall[] = "InStall";
/** @hideinitializer Stalls the IN endpoint. The only valid actions are haSETbyVAL and haCLEAR. The value field represents the endpoint to be stalled.*/
HAL_CONTROL_KEY hkOutStall[] = "OutStall";
/** @hideinitializer Writes the data direction of a control transaction to the USB control register. The only valid actions are haSETbyVAL. [HC_USB_DATADIR_OUT,HC_USB_DATADIR_IN]*/
HAL_CONTROL_KEY hkUsbDirCtrl[] = "UsbDir";
/** @hideinitializer Sets the USB device in the High Speed test mode specified in the value field. The only valid actions are haSETbyVAL.[HC_TEST_J_MODE,HC_TEST_K_MODE,HC_TEST_SE0_NAK_MODE,HC_TEST_PKT_MODE,HC_TEST_FORCE_EN_MODE]
*/
HAL_CONTROL_KEY hkUsbTestMode[] = "TestMode";
/** @hideinitializer Set the USB hardware in remote wakeup mode. 0-Remote wakeup disabled, 1-Remote wakeup enabled. */
HAL_CONTROL_KEY hkUsbRemWakeup[] = "RemWkup";

/* Stats */
HAL_CONTROL_KEY hkWrongAckErrors[]="WrongAck";
HAL_CONTROL_KEY hkRxErrors[]="RxErrors";
HAL_CONTROL_KEY hkAckErrors[]="AckErrors";
#if _CPHAL_STATS
HAL_CONTROL_KEY hkNumSuspInt[]="NumSuspInt";
HAL_CONTROL_KEY hkNumResrInt[]="NumResrInt";
HAL_CONTROL_KEY hkNumRstrInt[]="NumRstrInt";
HAL_CONTROL_KEY hkNumSetupInt[]="NumSetupInt";
HAL_CONTROL_KEY hkNumSetupOwInt[]="NumSetupOwInt";
HAL_CONTROL_KEY hkNumIEP0Int[]="NumIEP0Int";
HAL_CONTROL_KEY hkNumVbusInt[]="NumVbusInt";
HAL_CONTROL_KEY hkNumOEP0Int[]="NumOEP0Int";
HAL_CONTROL_KEY hkNumRxInt[]= "RxInt";
HAL_CONTROL_KEY hkNumTxInt[] = "TxInt";
#endif


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
HAL_CONTROL_KEY hkINEpBuffSize[]="INEpBuffSize";
/** @hideinitializer Specifies the address of USB IN endpoint buffer X. This is an offset in the buffer RAM. */
HAL_CONTROL_KEY hkINEpBuffBaseAddrX[]="INEpBuffBaseAddrX";
/** @hideinitializer Specifies the address of USB IN endpoint buffer Y. This is an offset in the buffer RAM. */
HAL_CONTROL_KEY hkINEpBuffBaseAddrY[]="INEpBuffBaseAddrY";
/** @hideinitializer CPPI Channel configuration parameter. Specifies the endpoint size for the OUT endpoint. Use hkAvailBuffSize to make sure that the buffer size can be accomodated.*/
HAL_CONTROL_KEY hkOUTEpBuffSize[]="OUTEpBuffSize";
/** @hideinitializer Specifies the address of USB OUT endpoint buffer X. This is an offset in the buffer RAM. */
HAL_CONTROL_KEY hkOUTEpBuffBaseAddrX[]="OUTEpBuffBaseAddrX";
/** @hideinitializer Specifies the address of USB OUT endpoint buffer Y. This is an offset in the buffer RAM. */
HAL_CONTROL_KEY hkOUTEpBuffBaseAddrY[]="OUTEpBuffBaseAddrY";
/** @hideinitializer Specifies if double buffering is enabled. [TRUE,FALSE] */
HAL_CONTROL_KEY hkINEpDbuf[]="INEpDbuf";
/** @hideinitializer Specifies if double buffering is enabled. [TRUE,FALSE] */
HAL_CONTROL_KEY hkOUTEpDbuf[]="OUTEpDbuf";
/*@}*/

/**
@defgroup CPUSB_Control_Endpoint_Config_Params Control Endpoint Configuration Parameters 
The HAL provides a default configuration for the Control Endpoint 0. Control endpoint always has IN and OUT endpoints enabled. In the default configuration, the control endpoint buffer sizes are selected to be 64 bytes. Double buffering is not enabled for the endpoint buffers.The IN and OUT USB interrupts are enabled.<br>
This section documents the required control endpoint configuration parameters. The driver can apply a different configuration to the control endpoint using @c halControl(). Note : The base addresses for the control endpoint, for different configurations, is specified in a table in the USB2.0 module specification.<br>

@{
*/
/* Control Endpoint Parameters */
/** @hideinitializer Specifies the address of control endpoint buffers. Both IN and OUT endpoints have the same size.*/
HAL_CONTROL_KEY hkCtrlEPBuffSize[]="CtrlEPBuffSize";
/** @hideinitializer Specifies if double buffering is enabled for IN and OUT control endpoint. [TRUE,FALSE] */
HAL_CONTROL_KEY hkCtrlEPDblBuff[]="CtrlEPDblBuff";
/** @hideinitializer Specifies if USB IN ACK interrupt is enabled [TRUE,FALSE] */
HAL_CONTROL_KEY hkInUSBIE[]="InUSBIE";
/** @hideinitializer Specifies if USB OUT ACK interrupt is enabled [TRUE,FALSE] */
HAL_CONTROL_KEY hkOutUSBIE[]="OutUSBIE";
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


