// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Me, myself, and I
 */

#include <asm/io.h>
#include <dm.h>
#include <net.h>
#include <clk-uclass.h>
#include <dt-bindings/clk/zx297520v3.h>

#define RM_MOD_CLKSEL		(0x13b000 + 0x3c)
#define RM_MOD_CLKDIV		(0x13b000 + 0x48)
#define RM_TIMER_SEL		1
#define RM_TIMER_DIV1		0

int board_early_init_f(void) {
	int val;

	writel_relaxed(readl_relaxed(RM_MOD_CLKSEL) | RM_TIMER_SEL, RM_MOD_CLKSEL);
	writel_relaxed((readl_relaxed(RM_MOD_CLKDIV) & (~0xf)) | RM_TIMER_DIV1, RM_MOD_CLKDIV);

	// setup arch timer:
	// setup frequency
	val = CFG_SYS_HZ_CLOCK;
	asm volatile("mcr p15, 0, %0, c14, c0, 0" : : "r" (val));

	// allow non-secure access
	asm volatile("mrc p15, 0, %0, c1, c1, 2" : "=r" (val));
	val |= BIT(10) | BIT(11);
	asm volatile("mcr p15, 0, %0, c1, c1, 2" : : "r" (val));

	asm volatile("mrc p15, 0, %0, c1, c1, 0" : "=r" (val));
	val |= BIT(10);
	asm volatile("mcr p15, 0, %0, c1, c1, 0" : : "r" (val));

	// just in case phys timer is not started
	val = 1;
	asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (val));

	return 0;
}

int board_late_init(void)
{
	struct udevice *clk_dev;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK, DM_DRIVER_GET(zx297520v3_matrix_clk), &clk_dev);
	if (ret) {
		printf("bad uclass\n");
		return ret;
	}

	struct clk clk = { .id = ZX297520_CPU_WCLK, .dev = clk_dev };
	printf("CPU running at %lu MHz\n", clk_get_rate(&clk) / 1000000);

	return 0;
}

