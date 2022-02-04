/**
 * @file app.h
 * @brief Main Application Header File
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
#ifndef APP_H_
#define APP_H_

/* ----------------------------------------------------------------------------
 * Include files
 * --------------------------------------------------------------------------*/
#include "app_init.h"
#include "wakeup_source_config.h"
#include "flash_rom.h"
#include <calibration.h>

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

extern sleep_mode_cfg app_sleep_mode_cfg;

/* Options for wakeup restart address */
#define SLEEP_MODE_TEST_NO_RETENTION               0
#define SLEEP_MODE_TEST_MEMORY_RETENTION           1
#define SLEEP_MODE_TEST_CORE_RETENTION             2
#define DEEP_SLEEP_TEST                            3

/** @brief Disable interrupts globally in the system.
 * This macro must be used in conjunction with the @ref GLOBAL_INT_RESTORE macro since this
 * last one will close the brace that the current macro opens.  This means that both
 * macros must be located at the same scope level.
 */

#define GLOBAL_INT_DISABLE() ;                                               \
    do {                                                                        \
        uint32_t __primask_status;                                              \
        asm volatile ("MRS %0, primask" : "=r" (__primask_status));            \
        asm volatile ("MSR primask, %0" : : "r" (1));                          \

#define GLOBAL_INT_RESTORE() ;                                               \
    asm volatile ("MSR primask, %0" : : "r" (__primask_status));           \
    } while (0)

/* Sleep test mode options:
 *   - SLEEP_MODE_TEST_NO_RETENTION
 *   - SLEEP_MODE_TEST_CORE_RETENTION
 *   - DEEP_SLEEP_TEST
 */
#define SLEEP_MODE_TEST                 SLEEP_MODE_TEST_CORE_RETENTION

/* Wake up Source Options for Sleep Mode:
 * WAKEUP_SRC_RTC_ALARM
 * WAKEUP_SRC_FIFO
 * WAKEUP_SRC_BB
 * WAKEUP_SRC_ADC
 * WAKEUP_SRC_NFC
 * WAKEUP_SRC_GPIO
 *
 * Wake up Source Options for Storage Mode:
 * WAKEUP_SRC_GPIO
 * WAKEUP_SRC_NFC
 * WAKEUP_SRC_SENSOR_DET
 */

#define SENSOR_NORMAL       1
#define SENSOR_IMPEDANCE    0
#define SENSOR_WUT          0

/* Enable Wakeup Sources for the application */
#define WAKEUP_SRC_RTC_ALARM_EN            0
#define WAKEUP_SRC_BB_EN                   0
#define WAKEUP_SRC_GPIO_EN                 1
#define WAKEUP_SRC_FIFO_EN				   0
#define WAKEUP_SRC_NFC_EN                  0
#define WAKEUP_SRC_ADC_THRESHOLD_EN        0
#define WAKEUP_SRC_SENSOR_DETECTION_EN     0

/* Wakeup source enabled mask */
#define WAKEUP_SRC_EN_MSK     ((WAKEUP_SRC_RTC_ALARM_EN         << WAKEUP_SRC_RTC_ALARM)           | \
                               (WAKEUP_SRC_BB_EN                << WAKEUP_SRC_BB)                  | \
                               (WAKEUP_SRC_GPIO_EN              << WAKEUP_SRC_GPIO)                | \
                               (WAKEUP_SRC_FIFO_EN              << WAKEUP_SRC_FIFO)                | \
                               (WAKEUP_SRC_ADC_THRESHOLD_EN     << WAKEUP_SRC_ADC)                 | \
                               (WAKEUP_SRC_NFC_EN               << WAKEUP_SRC_NFC)				   | \
							   (WAKEUP_SRC_SENSOR_DETECTION_EN  << WAKEUP_SRC_SENSOR_DET))

#define SYSTEM_CLK                      8000000

/* Set UART peripheral clock */
#define UART_CLK                        SYSTEM_CLK

/* Set sensor clock */
#define SENSOR_CLK                      32768

/* Set user clock */
#define USER_CLK                        1000000

/* Enable/disable buck converter
 * Options: VCC_BUCK or VCC_LDO
 */
#define VCC_BUCK_LDO_CTRL               VCC_LDO

/* Power Reduction Defines Begin */
/* Set this to 1 to Power Down Sensor Interface */
#define SENSOR_POWER_DISABLE            0

/* Set this to 1 to Power Down CryptoCell
 * notes: settings this to 1 will lock debug port and JTAG debug will not work */
#define CC312AO_POWER_DISABLE           0

/* GPIO number that is used for easy re-flashing (recovery mode) */
#define RECOVERY_GPIO                   7

/* Set this to 1 if you want to have debug GPIO capability for sleep mode or run mode */
#define DEBUG_SLEEP_GPIO                1

/* Set this to 1 to Power Down FPU
 * note: If FPU is used during run mode this should be left 0 */
#define POWER_DOWN_FPU                  0

/* Set this to 1 to Power Down Debug Unit
 * note: If Debug Port is used during run mode this should be left 0 */
#define POWER_DOWN_DBG                  0

/* GPIO used to output the sysclock */
#define SYSCLK_GPIO                     2

/* GPIO used to monitor FIFO FULL wakeup activity */
#define WAKEUP_ACTIVITY_FIFO_FULL       5

/* GPIO used to monitor GPIO1 wakeup activity */
#define WAKEUP_ACTIVITY_GPIO            4

/* GPIO used to monitor RTC wakeup activity */
#define WAKEUP_ACTIVITY_RTC             6

/* GPIO used to monitor BBTimer wakeup activity */
#define WAKEUP_ACTIVITY_BBTIMER         4

/* GPIO used to monitor Sensor Threshold wakeup activity */
#define WAKEUP_ACTIVITY_THRESHOLD       5

/* GPIO used to monitor NFC wakeup activity */
#define WAKEUP_ACTIVITY_NFC             4

/* GPIO used to monitor Sensor detection wakeup activity */
#define WAKEUP_ACTIVITY_SENSOR_DET      4

/* GPIO wakeup pin */
#define GPIO_WAKEUP_PIN                 1

/* GPIO used to indicate run and power mode
 *   - Run mode: low
 *   - Power mode (sleep or storage): high */
#define POWER_MODE_GPIO                 0

/* Define delay in run mode after wakeup from NFC in seconds */
#define APP_DELAY_S                     3

/* Disable pad retention */
#define DISABLE_PAD_RET()               ACS->BOOT_CFG &= \
    ~((uint32_t)(0x1U << ACS_BOOT_CFG_PADS_RETENTION_EN_Pos))

/* Disable VDDIF interface regulator */
#define VDDIF_POWER_DOWN                1

/* Sensor Calibration mode */
#define SENSOR_CALIB                    0

/* Convert time(ms) to RTC timer counter value */
#define CONVERT_TO_RTC_TIMER_COUNTER(x) 	(x * 32.768)

/* Flag to check if wakeup happened due to RTC */
extern uint8_t wakeup_due_to_RTC;

/* ---------------------------------------------------------------------------
* Function prototypes
* --------------------------------------------------------------------------*/
/**
 * @brief Performs application operations and enters power modes
 */
void Main_Loop(void);

void SoC_Sleep(void);

void WAKEUP_IRQHandler(void);

void FIFO_Wakeup_Process_Handler(void);

void GPIO1_Wakeup_Process_Handler(void);

void RTC_Alarm_Wakeup_Process_Handler(void);

void NFC_Wakeup_Process_Handler(void);

void Threshold_Wakeup_Process_Handler(void);

void Sensor_Detection_Wakeup_Process_Handler(void);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* APP_H_ */
