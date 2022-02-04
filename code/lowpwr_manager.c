/**
 * @file lowpwr_manager.c
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

/* sleep mode initialization variable */
sleep_mode_cfg app_sleep_mode_cfg;

void SoC_Sleep(void)
{
#if SLEEP_MODE_TEST == SLEEP_MODE_TEST_CORE_RETENTION

    /* Initialize sleep before entering sleep */
    Sys_PowerModes_Sleep_Init(&app_sleep_mode_cfg);

#if DEBUG_SLEEP_GPIO

    /* Set power mode GPIO to indicate power mode */
    Sys_GPIO_Set_High(POWER_MODE_GPIO);
#endif    /* if DEBUG_SLEEP_GPIO */

    /* Power Mode enter sleep with core retention */
    Sys_PowerModes_Sleep_Enter(&app_sleep_mode_cfg, SLEEP_CORE_RETENTION);

#elif SLEEP_MODE_TEST == SLEEP_MODE_TEST_NO_RETENTION

    /* Initialize sleep before entering sleep */
    Sys_PowerModes_Sleep_Init(&app_sleep_mode_cfg);

#if DEBUG_SLEEP_GPIO

    /* Set power mode GPIO to indicate power mode */
    Sys_GPIO_Set_High(POWER_MODE_GPIO);
#endif    /* if DEBUG_SLEEP_GPIO */
    /* To disable the clock detector, it is recommended to:
     * 1. Ignore the reset by setting the ACS_CLK_DET_CTRL.RESET_IGNORE bit.
     * 2. Disable the clock detector by resetting the ACS_CLK_DET_CTRL.ENABLE bit. */
    ACS->CLK_DET_CTRL |= ((uint32_t)(0x1U << ACS_CLK_DET_CTRL_RESET_IGNORE_Pos));
    ACS->CLK_DET_CTRL &= ~((uint32_t)(0x1U << ACS_CLK_DET_CTRL_ENABLE_Pos));

    /* Clear reset flags */
    RESET->DIG_STATUS = (uint32_t)0xFFFF;
    ACS->RESET_STATUS = (uint32_t)0xFFFF;

    /* Power Mode enter sleep with memory retention */
    Sys_PowerModes_Sleep_Enter(&app_sleep_mode_cfg, SLEEP_NO_RETENTION);

#elif SLEEP_MODE_TEST == DEEP_SLEEP_TEST

    /* Before activating the storage mode with active sensor detector
     * it has to be measured if the sensor indicates that no sensor is connected
     * if the sensor detected bit is high the software must immediately
     * disable the sensor detector
     */
    if(ACS->SENSOR_DET_CFG == SENSOR_DETECTED)
    {
    	ACS->SENSOR_DET_CFG = SENSOR_DET_DISABLED;
    	return;
    }

    /* Initialize sleep before entering sleep */
    Sys_PowerModes_DeepSleep_Init((deepsleep_mode_cfg *)&app_sleep_mode_cfg);

#if DEBUG_SLEEP_GPIO

    /* Set power mode GPIO to indicate power mode */
    Sys_GPIO_Set_High(POWER_MODE_GPIO);
#endif    /* if DEBUG_SLEEP_GPIO */
    /* To disable the clock detector, it is recommended to:
     * 1. Ignore the reset by setting the ACS_CLK_DET_CTRL.RESET_IGNORE bit.
     * 2. Disable the clock detector by resetting the ACS_CLK_DET_CTRL.ENABLE bit. */
    ACS->CLK_DET_CTRL |= ((uint32_t)(0x1U << ACS_CLK_DET_CTRL_RESET_IGNORE_Pos));
    ACS->CLK_DET_CTRL &= ~((uint32_t)(0x1U << ACS_CLK_DET_CTRL_ENABLE_Pos));

    /* Clear reset flags */
    RESET->DIG_STATUS = (uint32_t)0xFFFF;
    ACS->RESET_STATUS = (uint32_t)0xFFFF;

    /* Power Mode enter sleep with memory retention */
    Sys_PowerModes_DeepSleep_Enter((deepsleep_mode_cfg *)&app_sleep_mode_cfg);

#endif    /* if SLEEP_MODE_TEST == SLEEP_MODE_TEST_CORE_RETENTION */
}

/**
 * @brief FIFO Wakeup Handler routine
 */
void FIFO_Wakeup_Process_Handler(void)
{
    WAKEUP_FIFO_FULL_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO
    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_FIFO_FULL);
#endif    /* DEBUG_SLEEP_GPIO */

    /* The FIFO_LEVEL is asynchronous to the system clock it is recommended to
     * read two consecutive times to check the consistency of the value */
    uint8_t fifo_level = 0;
    uint8_t previous_fifo_level = 0;
    uint8_t i = 0;
    uint8_t read_flag = false;

    /* Read the first FIFO level to check if the level is equal to the FIFO size configured. */
    fifo_level = (uint8_t)((SENSOR->FIFO_CFG & SENSOR_FIFO_CFG_FIFO_LEVEL_Mask) >> SENSOR_FIFO_CFG_FIFO_LEVEL_Pos);

    do
    {
        previous_fifo_level = fifo_level;

        /* Read FIFO_LEVEL for second time to check the consistency */
        fifo_level = (uint8_t)(SENSOR->FIFO_CFG & SENSOR_FIFO_CFG_FIFO_LEVEL_Mask) >> SENSOR_FIFO_CFG_FIFO_LEVEL_Pos;

        /* Compare if consecutive read are equal */
        if (previous_fifo_level == fifo_level)
        {
            read_flag = true;
        }

        i++;
    }
    while ((i < 10) && (!read_flag));

    /* Force to reset FIFO in here */
    SensorFIFO_Reset();

#if DEBUG_SLEEP_GPIO

    /* Toggle this pin 6 times to emulate application operations
     * for FIFO wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_FIFO_FULL);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}

/**
 * @brief GPIO1 wakeup Handler routine
 */
void GPIO1_Wakeup_Process_Handler(void)
{
    WAKEUP_GPIO1_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO

    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_GPIO);

    /* Toggle this pin 6 times to emulate application operations
     * for GPIO wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_GPIO);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}

void RTC_Alarm_Wakeup_Process_Handler(void)
{
    WAKEUP_RTC_ALARM_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO

    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_RTC);

    /* Toggle this pin 6 times to emulate application operations
     * for RTC wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_RTC);
    }
#endif    /* DEBUG_SLEEP_GPIO */

    wakeup_due_to_RTC = 1;
}

void NFC_Wakeup_Process_Handler(void)
{
    WAKEUP_NFC_FIELD_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO

    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_NFC);

    /* Toggle this pin 6 times to emulate application operations
     * for NFC wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_NFC);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}

void Threshold_Wakeup_Process_Handler(void)
{
    WAKEUP_THRESHOLD_FULL_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO

    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_THRESHOLD);

    /* Toggle this pin 6 times to emulate application operations
     * for ADC threshold wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_THRESHOLD);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}

void Sensor_Detection_Wakeup_Process_Handler(void)
{
	WAKEUP_SENSOR_DETECT_FLAG_CLEAR();

#if DEBUG_SLEEP_GPIO

    Sys_GPIO_Set_Low(WAKEUP_ACTIVITY_SENSOR_DET);

    /* Toggle this pin 6 times to emulate application operations
     * for ADC threshold wakeup in run mode */
    for (uint8_t i = 0; i < 6; i++)
    {
        Sys_GPIO_Toggle(WAKEUP_ACTIVITY_SENSOR_DET);
    }
#endif    /* DEBUG_SLEEP_GPIO */
}


/**
 * @brief   Wakeup IRQ interrupt handler
 */
void WAKEUP_IRQHandler(void)
{
    SYS_WATCHDOG_REFRESH();

    /* Call GPIO handler to process wakeup event */
    if (ACS->WAKEUP_CTRL & WAKEUP_GPIO1_EVENT_SET)
    {
        GPIO1_Wakeup_Process_Handler();
    }

    /* Call BBTimer handler to process wakeup event */
    if (ACS->WAKEUP_CTRL & WAKEUP_BB_TIMER_EVENT_SET)
    {
        NVIC_EnableIRQ(BLE_SLP_IRQn);

        BBIF->CTRL = (BB_CLK_ENABLE | BBCLK_DIVIDER_8 | BB_DEEP_SLEEP);

        /* Wakeup the BB timer intentionally */
        BBIF->CTRL |= (BB_WAKEUP);

        Sys_Delay((SystemCoreClock / (32768)) * 2);

        while ((BBIF->STATUS & LOW_POWER_CLK) != MASTER_CLK)
        {
            SYS_WATCHDOG_REFRESH();
        }

        BBIF->CTRL &= (~BB_WAKEUP);

        /* Re intialize BB Timer for next wakeup */
        BB_Timer_Init();

        /* Clear the BB Timer sticky flag */
        WAKEUP_BB_TIMER_FLAG_CLEAR();
    }

    /* Call FIFO Handler to process wakeup event */
    if (ACS->WAKEUP_CTRL & WAKEUP_FIFO_FULL_EVENT_SET)
    {
        FIFO_Wakeup_Process_Handler();
    }

    if (ACS->WAKEUP_CTRL & WAKEUP_THRESHOLD_EVENT_SET)
    {
        Threshold_Wakeup_Process_Handler();
    }

    if (ACS->WAKEUP_CTRL & WAKEUP_RTC_ALARM_EVENT_SET)
    {
        RTC_Alarm_Wakeup_Process_Handler();
    }

    if (ACS->WAKEUP_CTRL & WAKEUP_NFC_FIELD_EVENT_SET)
    {
        NFC_Wakeup_Process_Handler();
    }

    if (ACS->WAKEUP_CTRL & WAKEUP_SENSOR_DET_EVENT_SET)
    {
    	Sensor_Detection_Wakeup_Process_Handler();
    }

    /* If there is an pending wakeup event set during the execution of this
     * Wakeup interrupt handler. Set the NVIC WAKEUP_IRQn so that the pending wakeup
     * event will be serviced again */
    if (ACS->WAKEUP_CTRL)
    {
        if (!NVIC_GetPendingIRQ(WAKEUP_IRQn))
        {
            NVIC_SetPendingIRQ(WAKEUP_IRQn);
        }
    }
}
