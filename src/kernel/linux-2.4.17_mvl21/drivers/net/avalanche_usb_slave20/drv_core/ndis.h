/*++

Copyright (c) 1993 Microsoft Corporation

Module name:

  ndis.h

Abstract:

    Main header file for the NDIS 3.X Wrapper, Macs and Protocols


Revision History

    31-Jul-91   Original

--*/


/* INC */

#ifndef _NDIS_
#define _NDIS_

#define NDIS_MAJOR_VERSION          0x03
#define NDIS_MINOR_VERSION          0x0A


/* ************************************************** */
/* MLR/FNI: I added these from the nt ddk (begin)	  */
/* ************************************************** */

/* Defines the state of the LAN media */
/* */
typedef enum _NDIS_MEDIA_STATE
{
	NdisMediaStateConnected,
	NdisMediaStateDisconnected
} NDIS_MEDIA_STATE, *PNDIS_MEDIA_STATE;


/* */
/*	PnP and PM OIDs */
/* */
#define	OID_PNP_CAPABILITIES					0xFD010100
#define	OID_PNP_SET_POWER						0xFD010101
#define	OID_PNP_QUERY_POWER						0xFD010102
#define OID_PNP_ADD_WAKE_UP_PATTERN				0xFD010103
#define OID_PNP_REMOVE_WAKE_UP_PATTERN			0xFD010104
#define	OID_PNP_WAKE_UP_PATTERN_LIST			0xFD010105
#define	OID_PNP_ENABLE_WAKE_UP					0xFD010106


/* the following flags define the -enabled- wake-up capabilities of the device */
/* passed in the Flags field of NDIS_PNP_CAPABILITIES structure */
/* */
#define NDIS_DEVICE_WAKE_UP_ENABLE          0x00000001

/* */
/*	The following bits are defined for OID_PNP_ENABLE_WAKE_UP */
/* */
#define	NDIS_PNP_WAKE_UP_MAGIC_PACKET			0x00000001
#define	NDIS_PNP_WAKE_UP_PATTERN_MATCH			0x00000002
#define	NDIS_PNP_WAKE_UP_LINK_CHANGE			0x00000004

/* */
/*	The following structure defines the device power states. */
/* */
typedef enum _NDIS_DEVICE_POWER_STATE
{
	NdisDeviceStateUnspecified = 0,
	NdisDeviceStateD0,
	NdisDeviceStateD1,
	NdisDeviceStateD2,
	NdisDeviceStateD3,
	NdisDeviceStateMaximum
} NDIS_DEVICE_POWER_STATE, *PNDIS_DEVICE_POWER_STATE;

/* */
/*	NDIS PnP routines and definitions. */
/* */
typedef struct _NDIS_PM_PACKET_PATTERN
{
	ULONG	Priority;					/* Importance of the given pattern. */
	ULONG	Reserved;					/* Context information for transports. */
	ULONG	MaskSize;					/* Size in bytes of the pattern mask. */
	ULONG	PatternOffset;				/* Offset from beginning of this */
										/* structure to the pattern bytes. */
	ULONG	PatternSize;				/* Size in bytes of the pattern. */
	ULONG	PatternFlags;				/* Flags (TBD). */
} NDIS_PM_PACKET_PATTERN, *PNDIS_PM_PACKET_PATTERN;


/* ************************************************** */
/* MLR/FNI: I added these from the nt ddk (end)		  */
/* ************************************************** */


/* INC */
#define BUFFER_POOL_SIGN     (UINT)0x4C50424E /* NBPL */
#define BUFFER_SIGN          (UINT)0x4655424E /* NBUF */
#define PACKET_POOL_SIGN     (UINT)0x4C50504E /* NPPL */
#define PACKET_SIGN          (UINT)0x4B41504E /* NPAK */
#define MAC_SIGN             (UINT)0x43414D4E /* NMAC */
#define ADAPTER_SIGN         (UINT)0x5044414E /* NADP */
#define PROTOCOL_SIGN        (UINT)0x5452504E /* NPRT */
#define OPEN_SIGN            (UINT)0x4E504F4E /* NOPN */

/* NOINC */

#define MAX_MCA_CHANNELS    8


#define MAX_EISA_CHANNELS   16

/*  */
/*  Masks for EISA function information */
/*  */

#define EISA_FUNCTION_ENABLED                   0x80
#define EISA_FREE_FORM_DATA                     0x40
#define EISA_HAS_PORT_INIT_ENTRY                0x20
#define EISA_HAS_PORT_RANGE                     0x10
#define EISA_HAS_DMA_ENTRY                      0x08
#define EISA_HAS_IRQ_ENTRY                      0x04
#define EISA_HAS_MEMORY_ENTRY                   0x02
#define EISA_HAS_TYPE_ENTRY                     0x01
#define EISA_HAS_INFORMATION                    EISA_HAS_PORT_RANGE + \
						EISA_HAS_DMA_ENTRY + \
						EISA_HAS_IRQ_ENTRY + \
						EISA_HAS_MEMORY_ENTRY + \
						EISA_HAS_TYPE_ENTRY

/*  */
/*  Masks for EISA memory configuration */
/*  */

#define EISA_MORE_ENTRIES                       0x80
#define EISA_SYSTEM_MEMORY                      0x00
#define EISA_MEMORY_TYPE_RAM                    0x01

/*  */
/*  Returned error code for EISA bios call */
/*  */

#define EISA_INVALID_SLOT                       0x80
#define EISA_INVALID_FUNCTION                   0x81
#define EISA_INVALID_CONFIGURATION              0x82
#define EISA_EMPTY_SLOT                         0x83
#define EISA_INVALID_BIOS_CALL                  0x86


/*  */
/*  Defines the Type in the RESOURCE_DESCRIPTOR */
/*  */

typedef enum _CM_RESOURCE_TYPE {
    CmResourceTypeNull = 0,    /*  Reserved */
    CmResourceTypePort,
    CmResourceTypeInterrupt,
    CmResourceTypeMemory,
    CmResourceTypeDma,
    CmResourceTypeDeviceSpecific,
    CmResourceTypeMaximum
} CM_RESOURCE_TYPE;

/*  */
/*  Defines the ShareDisposition in the RESOURCE_DESCRIPTOR */
/*  */

typedef enum _CM_SHARE_DISPOSITION {
    CmResourceShareUndetermined = 0,    /*  Reserved */
    CmResourceShareDeviceExclusive,
    CmResourceShareDriverExclusive,
    CmResourceShareShared
} CM_SHARE_DISPOSITION;

/*  */
/*  Define the bit masks for Flags when type is CmResourceTypeInterrupt */
/*  */

#define CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE 0
#define CM_RESOURCE_INTERRUPT_LATCHED         1

/*  */
/*  Define the bit masks for Flags when type is CmResourceTypeMemory */
/*  */

#define CM_RESOURCE_MEMORY_READ_WRITE       0x0000
#define CM_RESOURCE_MEMORY_READ_ONLY        0x0001
#define CM_RESOURCE_MEMORY_WRITE_ONLY       0x0002
#define CM_RESOURCE_MEMORY_PREFETCHABLE     0x0004

/*  */
/*  Define the bit masks for Flags when type is CmResourceTypePort */
/*  */

#define CM_RESOURCE_PORT_MEMORY 0
#define CM_RESOURCE_PORT_IO 1

enum WRAPPER_CONFIGURATION_CONTEXT_DRIVER_TYPE{
    wccDriverTypeMac,
    wccDriverTypeProtocol,
    wccDriverTypeUnknown
};

typedef enum {
    NdisInterruptLatched=1,
    NdisInterruptLevelSensitive=2
    } NDIS_INTERRUPT_MODE, *PNDIS_INTERRUPT_MODE;

/*  */
/*  Possible data types */
/*  */
typedef enum _NDIS_PARAMETER_TYPE {
    NdisParameterInteger,
    NdisParameterHexInteger,
    NdisParameterString,
    NdisParameterMultiString
} NDIS_PARAMETER_TYPE, *PNDIS_PARAMETER_TYPE;

/*  */
/*  Definitions for the "ProcessorType" keyword */
/*  */
typedef enum _NDIS_PROCESSOR_TYPE {
    NdisProcessorX86,
    NdisProcessorMips,
    NdisProcessorAlpha
} NDIS_PROCESSOR_TYPE, *PNDIS_PROCESSOR_TYPE;

/*  */
/*  Definitions for the "Environment" keyword */
/*  */
typedef enum _NDIS_ENVIRONMENT_TYPE {
    NdisEnvironmentWindows,
    NdisEnvironmentWindowsNt
} NDIS_ENVIRONMENT_TYPE, *PNDIS_ENVIRONMENT_TYPE;


/*  */
/*  Define the I/O bus interface types. from NT (ntddk.h) */
/*  */
typedef enum _INTERFACE_TYPE {
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

/*  */
/*  Possible Hardware Architecture. Define these to */
/*  match the HAL INTERFACE_TYPE enum (for NT compatibility). */
/*  */
typedef enum _NDIS_INTERFACE_TYPE {
    NdisInterfaceInternal = Internal,
    NdisInterfaceIsa = Isa,
    NdisInterfaceEisa = Eisa,
    NdisInterfaceMca = MicroChannel,
    NdisInterfaceTurboChannel = TurboChannel,
    NdisInterfacePci = PCIBus,
    NdisInterfacePcMcia = PCMCIABus
} NDIS_INTERFACE_TYPE, *PNDIS_INTERFACE_TYPE;


typedef enum _NDIS_REQUEST_TYPE {
    NdisRequestQueryInformation,
    NdisRequestSetInformation,
    NdisRequestQueryStatistics,
    NdisRequestOpen,
    NdisRequestClose,
    NdisRequestSend,
    NdisRequestTransferData,
    NdisRequestReset,
    NdisRequestGeneric1,
    NdisRequestGeneric2,
    NdisRequestGeneric3,
    NdisRequestGeneric4
} NDIS_REQUEST_TYPE, *PNDIS_REQUEST_TYPE;

/*  */
/*  Medium Ndis Driver is running on */
/*  */

typedef enum _NDIS_MEDIUM {
    NdisMedium802_3,
    NdisMedium802_5,
    NdisMediumFddi,
    NdisMediumWan,
    NdisMediumLocalTalk,
    NdisMediumDix,              /*  defined for convenience, not a real medium */
    NdisMediumArcnetRaw,
    NdisMediumArcnet878_2
} NDIS_MEDIUM, *PNDIS_MEDIUM;

/*  */
/*  Hardware status codes (OID_GEN_HARDWARE_STATUS). */
/*  */

typedef enum _NDIS_HARDWARE_STATUS {
    NdisHardwareStatusReady,
    NdisHardwareStatusInitializing,
    NdisHardwareStatusReset,
    NdisHardwareStatusClosing,
    NdisHardwareStatusNotReady
} NDIS_HARDWARE_STATUS, *PNDIS_HARDWARE_STATUS;


/*  */
/*  Defines the attachment types for FDDI (OID_FDDI_ATTACHMENT_TYPE). */
/*  */

typedef enum _NDIS_FDDI_ATTACHMENT_TYPE {
    NdisFddiTypeIsolated = 1,
    NdisFddiTypeLocalA,
    NdisFddiTypeLocalB,
    NdisFddiTypeLocalAB,
    NdisFddiTypeLocalS,
    NdisFddiTypeWrapA,
    NdisFddiTypeWrapB,
    NdisFddiTypeWrapAB,
    NdisFddiTypeWrapS,
    NdisFddiTypeCWrapA,
    NdisFddiTypeCWrapB,
    NdisFddiTypeCWrapS,
    NdisFddiTypeThrough
} NDIS_FDDI_ATTACHMENT_TYPE, *PNDIS_FDDI_ATTACHMENT_TYPE;


/*  */
/*  Defines the ring management states for FDDI (OID_FDDI_RING_MGT_STATE). */
/*  */

typedef enum _NDIS_FDDI_RING_MGT_STATE {
    NdisFddiRingIsolated = 1,
    NdisFddiRingNonOperational,
    NdisFddiRingOperational,
    NdisFddiRingDetect,
    NdisFddiRingNonOperationalDup,
    NdisFddiRingOperationalDup,
    NdisFddiRingDirected,
    NdisFddiRingTrace
} NDIS_FDDI_RING_MGT_STATE, *PNDIS_FDDI_RING_MGT_STATE;


/*  */
/*  Defines the Lconnection state for FDDI (OID_FDDI_LCONNECTION_STATE). */
/*  */

typedef enum _NDIS_FDDI_LCONNECTION_STATE {
    NdisFddiStateOff = 1,
    NdisFddiStateBreak,
    NdisFddiStateTrace,
    NdisFddiStateConnect,
    NdisFddiStateNext,
    NdisFddiStateSignal,
    NdisFddiStateJoin,
    NdisFddiStateVerify,
    NdisFddiStateActive,
    NdisFddiStateMaintenance
} NDIS_FDDI_LCONNECTION_STATE, *PNDIS_FDDI_LCONNECTION_STATE;


/*  */
/*  Defines the medium subtypes for WAN medium */
/*  */

typedef enum _NDIS_WAN_MEDIUM_SUBTYPE {
    NdisWanMediumHub,
    NdisWanMediumX_25,
    NdisWanMediumIsdn,
    NdisWanMediumSerial,
    NdisWanMediumFrameRelay
} NDIS_WAN_MEDIUM_SUBTYPE, *PNDIS_WAN_MEDIUM_SUBTYPE;

/*  */
/*  Defines the header format for WAN medium */
/*  */

typedef enum _NDIS_WAN_HEADER_FORMAT {
    NdisWanHeaderNative,       /*  src/dest based on subtype, followed by NLPID */
    NdisWanHeaderEthernet      /*  emulation of ethernet header */
} NDIS_WAN_HEADER_FORMAT, *PNDIS_WAN_HEADER_FORMAT;

/*  */
/*  Defines the line quality on an WAN line */
/*  */

typedef enum _NDIS_WAN_QUALITY {
    NdisWanRaw,
    NdisWanErrorControl,
    NdisWanReliable
} NDIS_WAN_QUALITY, *PNDIS_WAN_QUALITY;

typedef enum _NDIS_802_5_RING_STATE {
    NdisRingStateOpened = 1,
    NdisRingStateClosed,
    NdisRingStateOpening,
    NdisRingStateClosing,
    NdisRingStateOpenFailure,
    NdisRingStateRingFailure
} NDIS_802_5_RING_STATE, *PNDIS_802_5_RING_STATE;



/* Adapter PnP Flags */
#define NDISPNP_NODE_BEING_REMOVED          0x00000001
#define NDISPNP_NODE_REGISTERED             0x00000002
#define NDISPNP_NODE_STOPPED                0x00000004
#define NDISPNP_NODE_ALREADY_REMOVED        0x00000008
#define NDISPNP_NODE_TEST_REMOVE_SUCCEEDED  0x00000010
#define NDISPNP_NODE_INNEWDEVNODE           0x00000020
#define NDISPNP_NODE_RECEIVED_PREREMOVE     0x00000040
#define NDISPNP_NODE_DELAY_BINDING          0x00000080
#define NDISPNP_NODE_DONT_REMOVE_RESOURCES  0x00000100
#define NDISPNP_NODE_DISABLE_WARNING        0x00000200

/*  */
/*  Types of Memory (not mutually exclusive) */
/*  */

#define NDIS_MEMORY_CONTIGUOUS              0x00000001
#define NDIS_MEMORY_NONCACHED               0x00000002

/*  */
/*  Open options */
/*  */
#define NDIS_OPEN_RECEIVE_NOT_REENTRANT         0x00000001


/*  */
/*  NDIS_STATUS values */
/*  */

/* INC */
#define NDIS_STATUS_SUCCESS                 ((NDIS_STATUS)0x00000000L)
#define NDIS_STATUS_PENDING                 ((NDIS_STATUS)0x00000103L)
#define NDIS_STATUS_NOT_RECOGNIZED          ((NDIS_STATUS)0x00010001L)
#define NDIS_STATUS_NOT_COPIED              ((NDIS_STATUS)0x00010002L)
#define NDIS_STATUS_NOT_ACCEPTED            ((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_MAY_CLOSE               ((NDIS_STATUS)0x00010004L)

#define NDIS_STATUS_ONLINE                  ((NDIS_STATUS)0x40010003L)
#define NDIS_STATUS_RESET_START             ((NDIS_STATUS)0x40010004L)
#define NDIS_STATUS_RESET_END               ((NDIS_STATUS)0x40010005L)
#define NDIS_STATUS_RING_STATUS             ((NDIS_STATUS)0x40010006L)
#define NDIS_STATUS_CLOSED                  ((NDIS_STATUS)0x40010007L)
#define NDIS_STATUS_WAN_LINE_UP             ((NDIS_STATUS)0x40010008L)
#define NDIS_STATUS_WAN_LINE_DOWN           ((NDIS_STATUS)0x40010009L)
#define NDIS_STATUS_WAN_FRAGMENT            ((NDIS_STATUS)0x4001000AL)

#define NDIS_STATUS_NOT_RESETTABLE          ((NDIS_STATUS)0x80010001L)
#define NDIS_STATUS_SOFT_ERRORS             ((NDIS_STATUS)0x80010003L)
#define NDIS_STATUS_HARD_ERRORS             ((NDIS_STATUS)0x80010004L)

#define NDIS_STATUS_FAILURE                 ((NDIS_STATUS)0xC0000001L)
#define NDIS_STATUS_RESOURCES               ((NDIS_STATUS)0xC000009AL)

#define NDIS_STATUS_CLOSING                 ((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_BAD_VERSION             ((NDIS_STATUS)0xC0010004L)
#define NDIS_STATUS_BAD_CHARACTERISTICS     ((NDIS_STATUS)0xC0010005L)
#define NDIS_STATUS_ADAPTER_NOT_FOUND       ((NDIS_STATUS)0xC0010006L)
#define NDIS_STATUS_OPEN_FAILED             ((NDIS_STATUS)0xC0010007L)
#define NDIS_STATUS_DEVICE_FAILED           ((NDIS_STATUS)0xC0010008L)
#define NDIS_STATUS_MULTICAST_FULL          ((NDIS_STATUS)0xC0010009L)
#define NDIS_STATUS_MULTICAST_EXISTS        ((NDIS_STATUS)0xC001000AL)
#define NDIS_STATUS_MULTICAST_NOT_FOUND     ((NDIS_STATUS)0xC001000BL)
#define NDIS_STATUS_REQUEST_ABORTED         ((NDIS_STATUS)0xC001000CL)
#define NDIS_STATUS_RESET_IN_PROGRESS       ((NDIS_STATUS)0xC001000DL)
#define NDIS_STATUS_CLOSING_INDICATING      ((NDIS_STATUS)0xC001000EL)
#define NDIS_STATUS_NOT_SUPPORTED           ((NDIS_STATUS)0xC00000BBL)
#define NDIS_STATUS_INVALID_PACKET          ((NDIS_STATUS)0xC001000FL)
#define NDIS_STATUS_OPEN_LIST_FULL          ((NDIS_STATUS)0xC0010010L)
#define NDIS_STATUS_ADAPTER_NOT_READY       ((NDIS_STATUS)0xC0010011L)
#define NDIS_STATUS_ADAPTER_NOT_OPEN        ((NDIS_STATUS)0xC0010012L)
#define NDIS_STATUS_NOT_INDICATING          ((NDIS_STATUS)0xC0010013L)
#define NDIS_STATUS_INVALID_LENGTH          ((NDIS_STATUS)0xC0010014L)
#define NDIS_STATUS_INVALID_DATA            ((NDIS_STATUS)0xC0010015L)
#define NDIS_STATUS_BUFFER_TOO_SHORT        ((NDIS_STATUS)0xC0010016L)
#define NDIS_STATUS_INVALID_OID             ((NDIS_STATUS)0xC0010017L)
#define NDIS_STATUS_ADAPTER_REMOVED         ((NDIS_STATUS)0xC0010018L)
#define NDIS_STATUS_UNSUPPORTED_MEDIA       ((NDIS_STATUS)0xC0010019L)
#define NDIS_STATUS_GROUP_ADDRESS_IN_USE    ((NDIS_STATUS)0xC001001AL)
#define NDIS_STATUS_FILE_NOT_FOUND          ((NDIS_STATUS)0xC001001BL)
#define NDIS_STATUS_ERROR_READING_FILE      ((NDIS_STATUS)0xC001001CL)
#define NDIS_STATUS_ALREADY_MAPPED          ((NDIS_STATUS)0xC001001DL)
#define NDIS_STATUS_RESOURCE_CONFLICT       ((NDIS_STATUS)0xC001001EL)
#define NDIS_STATUS_NO_CABLE                ((NDIS_STATUS)0xC001001FL)

#define NDIS_STATUS_TOKEN_RING_OPEN_ERROR   ((NDIS_STATUS)0xC0011000L)

/*  For NT Compatability */


/*  */
/*  used in error logging */
/*  */

#define NDIS_ERROR_CODE LONG

#define NDIS_ERROR_CODE_RESOURCE_CONFLICT               0xAA000001L /* EVENT_NDIS_RESOURCE_CONFLICT */
#define NDIS_ERROR_CODE_OUT_OF_RESOURCES                0xAA000002L /* EVENT_NDIS_OUT_OF_RESOURCE */
#define NDIS_ERROR_CODE_HARDWARE_FAILURE                0xAA000003L /* EVENT_NDIS_HARDWARE_FAILURE */
#define NDIS_ERROR_CODE_ADAPTER_NOT_FOUND               0xAA000004L /* EVENT_NDIS_ADAPTER_NOT_FOUND */
#define NDIS_ERROR_CODE_INTERRUPT_CONNECT               0xAA000005L /* EVENT_NDIS_INTERRUPT_CONNECT */
#define NDIS_ERROR_CODE_DRIVER_FAILURE                  0xAA000006L /* EVENT_NDIS_DRIVER_FAILURE */
#define NDIS_ERROR_CODE_BAD_VERSION                     0xAA000007L /* EVENT_NDIS_BAD_VERSION */
#define NDIS_ERROR_CODE_TIMEOUT                         0xAA000008L /* EVENT_NDIS_TIMEOUT */
#define NDIS_ERROR_CODE_NETWORK_ADDRESS                 0xAA000009L /* EVENT_NDIS_NETWORK_ADDRESS */
#define NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION       0xAA00000AL /* EVENT_NDIS_UNSUPPORTED_CONFIGURATION */
#define NDIS_ERROR_CODE_INVALID_VALUE_FROM_ADAPTER      0xAA00000BL /* EVENT_NDIS_INVALID_VALUE_FROM_ADAPTER */
#define NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER 0xAA00000CL /* EVENT_NDIS_MISSING_CONFIGURATION_PARAMETER */
#define NDIS_ERROR_CODE_BAD_IO_BASE_ADDRESS             0xAA00000DL /* EVENT_NDIS_BAD_IO_BASE_ADDRESS */
#define NDIS_ERROR_CODE_RECEIVE_SPACE_SMALL             0xAA00000EL /* EVENT_NDIS_RECEIVE_SPACE_SMALL */
#define NDIS_ERROR_CODE_ADAPTER_DISABLED                0xAA00000FL /* EVENT_NDIS_ADAPTER_DISABLED */



/*  */
/*  Ndis Packet Filter Bits */
/*  */

#define NDIS_PACKET_TYPE_DIRECTED           0x0001
#define NDIS_PACKET_TYPE_MULTICAST          0x0002
#define NDIS_PACKET_TYPE_ALL_MULTICAST      0x0004
#define NDIS_PACKET_TYPE_BROADCAST          0x0008
#define NDIS_PACKET_TYPE_SOURCE_ROUTING     0x0010
#define NDIS_PACKET_TYPE_PROMISCUOUS        0x0020
#define NDIS_PACKET_TYPE_SMT                0x0040
#define NDIS_PACKET_TYPE_MAC_FRAME          0x8000
#define NDIS_PACKET_TYPE_FUNCTIONAL         0x4000
#define NDIS_PACKET_TYPE_ALL_FUNCTIONAL     0x2000
#define NDIS_PACKET_TYPE_GROUP              0x1000

/*  */
/*  Ndis Token-Ring Ring Status Codes */
/*  */

#define NDIS_RING_SIGNAL_LOSS               0x00008000
#define NDIS_RING_HARD_ERROR                0x00004000
#define NDIS_RING_SOFT_ERROR                0x00002000
#define NDIS_RING_TRANSMIT_BEACON           0x00001000
#define NDIS_RING_LOBE_WIRE_FAULT           0x00000800
#define NDIS_RING_AUTO_REMOVAL_ERROR        0x00000400
#define NDIS_RING_REMOVE_RECEIVED           0x00000200
#define NDIS_RING_COUNTER_OVERFLOW          0x00000100
#define NDIS_RING_SINGLE_STATION            0x00000080
#define NDIS_RING_RING_RECOVERY             0x00000040

/*  */
/*  Ndis protocol option bits (OID_GEN_PROTOCOL_OPTIONS). */
/*  */

#define NDIS_PROT_OPTION_ESTIMATED_LENGTH   0x00000001
#define NDIS_PROT_OPTION_NO_LOOPBACK        0x00000002

/*  */
/*  Ndis MAC option bits (OID_GEN_MAC_OPTIONS). */
/*  */

#define NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA             0x00000001
#define NDIS_MAC_OPTION_RECEIVE_SERIALIZED              0x00000002
#define NDIS_MAC_OPTION_TRANSFERS_NOT_PEND              0x00000004
#define NDIS_MAC_OPTION_NO_LOOPBACK                     0x00000008
/*  */
/*  Object Identifiers used by NdisRequest Query/Set Information */
/*  */


/*  */
/*       NDIS media capabilities bits (OID_GEN_MEDIA_CAPABILITIES). */
/*  */
#define NDIS_MEDIA_CAP_TRANSMIT                                 0x00000001
#define NDIS_MEDIA_CAP_RECEIVE                                  0x00000002


/*  */
/*  General Objects */
/*  */

#define OID_GEN_SUPPORTED_LIST              0x00010101
#define OID_GEN_HARDWARE_STATUS             0x00010102
#define OID_GEN_MEDIA_SUPPORTED             0x00010103
#define OID_GEN_MEDIA_IN_USE                0x00010104
#define OID_GEN_MAXIMUM_LOOKAHEAD           0x00010105
#define OID_GEN_MAXIMUM_FRAME_SIZE          0x00010106
#define OID_GEN_LINK_SPEED                  0x00010107
#define OID_GEN_TRANSMIT_BUFFER_SPACE       0x00010108
#define OID_GEN_RECEIVE_BUFFER_SPACE        0x00010109
#define OID_GEN_TRANSMIT_BLOCK_SIZE         0x0001010A
#define OID_GEN_RECEIVE_BLOCK_SIZE          0x0001010B
#define OID_GEN_VENDOR_ID                   0x0001010C
#define OID_GEN_VENDOR_DESCRIPTION          0x0001010D
#define OID_GEN_CURRENT_PACKET_FILTER       0x0001010E
#define OID_GEN_CURRENT_LOOKAHEAD           0x0001010F
#define OID_GEN_DRIVER_VERSION              0x00010110
#define OID_GEN_MAXIMUM_TOTAL_SIZE          0x00010111
#define OID_GEN_PROTOCOL_OPTIONS            0x00010112
#define OID_GEN_MAC_OPTIONS                 0x00010113



/*  */
/*       Optional OIDs */
/*  */
#define OID_GEN_MEDIA_CAPABILITIES          0x00010201


#define OID_GEN_XMIT_OK                     0x00020101
#define OID_GEN_RCV_OK                      0x00020102
#define OID_GEN_XMIT_ERROR                  0x00020103
#define OID_GEN_RCV_ERROR                   0x00020104
#define OID_GEN_RCV_NO_BUFFER               0x00020105

#define OID_GEN_DIRECTED_BYTES_XMIT         0x00020201
#define OID_GEN_DIRECTED_FRAMES_XMIT        0x00020202
#define OID_GEN_MULTICAST_BYTES_XMIT        0x00020203
#define OID_GEN_MULTICAST_FRAMES_XMIT       0x00020204
#define OID_GEN_BROADCAST_BYTES_XMIT        0x00020205
#define OID_GEN_BROADCAST_FRAMES_XMIT       0x00020206
#define OID_GEN_DIRECTED_BYTES_RCV          0x00020207
#define OID_GEN_DIRECTED_FRAMES_RCV         0x00020208
#define OID_GEN_MULTICAST_BYTES_RCV         0x00020209
#define OID_GEN_MULTICAST_FRAMES_RCV        0x0002020A
#define OID_GEN_BROADCAST_BYTES_RCV         0x0002020B
#define OID_GEN_BROADCAST_FRAMES_RCV        0x0002020C

#define OID_GEN_RCV_CRC_ERROR               0x0002020D
#define OID_GEN_TRANSMIT_QUEUE_LENGTH       0x0002020E






/*  */
/*  802.3 Objects (Ethernet) */
/*  */

#define OID_802_3_PERMANENT_ADDRESS         0x01010101
#define OID_802_3_CURRENT_ADDRESS           0x01010102
#define OID_802_3_MULTICAST_LIST            0x01010103
#define OID_802_3_MAXIMUM_LIST_SIZE         0x01010104

#define OID_802_3_RCV_ERROR_ALIGNMENT       0x01020101
#define OID_802_3_XMIT_ONE_COLLISION        0x01020102
#define OID_802_3_XMIT_MORE_COLLISIONS      0x01020103

#define OID_802_3_XMIT_DEFERRED             0x01020201
#define OID_802_3_XMIT_MAX_COLLISIONS       0x01020202
#define OID_802_3_RCV_OVERRUN               0x01020203
#define OID_802_3_XMIT_UNDERRUN             0x01020204
#define OID_802_3_XMIT_HEARTBEAT_FAILURE    0x01020205
#define OID_802_3_XMIT_TIMES_CRS_LOST       0x01020206
#define OID_802_3_XMIT_LATE_COLLISIONS      0x01020207


/*  */
/*  802.5 Objects (Token-Ring) */
/*  */

#define OID_802_5_PERMANENT_ADDRESS         0x02010101
#define OID_802_5_CURRENT_ADDRESS           0x02010102
#define OID_802_5_CURRENT_FUNCTIONAL        0x02010103
#define OID_802_5_CURRENT_GROUP             0x02010104
#define OID_802_5_LAST_OPEN_STATUS          0x02010105
#define OID_802_5_CURRENT_RING_STATUS       0x02010106
#define OID_802_5_CURRENT_RING_STATE        0x02010107

#define OID_802_5_LINE_ERRORS               0x02020101
#define OID_802_5_LOST_FRAMES               0x02020102

#define OID_802_5_BURST_ERRORS              0x02020201
#define OID_802_5_AC_ERRORS                 0x02020202
#define OID_802_5_ABORT_DELIMETERS          0x02020203
#define OID_802_5_FRAME_COPIED_ERRORS       0x02020204
#define OID_802_5_FREQUENCY_ERRORS          0x02020205
#define OID_802_5_TOKEN_ERRORS              0x02020206
#define OID_802_5_INTERNAL_ERRORS           0x02020207


/*  */
/*  FDDI Objects */
/*  */

#define OID_FDDI_LONG_PERMANENT_ADDR        0x03010101
#define OID_FDDI_LONG_CURRENT_ADDR          0x03010102
#define OID_FDDI_LONG_MULTICAST_LIST        0x03010103
#define OID_FDDI_LONG_MAX_LIST_SIZE         0x03010104
#define OID_FDDI_SHORT_PERMANENT_ADDR       0x03010105
#define OID_FDDI_SHORT_CURRENT_ADDR         0x03010106
#define OID_FDDI_SHORT_MULTICAST_LIST       0x03010107
#define OID_FDDI_SHORT_MAX_LIST_SIZE        0x03010108


/*  */
/*  WAN objects */
/*  */

#define OID_WAN_PERMANENT_ADDRESS           0x04010101
#define OID_WAN_CURRENT_ADDRESS             0x04010102
#define OID_WAN_QUALITY_OF_SERVICE          0x04010103
#define OID_WAN_PROTOCOL_TYPE               0x04010104
#define OID_WAN_MEDIUM_SUBTYPE              0x04010105
#define OID_WAN_HEADER_FORMAT               0x04010106

#define OID_WAN_GET_INFO                    0x04010107
#define OID_WAN_SET_LINK_INFO               0x04010108
#define OID_WAN_GET_LINK_INFO               0x04010109

#define OID_WAN_LINE_COUNT                  0x0401010A

#define OID_WAN_GET_BRIDGE_INFO             0x0401020A
#define OID_WAN_SET_BRIDGE_INFO             0x0401020B
#define OID_WAN_GET_COMP_INFO               0x0401020C
#define OID_WAN_SET_COMP_INFO               0x0401020D
#define OID_WAN_GET_STATS_INFO              0x0401020E


/*  */
/*  TAPI objects */
/*  */
#define OID_TAPI_ACCEPT                     0x07030101
#define OID_TAPI_ANSWER                     0x07030102
#define OID_TAPI_CLOSE                      0x07030103
#define OID_TAPI_CLOSE_CALL                 0x07030104
#define OID_TAPI_CONDITIONAL_MEDIA_DETECTION 0x07030105
#define OID_TAPI_CONFIG_DIALOG              0x07030106
#define OID_TAPI_DEV_SPECIFIC               0x07030107
#define OID_TAPI_DIAL                       0x07030108
#define OID_TAPI_DROP                       0x07030109
#define OID_TAPI_GET_ADDRESS_CAPS           0x0703010A
#define OID_TAPI_GET_ADDRESS_ID             0x0703010B
#define OID_TAPI_GET_ADDRESS_STATUS         0x0703010C
#define OID_TAPI_GET_CALL_ADDRESS_ID        0x0703010D
#define OID_TAPI_GET_CALL_INFO              0x0703010E
#define OID_TAPI_GET_CALL_STATUS            0x0703010F
#define OID_TAPI_GET_DEV_CAPS               0x07030110
#define OID_TAPI_GET_DEV_CONFIG             0x07030111
#define OID_TAPI_GET_EXTENSION_ID           0x07030112
#define OID_TAPI_GET_ID                     0x07030113
#define OID_TAPI_GET_LINE_DEV_STATUS        0x07030114
#define OID_TAPI_MAKE_CALL                  0x07030115
#define OID_TAPI_NEGOTIATE_EXT_VERSION      0x07030116
#define OID_TAPI_OPEN                       0x07030117
#define OID_TAPI_PROVIDER_INITIALIZE        0x07030118
#define OID_TAPI_PROVIDER_SHUTDOWN          0x07030119
#define OID_TAPI_SECURE_CALL                0x0703011A
#define OID_TAPI_SELECT_EXT_VERSION         0x0703011B
#define OID_TAPI_SEND_USER_USER_INFO        0x0703011C
#define OID_TAPI_SET_APP_SPECIFIC           0x0703011D
#define OID_TAPI_SET_CALL_PARAMS            0x0703011E
#define OID_TAPI_SET_DEFAULT_MEDIA_DETECTION 0x0703011F
#define OID_TAPI_SET_DEV_CONFIG             0x07030120
#define OID_TAPI_SET_MEDIA_MODE             0x07030121
#define OID_TAPI_SET_STATUS_MESSAGES        0x07030122


/*  */
/*  LocalTalk objects */
/*  */

#define OID_LTALK_CURRENT_NODE_ID           0x05010102

#define OID_LTALK_IN_BROADCASTS             0x05020101
#define OID_LTALK_IN_LENGTH_ERRORS          0x05020102

#define OID_LTALK_OUT_NO_HANDLERS           0x05020201
#define OID_LTALK_COLLISIONS                0x05020202
#define OID_LTALK_DEFERS                    0x05020203
#define OID_LTALK_NO_DATA_ERRORS            0x05020204
#define OID_LTALK_RANDOM_CTS_ERRORS         0x05020205
#define OID_LTALK_FCS_ERRORS                0x05020206


/*  */
/*  Arcnet objects */
/*  */
#define OID_ARCNET_PERMANENT_ADDRESS        0x06010101
#define OID_ARCNET_CURRENT_ADDRESS          0x06010102
#define OID_ARCNET_RECONFIGURATIONS         0x06020201


/*  */
/*  Chicago Implementation Specific objects */
/*  */

#define OID_PRIVATE_PROTOCOL_HANDLE         0xff010207


/*  */
/*  OS specific interface for reading adapter resources */
/*  */

typedef enum _WRAPPER_RESOURCE_TYPE{
    WrapperResourceTypeAll  = 0x00000000,
    WrapperResourceTypeMem  = 0x00000001,
    WrapperResourceTypeIO   = 0x00000002,
    WrapperResourceTypeDMA  = 0x00000003,
    WrapperResourceTypeIRQ  = 0x00000004
    } WRAPPER_RESOURCE_TYPE, *PWRAPPER_RESOURCE_TYPE;

/*  */
/*  OS Specific Services, Macros, etc. */
/*  */

/*  */
typedef enum _NET_COMPONENT_TYPE{
    ComponentTypeAdapter,
    ComponentTypeMac,
    ComponentTypeProtocol
    } NET_COMPONENT_TYPE, * PNET_COMPONENT_TYPE;

#endif  /*  _NDIS_ */

