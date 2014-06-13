/* $Id: unsetenv.S,v 1.1 2002/04/08 18:15:42 jharrell Exp $
 *  
 *   Copyright(C) Texas Instruments, 2000-2001
*/               

#include <asm/asm.h>
#include <asm/regdef.h>

#include "quick.h"
        
        .text

        .set noreorder
        .set noat
        
	quick_link (adam2_unsetenv, 0, 15)
        
        .set at
