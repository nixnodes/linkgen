/*
 * init.c
 *
 *  Created on: Jan 3, 2013
 *      Author: reboot
 */
#include <common.h>
#include <register.h>
#include <lparse.h>

int mem_clean
(void)
{
	int r = 0;

	r += free_register(linkdb.m_data);
	r += free_register(asdb.m_data);

	return r;
}
