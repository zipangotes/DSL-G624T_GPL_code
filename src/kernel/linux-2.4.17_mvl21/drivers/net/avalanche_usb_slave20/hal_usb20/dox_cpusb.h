/*****************************************************************************
 *  TNETDxxxx Software Support
 *  Copyright (c) 2002 Texas Instruments Incorporated. All Rights Reserved.
 *
 *  FILE:   
 *
 *  DESCRIPTION:
 *      This file contains documentation for the CPUSB HAL2.0
 *
 *  HISTORY:
 *  @author Menaka Venkateswaran
 *  @version 1.00
 *  @date    09-Apr-2003
 *****************************************************************************/
#ifndef _INC_DOX_CPUSB
#define _INC_DOX_CPUSB
/**
@defgroup CPUSB_Functions Additional Functions for CPUSB Implementation

These functions are used only by the CPUSB module.
*/

/**
@page CPUSB_Implementation_Details

@section cpusb_ver Version

@copydoc CPUSB_Version

@section CPUSB_intro Introduction

The USB20 module has five endpoints (0-4). Endpoint 0 is the control endpoint.The CPPI DMAs operate only on endpoints 1-4. 

The USB device endpoints 1-4 are analogous to CPPI channels 0-3. They can be configured as bulk or interrupt. The CPUSB implementation has 4 CPPI channels that can transmit and/or receive. Each of the transmit and receive channels have 1 queue associated with it. Typical application's use of USB endpoints is fixed so it is recommended that all required channels be configured and torn down together. 

The USB device can be a full-speed or high-speed device. The host, to which the device is connected, determines the speed of the device. The OS can override the host speed selection by forcing the device to be a full speed device. This override option should be provided before @c Open(). <BR>

The USB device can operate in @p Transparent mode (default) or in @p RNDIS-Performance mode. In @p Transparent mode, each data packet must be equal to or less than the USB endpoint size. This is the default mode. Each packet generates a CPPI DMA interrupt. RNDIS-Performance mode is used to support RNDIS drivers. An RNDIS data packet is terminated by a short USB packet. A large RNDIS data packet can consist of several USB packets and generate one CPPI DMA interrupt. When transmitting large packets, it is recommended to operate in @p RNDIS-Performance mode for best driver/system performance as it significantly lowers the number of interrupts to be processed. <BR>


@section CPUSB_details API Implementation Details

ATTENTION: Documentation given here supplements the documentation given in the general
CPHAL API section.  The following details are crucial to correct usage of the
USB20 CPHAL.

@par Receive()
The least significant byte of @p Mode contains the channel number.At present, the remaining bytes of @p Mode are unsed. 

@par Send()
Valid channel numbers are 0-3. For the @p Mode parameter, The lower 8 bits of the Mode contains the Channel info. Bit 8 of the @p Mode is the zero byte enable bit. If this bit is set, the hardware transmits a zero byte packet. The fraglist transmitted in @c halSend() should contain a one-byte long packet with data 0xff. The most significant 16 bits of @p Mode contains the transmit buffer offset. @c Send() will reserve this many bytes at the beginning of the transmit buffer prior to the first byte of valid data. 
Mode Parameter Breakdown: <BR>
  -   31-16  Buffer Offset. <BR>
  -   15-09  Unused. <BR>
  -   08-08  Zero byte enable. <BR>
  -   07-00  Channel Number.

@par ChannelSetup()
The USB20 @c ChannelSetup() configures the Tx and Rx side of all channel connections in the same calls. Us DIRECTION_BOTH to specify that Tx and Rx needs to be configured.

@par ChannelTeardown()
The use of channels in typical USB applications is fixed and hence all setup channels have to be tore down simultaneously in a blocking operation. 

@par TeardownComplete()
The value for the @p Direction parameter should be ignored for the CPUSB implementation,since both directions (Tx and Rx) are always torndown in response to a @c ChannelTeardown() command.

@section CPUSB_params CPUSB Configuration Parameters
The USB HAL only supports a Push Model.

@latexonly
\input{group__CPUSB__Config__Params}
@endlatexonly

@latexonly
\input{group__CPUSB__Control__Endpoint__Config__Params}
@endlatexonly

@latexonly
\input{group__CPUSB__Channel__Config__Params}
@endlatexonly



*/

/**
 * @ingroup CPUSB_Functions
 * This function provides support for data transfers to and from the control endpoint buffers and system memory. 
 * This function can be called only after @c Open(). The driver must provide a pointer to an existing buffer. 
 * 
 *  
 * @param   HalDev     CPHAL module instance. (set by xxxInitModule())   
 * @param   p_len      The HAL returns the length of the data read in the p_len pointer
 * @param   p_data     The HAL returns the data read from the control endpoint's OUT buffer RAM X in the p_data pointer
 * @return  0 OK, Non-zero Not OK
 */
  int  (*CtrlEPReadX)    (HAL_DEVICE *HalDev, bit32u *p_len, bit32u *p_data);
/**
 * @ingroup CPUSB_Functions
 * This function provides support for data transfers to and from the control endpoint buffers and system memory. 
 * This function can be called only after @c Open(). The driver must provide a pointer to an existing buffer. 
 * 
 *  
 * @param   HalDev     CPHAL module instance. (set by xxxInitModule())   
 * @param   len        The OS passes the length of the data to be written.
 * @param   p_data     The HAL writes the data in p_data buffer to the control endpoint's IN buffer RAM X.
 *
 * @return  0 OK, Non-zero Not OK
 */
  int  (*CtrlEPWriteX)   (HAL_DEVICE *HalDev, bit32u len,  bit32u *p_data);
/**
 * @ingroup CPUSB_Functions
 * This function provides support for data transfers to and from the control endpoint buffers and system memory. 
 * This function can be called only after @c Open(). The driver must provide a pointer to an existing buffer. 
 * 
 *  
 * @param   HalDev     CPHAL module instance. (set by xxxInitModule())   
 * @param   p_len      The HAL returns the length of the data read in the p_len pointer
 * @param   p_data     The HAL returns the data read from the control endpoint's OUT buffer RAM Y in the p_data pointer   
 *
 * @return  0 OK, Non-zero Not OK
 */
  int  (*CtrlEPReadY)    (HAL_DEVICE *HalDev, bit32u *p_len, bit32u *p_data);
/**
 * @ingroup CPUSB_Functions
 * This function provides support for data transfers to and from the control endpoint buffers and system memory. 
 * This function can be called only after @c Open(). The driver must provide a pointer to an existing buffer. 
 * 
 *  
 * @param   HalDev     CPHAL module instance. (set by xxxInitModule())   
 * @param   len        The OS passes the length of the data to be written.
 * @param   p_data     The HAL writes the data in p_data buffer to the control endpoint's IN buffer RAM Y.
 *
 * @return  0 OK, Non-zero Not OK
 */
  int  (*CtrlEPWriteY)   (HAL_DEVICE *HalDev, bit32u len,  bit32u *p_data);

@section INTR_processing Interrupt processing
The USB20 device has three interrupts - USB, VBUS(CPPI) and remote wakeup. The OS driver should push the interrupt information to the HAL as a part of the device initialization. The HAL is not aware of VLYNQ. The HAL registers the interrupt service routine with the same number that was pushed to it. The OS can provide its own mechanism to deal with interrupts across VLYNQ. <br>


#endif
