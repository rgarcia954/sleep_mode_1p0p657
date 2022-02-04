/**
 * @file platform_config.h
 * @brief This files contains the mapping of the platform so as the
 * available tool functions
 *
 *
 */

#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif    /* ifdef __cplusplus */

#ifndef PLATFORM
#define PLATFORM
#endif    /* ifndef PLATFORM */

/* HF Peripheral*/

#define PLATFORM_HF_BUFFER_ADDR        0x20048000
#define PLATFORM_HF_BANK_ADDR          0x40040000

static inline void PLATFORM_SET_WATCHDOG(int offset)
{
}

static inline void HW_countDown(int timeout)
{
}

#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* __PLATFORM_CONFIG_H__ */
