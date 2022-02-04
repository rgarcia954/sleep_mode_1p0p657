/**
 * @file api_isohfllhw.c
 *
 * @brief This file contains the Low Level Hardware functions
 * used to safely drive the HF subsystem (ISO14443)
 *
 * @author Bettina REBAUD (INVIA)
 *
 */

#include "api_isohfllhw.h"

/**
 * Disable the HF Fc Clock
 *
 * This LLHW switches off the Fc clock extractor
 *
 */
void _LLHW_isohf_disableFc(HFCTRL isohf)
{
    _isohf_setTestCtrl(isohf, _isohf_getTestCtrl(isohf) | HF_TEST_CLK_EXTR_DIS);
}

/**
 * Enables the HF Fc Clock
 *
 * This LLHW renables the HF Fc clock extractor
 *
 */
void _LLHW_isohf_enableFc(HFCTRL isohf)
{
    _isohf_setTestCtrl(isohf, _isohf_getTestCtrl(isohf) & ~HF_TEST_CLK_EXTR_DIS);
}

/**
 * Wait until the SW reaches the Exec period
 *
 */
void _LLHW_isohf_waitUntilExec(HFCTRL isohf)
{
    while (_isohf_getComStatus(HFCTRL_IP) != HF_STATUS_COM_EXEC)
    {
    }
}

/**
 * Wait until the SW reaches the Tx period
 *
 */
void _LLHW_isohf_waitUntilTx(HFCTRL isohf)
{
    while (_isohf_getComStatus(HFCTRL_IP) != HF_STATUS_COM_TX)
    {
    }
}

/**
 * Wait until the SW reaches the Rx period
 *
 */
void _LLHW_isohf_waitUntilRx(HFCTRL isohf)
{
    while (_isohf_getComStatus(HFCTRL_IP) != HF_STATUS_COM_RX)
    {
    }
}

/**
 * Wait until the SW has the hand on HF digital controller
 *
 */
void _LLHW_isohf_waitUntilPlatformHand(HFCTRL isohf)
{
    while (_isohf_getPlatformHandStatus(HFCTRL_IP) != 0x0)
    {
    }
}

/**
 * Skip anticollision ISOA Layer3
 *
 * This LLHW allows to directly jump in ISOA Layer 4
 * Shall be launch before HW FSM triggering with WaitRx
 *
 */
void _LLHW_isohf_configSkipISOALayer3(HFCTRL isohf)
{
    _isohf_setTestCtrl(isohf, HF_TEST_SKIP_ANTICOL);
}

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
void _LLHW_isohf_waitForRx(HFCTRL isohf, uint32_t back_to_halt)
{
    _isohf_setProtocolCtrl(isohf, (back_to_halt | HF_P_CTRL_WAIT_RX));
}

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
                          uint32_t end_of_transaction)
{
    _isohf_setProtocolCtrl(isohf,
                           ((back_to_halt | HF_P_CTRL_LAUNCH_TX) | (silent_time << HF_P_CTRL_SILENT_TIME_SHIFT) |
                            (tx_frame_size << HF_P_CTRL_TX_FRAME_SIZE_SHIFT) | end_of_transaction));
}

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
uint32_t _LLHW_isohf_getSilentTime(HFCTRL isohf,  uint32_t min_n_val)
{
    uint32_t silent_time, current_slot, slot, min_slot_val;
    min_slot_val = min_n_val - 2;

    current_slot = _isohf_getSlotCounterStatus(isohf);

    if (0 < current_slot && current_slot < min_slot_val)
    {
        /* Include a waiting time until min_slot_val */
        silent_time  = min_slot_val - current_slot - 1;
    }
    else
    {
        if (current_slot == min_slot_val)
        {
            silent_time = 1;
        }
        else    /* current_slot > min_slot_val */
        {
            slot = current_slot - min_slot_val;
            if (slot < 15)
            {
                silent_time = slot;
            }
            else
            {
                silent_time = 15;
            }
        }
    }

    return silent_time;
}

/**
 * Front-end digital selection for Type A - Tx
 *
 * This LLHW selects the :
 * - OOK or BPSK modulation (Tx)
 * - bit rates
 *
 * It keeps the last Rx configuration.
 *
 * @param[in] tx_bit_rate    Bit rate from 106 to 424
 *                           use DSI
 *                             0 = 106 kbits/s
 *                             1 = 212 kbits/s
 *                             2 = 424 kbits/s
 *
 */
void _LLHW_isohf_configTxDig4TypeA(HFCTRL isohf, uint32_t tx_bit_rate)
{
    uint32_t tx_config = 0;

    /*ToDo: ask Hamid if there is a new isohf.h file that I am missing, because these constants aren't defined. */
    switch (tx_bit_rate)    /* = dsi */
    {
        case 0:
        {
            tx_config = (HF_DIG_CFG_BIT_RATE_106K | HF_DIG_CFG_COD_TYPE_OOK);
        }
        break;

        case 1:
        {
            tx_config = (HF_DIG_CFG_BIT_RATE_212K | HF_DIG_CFG_COD_TYPE_BPSK);
        }
        break;

        case 2:
        {
            tx_config = (HF_DIG_CFG_BIT_RATE_424K | HF_DIG_CFG_COD_TYPE_BPSK);
        }
        break;

        default:
        {
        }
        break;
    }

    _isohf_setDigitalCfg(isohf, (tx_config & (HF_DIG_CFG_BIT_RATE_MASK | HF_DIG_CFG_COD_TYPE_MASK)));
}

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
void _LLHW_isohf_configRxDig4TypeA(HFCTRL isohf, uint32_t rx_bit_rate, uint32_t RxConfig_table[3][2])
{
    _isohf_setDigitalCnt0Cfg(isohf, RxConfig_table[rx_bit_rate][0]);
    _isohf_setDigitalCnt1Cfg(isohf, RxConfig_table[rx_bit_rate][1]);

    /* ~_isohf_setDigitalCnt2Cfg(    isohf,RxConfig_table[rx_bit_rate][2]); // Spares
     * ~ _isohf_setAnalogCfg(         isohf,RxConfig_table[rx_bit_rate][3]); // Useful ?
     * ~ _isohf_setDigitalFDTTimerCfg(isohf,RxConfig_table[rx_bit_rate][4]); // Useful ? */
}
