// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#ifndef __PUREPHONE_T7_H
#define __PUREPHONE_T7_H

#include <asm/arch/imx-regs.h>

#define ESDHCI_QUIRK_BROKEN_TIMEOUT_VALUE	1

#define CONFIG_SYS_FLASH_BASE		0x20002000

#define PHYS_SDRAM			0x80000000
#define PHYS_SDRAM_SIZE			(64 * 1024 * 1024)

#define DMAMEM_SZ_ALL			(1 * 1024 * 1024)
#define DMAMEM_BASE			(PHYS_SDRAM + PHYS_SDRAM_SIZE - \
					 DMAMEM_SZ_ALL)

/*
 * Configuration of the external SDRAM memory
 */

#define CONFIG_SYS_UBOOT_START		0x800023FD

#endif /* __PUREPHONE_T7_H */
