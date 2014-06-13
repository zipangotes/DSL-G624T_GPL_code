            Communication Processor Software Group Hardware Abstraction Layer
                         Broadband Silicon Group
                           Texas Instructments

This module contains the support files for the CPUSB module. The APIs are documented in the CPSWHAL 2.0 document.

To rebuild the CPUSB module, compile all the 'c' files present. It is recommended that the options '-Os' be used except for the following files which should use the '-O3' option.
 * cpswhalcpusb_cppi.c
 * cpswhalcpusb_isr.c


Release History
---------------
15  June 2004   CPUSB 2.0.5 (external release)
CC Labels: REL_20040615_HALdallas_cpusb_02.00.05
Bug fixes: ChannelTeardown hardcoded address, now fixed.
New features: NTR
Issues: NTR
Comments: Use with CPSWHALcommon - REL_20040310_HALdallas_common_02.00.07


6  November 2003   CPUSB 2.0.1 (external release)
CC Labels: 
REL_20031106_HALdallas_cpusb_02.00.01
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments: 
1. Updated param keys from hcxxx to hkxxx to avoid namespace pollution. 
2. This release uses the CPSWHAL COMMON release REL_20031106_HALdallas_common_02.00.02 and above.


31 October  2003   CPUSB 2.0.0 (external release)
CC Labels: 
REL_20031031_HALdallas_cpusb_02.00.00
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments: 
1. Initial release to TII PSP team for V2U software development. 
2. This release uses the CPSWHAL COMMON release REL_20031031_HALdallas_common_02.00.01 and above.