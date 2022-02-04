/**
 * @file app_init.h
 * @brief Power Mode Test Application Initialization Header File
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
#ifndef APP_INIT_H_
#define APP_INIT_H_

/* ----------------------------------------------------------------------------
 * Include files
 * --------------------------------------------------------------------------*/
#include <hw.h>
#include "montana.h"

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ---------------------------------------------------------------------------
* Function prototype definitions
* --------------------------------------------------------------------------*/
/**
 * @brief Initializes System Clock
 */
void App_Clock_Config(void);

/**
 * @brief      Configures GPIOs to be used for test
 */
void App_GPIO_Config(void);

void App_Sleep_Initialization(void);

void DeviceInit(void);

void DisableAppInterrupts(void);

void EnableAppInterrupts(void);

/**
 * @brief Power Down the FPU Unit
 * @return FPU_Q_ACCEPTED if the FPU power down was successful.<br>
 *         FPU_Q_DENIED if the FPU power down failed.
 */
uint32_t Power_Down_FPU(void);

/**
 * @brief Power Down the DBG Unit
 * @return DBG_Q_ACCEPTED if the DBG power down was successful.<br>
 *         DBG_Q_DENIED if the DBG power down failed.
 */
uint32_t Power_Down_Debug(void);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* APP_INIT_H_ */
