// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Me, myself, and I
 */

#include <asm/io.h>
#include <dm.h>
#include <net.h>

#define RM_MOD_CLKSEL		(0x13b000 + 0x3c)
#define RM_MOD_CLKDIV		(0x13b000 + 0x48)
#define RM_TIMER_SEL		1
#define RM_TIMER_DIV1	0

int board_early_init_f(void) {
	writel_relaxed(readl_relaxed(RM_MOD_CLKSEL) | RM_TIMER_SEL, RM_MOD_CLKSEL);
	writel_relaxed((readl_relaxed(RM_MOD_CLKDIV) & (~0xf)) | RM_TIMER_DIV1, RM_MOD_CLKDIV);

	return 0;
}

int board_init(void)
{
	return 0;
}

