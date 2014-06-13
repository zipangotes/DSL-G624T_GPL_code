/*
 * BRIEF MODULE DESCRIPTION
 *	NEC VR4122 Clock Mask Unit routines.
 *
 * Copyright 2001,2002 MontaVista Software Inc.
 * Author: Yoichi Yuasa
 *		yyuasa@mvista.com or source@mvista.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/vr4122/vr4122.h>

rwlock_t vr4122_cmu_lock = RW_LOCK_UNLOCKED;

void vr4122_clock_supply(unsigned short mask)
{
	unsigned long flags;
	unsigned short val;

	write_lock_irqsave(&vr4122_cmu_lock, flags);
	val = readw(VR4122_CMUCLKMSK);
	val |= mask;
	writew(val, VR4122_CMUCLKMSK);
	write_unlock_irqrestore(&vr4122_cmu_lock, flags);
}

void vr4122_clock_mask(unsigned short mask)
{
	unsigned long flags;
	unsigned short val;

	write_lock_irqsave(&vr4122_cmu_lock, flags);
	val = readw(VR4122_CMUCLKMSK);
	val &= ~mask;
	writew(val, VR4122_CMUCLKMSK);
	write_unlock_irqrestore(&vr4122_cmu_lock, flags);
}
