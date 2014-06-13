            Communication Processor Software Group Hardward Abstraction Layer
                         Broadband Silicon Group
                           Texas Instructments


This module contains the support files for the CPSWHAL, known as the COMMON module.
This module is intended for use by other CPSWHAL modules and contains no API support for the user.

To rebuild the COMMON module compile all the 'c' files present. 

A list of functions which must be exported is provided in 'export.txt'.

Release History
---------------

15 June 2004     COMMON 2.1.0 (external release)
CC Labels: REL_20040615_HALdallas_common_02.01.00
Bug fixes: 1) The assembly cache macros (common_support.c) have been updated to support both Linux and VxWorks.
           The previous multi-line macro implementation was flawed.
New features: 1) Removed support for PULL model functionality, and converted all keys from strings to enums.
              2) Code has been trimmed in size.
              3) Version number has been moved to a new file (cpswhalcommon_version.c).
              4) Added cpswhalSet/Get() macros.  These should be used for setting or getting parameter values.
              5) cpswhalcommon_global.c file has been removed from the module.  Users should modify makefiles
                 appropriately.
              6) Removed support for the StatsGet functionality.  The OS should use the cpswhalGet() macro
                 to collect statistics information.
Issues: NTR
Comments: The COMMON 2.1.0 release will ONLY work with other module releases that are version 2.1.0 and later.
          To use the code in debug mode, you must compile all modules (CPSAR/CPMAC/CPUSB) in debug mode, not 
          just the COMMON module.  To build in debug mode, compile with this flag: __CPHAL_DEBUG.

23 January 2004  COMMON 2.0.4 (external release)
CC Labels: REL_20040123_HALdallas_common_02.00.04
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments:  Replaced stdefs.h with defs.h to cope with Linux build issue.
Added Auto Mdix support; Added Random Number Support; Removed TxNumQueues; Added some Gigabit defines

12 November 2003  COMMON 2.0.3 (external release)
CC Labels: REL_20031112_HALdallas_common_02.00.03
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments:  Minor code and internal documentation updates.

06 November 2003  COMMON 2.0.2 (external release)
CC Labels: REL_20031106_HALdallas_common_02.00.02
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments:  Changed hcXXX keys to hkXXX, to avoid namespace pollution
           with previous HAL releases.

31 October 2003   COMMON 2.0.1 (external release)
CC Labels: REL_20031031_HALdallas_common_02.00.01
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments:  Minor updates added for cpusb support.


23 October 2003   COMMON 2.0.0 (external release)

CC Labels: REL_20031023_HALdallas_common_02.00.00
Bug fixes: NTR
New features: NTR
Issues: NTR
Comments: For cpusb support
