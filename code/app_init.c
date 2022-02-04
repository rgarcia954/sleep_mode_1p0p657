/**
 * @file app_init.c
 * @brief Power Mode Test Application Initialization Source Code
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

#define XTAL32K_CTRIM_21P6PF 				((uint32_t)(0x36U << ACS_XTAL32K_CTRL_CLOAD_TRIM_Pos))
#define XTAL32K_CTRIM_23P2PF             	((uint32_t)(0x3AU << ACS_XTAL32K_CTRL_CLOAD_TRIM_Pos))

void App_GPIO_Config(void)
{
    /* Disable JTAG TDI, TDO, and TRST connections to GPIO 2, 3, and 4 */
    GPIO->JTAG_SW_PAD_CFG &= ~(CM33_JTAG_DATA_ENABLED | CM33_JTAG_TRST_ENABLED);

    /* Configure POWER_MODE_GPIO */
    SYS_GPIO_CONFIG(POWER_MODE_GPIO, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                      GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Clear power mode GPIO to indicate run mode */
    Sys_GPIO_Set_Low(POWER_MODE_GPIO);

    /* Configure SYSCLK_GPIO to output system clock */
    SYS_GPIO_CONFIG(SYSCLK_GPIO, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                  GPIO_WEAK_PULL_UP | GPIO_MODE_SYSCLK));

    /* Configure APP_TEST_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_GPIO, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                           GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure FIFO_FULL_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_FIFO_FULL, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                                GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure RTC_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_RTC, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                          GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure NFC_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_NFC, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                          GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure BBTIMER_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_BBTIMER, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                              GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure ADC_THRESHOLD_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_THRESHOLD, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                                GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));

    /* Configure SENSOR_DET_GPIO */
    SYS_GPIO_CONFIG(WAKEUP_ACTIVITY_SENSOR_DET, (GPIO_2X_DRIVE | GPIO_LPF_DISABLE |
                                                GPIO_WEAK_PULL_UP | GPIO_MODE_GPIO_OUT));
}

void DeviceInit(void)
{
    /* Hold application here if recovery GPIO is held low during boot.
     * This makes it easier for the debugger to connect and reprogram the device. */
    SYS_GPIO_CONFIG(RECOVERY_GPIO, (GPIO_MODE_GPIO_IN | GPIO_LPF_DISABLE |
                                    GPIO_WEAK_PULL_UP  | GPIO_6X_DRIVE));

    while ((Sys_GPIO_Read(RECOVERY_GPIO)) == 0)
    {
        SYS_WATCHDOG_REFRESH();
    }

    /* Load default regulator trim values. */
    uint32_t trim_error __attribute__ ((unused)) = SYS_TRIM_LOAD_DEFAULT();

    /* Set all the GPIOs to a known state to minimize the leakage current from GPIO pins */
    for (uint8_t i = 0; i < GPIO_PAD_COUNT; i++)
    {
        SYS_GPIO_CONFIG(i, (GPIO_MODE_DISABLE | GPIO_LPF_DISABLE |
                            GPIO_STRONG_PULL_UP | GPIO_2X_DRIVE));
    }

    /* Calibrate the board */
   #if ((CALIB_RECORD == SUPPLEMENTAL_CALIB) || (CALIB_RECORD == MANU_CALIB))
    if (Load_Trim_Values() != VOLTAGES_CALIB_NO_ERROR)
    {
        /* Hold here to notify error(s) in voltage calibrations */
        while (true)
        {
            SYS_WATCHDOG_REFRESH();
        }
    }
   #elif (CALIB_RECORD == USER_CALIB)
    if (Calculate_Trim_Values_And_Calibrate() !=
        VOLTAGES_CALIB_NO_ERROR)
    {
        /* Hold here to notify error(s) in voltage calibrations */
        while (true)
        {
            SYS_WATCHDOG_REFRESH();
        }
    }
   #endif    /* CALIB_RECORD */

    /* Enable/disable buck converter */
    ACS->VCC_CTRL = (((ACS->VCC_CTRL) & (~(VCC_BUCK))) | (VCC_BUCK_LDO_CTRL));

    /* Configure and initialize system clock */
    App_Clock_Config();

#if DEBUG_SLEEP_GPIO

    /* Configure GPIOs */
    App_GPIO_Config();
#endif    /* if DEBUG_SLEEP_GPIO */

#ifdef VOLTAGES_CALIB_VERIFY

    /* Hold here to verify calibrated voltages */
    while (true)
    {
        SYS_WATCHDOG_REFRESH();
    }
#endif    /* ifdef VOLTAGES_CALIB_VERIFY */

    /* If applicable, power down the following four blocks
     * to lower the power consumption. */
#if SENSOR_POWER_DISABLE

    Sys_Sensor_Disable();
#endif    /* if SENSOR_POWER_DISABLE */

#if CC312AO_POWER_DISABLE

    Sys_Power_CC312AO_Disable();
#endif    /* if CC312AO_POWER_DISABLE */

#if POWER_DOWN_FPU

    Power_Down_FPU();
#endif    /* if POWER_DOWN_FPU */

#if POWER_DOWN_DBG

    Power_Down_Debug();
#endif    /* if POWER_DOWN_DBG */

#if VDDIF_POWER_DOWN

    ACS->VDDIF_CTRL = VDDIF_DISABLE;
#endif	/* if VDDIF_POWER_DOWN */

    /* Enable the wakeup source configuration */
    Wakeup_Source_Config();

    /* Sleep Initialization for Power Mode */
    App_Sleep_Initialization();

    /* Clear reset flags */
    RESET->DIG_STATUS = (uint32_t)0x1F00;
    ACS->RESET_STATUS = (uint32_t)0x3FF;
}

void DisableAppInterrupts(void)
{
    Sys_NVIC_DisableAllInt();
    Sys_NVIC_ClearAllPendingInt();
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_DISABLE_INTERRUPTS);
}

void EnableAppInterrupts(void)
{
    __set_FAULTMASK(FAULTMASK_ENABLE_INTERRUPTS);
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
}

void App_Clock_Config(void)
{
    uint8_t systemclk_div;

    /* Set the system clock prescale to output value defined in
     * SYSTEM_CLK */
    systemclk_div = (RFCLK_BASE_FREQ / SYSTEM_CLK);

    /* Boundary condition check for the system clock division */
    /* If the prescale is greater than CK_DIV_1_6_PRESCALE_7_BYTE set it to
     * CK_DIV_1_6_PRESCALE_7_BYTE to avoid system clock being an unknown value */
    if (systemclk_div > CK_DIV_1_6_PRESCALE_7_BYTE)
    {
        systemclk_div = CK_DIV_1_6_PRESCALE_7_BYTE;
    }
    /* If the system clock prescale is less than CK_DIV_1_6_PRESCALE_1_BYTE set it to
     * CK_DIV_1_6_PRESCALE_1_BYTE to avoid system clock being an unknown value */
    if (systemclk_div < CK_DIV_1_6_PRESCALE_1_BYTE)
    {
        systemclk_div = CK_DIV_1_6_PRESCALE_1_BYTE;
    }

    /* Start 48 MHz XTAL oscillator */
    Sys_Clocks_XTALClkConfig(systemclk_div);

    /* Switch to (divided 48 MHz) oscillator clock, and update the
     * SystemCoreClock global variable. */
    Sys_Clocks_SystemClkConfig(SYSCLK_CLKSRC_RFCLK);

    /* Configure clock dividers */
    Sys_Clocks_DividerConfig(UART_CLK, SENSOR_CLK, USER_CLK);

   /* Enable standby clock */
#if (WAKEUP_SRC_RTC_ALARM_EN || WAKEUP_SRC_BB_EN || WAKEUP_SRC_FIFO_EN ||\
		WAKEUP_SRC_ADC_THRESHOLD_EN )

    /* Enable XTAL32k */
    ACS->XTAL32K_CTRL = XTAL32K_XIN_CAP_BYPASS_DISABLE | XTAL32K_NOT_FORCE_READY | XTAL32K_CTRIM_21P6PF |
    		XTAL32K_ITRIM_160NA | XTAL32K_ENABLE | XTAL32K_AMPL_CTRL_ENABLE;

    /* Wait for XTAL32k to be configured */
    while (ACS->XTAL32K_CTRL && ((0x1U << ACS_XTAL32K_CTRL_READY_Pos) != XTAL32K_OK));
#endif    /* if (WAKEUP_SRC_RTC_ALARM_EN || WAKEUP_SRC_BB_EN || WAKEUP_SRC_FIFO_EN ||\
		   * WAKEUP_SRC_ADC_THRESHOLD_EN ) */
}

void App_Sleep_Initialization(void)
{
#if DEBUG_SLEEP_GPIO

    app_sleep_mode_cfg.app_gpio_config = App_GPIO_Config;
#else    /* if DEBUG_SLEEP_GPIO */

    app_sleep_mode_cfg.app_gpio_config = NULL;
#endif    /* if DEBUG_SLEEP_GPIO */

    /* Wakeup Configuration */
    app_sleep_mode_cfg.wakeup_cfg = WAKEUP_DELAY_16                     |
                                    WAKEUP_GPIO1_ENABLE                 |
                                    WAKEUP_GPIO1_RISING                 |
#if (WAKEUP_SRC_FIFO_EN)
                                    WAKEUP_FIFO_ENABLE                  |
#endif    /* if (WAKEUP_SRC_FIFO_EN) */
#if (WAKEUP_SRC_NFC_EN)
                                    WAKEUP_NFC_FIELD_ENABLE             |
#endif    /* if (WAKEUP_SRC_NFC_EN) */
                                    WAKEUP_DCDC_OVERLOAD_DISABLE;

    /* Clock Configuration for Run Mode */
    app_sleep_mode_cfg.clock_cfg.sensorclk_freq = SENSOR_CLK;
    app_sleep_mode_cfg.clock_cfg.systemclk_freq = SYSTEM_CLK;
    app_sleep_mode_cfg.clock_cfg.uartclk_freq = UART_CLK;
    app_sleep_mode_cfg.clock_cfg.userclk_freq = USER_CLK;

    /* VDD Retention Regulator Configuration */
    /*  By default TRIM values are set to 0x03. The user can to 0x00 to
     *  further reduce power consumption but this will limit operational
     *  capabilities across extended temperature range. */
    app_sleep_mode_cfg.vddret_ctrl.vddm_ret_trim = VDDMRETENTION_TRIM_MAXIMUM;
    app_sleep_mode_cfg.vddret_ctrl.vddc_ret_trim = VDDCRETENTION_TRIM_MAXIMUM;
    app_sleep_mode_cfg.vddret_ctrl.vddacs_ret_trim = VDDACSRETENTION_TRIM_MAXIMUM;

#if WAKEUP_SRC_BB_EN

    /* Enable VDDT Baseband Timer if BLE is Present */
    app_sleep_mode_cfg.vddret_ctrl.vddt_ret = VDDTRETENTION_ENABLE;
#endif    /* if WAKEUP_SRC_BB_EN */

    /* BLE functionality is present in this application */
    app_sleep_mode_cfg.ble_present = BLE_NOT_PRESENT;

    /* Boot Configuration */
#if SLEEP_MODE_TEST == SLEEP_MODE_TEST_CORE_RETENTION

    app_sleep_mode_cfg.boot_cfg = BOOT_FLASH_XTAL_DEFAULT_TRIM    |
                                  BOOT_PWR_CAL_BYPASS_ENABLE      |
                                  BOOT_ROT_BYPASS_ENABLE;

#elif SLEEP_MODE_TEST == SLEEP_MODE_TEST_NO_RETENTION

    app_sleep_mode_cfg.boot_cfg = BOOT_FLASH_XTAL_DEFAULT_TRIM    |
                                  BOOT_PWR_CAL_BYPASS_ENABLE      |
                                  BOOT_ROT_BYPASS_ENABLE;

#elif SLEEP_MODE_TEST == DEEP_SLEEP_TEST

    app_sleep_mode_cfg.boot_cfg = BOOT_FLASH_XTAL_DEFAULT_TRIM    |
                                  BOOT_PWR_CAL_BYPASS_ENABLE      |
                                  BOOT_ROT_BYPASS_ENABLE;
#endif    /* if SLEEP_MODE_TEST == SLEEP_MODE_TEST_CORE_RETENTION */
}

uint32_t Power_Down_FPU(void)
{
    uint32_t success;

    /* Request power down of FPU */
    SYSCTRL->FPU_PWR_CFG = FPU_WRITE_KEY | ((SYSCTRL->FPU_PWR_CFG &
                                             ~(0x1U << SYSCTRL_FPU_PWR_CFG_FPU_Q_REQ_Pos)) | FPU_Q_REQUEST);

    /* Wait until it is accepted or denied */
    while ((SYSCTRL->FPU_PWR_CFG & (0x1U << SYSCTRL_FPU_PWR_CFG_FPU_Q_ACCEPT_Pos)) !=
           FPU_Q_ACCEPTED &&
           (SYSCTRL->FPU_PWR_CFG & (0x1U << SYSCTRL_FPU_PWR_CFG_FPU_Q_DENY_Pos))   !=
           FPU_Q_DENIED);

    if ((SYSCTRL->FPU_PWR_CFG & (0x1U << SYSCTRL_FPU_PWR_CFG_FPU_Q_ACCEPT_Pos)) ==
        FPU_Q_ACCEPTED)
    {
        /* If it is accepted, isolate and power down FPU */
        SYSCTRL->FPU_PWR_CFG = FPU_WRITE_KEY | FPU_Q_REQUEST | FPU_ISOLATE |
                               FPU_PWR_TRICKLE_ENABLE  | FPU_PWR_HAMMER_ENABLE;
        SYSCTRL->FPU_PWR_CFG = FPU_WRITE_KEY | FPU_Q_REQUEST | FPU_ISOLATE |
                               FPU_PWR_TRICKLE_DISABLE | FPU_PWR_HAMMER_DISABLE;
        success = FPU_Q_ACCEPTED;
    }
    else
    {
        /* If it is denied, cancel request */
        SYSCTRL->FPU_PWR_CFG = FPU_WRITE_KEY | ((SYSCTRL->FPU_PWR_CFG &
                                                 ~(0x1U << SYSCTRL_FPU_PWR_CFG_FPU_Q_REQ_Pos)) | FPU_Q_NOT_REQUEST);
        success = FPU_Q_DENIED;
    }

    return success;
}

uint32_t Power_Down_Debug(void)
{
    uint32_t success;

    /* Request power down of DBG */
    SYSCTRL->DBG_PWR_CFG = DBG_WRITE_KEY | ((SYSCTRL->DBG_PWR_CFG &
                                             ~(0x1U << SYSCTRL_DBG_PWR_CFG_DBG_Q_REQ_Pos)) | DBG_Q_REQUEST);

    /* Wait until it is accepted or denied */
    while ((SYSCTRL->DBG_PWR_CFG & (0x1U << SYSCTRL_DBG_PWR_CFG_DBG_Q_ACCEPT_Pos)) !=
           DBG_Q_ACCEPTED &&
           (SYSCTRL->DBG_PWR_CFG & (0x1U << SYSCTRL_DBG_PWR_CFG_DBG_Q_DENY_Pos))   !=
           DBG_Q_DENIED);

    if ((SYSCTRL->DBG_PWR_CFG & (0x1U << SYSCTRL_DBG_PWR_CFG_DBG_Q_ACCEPT_Pos)) ==
        DBG_Q_ACCEPTED)
    {
        /* If it is accepted, isolate and power down DBG */
        SYSCTRL->DBG_PWR_CFG = DBG_WRITE_KEY | DBG_Q_REQUEST | DBG_ISOLATE |
                               DBG_PWR_TRICKLE_ENABLE  | DBG_PWR_HAMMER_ENABLE;
        SYSCTRL->DBG_PWR_CFG = DBG_WRITE_KEY | DBG_Q_REQUEST | DBG_ISOLATE |
                               DBG_PWR_TRICKLE_DISABLE | DBG_PWR_HAMMER_DISABLE;
        success = DBG_Q_ACCEPTED;
    }
    else
    {
        /* If it is denied, cancel request */
        SYSCTRL->DBG_PWR_CFG = DBG_WRITE_KEY | ((SYSCTRL->DBG_PWR_CFG &
                                                 ~(0x1U << SYSCTRL_DBG_PWR_CFG_DBG_Q_REQ_Pos)) | DBG_Q_NOT_REQUEST);
        success = DBG_Q_DENIED;
    }

    return success;
}
