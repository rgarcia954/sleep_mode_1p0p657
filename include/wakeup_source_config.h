/**
 * @file wakeup_source_config.h
 * @brief Wakeup source configuration header file
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
#ifndef WAKEUP_SOURCE_CONFIG_H_
#define WAKEUP_SOURCE_CONFIG_H_

/* ----------------------------------------------------------------------------
 * Include files
 * --------------------------------------------------------------------------*/
#include "montana.h"
#include "api_isohfllhw.h"
#include "platform_config.h"
#include "nfc.h"
#include "iso14443.h"

/* Wake up sources */
#define WAKEUP_SRC_RTC_ALARM            0
#define WAKEUP_SRC_RTC_CLOCK            1
#define WAKEUP_SRC_FIFO                 2
#define WAKEUP_SRC_BB                   3
#define WAKEUP_SRC_NFC                  4
#define WAKEUP_SRC_ADC                  5
#define WAKEUP_SRC_GPIO                 6
#define WAKEUP_SRC_SENSOR_DET           7

#define WAKEUP_SRC_FLAG_BIT_SET              1

/* Clear all sticky wake up flags */
#define WAKEUP_FLAGS_CLEAR()                 ACS->WAKEUP_CTRL = (uint32_t)(0xFFFF)

/* Clear sticky wake up RTC alarm flag */
#define WAKEUP_RTC_ALARM_FLAG_CLEAR()        ACS->WAKEUP_CTRL |= WAKEUP_RTC_ALARM_EVENT_CLEAR

/* Clear sticky wake up GPIO0 flag */
#define WAKEUP_GPIO0_FLAG_CLEAR()            ACS->WAKEUP_CTRL |= WAKEUP_GPIO0_EVENT_CLEAR

/* Clear sticky wake up GPIO1 flag */
#define WAKEUP_GPIO1_FLAG_CLEAR()            ACS->WAKEUP_CTRL |= WAKEUP_GPIO1_EVENT_CLEAR

/* Clear sticky wake up GPIO2 flag */
#define WAKEUP_GPIO2_FLAG_CLEAR()            ACS->WAKEUP_CTRL |= WAKEUP_GPIO2_EVENT_CLEAR

/* Clear sticky wake up GPIO3 flag */
#define WAKEUP_GPIO3_FLAG_CLEAR()            ACS->WAKEUP_CTRL |= WAKEUP_GPIO3_EVENT_CLEAR

/* Clear sticky wake up BB Timer flag */
#define WAKEUP_BB_TIMER_FLAG_CLEAR()         ACS->WAKEUP_CTRL |= WAKEUP_BB_TIMER_CLEAR

/* Clear sticky wake up SENSOR DETECT flag */
#define WAKEUP_SENSOR_DETECT_FLAG_CLEAR()    ACS->WAKEUP_CTRL |= WAKEUP_SENSOR_DET_EVENT_CLEAR

/* Clear sticky wake up FIFO FULL flag */
#define WAKEUP_FIFO_FULL_FLAG_CLEAR()        ACS->WAKEUP_CTRL |= WAKEUP_FIFO_FULL_EVENT_CLEAR

/* Clear sticky wake up THRESHOLD FULL flag */
#define WAKEUP_THRESHOLD_FULL_FLAG_CLEAR()   ACS->WAKEUP_CTRL |= THRESHOLD_FULL_EVENT_CLEAR

/* Clear sticky wake up NFC FIELD flag */
#define WAKEUP_NFC_FIELD_FLAG_CLEAR()        ACS->WAKEUP_CTRL |= WAKEUP_NFC_FIELD_EVENT_CLEAR

/* Clock source for RTC
 * Possible options:
 *   - RTC_CLK_SRC_RC_OSC: 32kHz RC oscillator
 *   - RTC_CLK_SRC_XTAL32K: 32kHz crystal oscillator
 *   - RTC_CLK_SRC_GPIO0: external oscillator on GPIO0
 *   - RTC_CLK_SRC_GPIO1: external oscillator on GPIO1 */
#define RTC_CLK_SRC                     RTC_CLK_SRC_XTAL32K

/* Clock source for sensor
 * Possible options:
 *   - SENSOR_CLK_RTC: RTC clock
 *   - SENSOR_CLK_SLOWCLK: slow clock */
#define SENSOR_CLK_SRC                  SENSOR_CLK_RTC

/* RTC sleep duration(milliseconds) used for power mode
 * Possible options:
 * 	- Any value between 5 to 300000 inclusive
 */
#define RTC_SLEEP_TIME_MS				(uint32_t)(10000)

/* Define the time in low power baseband clock cycles to spend in deep sleep mode
 * before waking-up the device using baseband timer
 * Possible options:
 *   - DEEPSLTIME_0
 *   - ((uint32_t)(0x1U << BB_DEEPSLWKUP_DEEPSLTIME_Pos))
 *   - etc. */
#define BB_DEEP_SLEEP_TIME              ((uint32_t)(0x4FFFF << BB_DEEPSLWKUP_DEEPSLTIME_Pos))

/* Define the time in low power clock cycles allowed
 * for stabilization of the high frequency oscillator (TWOSC) */
#define TWOSC_VALUE                     (uint32_t)(0x60)

/* Define the time in low power clock cycles allowed
 * for the radio module to leave low-power mode (TWRM) */
#define TWRM_VALUE                      (uint32_t)(0x0)

/* The number of ADC samples stored before waking up the core
 * Possible values:
 *   - SENSOR_FIFO_SIZE1: SIZE = 1
 *   - SENSOR_FIFO_SIZE2: SIZE = 2
 *   - etc. */
#define FIFO_SIZE_VALUE                 SENSOR_FIFO_SIZE1
//#define FIFO_SIZE_VALUE                 SENSOR_FIFO_SIZE2

/* The number of samples used by summation and threshold mode
 * Possible values:
 *   - SENSOR_NBR_SAMPLES_1: 1 sample or 1 pair used
 *   - SENSOR_NBR_SAMPLES_2: 2 sample or 2 pair used
 *   - ((uint32_t)(0x2U << SENSOR_PROCESSING_NBR_SAMPLES_Pos))
 *   - etc. */
#define NBR_SAMPLES_VALUE               SENSOR_NBR_SAMPLES_2;

/* The sensor data value threshold for wake up
 * Possible values:
 *   - SENSOR_THRESHOLD_DISABLED: Sensor threshold functionality is disabled
 *   - SENSOR_THRESHOLD_1: Increments counter if samples (LSBs) is >=0x0001
 *   - ((uint32_t)(0x2 << SENSOR_PROCESSING_THRESHOLD_Pos))
 *   - etc. */
#define ADC_THRESHOLD_VALUE             ((uint32_t)(0x01 << SENSOR_PROCESSING_THRESHOLD_Pos))
#define ADC_THRESHOLD_VALUE_1NA         ((uint32_t)(0x8333 << SENSOR_PROCESSING_THRESHOLD_Pos))
#define ADC_THRESHOLD_VALUE_77P75NA     ((uint32_t)(0x3e << SENSOR_PROCESSING_THRESHOLD_Pos))

/* The maximum number of sample that can be stored in the ADC data array (length) */
#define ADC_DATA_LENGTH                 16

/* Duration of "Pulse Count Sample" state
 * Possible values:
 *   - PULSE_COUNT_INT_0: Number of periods for "Pulse Count Sample State" = 1
 *                        (Duration of the state is 0.976 ms)
 *   - PULSE_COUNT_INT_1: Number of periods for "Pulse Count Sample State" = 2
 *                        (Duration of the state is 1.953 ms)
 *   - ((uint32_t)(0x2U << SENSOR_INT_CFG_PULSE_COUNT_INT_Pos))
 *   - etc. */
#define PULSE_COUNT_INT_VALUE           PULSE_COUNT_INT_3

/* Absolute Value of main counter to trigger the change of "Pre Count Sample"
 * state
 * Possible values:
 *   - PRE_COUNT_INT_0: Number of periods for "Pre Count Int State" = 1
 *                      (Duration of the state is 0.976 ms)
 *   - PRE_COUNT_INT_1: Number of periods for "Pre Count Int State" = 2
 *                      (Duration of the state is 1.953 ms)
 *   - ((uint32_t)(0x2U << SENSOR_INT_CFG_PRE_COUNT_INT_Pos))
 *   - etc. */
//#define PRE_COUNT_INT_VALUE             ((uint32_t)(0x77FF << SENSOR_INT_CFG_PRE_COUNT_INT_Pos))
#define PRE_COUNT_INT_VALUE_2S             ((uint32_t)(0x7FF << SENSOR_INT_CFG_PRE_COUNT_INT_Pos))
#define PRE_COUNT_INT_VALUE_3P9MS          ((uint32_t)(0x03 << SENSOR_INT_CFG_PRE_COUNT_INT_Pos))

/* Absolute Value of main counter to trigger the change of "Delay 1 WE_L" state
 * Possible Values:
 *   - DLY1_WE_L_0: Number of periods for "Delay_1_WE_L" state = 1
 *                  (Duration of the state is 0.976 ms @1kHz or 30.5 us @32kHz)
 *   - DLY1_WE_L_1: Number of periods for "Delay_1_WE_L" state = 2
 *                  (Duration of the state is 1.953 ms @1kHz or 61 us @32kHz)
 *   - ((uint32_t)(0x2U << SENSOR_DELAY_L_CFG_DLY1_WE_L_Pos))
 *   - etc. */
#define DLY1_WE_L_VALUE_1S               ((uint32_t)(0x3FF << SENSOR_DELAY_L_CFG_DLY1_WE_L_Pos))
#define DLY1_WE_L_VALUE_117P19MS         ((uint32_t)(0x77 << SENSOR_DELAY_L_CFG_DLY1_WE_L_Pos))

/* Absolute Value of main counter to trigger the change of "Delay 2 WE_L" state
 * Possible Values:
 *   - DLY2_WE_L_0: Number of periods for "Delay_2_WE_L" state = 1
 *                  (Duration of the state is 0.976 ms @1kHz or 30.5 us @32kHz)
 *   - DLY2_WE_L_1: Number of periods for "Delay_2_WE_L" state = 2
 *                  (Duration of the state is 1.953 ms @1kHz or 61 us @32kHz)
 *   - ((uint32_t)(0x2U << SENSOR_DELAY_L_CFG_DLY2_WE_L_Pos))
 *   - etc. */
#define DLY2_WE_L_VALUE                 DLY2_WE_L_0_SHORT

/* Absolute Value of main counter to trigger the change of "Delay 1 WE_H" state
 * Possible Values:
 *   - DLY1_WE_H_0: Number of periods for "Delay_1_WE_H" state = 1
 *                  (Duration of the state is 0.976 ms @1kHz or 30.5 us @32kHz)
 *   - DLY1_WE_H_1: Number of periods for "Delay_1_WE_H" state = 2
 *                  (Duration of the state is 1.953 ms @1kHz or 61 us @32kHz)
 *   - ((uint32_t)(0x2U << SENSOR_DELAY_H_CFG_DLY1_WE_H_Pos))
 *   - etc. */
#define DLY1_WE_H_VALUE                 DLY1_WE_H_0_SHORT

/* Absolute Value of main counter to trigger the change of "Delay 2 WE_H" state
 * Possible Values:
 *   - DLY2_WE_H_0: Number of periods for "Delay_2_WE_H" state = 1
 *                  (Duration of the state is 0.976 ms @1kHz or 30.5 us @32kHz)
 *   - DLY2_WE_H_1: Number of periods for "Delay_2_WE_H" state = 2
 *                  (Duration of the state is 1.953 ms @1kHz or 61 us @32kHz)
 *   - ((uint32_t)(0x2U << SENSOR_DELAY_H_CFG_DLY2_WE_H_Pos))
 *   - etc. */
#define DLY2_WE_H_VALUE                 DLY2_WE_H_0_SHORT

/* Absolute Value of main counter to trigger the change of "Idle Time" state
 * Possible Values:
 *   - IDLE_TIME_0: Number of periods for "Idle Time" state = 1
 *                  (Duration of the state is 0.976 ms)
 *   - IDLE_TIME_1: Number of periods for "Idle Time" state = 2
 *                  (Duration of the state is 1.953 ms)
 *   - ((uint32_t)(0x2U << SENSOR_IDLE_CFG_IDLE_TIME_Pos))
 *   - etc. */
#define IDLE_TIME_VALUE_27S             ((uint32_t)(0x6BFF << SENSOR_IDLE_CFG_IDLE_TIME_Pos))
#define IDLE_TIME_VALUE_29P875S         ((uint32_t)(0x777F << SENSOR_IDLE_CFG_IDLE_TIME_Pos))

/* ---------------------------------------------------------------------------
* Function prototype definitions
* --------------------------------------------------------------------------*/

void RTC_ALARM_Init(void);

void RTC_ClockSource_Init(void);

void RTC_ALARM_Reconfig(uint32_t timer_counter);

void GPIO_Interrupt_Init(void);

void BB_Timer_Init(void);

void Sensor_Init(void);

void ADC_Threshold_Init(void);

void ADC_FIFO_Init(void);

void NFC_Init(void);

void SENSOR_DET_Init(void);

void SensorFIFO_Reset(void);

void GPIO_Wakeup_Init(void);

void Wakeup_Source_Config(void);

void _isohf_configTypeALayer3BootAndWait_local(HFCTRL isohf, uint8_t *Layer3Source);

#endif    /* WAKEUP_SOURCE_CONFIG_H_ */
