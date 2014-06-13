/* $Id: fixenv.S,v 1.1 2002/08/13 21:26:01 sneha Exp $
 *  
 *   Copyright(C) Texas Instruments, 2000-2001
*/               

#include <asm/asm.h>
#include <asm/regdef.h>

#include "quick.h"
        
        .text

        .set noreorder
        .set noat
        
	quick_link (adam2_fixenv, 0, 29)
        
        .set at
