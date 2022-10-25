// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <common.h>
#include <dm.h>
#include <init.h>
#include <log.h>
#include <ram.h>
#include <spl.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/armv7m.h>
#include <serial.h>

DECLARE_GLOBAL_DATA_PTR;

void memcheck(uint32_t addr, size_t size, uint32_t value){
    uint32_t * ptr = (uint32_t*)addr;

    for(uint32_t i = 0; i< size/sizeof(*ptr);i+=1){
        *(ptr+i) = value;
        if(*(ptr+i) != value){
            printf("elko\r\n");
        }
    }
}
#include "fsl_clock.h"
void print_clocks(){
    g_xtalFreq = 24000000;
    const char *_PLLNames[22] = {
            "kCLOCK_CpuClk",  /*!< CPU clock */
            "kCLOCK_AhbClk",  /*!< AHB clock */
            "kCLOCK_SemcClk", /*!< SEMC clock */
            "kCLOCK_IpgClk",  /*!< IPG clock */

            "kCLOCK_OscClk", /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
            "kCLOCK_RtcClk", /*!< RTC clock. (RTCCLK) */

            "kCLOCK_ArmPllClk", /*!< ARMPLLCLK. */

            "kCLOCK_Usb1PllClk",     /*!< USB1PLLCLK. */
            "kCLOCK_Usb1PllPfd0Clk", /*!< USB1PLLPDF0CLK. */
            "kCLOCK_Usb1PllPfd1Clk", /*!< USB1PLLPFD1CLK. */
            "kCLOCK_Usb1PllPfd2Clk", /*!< USB1PLLPFD2CLK. */
            "kCLOCK_Usb1PllPfd3Clk", /*!< USB1PLLPFD3CLK. */

            "kCLOCK_Usb2PllClk", /*!< USB2PLLCLK. */

            "kCLOCK_SysPllClk",     /*!< SYSPLLCLK. */
            "kCLOCK_SysPllPfd0Clk", /*!< SYSPLLPDF0CLK. */
            "kCLOCK_SysPllPfd1Clk", /*!< SYSPLLPFD1CLK. */
            "kCLOCK_SysPllPfd2Clk", /*!< SYSPLLPFD2CLK. */
            "kCLOCK_SysPllPfd3Clk", /*!< SYSPLLPFD3CLK. */

            "kCLOCK_EnetPll0Clk", /*!< Enet PLLCLK ref_enetpll0. */
            "kCLOCK_EnetPll1Clk", /*!< Enet PLLCLK ref_enetpll1. */

            "kCLOCK_AudioPllClk", /*!< Audio PLLCLK. */
            "kCLOCK_VideoPllClk", /*!< Video PLLCLK. */
    };
    int i;

    for (i = 0; i < 22; i++) {
        printf("%s: %lu Hz\r\n", _PLLNames[i], CLOCK_GetFreq((clock_name_t)i));
    }

    printf("PerphSourceClock_USDHC2: %lu\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk) / (CLOCK_GetDiv(kCLOCK_Usdhc2Div) + 1U));
}
int dram_init(void)
{
    int rv;
    struct udevice *dev;

    rv = uclass_get_device(UCLASS_RAM, 0, &dev);
    if (rv) {
        debug("DRAM init failed: %d\n", rv);
        return rv;
    }
    print_clocks();
    memcheck(0x80000000,0x4000000,0x12344321);

    return fdtdec_setup_mem_size_base();
}

int dram_init_banksize(void)
{
    return fdtdec_setup_memory_banksize();
}

#ifdef CONFIG_SPL_BUILD
#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
	debug("SPL: booting kernel\n");
	/* break into full u-boot on 'c' */
	return serial_tstc() && serial_getc() == 'c';
}
#endif

int spl_dram_init(void)
{
	struct udevice *dev;
	int rv;

	rv = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (rv)
		debug("DRAM init failed: %d\n", rv);
	return rv;
}

void spl_board_init(void)
{
	preloader_console_init();
	spl_dram_init();
	arch_cpu_init(); /* to configure mpu for sdram rw permissions */

    print_clocks();
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}
#endif

int board_init(void)
{
    return 0;
}