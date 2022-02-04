/**
 * @file app.c
 * @brief Sleep Mode Test Application
 *
 * @copyright @parblock
 * Copyright (c) 2022 Semiconductor Components Industries, LLC (d/b/a
 * onsemi), All Rights Reserved
 *
 * This code is the property of onsemi and may not be redistributed
 * in any form without prior written permission from onsemi.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between onsemi and the licensee.
 *
 * This is Reusable Code.
 * @endparblock
 */
#include "app.h"

/* Mask to get all reset flags from ACS_RESET_STATUS register */
#define ACS_RESET_STATUS_RESET_FLAGS_MASK ((uint32_t)(0x03FF0000))

/* Mask to get ACS Reset flag from RESET_STATUS_DIG register */
#define RESET_DIG_STATUS_ACS_RESET_FLAGS_MASK ((uint32_t)(0x00000001))

/* Wakeup flag for RTC */
uint8_t wakeup_due_to_RTC = 0;

void BLE_SLP_IRQHandler(void)
{
#if DEBUG_SLEEP_GPIO
    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_BBTIMER);

    /* Toggle this pin 6 times to emulate application operations
     * for BB timer wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_BBTIMER);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}

void Main_Loop(void)
{
	RTC_ALARM_Reconfig(CONVERT_TO_RTC_TIMER_COUNTER(RTC_SLEEP_TIME_MS));

    while (1)
    {
    	SYS_WATCHDOG_REFRESH();

    	GLOBAL_INT_DISABLE();

    	SoC_Sleep();

    	GLOBAL_INT_RESTORE();

    	/* Only reached in the case where VDDC is enabled */

    	if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_NFC) & WAKEUP_SRC_EN_MSK)
    	{
    		/* Stay in run mode for a defined number of seconds
    		 * after waking up from NFC to reduce fluctuation */
    		for (unsigned int i = 0; i < APP_DELAY_S; i++)
    		{
    			SYS_WATCHDOG_REFRESH();
    			Sys_Delay(SystemCoreClock);
    		}
    	}

    	/* If wakeup due to RTC timeout */
    	if(wakeup_due_to_RTC)
    	{
    		/* clear flag to consider next wakeup */
    		wakeup_due_to_RTC = 0;

    		/* Configure RTC alarm again for next wakeup */
    		RTC_ALARM_Reconfig(CONVERT_TO_RTC_TIMER_COUNTER(RTC_SLEEP_TIME_MS));
    	}
    }
}

/**
 * @brief Sleep mode main application
 */
int main(void)
{
    /* Check if reset due to wakeup from sleep mode:
     *   - All reset flags from ACS_RESET_STATUS register are clear, and
     *   - ACS Reset flag from RESET_STATUS_DIG register is set (regardless of
     *     all other flags) */
    if (((ACS->RESET_STATUS & ACS_RESET_STATUS_RESET_FLAGS_MASK) == 0x0) &&
        ((RESET->DIG_STATUS & RESET_DIG_STATUS_ACS_RESET_FLAGS_MASK) == 0x1))
    {
        App_Sleep_Initialization();

        /* Reinitialize the system after wakeup */
        Sys_PowerModes_Wakeup_WithReset(&app_sleep_mode_cfg);

        EnableAppInterrupts();

        if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_NFC) & WAKEUP_SRC_EN_MSK)
        {
        	/* Stay in run mode for a defined number of seconds
        	 * after waking up from NFC to reduce fluctuation */
        	for (unsigned int i = 0; i < APP_DELAY_S; i++)
        	{
        		SYS_WATCHDOG_REFRESH();
        		Sys_Delay(SystemCoreClock);
        	}
        }
    }
    else    /* Else: Not wakeup from SLEEP mode */
    {
        DisableAppInterrupts();

        /* IMPORTANT: always have this to make sure RECOVERY_GPIO is not frozen
         * (in case sleep then wake up from FLASH, main() is executed and in here
         * pad retention could be enabled)*/

        /* Disable pad retention */
        ACS_BOOT_CFG->PADS_RETENTION_EN_BYTE = PADS_RETENTION_DISABLE_BYTE;

        /* Configure clocks, GPIOs, trace interface and load calibration data */
        DeviceInit();

        /* Enable all interrupts */
        EnableAppInterrupts();
    }

    /* Execute main loop */
    Main_Loop();
}
