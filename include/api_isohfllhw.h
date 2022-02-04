/**
 * @file api_isohfllhw.h
 *
 * @brief This file contains the Low Level Hardware functions
 * used to safely drive the HF subsystem (ISO14443)
 *
 * @author Bettina REBAUD (INVIA)
 *
 */

#ifndef __API_ISO14443LLHW_H__
#define __API_ISO14443LLHW_H__

#ifdef __cplusplus
extern "C" {
#endif    /* ifdef __cplusplus */

#include <stdint.h>
#include "platform_config.h"
#include "iso14443.h"

/**
 * Disable the HF Fc Clock
 *
 * This LLHW switches off the Fc clock extractor
 *
 */
void _LLHW_isohf_disableFc(HFCTRL isohf);

/**
 * Enables the HF Fc Clock
 *
 * This LLHW renables the HF Fc clock extractor
 *
 */
void _LLHW_isohf_enableFc(HFCTRL isohf);

/**
 * Wait until the SW reaches the Exec period
 *
 */
void _LLHW_isohf_waitUntilExec(HFCTRL isohf);

/**
 * Wait until the SW reaches the Tx period
 *
 */
void _LLHW_isohf_waitUntilTx(HFCTRL isohf);

/**
 * Wait until the SW reaches the Rx period
 *
 */
void _LLHW_isohf_waitUntilRx(HFCTRL isohf);

/**
 * Wait until the SW has the hand on HF digital controller
 *
 */
void _LLHW_isohf_waitUntilPlatformHand(HFCTRL isohf);

/**
 * Skip anticollision ISOA Layer3
 *
 * This LLHW allows to directly jump in ISOA Layer 4
 * Shall be launch before HW FSM triggering with WaitRx
 *
 */
void _LLHW_isohf_configSkipISOALayer3(HFCTRL isohf);

/**
 * Configures and controls the HF Subsystem for catching next Rx frame
 *
 * This LLHW configures the control register, sets the WAIT RX control
 *
 * Caution: The IO RAM is assumed ready to be used by the HF digital controller
 *
 * @param[in] back_to_halt   Allows to return in ISO Type A layer 3 and wait for new Rx frame
 *                           use HF_P_CTRL_BACK2HALT when calling the LLHW or set to 0
 */
void _LLHW_isohf_waitForRx(HFCTRL isohf, uint32_t back_to_halt);

/**
 * Configures and controls the HF Subsystem for launching the next Tx frame
 *
 * This LLHW configures the control register, sets the LAUNCH TX control
 *
 * Caution: The IO RAM is assumed ready to be used by the HF digital controller
 *
 * @param[in] back_to_halt        Allows to return in ISO Type A layer 3 and wait for new Rx frame
 *                                use HF_P_CTRL_BACK2HALT when calling the LLHW or set to 0
 * @param[in] silent_time         Defines the number of slots to be waited before Tx frame sending
 *                                0 to 15 - no check on the value
 * @param[in] tx_frame_size       Defines the Tx frame size in RAM buffer
 *                                1 to 0x400 - no check on the value
 * @param[in] end_of_transaction  Removes the Tx to Rx automatic reversal
 *                                use HF_P_CTRL_ENDOFTRANSAC when calling the driver or set to 0
 */
void _LLHW_isohf_launchTx(HFCTRL isohf,
                          uint32_t back_to_halt,
                          uint32_t silent_time,
                          uint32_t tx_frame_size,
                          uint32_t end_of_transaction);

/**
 * Get the Silent Time Ts depending on current slot timer read value
 *
 * To be done before Tx launch
 *
 * @param[in] min_n_val     the LLHW will set the silent time such as to be reached the min_n_val in any case.
 *                          Shall be set to 8 or 9
 *
 * @return the LLHW returns the silent time to be used
 *
 */
uint32_t _LLHW_isohf_getSilentTime(HFCTRL isohf,  uint32_t min_n_val);

/**
 * Front-end digital selection for Type A - Tx
 *
 * This LLHW selects the :
 * - OOK or BPSK modulation (Tx)
 * - bit rates
 * Configures and controls the HF Subsystem for catching next Rx frame
 *
 * It keeps the last Rx configuration.
 * This LLHW configures the control register, sets the WAIT RX control
 *
 * @param[in] tx_bit_rate    Bit rate from 106 to 424
 *                           use DSI
 *                             0 = 106 kbits/s
 *                             1 = 212 kbits/s
 *                             2 = 424 kbits/s
 *
 */
void _LLHW_isohf_configTxDig4TypeA(HFCTRL isohf, uint32_t tx_bit_rate);

/**
 * Front-end digital configuration for Type A - Rx
 *
 * This LLHW configure the threshold of the $RX$ decoder.
 *
 * @param[in] rx_bit_rate    Bit rate from 106 to 424
 *                           use DRI
 *                             0 = 106 kbits/s
 *                             1 = 212 kbits/s
 *                             2 = 424 kbits/s
 * @param[in] RxConfig_table Table with 2 entries containing the Rx configurations for the whole
 *                           Rx bit rates
 *                             - Counter threshold [HFCTRL_DIGITAL_CNT0_CFG, HFCTRL_DIGITAL_CNT1_CFG]
 *
 */
void _LLHW_isohf_configRxDig4TypeA(HFCTRL isohf, uint32_t rx_bit_rate, uint32_t RxConfig_table[3][2]);

#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* __API_ISO14443LLHW_H__ */
