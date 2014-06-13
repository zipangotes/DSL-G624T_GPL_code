/****************************************************************************
        TNETD73xx Software Support
        Copyright(c) 2003, Texas Instruments Incorporated. All Rights Reserved.

        FILE: cpswhalcpusb_regs.h   Register definitions for the CPUSB module

         DESCRIPTION:
                This include file contains register definitions for the
                CPUSB module.

         HISTORY:
              10Oct03 MENAKA  Original version written
*****************************************************************************/
#ifndef _INC_CPSWHALCPUSB_REG
#define _INC_CPSWHALCPUSB_REG

#ifndef MEM_PTR
#define MEM_PTR volatile bit32u *
#endif

#define pUSB20_REV(base)                  ((MEM_PTR)(base+0x000))
#define USB20_REV(base)                   ( *pUSB20_REV(base))
/*CPPI Regs */
#define pUSB20_TXCTL(base)                 ((MEM_PTR)(base+0x004))
#define USB20_TXCTL(base)                  ( *pUSB20_TXCTL(base)) 
#define pUSB20_TXTRDN(base)                ((MEM_PTR)(base+0x008))
#define USB20_TXTRDN(base)                 ( *pUSB20_TXTRDN(base))
#define pUSB20_RXCTL(base)                 ((MEM_PTR)(base+0x014))
#define USB20_RXCTL(base)                  ( *pUSB20_RXCTL(base))
#define pUSB20_TXENDCTL(base)              ((MEM_PTR)(base+0x040))
#define USB20_TXENDCTL(base)               ( *pUSB20_TXENDCTL(base))
#define pUSB20_RXENDCTL(base)              ((MEM_PTR)(base+0x044))
#define USB20_RXENDCTL(base)               ( *pUSB20_RXENDCTL(base))
#define pUSB20_RXCH_FREEBUFCNT(base,ch)      ((MEM_PTR)(base+0x140+(0x4*ch)))
#define USB20_RXCH_FREEBUFCNT(base,ch)       ( *pUSB20_RXCH_FREEBUFCNT(base,ch))
#define pUSB20_RXCH0_FREEBUFCNT(base)      ((MEM_PTR)(base+0x140))
#define USB20_RXCH0_FREEBUFCNT(base)       ( *pUSB20_RXCH0_FREEBUFCNT(base))
#define pUSB20_RXCH1_FREEBUFCNT(base)      ((MEM_PTR)(base+0x144))
#define USB20_RXCH1_FREEBUFCNT(base)       ( *pUSB20_RXCH1_FREEBUFCNT(base))
#define pUSB20_RXCH2_FREEBUFCNT(base)      ((MEM_PTR)(base+0x148))
#define USB20_RXCH2_FREEBUFCNT(base)      ( *pUSB20_RXCH2_FREEBUFCNT(base))
#define pUSB20_RXCH3_FREEBUFCNT(base)      ((MEM_PTR)(base+0x14c)) 
#define USB20_RXCH3_FREEBUFCNT(base)       ( *pUSB20_RXCH3_FREEBUFCNT(base))
#define pUSB20_TXRAWINTR_STAT(base)        ((MEM_PTR)(base+0x170))
#define USB20_TXRAWINTR_STAT(base)         ( *pUSB20_TXRAWINTR_STAT(base))
#define pUSB20_TXMSKINTR_STAT(base)        ((MEM_PTR)(base+0x174)) 
#define USB20_TXMSKINTR_STAT(base)         ( *pUSB20_TXMSKINTR_STAT(base))
#define pUSB20_TXINTREN_SET(base)          ((MEM_PTR)(base+0x178)) 
#define USB20_TXINTREN_SET(base)           ( *pUSB20_TXINTREN_SET(base))
#define pUSB20_TXINTREN_CLR(base)          ((MEM_PTR)(base+0x17c)) 
#define USB20_TXINTREN_CLR(base)           ( *pUSB20_TXINTREN_CLR(base))
#define pUSB20_VBUS_INTVEC(base)           ((MEM_PTR)(base+0x180)) 
#define USB20_VBUS_INTVEC(base)            ( *pUSB20_VBUS_INTVEC(base))
#define pUSB20_VBUS_EOI(base)              ((MEM_PTR)(base+0x184)) 
#define USB20_VBUS_EOI(base)               ( *pUSB20_VBUS_EOI(base))
#define pUSB20_RXRAWINTR_STAT(base)        ((MEM_PTR)(base+0x190)) 
#define USB20_RXRAWINTR_STAT(base)         ( *pUSB20_RXRAWINTR_STAT(base))
#define pUSB20_RXMSKINTR_STAT(base)        ((MEM_PTR)(base+0x194)) 
#define USB20_RXMSKINTR_STAT(base)         ( *pUSB20_RXMSKINTR_STAT(base))
#define pUSB20_RXINTREN_SET(base)          ((MEM_PTR)(base+0x198)) 
#define USB20_RXINTREN_SET(base)           ( *pUSB20_RXINTREN_SET(base))
#define pUSB20_RXINTREN_CLR(base)          ((MEM_PTR)(base+0x19c)) 
#define USB20_RXINTREN_CLR(base)           ( *pUSB20_RXINTREN_CLR(base))
#define pUSB20_MODULE_RST_REG(base)        ((MEM_PTR)(base+0x1a0)) 
#define USB20_MODULE_RST_REG(base)         ( *pUSB20_MODULE_RST_REG(base))
#define pUSB20_TEARDOWN_REG(base)          ((MEM_PTR)(base+0x1a4)) 
#define USB20_TEARDOWN_REG(base)           ( *pUSB20_TEARDOWN_REG(base))


/* USB Regs */
#define pUSB20_USB_STAT(base)              ((MEM_PTR)(base+0x200)) 
#define USB20_USB_STAT(base)               ( *pUSB20_USB_STAT(base))
#define pUSB20_USB_CTRL(base)              ((MEM_PTR)(base+0x204)) 
#define USB20_USB_CTRL(base)                ( *pUSB20_USB_CTRL(base))
#define pUSB20_USB_IF_STAT(base)           ((MEM_PTR)(base+0x210)) 
#define USB20_USB_IF_STAT(base)            ( *pUSB20_USB_IF_STAT(base))
#define pUSB20_USB_IF_ERR(base)            ((MEM_PTR)(base+0x214)) 
#define USB20_USB_IF_ERR(base)             ( *pUSB20_USB_IF_ERR(base))
#define pUSB20_USB_IF_SM(base)             ((MEM_PTR)(base+0x218)) 
#define USB20_USB_IF_SM(base)              ( *pUSB20_USB_IF_SM(base))

/* Control Endpoint Registers  */
#define pUSB20_USB_EP0_CFG(base)           ((MEM_PTR)(base+0x220)) 
#define USB20_USB_EP0_CFG(base)            ( *pUSB20_USB_EP0_CFG(base))
#define pUSB20_USB_EP0_CNT(base)           ((MEM_PTR)(base+0x224)) 
#define USB20_USB_EP0_CNT(base)            ( *pUSB20_USB_EP0_CNT(base))

/* Other Endpoint Registers  */
#define pUSB20_EP_CFG(base,ch)           ( (MEM_PTR)(base+0x230+(ch*0x10)) ) 
#define USB20_EP_CFG(base,ch)            ( *pUSB20_EP_CFG(base,ch))
#define pUSB20_EP_IN_BC(base,ch)         ( (MEM_PTR)(base+0x234 + (ch*0x10)) ) 
#define USB20_EP_IN_BC(base,ch)          ( *pUSB20_EP_IN_BC(base,ch))
#define pUSB20_EP_OUT_BC(base,ch)        ( (MEM_PTR)(base+0x238+(ch*0x10)) ) 
#define USB20_EP_OUT_BC(base,ch)         ( *pUSB20_EP_OUT_BC(base,ch))
#define pUSB20_EP_AD(base,ch)            ( (MEM_PTR)(base+0x23c+(ch*0x10)) ) 
#define USB20_EP_AD(base,ch)             ( *pUSB20_EP_AD(base,ch))

#define pUSB20_USB_EP1_CFG(base)           ((MEM_PTR)(base+0x230)) 
#define USB20_USB_EP1_CFG(base)            ( *pUSB20_USB_EP1_CFG(base))
#define pUSB20_USB_EP1_IN_BC(base)         ((MEM_PTR)(base+0x234)) 
#define USB20_USB_EP1_IN_BC(base)          ( *pUSB20_USB_EP1_IN_BC(base))
#define pUSB20_USB_EP1_OUT_BC(base)        ((MEM_PTR)(base+0x238)) 
#define USB20_USB_EP1_OUT_BC(base)         ( *pUSB20_USB_EP1_OUT_BC(base))
#define pUSB20_USB_EP1_AD(base)            ((MEM_PTR)(base+0x23c)) 
#define USB20_USB_EP1_AD(base)             ( *pUSB20_USB_EP1_AD(base))
#define pUSB20_USB_EP2_CFG(base)           ((MEM_PTR)(base+0x240)) 
#define USB20_USB_EP2_CFG(base)            ( *pUSB20_USB_EP2_CFG(base))
#define pUSB20_USB_EP2_IN_BC(base)         ((MEM_PTR)(base+0x244)) 
#define USB20_USB_EP2_IN_BC(base)          ( *pUSB20_USB_EP2_IN_BC(base))
#define pUSB20_USB_EP2_OUT_BC(base)        ((MEM_PTR)(base+0x248)) 
#define USB20_USB_EP2_OUT_BC(base)         ( *pUSB20_USB_EP2_OUT_BC(base))
#define pUSB20_USB_EP2_AD(base)            ((MEM_PTR)(base+0x24c)) 
#define USB20_USB_EP2_AD(base)             ( *pUSB20_USB_EP2_AD(base))
#define pUSB20_USB_EP3_CFG(base)           ((MEM_PTR)(base+0x250)) 
#define USB20_USB_EP3_CFG(base)            ( *pUSB20_USB_EP3_CFG(base))
#define pUSB20_USB_EP3_IN_BC(base)         ((MEM_PTR)(base+0x254)) 
#define USB20_USB_EP3_IN_BC(base)          ( *pUSB20_USB_EP3_IN_BC(base))
#define pUSB20_USB_EP3_OUT_BC(base)        ((MEM_PTR)(base+0x258)) 
#define USB20_USB_EP3_OUT_BC(base)         ( *pUSB20_USB_EP3_OUT_BC(base))
#define pUSB20_USB_EP3_AD(base)            ((MEM_PTR)(base+0x25c)) 
#define USB20_USB_EP3_AD(base)             ( *pUSB20_USB_EP3_AD(base))
#define pUSB20_USB_EP4_CFG(base)           ((MEM_PTR)(base+0x260)) 
#define USB20_USB_EP4_CFG(base)            ( *pUSB20_USB_EP4_CFG(base))
#define pUSB20_USB_EP4_IN_BC(base)         ((MEM_PTR)(base+0x264)) 
#define USB20_USB_EP4_IN_BC(base)          ( *pUSB20_USB_EP4_IN_BC(base))
#define pUSB20_USB_EP4_OUT_BC(base)        ((MEM_PTR)(base+0x268)) 
#define USB20_USB_EP4_OUT_BC(base)         ( *pUSB20_USB_EP4_OUT_BC(base))
#define pUSB20_USB_EP4_AD(base)            ((MEM_PTR)(base+0x26c)) 
#define USB20_USB_EP4_AD(base)             ( *pUSB20_USB_EP4_AD(base))

/*  CPPI Regs  */
#define pUSB20_CPPI_IRP_MODESEL(base)      ((MEM_PTR)(base+0x300)) 
#define USB20_CPPI_IRP_MODESEL(base)       ( *pUSB20_CPPI_IRP_MODESEL(base))
#define pUSB20_CPPI_MODESEL(base)          ((MEM_PTR)(base+0x300)) 
#define USB20_CPPI_MODESEL(base)           ( *pUSB20_CPPI_MODESEL(base))
#define pUSB20_UCP_VIDPID(base)            ((MEM_PTR)(base+0x304)) 
#define USB20_UCP_VIDPID(base)             ( *pUSB20_UCP_VIDPID(base))
#define pUSB20_UCP_START(base)             ((MEM_PTR)(base+0x308)) 
#define USB20_UCP_START(base)              ( *pUSB20_UCP_START(base))
#define pUSB20_CELLDMA_EN(base)            ((MEM_PTR)(base+0x30c)) 
#define USB20_CELLDMA_EN(base)             ( *pUSB20_CELLDMA_EN(base))
#define pUSB20_USB_INTVEC(base)            ((MEM_PTR)(base+0x310)) 
#define USB20_USB_INTVEC(base)             ( *pUSB20_USB_INTVEC(base))
#define pUSB20_USB_EOI(base)               ((MEM_PTR)(base+0x314)) 
#define USB20_USB_EOI(base)                ( *pUSB20_USB_EOI(base))  



#define pUSB20_TXDMA_STWORD(base,ch,wd)          ((MEM_PTR)(base+0x800+ (wd*0x4) +(ch*0x40)))
#define USB20_TXDMA_STWORD(base,ch,wd)           ( *pUSB20_TXDMA_STWORD(base,ch,wd))
#define pUSB20_RXDMA_STWORD(base,ch,wd)          ((MEM_PTR)(base+0x820+ (wd*0x4) + (ch*0x40)))
#define USB20_RXDMA_STWORD(base,ch,wd)           ( *pUSB20_RXDMA_STWORD(base,ch,wd))
#define pUSB20_TXDMA_COMPPTR(base,ch)          ((MEM_PTR)(base+0x81c+(ch*0x40)))
#define USB20_TXDMA_COMPPTR(base,ch)           ( *pUSB20_TXDMA_COMPPTR(base,ch))
#define pUSB20_RXDMA_COMPPTR(base,ch)          ((MEM_PTR)(base+0x83c+(ch*0x40)))
#define USB20_RXDMA_COMPPTR(base,ch)           ( *pUSB20_RXDMA_COMPPTR(base,ch))

#define pUSB20_TXDMA_STWORD0(base,ch)          ((MEM_PTR)(base+0x800+(ch*0x40)))
#define USB20_TXDMA_STWORD0(base,ch)           ( *pUSB20_TXDMA_STWORD0(base,ch))
#define pUSB20_TXDMA_CH0_STWORD0(base)         ((MEM_PTR)(base+0x800)) 
#define USB20_TXDMA_CH0_STWORD0(base)          ( *pUSB20_TXDMA_CH0_STWORD0(base))
#define pUSB20_TXDMA_CH0_STWORD1(base)         ((MEM_PTR)(base+0x804)) 
#define USB20_TXDMA_CH0_STWORD1(base)          ( *pUSB20_TXDMA_CH0_STWORD1(base))
#define pUSB20_TXDMA_CH0_STWORD2(base)         ((MEM_PTR)(base+0x808)) 
#define USB20_TXDMA_CH0_STWORD2(base)          ( *pUSB20_TXDMA_CH0_STWORD2(base))
#define pUSB20_TXDMA_CH0_STWORD3(base)         ((MEM_PTR)(base+0x80c)) 
#define USB20_TXDMA_CH0_STWORD3(base)          ( *pUSB20_TXDMA_CH0_STWORD3(base))
#define pUSB20_TXDMA_CH0_STWORD4(base)         ((MEM_PTR)(base+0x810)) 
#define USB20_TXDMA_CH0_STWORD4(base)          ( *pUSB20_TXDMA_CH0_STWORD4(base))
#define pUSB20_TXDMA_CH0_STWORD5(base)         ((MEM_PTR)(base+0x814)) 
#define USB20_TXDMA_CH0_STWORD5(base)          ( *pUSB20_TXDMA_CH0_STWORD5(base))
#define pUSB20_TXDMA_CH0_COMPPTR(base)         ((MEM_PTR)(base+0x81c)) 
#define USB20_TXDMA_CH0_COMPPTR(base)          ( *pUSB20_TXDMA_CH0_COMPPTR(base))



#define pUSB20_RXDMA_STWORD1(base,ch)          ((MEM_PTR)(base+0x824+(ch*0x40)))
#define USB20_RXDMA_STWORD1(base,ch)           ( *pUSB20_RXDMA_STWORD1(base,ch))
#define pUSB20_RXDMA_CH0_STWORD0(base)         ((MEM_PTR)(base+0x820)) 
#define USB20_RXDMA_CH0_STWORD0(base)          ( *pUSB20_RXDMA_CH0_STWORD0(base))
#define pUSB20_RXDMA_CH0_STWORD1(base)         ((MEM_PTR)(base+0x824)) 
#define USB20_RXDMA_CH0_STWORD1(base)          ( *pUSB20_RXDMA_CH0_STWORD1(base))
#define pUSB20_RXDMA_CH0_STWORD2(base)         ((MEM_PTR)(base+0x828)) 
#define USB20_RXDMA_CH0_STWORD2(base)          ( *pUSB20_RXDMA_CH0_STWORD2(base))
#define pUSB20_RXDMA_CH0_STWORD3(base)         ((MEM_PTR)(base+0x82c)) 
#define USB20_RXDMA_CH0_STWORD3(base)          ( *pUSB20_RXDMA_CH0_STWORD3(base))
#define pUSB20_RXDMA_CH0_STWORD4(base)         ((MEM_PTR)(base+0x830)) 
#define USB20_RXDMA_CH0_STWORD4(base)          ( *USB20_RXDMA_CH0_STWORD3(base))
#define pUSB20_RXDMA_CH0_STWORD5(base)         ((MEM_PTR)(base+0x834)) 
#define USB20_RXDMA_CH0_STWORD5(base)          ( *pUSB20_RXDMA_CH0_STWORD5(base))
#define pUSB20_RXDMA_CH0_STWORD6(base)         ((MEM_PTR)(base+0x838)) 
#define USB20_RXDMA_CH0_STWORD6(base)          ( *pUSB20_RXDMA_CH0_STWORD6(base))
#define pUSB20_RXDMA_CH0_COMPPTR(base)         ((MEM_PTR)(base+0x83c)) 
#define USB20_RXDMA_CH0_COMPPTR(base)          ( *pUSB20_RXDMA_CH0_COMPPTR(base))

#define pUSB20_TXDMA_CH1_STWORD0(base)         ((MEM_PTR)(base+0x840)) 
#define USB20_TXDMA_CH1_STWORD0(base)          ( *pUSB20_TXDMA_CH1_STWORD0(base))
#define pUSB20_TXDMA_CH1_STWORD1(base)         ((MEM_PTR)(base+0x844)) 
#define USB20_TXDMA_CH1_STWORD1(base)          ( *pUSB20_TXDMA_CH1_STWORD1(base))
#define pUSB20_TXDMA_CH1_STWORD2(base)         ((MEM_PTR)(base+0x848)) 
#define USB20_TXDMA_CH1_STWORD2(base)          ( *pUSB20_TXDMA_CH1_STWORD2(base))
#define pUSB20_TXDMA_CH1_STWORD3(base)         ((MEM_PTR)(base+0x84c)) 
#define USB20_TXDMA_CH1_STWORD3(base)          ( *pUSB20_TXDMA_CH1_STWORD3(base))
#define pUSB20_TXDMA_CH1_STWORD4(base)         ((MEM_PTR)(base+0x850)) 
#define USB20_TXDMA_CH1_STWORD4(base)          ( *pUSB20_TXDMA_CH1_STWORD4(base))
#define pUSB20_TXDMA_CH1_STWORD5(base)         ((MEM_PTR)(base+0x854)) 
#define USB20_TXDMA_CH1_STWORD5(base)          ( *pUSB20_TXDMA_CH1_STWORD5(base))
#define pUSB20_TXDMA_CH1_COMPPTR(base)         ((MEM_PTR)(base+0x85c)) 
#define USB20_TXDMA_CH1_COMPPTR(base)          ( *pUSB20_TXDMA_CH1_COMPPTR(base))

#define pUSB20_RXDMA_CH1_STWORD0(base)         ((MEM_PTR)(base+0x860)) 
#define USB20_RXDMA_CH1_STWORD0(base)          ( *pUSB20_RXDMA_CH1_STWORD0(base))
#define pUSB20_RXDMA_CH1_STWORD1(base)         ((MEM_PTR)(base+0x864)) 
#define USB20_RXDMA_CH1_STWORD1(base)          ( *pUSB20_RXDMA_CH1_STWORD1(base))
#define pUSB20_RXDMA_CH1_STWORD2(base)         ((MEM_PTR)(base+0x868)) 
#define USB20_RXDMA_CH1_STWORD2(base)          ( *pUSB20_RXDMA_CH1_STWORD2(base))
#define pUSB20_RXDMA_CH1_STWORD3(base)         ((MEM_PTR)(base+0x86c)) 
#define USB20_RXDMA_CH1_STWORD3(base)          ( *pUSB20_RXDMA_CH1_STWORD3(base))
#define pUSB20_RXDMA_CH1_STWORD4(base)         ((MEM_PTR)(base+0x870)) 
#define USB20_RXDMA_CH1_STWORD4(base)          ( *pUSB20_RXDMA_CH1_STWORD4(base))
#define pUSB20_RXDMA_CH1_STWORD5(base)         ((MEM_PTR)(base+0x874)) 
#define USB20_RXDMA_CH1_STWORD5(base)          ( *pUSB20_RXDMA_CH1_STWORD5(base))
#define pUSB20_RXDMA_CH1_STWORD6(base)         ((MEM_PTR)(base+0x878)) 
#define USB20_RXDMA_CH1_STWORD6(base)          ( *pUSB20_RXDMA_CH1_STWORD6(base))
#define pUSB20_RXDMA_CH1_COMPPTR(base)         ((MEM_PTR)(base+0x87c)) 
#define USB20_RXDMA_CH1_COMPPTR(base)          ( *pUSB20_RXDMA_CH1_COMPPTR(base))

#define pUSB20_TXDMA_CH2_STWORD0(base)         ((MEM_PTR)(base+0x880)) 
#define USB20_TXDMA_CH2_STWORD0(base)          ( *pUSB20_TXDMA_CH2_STWORD0(base))
#define pUSB20_TXDMA_CH2_STWORD1(base)         ((MEM_PTR)(base+0x884)) 
#define USB20_TXDMA_CH2_STWORD1(base)          ( *pUSB20_TXDMA_CH2_STWORD1(base))
#define pUSB20_TXDMA_CH2_STWORD2(base)         ((MEM_PTR)(base+0x888)) 
#define USB20_TXDMA_CH2_STWORD2(base)          ( *pUSB20_TXDMA_CH2_STWORD2(base))
#define pUSB20_TXDMA_CH2_STWORD3(base)         ((MEM_PTR)(base+0x88c)) 
#define USB20_TXDMA_CH2_STWORD3(base)          ( *pUSB20_TXDMA_CH2_STWORD3(base))
#define pUSB20_TXDMA_CH2_STWORD4(base)         ((MEM_PTR)(base+0x890)) 
#define USB20_TXDMA_CH2_STWORD4(base)          ( *pUSB20_TXDMA_CH2_STWORD4(base))
#define pUSB20_TXDMA_CH2_STWORD5(base)         ((MEM_PTR)(base+0x894)) 
#define USB20_TXDMA_CH2_STWORD5(base)          ( *pUSB20_TXDMA_CH2_STWORD5(base))
#define pUSB20_TXDMA_CH2_COMPPTR(base)         ((MEM_PTR)(base+0x89c)) 
#define USB20_TXDMA_CH2_COMPPTR(base)          ( *pUSB20_TXDMA_CH2_COMPPTR(base))

#define pUSB20_RXDMA_CH2_STWORD0(base)         ((MEM_PTR)(base+0x8a0)) 
#define USB20_RXDMA_CH2_STWORD0(base)          ( *pUSB20_RXDMA_CH2_STWORD0(base))
#define pUSB20_RXDMA_CH2_STWORD1(base)         ((MEM_PTR)(base+0x8a4)) 
#define USB20_RXDMA_CH2_STWORD1(base)          ( *pUSB20_RXDMA_CH2_STWORD1(base))
#define pUSB20_RXDMA_CH2_STWORD2(base)         ((MEM_PTR)(base+0x8a8)) 
#define USB20_RXDMA_CH2_STWORD2(base)          ( *pUSB20_RXDMA_CH2_STWORD2(base))
#define pUSB20_RXDMA_CH2_STWORD3(base)         ((MEM_PTR)(base+0x8ac)) 
#define USB20_RXDMA_CH2_STWORD3(base)           ( *pUSB20_RXDMA_CH2_STWORD3(base))
#define pUSB20_RXDMA_CH2_STWORD4(base)         ((MEM_PTR)(base+0x8b0)) 
#define USB20_RXDMA_CH2_STWORD4(base)          ( *pUSB20_RXDMA_CH2_STWORD4(base))
#define pUSB20_RXDMA_CH2_STWORD5(base)         ((MEM_PTR)(base+0x8b4)) 
#define USB20_RXDMA_CH2_STWORD5(base)          ( *pUSB20_RXDMA_CH2_STWORD5(base))
#define pUSB20_RXDMA_CH2_STWORD6(base)         ((MEM_PTR)(base+0x8b8)) 
#define USB20_RXDMA_CH2_STWORD6(base)          ( *pUSB20_RXDMA_CH2_STWORD6(base))
#define pUSB20_RXDMA_CH2_COMPPTR(base)         ((MEM_PTR)(base+0x8bc)) 
#define USB20_RXDMA_CH2_COMPPTR(base)          ( *pUSB20_RXDMA_CH2_COMPPTR(base))

#define pUSB20_TXDMA_CH3_STWORD0(base)         ((MEM_PTR)(base+0x8c0)) 
#define USB20_TXDMA_CH3_STWORD0(base)          ( *pUSB20_TXDMA_CH3_STWORD0(base))
#define pUSB20_TXDMA_CH3_STWORD1(base)         ((MEM_PTR)(base+0x8c4)) 
#define USB20_TXDMA_CH3_STWORD1(base)          ( *pUSB20_TXDMA_CH3_STWORD1(base))
#define pUSB20_TXDMA_CH3_STWORD2(base)         ((MEM_PTR)(base+0x8c8)) 
#define USB20_TXDMA_CH3_STWORD2(base)          ( *pUSB20_TXDMA_CH3_STWORD2(base))
#define pUSB20_TXDMA_CH3_STWORD3(base)         ((MEM_PTR)(base+0x8cc)) 
#define USB20_TXDMA_CH3_STWORD3(base)          ( *pUSB20_TXDMA_CH3_STWORD3(base))
#define pUSB20_TXDMA_CH3_STWORD4(base)         ((MEM_PTR)(base+0x8d0)) 
#define USB20_TXDMA_CH3_STWORD4(base)          ( *pUSB20_TXDMA_CH3_STWORD4(base))
#define pUSB20_TXDMA_CH3_STWORD5(base)         ((MEM_PTR)(base+0x8d4)) 
#define USB20_TXDMA_CH3_STWORD5(base)          ( *pUSB20_TXDMA_CH3_STWORD5(base))
#define pUSB20_TXDMA_CH3_COMPPTR(base)         ((MEM_PTR)(base+0x8dc)) 
#define USB20_TXDMA_CH3_COMPPTR(base)          ( *pUSB20_TXDMA_CH3_COMPPTR(base))

#define pUSB20_RXDMA_CH3_STWORD0(base)         ((MEM_PTR)(base+0x8e0)) 
#define USB20_RXDMA_CH3_STWORD0(base)         ( *pUSB20_RXDMA_CH3_STWORD0(base))
#define pUSB20_RXDMA_CH3_STWORD1(base)         ((MEM_PTR)(base+0x8e4)) 
#define USB20_RXDMA_CH3_STWORD1(base)          ( *pUSB20_RXDMA_CH3_STWORD1(base))
#define pUSB20_RXDMA_CH3_STWORD2(base)         ((MEM_PTR)(base+0x8e8)) 
#define USB20_RXDMA_CH3_STWORD2(base)          ( *pUSB20_RXDMA_CH3_STWORD2(base))
#define pUSB20_RXDMA_CH3_STWORD3(base)         ((MEM_PTR)(base+0x8ec)) 
#define USB20_RXDMA_CH3_STWORD3(base)          ( *pUSB20_RXDMA_CH3_STWORD3(base))
#define pUSB20_RXDMA_CH3_STWORD4(base)         ((MEM_PTR)(base+0x8f0)) 
#define USB20_RXDMA_CH3_STWORD4(base)          ( *pUSB20_RXDMA_CH3_STWORD4(base))
#define pUSB20_RXDMA_CH3_STWORD5(base)         ((MEM_PTR)(base+0x8f4)) 
#define USB20_RXDMA_CH3_STWORD5(base)          ( *pUSB20_RXDMA_CH3_STWORD5(base))
#define pUSB20_RXDMA_CH3_STWORD6(base)         ((MEM_PTR)(base+0x8f8)) 
#define USB20_RXDMA_CH3_STWORD6(base)          ( *pUSB20_RXDMA_CH3_STWORD6(base))
#define pUSB20_RXDMA_CH3_COMPPTR(base)         ((MEM_PTR)(base+0x8fc)) 
#define USB20_RXDMA_CH3_COMPPTR(base)          ( *pUSB20_RXDMA_CH3_COMPPTR(base))

#define pUSB20_USB_BUFFRAM(base)               ((MEM_PTR)(base+0x1000)) 
#define USB20_USB_BUFFRAM(base)                ( *pUSB20_USB_BUFFRAM(base))
#define USB20_USB_BUFFRAM_SIZE                 0x1000

/****************************************************************************/
/*                                                                          */
/*         R E G I S T E R  B I T  D E F I N I T I O N S                    */
/*                                                                          */
/****************************************************************************/

/* USB CONTROL */
#define CONT                (1 << 7)
#define FULL_SPEED          (5 << 1)
#define RWUPEN              (1 << 5 )

/* TX_CONTROL */
#define TX_EN               (1 << 0)

/* RX_CONTROL */
#define RX_EN               (1 << 0)

/* ENDPOINT CONFIG 1-4 */

#define OEP_EN              (1 << 15)
#define OEP_NAK_IE          (1 << 14)
#define OEP_DBUF            (1 << 12)
#define OEP_STALL           (1 << 11)
#define OEP_ACKIE           (1 << 10)
#define IEP_EN              (1 << 7)
#define IEP_NAK_IE          (1 << 6)
#define IEP_DBUF            (1 << 4)
#define IEP_STALL           (1 << 3)
#define IEP_ACKIE           (1 << 2)

/* CELL_DMA_EN */
#define RX_CELLDMA_EN       (1 << 0)
#define TX_CELLDMA_EN       (1 << 1)

/* SPEED SELECT BIT */
#define TRANSCEIVER_SPEED_SELECT (1<<5)


/* Error shift bits */
#define RX_CRC_ERRORS_SFT 24
#define WRONG_ACK_ERRORS_SFT 16
#define ACK_ERRORS_SFT 8
#define RX_ERRORCNT_SFT 0

#define CPUSB_EP0_IN_EN          0x00000080
#define CPUSB_EP0_OUT_EN         0x00800000
#define CPUSB_EP0_IN_INT_EN      0x00000004
#define CPUSB_EP0_OUT_INT_EN     0x00040000

#endif /* _INC_CPSWHALCPUSB_REG_H */
