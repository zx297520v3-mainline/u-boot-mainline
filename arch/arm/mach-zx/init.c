// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Me, myself, and I
 */

#include <fdtdec.h>
#include <init.h>
#include <asm/system.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

int print_cpuinfo(void)
{
	printf("CPU:   Sanechips ZX297520V3\n");
	return 0;
}

int dram_init(void)
{
	int ret;

	ret = fdtdec_setup_mem_size_base();
	if (ret)
		return ret;

	gd->ram_size = get_ram_size((void *)gd->ram_base, SZ_128M);

	return 0;
}

void reset_cpu(void)
{
	writel(1, 0x13b000);
	while(1);
}

