/*
 * This program is free software; you can redistribute	it and/or modify it
 * under  the terms of	the GNU General	 Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Copyright (C) 1997, 2001 Ralf Baechle
 * Copyright 2001 MontaVista Software Inc.
 * Author: jsun@mvista.com or jsun@junsun.net
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/reboot.h>
#include <asm/system.h>

void nec_osprey_restart(char *command)
{
	change_cp0_status((ST0_BEV | ST0_ERL), (ST0_BEV | ST0_ERL));
	change_cp0_config(CONF_CM_CMASK, CONF_CM_UNCACHED);
	flush_cache_all();
	write_32bit_cp0_register(CP0_WIRED, 0);
	__asm__ __volatile__("jr\t%0"::"r"(0xbfc00000));
}

void nec_osprey_halt(void)
{
	printk(KERN_NOTICE "\n** You can safely turn off the power\n");
	while (1)
		__asm__(".set\tmips3\n\t"
			"wait\n\t"
			".set\tmips0");
}

void nec_osprey_power_off(void)
{
	nec_osprey_halt();
}
