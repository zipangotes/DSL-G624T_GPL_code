#ifndef  __CM_INTERFACE_H__
#define  __CM_INTERFACE_H__

#define		INTERFACE_UNKNOWN	0
#define		INTERFACE_802_1D	1
#define		INTERFACE_VC		2
#define		INTERFACE_ATM		3
#define		INTERFACE_USB		4
#define		INTERFACE_PHY		5

int	get_interface_type(char *text);

#endif /*__CM_INTERFACE_H__*/
