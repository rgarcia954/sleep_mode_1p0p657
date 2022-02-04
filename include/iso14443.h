/*
 * @file iso14443.h
 * @brief Drivers to manage the HF digital controller
 * This IP is in charge of HF communication interface
 *
 * @author Bettina REBAUD - Alexandre BERZATI - Yannick Casamatta (INVIA)
 *
 */

#ifndef __ISOHF14443_H__
#define __ISOHF14443_H__

#ifdef __cplusplus
extern "C" {
#endif    /* ifdef __cplusplus */

#include "platform_config.h"

#define ZERO_VAL                          0x00000000

/***************************************************************************
 * HF_CTRL Registers
 **************************************************************************/

/* HFCTRL_TEST_FC_CNT */
#define HF_TEST_FC_CNT_STATUS_MASK        0x00000FFF
#define HF_TEST_FC_CNT_STATUS_SHIFT       0
#define HF_TEST_FC_CNT_ENA                0x00010000
#define HF_TEST_FC_CNT_ENA_SHIFT          16
#define HF_TEST_FC_CNT_CLEAR              0x00020000
#define HF_TEST_FC_CNT_CLEAR_SHIFT        17
#define HF_TEST_FC_CNT_RST_VAL            ZERO_VAL

/* HFCTRL_TEST_SDA_CNT */
#define HF_TEST_SDA_CNT_STATUS_MASK       0x00000FFF
#define HF_TEST_SDA_CNT_STATUS_SHIFT      0
#define HF_TEST_SDA_CNT_ENA               0x00010000
#define HF_TEST_SDA_CNT_ENA_SHIFT         16
#define HF_TEST_SDA_CNT_CLEAR             0x00020000
#define HF_TEST_SDA_CNT_CLEAR_SHIFT       17
#define HF_TEST_SDA_CNT_RST_VAL           ZERO_VAL

/* HFCTRL_TEST_CTRL */
#define HF_TEST_DEM_ACCESS_ENA            0x00000001
#define HF_TEST_DEM_ACCESS_SHIFT          0
#define HF_TEST_DEM_TYPEA_ENA             0x00000002
#define HF_TEST_DEM_TYPEA_SHIFT           1
#define HF_TEST_VSEL_MASK                 0x00000030
#define HF_TEST_VSEL_SHIFT                4
#define HF_TEST_VSEL_HIZ                  0x00000000
#define HF_TEST_VSEL_VDC                  0x00000010
#define HF_TEST_VSEL_VDDA                 0x00000020
#define HF_TEST_RETRO_MASK                0x00000300
#define HF_TEST_RETRO_SHIFT               8
#define HF_TEST_RETRO_FSM                 0x00000000
#define HF_TEST_RETRO_ONE                 0x00000100
#define HF_TEST_RETRO_FC16                0x00000200
#define HF_TEST_RETRO_FC8                 0x00000300
#define HF_TEST_CLK_EXTR_DIS              0x00001000
#define HF_TEST_CLK_EXTR_DIS_SHIFT        12
#define HF_TEST_SKIP_ANTICOL              0x01000000
#define HF_TEST_SKIP_ANTICOL_SHIFT        24
#define HF_TEST_CTRL_RST_VAL              ZERO_VAL

/* HFCTRL_PROTOCOL_CFG */
#define HF_P_CFG_TYPE_A                   0x00000000
#define HF_P_CFG_TYPE_B                   0x00000001
#define HF_P_CFG_TYPE_MASK                0x00000001
#define HF_P_CFG_TYPE_SHIFT               0
#define HF_P_CFG_REQA_IGNORE_ENA          0x00000004
#define HF_P_CFG_REQA_IGNORE_MASK         0x00000004
#define HF_P_CFG_REQA_IGNORE_SHIFT        2
#define HF_P_CFG_TYPEA_LAYER4_ENA         0x00000008
#define HF_P_CFG_TYPEA_LAYER4_MASK        0x00000008
#define HF_P_CFG_TYPEA_LAYER4_SHIFT       3
#define HF_P_CFG_UID_1                    0x00000001
#define HF_P_CFG_UID_2                    0x00000002
#define HF_P_CFG_UID_3                    0x00000003
#define HF_P_CFG_UID_MASK                 0x00000030
#define HF_P_CFG_UID_SHIFT                4
#define HF_P_CFG_L4_ERR_FRAME_SHIFT       6
#define HF_P_CFG_L4_ERR_FRAME_MASK        (0x1 << HF_P_CFG_L4_ERR_FRAME_SHIFT)
#define HF_P_CFG_L4_ERR_FRAME_ENA         HF_P_CFG_L4_ERR_FRAME_MASK
#define HF_P_CFG_CNT_WP_MASK              0xFFFFFF00
#define HF_P_CFG_CNT_WP_SHIFT             8
#define HF_P_CFG_CNT_WP_RST_VAL           0xF000
#define HF_P_CFG_RST_VAL                  (ZERO_VAL | (HF_P_CFG_UID_1 << HF_P_CFG_UID_SHIFT) | \
                                           (HF_P_CFG_CNT_WP_RST_VAL << HF_P_CFG_CNT_WP_SHIFT))

/* HFCTRL_PROTOCOL_CTRL */
#define HF_P_CTRL_WAIT_RX                 0x00000001
#define HF_P_CTRL_LAUNCH_TX               0x00000002
#define HF_P_CTRL_BACK2HALT               0x00000010
#define HF_P_CTRL_ENDOFTRANSAC            0x00000020
#define HF_P_CTRL_PARITY_DIS              0x00000040
#define HF_P_CTRL_CRC_RAM_ENA             0x00000080
#define HF_P_CTRL_SILENT_TIME_MASK        0x0000F000
#define HF_P_CTRL_SILENT_TIME_SHIFT       12
#define HF_P_CTRL_SILENT_TIME_RST_VAL     0x5
#define HF_P_CTRL_TX_FRAME_SIZE_MASK      0x07FF0000
#define HF_P_CTRL_TX_FRAME_SIZE_SHIFT     16
#define HF_P_CTRL_TX_FRAME_BIT_NUM_MASK   0xF0000000
#define HF_P_CTRL_TX_FRAME_BIT_NUM_SHIFT  28
#define HF_P_CTRL_RST_VAL                (ZERO_VAL | (HF_P_CTRL_SILENT_TIME_RST_VAL << HF_P_CTRL_SILENT_TIME_SHIFT))

/* HFCTRL_COUNTER_STATUS */
#define HF_CNT_STAT_MASK                  0x0007FFFF
#define HF_CNT_STAT_SHIFT                 0
#define HF_CNT_STAT_RST_VAL               ZERO_VAL

/* HFCTRL_STATUS */
#define HF_STATUS_COM_INFO_MASK           0x00000003
#define HF_STATUS_COM_INFO_SHIFT          0
#define HF_STATUS_COM_NA                  0x00000000
#define HF_STATUS_COM_RX                  0x00000001
#define HF_STATUS_COM_TX                  0x00000002
#define HF_STATUS_COM_EXEC                0x00000003
#define HF_STATUS_PLATHAND_ENA            0x00000000
#define HF_STATUS_PLATHAND_MASK           0x00000004
#define HF_STATUS_PLATHAND_SHIFT          2
#define HF_STATUS_ENDOFCOM_MASK           0x00000008
#define HF_STATUS_ENDOFCOM_SHIFT          3
#define HF_STATUS_ERR_FRAME_LOGIC_MASK    0x00000010
#define HF_STATUS_ERR_FRAME_LOGIC_SHIFT   4
#define HF_STATUS_ERR_FRAME_PB_MASK       0x00000020
#define HF_STATUS_ERR_FRAME_PB_SHIFT      5
#define HF_STATUS_ERR_FRAME_SIZE_MASK     0x00000040
#define HF_STATUS_ERR_FRAME_SIZE_SHIFT    6
#define HF_STATUS_ERR_FRAME_CRC_MASK      0x00000080
#define HF_STATUS_ERR_FRAME_CRC_SHIFT     7
#define HF_STATUS_ERR_RAM_OFLOW_MASK      0x00000100
#define HF_STATUS_ERR_RAM_OFLOW_SHIFT     8
#define HF_STATUS_ERR_RX_MASK             0x000001F0
#define HF_STATUS_ERR_CNT_OFLOW_MASK      0x00001000
#define HF_STATUS_ERR_CNT_OFLOW_SHIFT     12
#define HF_STATUS_CNT_WP_MASK             0x00002000
#define HF_STATUS_CNT_WP_SHIFT            13
#define HF_STATUS_RX_FRAME_SIZE_MASK      0x07FF0000
#define HF_STATUS_RX_FRAME_SIZE_SHIFT     16
#define HF_STATUS_RX_FRAME_BIT_NUM_MASK   0xF0000000
#define HF_STATUS_RX_FRAME_BIT_NUM_SHIFT  28
#define HF_STATUS_RST_VAL                 ZERO_VAL

/* HFCTRL_ITENA */
#define HF_ITENA_ENDOFCOM_MASK            0x00000008
#define HF_ITENA_ENDOFCOM_SHIFT           3
#define HF_ITENA_CNT_OFLOW_MASK           0x00001000
#define HF_ITENA_CNT_OFLOW_SHIFT          12
#define HF_ITENA_CNT_WP_MASK              0x00002000
#define HF_ITENA_CNT_WP_SHIFT             13
#define HF_ITENA_RST_VAL                 ZERO_VAL

/* HFCTRL_ANALOG_CFG */
#define HF_ANA_CFG_TR_DEM_A_MASK          0x0000003F
#define HF_ANA_CFG_TR_DEM_A_SHIFT         0
#define HF_ANA_CFG_TR_DEM_A_RST_VAL       0x25
#define HF_ANA_CFG_LOAD_MOD_MASK          0x000F0000
#define HF_ANA_CFG_LOAD_MOD_SHIFT         16
#define HF_ANA_CFG_LOAD_MOD_RST_VAL       0x7
#define HF_ANA_CFG_VREG_MASK              0x07000000
#define HF_ANA_CFG_VREG_SHIFT             24
#define HF_ANA_CFG_VREG_RST_VAL           0x4
#define HF_ANA_CFG_RST_VAL                (ZERO_VAL |  (HF_ANA_CFG_LOAD_MOD_RST_VAL << HF_ANA_CFG_LOAD_MOD_SHIFT) | \
                                           (HF_ANA_CFG_TR_DEM_A_RST_VAL << HF_ANA_CFG_TR_DEM_A_SHIFT) | \
                                           (HF_ANA_CFG_VREG_RST_VAL << HF_ANA_CFG_VREG_SHIFT))

/* HFCTRL_DIGITAL_CFG */
#define HF_DIG_CFG_BIT_RATE_MASK           0x00000003
#define HF_DIG_CFG_BIT_RATE_106K           0x00000000
#define HF_DIG_CFG_BIT_RATE_212K           0x00000001
#define HF_DIG_CFG_BIT_RATE_424K           0x00000002
#define HF_DIG_CFG_BIT_RATE_SHIFT          0
#define HF_DIG_CFG_GLITCH_FILTER_DIS_SHIFT 16
#define HF_DIG_CFG_GLITCH_FILTER_DIS_MASK  (0x1 << HF_DIG_CFG_GLITCH_FILTER_DIS_SHIFT)
#define HF_DIG_CFG_GLITCH_FILTER_DIS       (0x1 << HF_DIG_CFG_GLITCH_FILTER_DIS_SHIFT)
#define HF_DIG_CFG_GLITCH_FILTER_ENA       (0x0 << HF_DIG_CFG_GLITCH_FILTER_DIS_SHIFT)
#define HF_DIG_CFG_REBOUND_FILTER_RST_VAL   0x0
#define HF_DIG_CFG_REBOUND_FILTER_SHIFT     20
#define HF_DIG_CFG_REBOUND_FILTER_MASK      (0xF << HF_DIG_CFG_REBOUND_FILTER_SHIFT)
#define HF_DIG_CFG_BIT_RATE_SHIFT           0
#define HF_DIG_CFG_COD_TYPE_MASK            0x00000100
#define HF_DIG_CFG_COD_TYPE_OOK             0x00000000
#define HF_DIG_CFG_COD_TYPE_BPSK            0x00000100
#define HF_DIG_CFG_COD_TYPE_SHIFT           8
#define HF_DIG_CFG_RST_VAL                (ZERO_VAL | HF_DIG_CFG_BIT_RATE_106K | HF_DIG_CFG_COD_TYPE_OOK | \
                                           (HF_DIG_CFG_REBOUND_FILTER_RST_VAL << HF_DIG_CFG_REBOUND_FILTER_OFFSET))

/* HFCTRL_DIGITAL_CNT0_CFG */
#define HF_DIG_CNT0_MASK                  0x000003FF
#define HF_DIG_CNT0_SHIFT                 0
#define HF_DIG_CNT1_MASK                  0x03FF0000
#define HF_DIG_CNT1_SHIFT                 16
#define HF_DIG_CFG_DEC_TH_MASK            0x10000000
#define HF_DIG_CFG_DEC_TH_SHIFT           28
#define DEC_TH_0_RST_VAL                  0x18
#define DEC_TH_1_RST_VAL                  0x78
#define HFCTRL_DIGITAL_CNT0_RST_VAL       (ZERO_VAL | (DEC_TH_0_RST_VAL <<  HF_DIG_CNT0_SHIFT) | \
                                           (DEC_TH_1_RST_VAL << HF_DIG_CNT1_SHIFT))

/* HFCTRL_DIGITAL_CNT1_CFG */
#define HF_DIG_CNT2_MASK                  0x000003FF
#define HF_DIG_CNT2_SHIFT                 0
#define HF_DIG_CNT3_MASK                  0x03FF0000
#define HF_DIG_CNT3_SHIFT                 16
#define DEC_TH_2_RST_VAL                  0xB8
#define DEC_TH_3_RST_VAL                  0xF8
#define HFCTRL_DIGITAL_CNT1_RST_VAL       (ZERO_VAL | (DEC_TH_2_RST_VAL <<  HF_DIG_CNT2_SHIFT) | \
                                           (DEC_TH_3_RST_VAL << HF_DIG_CNT3_SHIFT))

/* HFCTRL_DIGITAL_CNT2_CFG */
#define HF_DIG_CNT4_MASK                  0x000003FF
#define HF_DIG_CNT4_SHIFT                 0
#define DEC_TH_4_RST_VAL                  0x30F
#define DEC_TH_5_RST_VAL                  0x0
#define HFCTRL_DIGITAL_CNT2_RST_VAL       (ZERO_VAL | (DEC_TH_4_RST_VAL <<  HF_DIG_CNT4_SHIFT))
#define HF_DIG_CNT_NUM                    6

/* HFCTRL_FDT_TIMER_CFG */
#define HF_FDT_L3_VAL_MASK                0x00000F00
#define HF_FDT_L3_VAL_SHIFT               8
#define HF_FDT_L3_VAL_RST_VAL             0x5
#define HF_OFFSET0_VAL_MASK               0x00FF0000
#define HF_OFFSET0_VAL_SHIFT              16
#define HF_OFFSET0_RST_VAL                0x4F
#define HF_OFFSET1_VAL_MASK               0xFF000000
#define HF_OFFSET1_VAL_SHIFT              24
#define HF_OFFSET1_RST_VAL                0x4F
#define HF_FDT_TIMER_RST_VAL              (ZERO_VAL  | (HF_FDT_L3_VAL_RST_VAL <<  HF_FDT_L3_VAL_SHIFT)   | \
                                           (HF_OFFSET0_RST_VAL << HF_OFFSET0_VAL_SHIFT) | \
                                           (HF_OFFSET1_RST_VAL << HF_OFFSET1_VAL_SHIFT))

/* HFCTRL_POWER_CFG */
#define HF_RF_ON_THRESHOLD_MASK           0x3
#define HF_RF_ON_THRESHOLD_SHIFT          0
#define HF_RF_ON_THRESHOLD_RST_VAL        0x0
#define HF_RF_ON_ITENA_SHIFT              2
#define HF_RF_ON_ITENA_MASK              (0x1 << HF_RF_ON_ITENA_SHIFT)
#define HF_RF_OFF_ITENA_SHIFT             3
#define HF_RF_OFF_ITENA_MASK             (0x1 << HF_RF_OFF_ITENA_SHIFT)
#define HF_RESET_CFG_SHIFT                8
#define HF_RESET_CFG_MASK                (0x1 << HF_RESET_CFG_SHIFT)
#define HF_RESET_ENA_SHIFT                9
#define HF_RESET_ENA_MASK                (0x1 << HF_RESET_ENA_SHIFT)
#define HF_RESET_ITENA_SHIFT             10
#define HF_RESET_ITENA_MASK              (0x1 << HF_RESET_ITENA_SHIFT)
#define HF_POWER_CFG_RST_VAL             HF_RF_ON_THRESHOLD_RST_VAL

/* HFCTRL_POWER_STATUS */
#define HF_DET_LS_MASK                    0x1F
#define HF_DET_LS_SHIFT                   0
#define HF_DET_LS_RST_VAL                 0x1F
#define HF_RF_ON_STATUS_SHIFT             8
#define HF_RF_ON_STATUS_MASK             (0x1 << HF_RF_ON_STATUS_SHIFT)
#define HF_RF_OFF_STATUS_SHIFT            9
#define HF_RF_OFF_STATUS_MASK            (0x1 << HF_RF_OFF_STATUS_SHIFT)
#define HF_RESET_STATUS_SHIFT             10
#define HF_RESET_STATUS_MASK             (0x1 << HF_RESET_STATUS_SHIFT)
#define HF_POWER_STATUS_RST_VAL           HF_DET_LS_RST_VAL

/***************************************************************************/
#define HF_REGISTER_NUMBER                15

/***************************************************************************
 * HF_IO_RAM Buffer
 **************************************************************************/
#define HF_IO_RAM_START_ADD               ((uint32_t *)PLATFORM_HF_BUFFER_ADDR)
#define HF_IO_RAM_OFFSET                  0x00000FFF
#define HF_IO_RAM_SIZE                    0x00001000
#define HF_IO_RAM_SIZE_BYTE_NUM           1024
#define HF_IO_RAM_END_ADD                 (uint32_t *)(HF_IO_RAM_START_ADD + HF_IO_RAM_OFFSET)
#define HF_IO_RAM_EMPTY_SIZE              0x00000100
#define HF_IO_RAM_EMPTY_OFFSET            HF_IO_RAM_EMPTY_SIZE
#define HF_IO_RAM_CONFIG_TYPEA_START_ADD  (uint32_t *)(HF_IO_RAM_START_ADD + HF_IO_RAM_EMPTY_OFFSET)
#define HF_IO_RAM_INIT_ISOALAYER3         14

/***************************************************************************
 * Protection for Assembly inclusion
 **************************************************************************/
#ifndef ASSEMBLY

#include <stdint.h>

enum HFRegisters
{
    /* Base address + 0x00 - TEST Fc COUNTER Register
     * \invia_beginReg
     * 11:0  & FC\_CNT\_STAT       &  0  &  0  & R/-   & $Fc$ clock counter status\\ \hline
     * 15:12 & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 16    & FC\_CNT\_ENA        & '0' & '0' & R/W   & When '0' : the counter is stopped \\
     *       &                     &     &     &       & When '1' : the counter is decounting \\ \hline
     * 17    & FC\_CNT\_CLR        & '0' & '0' & R/W   & Pulse to clear the FC\_CNT\_STATUS value to 0x000 \\
     *       &                     &     &     &       & Set/Reset by SW, only active when FC\_CNT\_ENA =1  \\ \hline
     * 31:18 & RFU                 &  0  &  0  & R/-   & RFU \\
     *
     * \invia_endReg
     */
    HFCTRL_TEST_FC_CNT = 0,

    /* Base address + 0x04 - TEST SDA COUNTER Register
     * \invia_beginReg
     * 13:0  & SDA\_CNT\_STAT      &  0  &  0  & R/-   & $SDA$ clock counter status \\ \hline
     * 15:14 & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 16    & SDA\_CNT\_ENA       & '0' & '0' & R/W   & When '0' : the counter is stopped \\
     *       &                     &     &     &       & When '1' : the counter is counting \\ \hline
     * 17    & SDA\_CNT\_CLR       & '0' & '0' & R/W   & Pulse to clear the SDA\_CNT\_STATUS value to 0x0000 \\
     *       &                     &     &     &       & Set/Reset by SW, only active when SDA\_CNT\_ENA =1\\ \hline
     * 31:18 & RFU                 &  0  &  0  & R/-   & RFU \\
     *
     * \invia_endReg
     */
    HFCTRL_TEST_SDA_CNT = 1,

    /* Base address + 0x08 - TEST CTRL Register
     * \invia_beginReg
     * 0     & DEM\_ACCESS         & '0' & '0' & R/W   & '1' : The $SW$ has the hand to enable or disable the
     * demodulators (TEST mode)\\
     *       &                     &     &     &       & '0' : The $FSM$ of the digital controller has the hand on
     * demodulators activation according to PROTOCOL\_CFG.TYPE\\ \hline
     * 1     & DEM\_TYPE\_A        & '0' & '0' & R/W   & Only active when DEM\_ACCESS=1 \\
     *       & ENA                 &     &     &       & '1' : Enable the demodulator Type A analog front-end ($Rx$)\\
     *       &                     &     &     &       & '0' : Disable the demodulator Type A analog front-end   \\
     *\hline
     * 3:2   & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 5:4   & VOLTAGE\_SEL        & '00'& '00'& R/W   & Voltage selection for Analog Testing PAD redirection \\
     *       &                     &     &     &       & '00' : High-Z \\
     *       &                     &     &     &       & '01' : $Vdc$  \\
     *       &                     &     &     &       & '10' : $Vdda$ \\
     *       &                     &     &     &       & '11' : Forbidden \\\hline
     * 7:6   & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 9:8   & RETRO\_TEST         & '00'& '00'& R/W   & retromodulation control testing features \\
     *       &                     &     &     &       & '00' : retro is monitored by the $FSM$ \\
     *       &                     &     &     &       & '01' : retro='1' (TEST mode or power saving configuration when
     *$HF$ not used)  \\
     *       &                     &     &     &       & '10' : retro=$Fc$/16 (TEST mode)\\
     *       &                     &     &     &       & '11' : retro=$Fc$/8 (TEST mode) \\\hline
     * 11:10 & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 12    & CLK\_EXTR\_DIS      &  0  &  0  & R/W   & When '1', the clock extractor of the Analog Front end is
     * disabled (power saving configuration when $HF$ not used) \\ \hline
     * 23:13 & RFU                 &  0  &  0  & R/-   & RFU \\ \hline
     * 24    & SKIP\_ANTICOL       & '0' & '0' & R/W   & '0' : When ISO Type A, the Layer 3 anticollision sequence is
     * managed by the $HW$ $FSM$ \\
     *       &                     &     &     &       & '1' : When ISO Type A, the Layer 3 anticollision sequence is
     * skipped with direct $SW$ access\\ \hline
     * 31:25 & RFU                 &  0  &  0  & R/-   & RFU \\
     * \invia_endReg
     */
    HFCTRL_TEST_CTRL = 2,

    /* Base address + 0x0C - PROTOCOL and COMMUNICATION CONFIG register
     * \invia_beginReg
     * 1:0   & RFU                 & '0'   & '0'   & R/-   & RFU  \\ \hline
     * 2     & REQA\_IGNORE        & '0'   & '0'   & R/W   & The SW shall set this bit to '1' after the first valid
     *$RATS$ or C-$RATS$  \\
     *       &                     &       &       &       & '0': a BACK\_TO\_HALT will make the HW $FSM$ in IDLE state
     *(like after $POR$). \\
     *       &                     &       &       &       & '1': a BACK\_TO\_HALT will make the HW $FSM$ in HALT state.
     *\\ \hline
     * 3     & TYPEA\_L4\_ENA      & '0'   & '0'   & R/W   & The SW shall set this bit to '1' when entering in ISO Type
     * A Layer 4 \\
     *       &                     &       &       &       & The SW shall set this bit to '0' after a valid DESELECT
     * command \\
     *       &                     &       &       &       & '0': only 64 bytes of the IO $RAM$ buffer can be filled
     * during $Rx$ period \\
     *       &                     &       &       &       & '1': the 1 kbytes of the IO $RAM$ can be filled during the
     *$Rx$ period  \\ \hline
     * 5:4   & UID\_SIZE           & '01'  & '01'  & R/W   & $UID$ size when Type A (Layer 3)    \\
     *       &                     &       &       &       & '01': $UID$ size=1     \\
     *       &                     &       &       &       & '10': $UID$ size=2     \\
     *       &                     &       &       &       & '11': $UID$ size=3     \\
     *       &                     &       &       &       & '00': not applicable   \\  \hline
     * 6     & L4\_ERR\_FRAME      & '0'   & '0'   & R/W   & Layer 4 Transmission Error frame ignore \\
     *       & \_IGNORE            &       &       &       & '1': ignores $Rx$ frame with error, waits for the next
     * frame \\
     *       &                     &       &       &       & '0': reports all the $Rx$ frame errors\\ \hline
     * 7     & RFU                 & '0'   & '0'   & R/-   & RFU  \\ \hline
     * 27:8  & CNTER\_WP           & hF000 & hF000 & R/W   & Slot counter watchpoint value \\
     *       & \_VALUE             &       &       &       & ($FDT$ $PCD$ to $PICC$), reset value represents 4.5 ms   \\
     *\hline
     * 31:28 & RFU                 &  0    &  0    & R/-   & RFU                                 \\
     * \invia_endReg
     */
    HFCTRL_PROTOCOL_CFG = 3,

    /* Base address + 0x10 - PROTOCOL and COMMUNICATION CTRL Register
     * \invia_beginReg
     * 0     & WAIT\_RX            & '0'   & '0'   & R/W   & When STATUS.COM\_INFO='00', launch the $HF$ digital
     * controller for Type A Layer 3 handling  \\
     *       &                     &       &       &       & When STATUS.COM\_INFO='11', it will force the $Tx$ frame
     * ignorance : the $PICC$ will not answer (mute), the $HF$ digital controller returns in $Rx$ mode \\
     *       &                     &       &       &       & This bit shall not be launched when STATUS.COM\_INFO='01'
     * or '10' \\
     *       &                     &       &       &       & Note that at each WAIT\_RX='1', the bit rates
     * configurations are reloaded inside the digital controller \\
     *       &                     &       &       &       & Set Only by SW, reset by HW\\ \hline
     * 1     & LAUNCH\_TX          & '0'   & '0'   & R/W   & Launch the $Tx$ frame with the $IO$ $RAM$ buffer content
     * after waiting SILENT\_TIME slots \\
     *       &                     &       &       &       & Set Only by SW, reset by HW\\ \hline
     * 3:2   & RFU                 &  0    &  0    & R/-   & RFU                                  \\ \hline
     * 4     & BACK\_TO\_HALT      & '0'   & '0'   & R/W   & When '1': Allows to return in IDLE/HALT ISO Type A layer 3
     * \\
     *       &                     &       &       &       & - If launched with WAIT\_RX: $PICC$ mode is in HALT or IDLE
     * state immediatly\\
     *       &                     &       &       &       & - If launched with LAUNCH\_TX: the HALT or IDLE  state will
     * be reached after the end of the $Tx$ frame \\
     *       &                     &       &       &       & The IDLE or HALT state is chosen with the REQA\_IGNORE
     * configuration  \\
     *       &                     &       &       &       & Set Only by SW, reset by HW \\ \hline
     * 5     & END\_OF\_           & '0'   & '0'   & R/W   & Indicates the end of transaction\\
     *       & TRANSAC             &       &       &       & Shall be set before the Last $Tx$ \\
     *       &                     &       &       &       & Set Only by SW, reset by HW\\ \hline
     * 6     & PARITY\_DIS         & '0'   & '0'   & R/W   & '1' : Tx only. Parity bit is not sent\\
     *       &                     &       &       &       & '0' : Tx only. Parity bit is sent\\ \hline
     * 7     & CRC\_RAM            & '0'   & '0'   & R/W   & '1' : Tx only. No $CRC$ bytes added at end of frame, final
     * size is  TX\_RAM\_FRAME\_SIZE\\
     *       &                     &       &       &       & '0' : Tx only. The $FSM$ generates on the fly the $CRC$
     * from the data information present inside the $IO$ $RAM$ buffer\\ \hline
     * 11:8  & RFU                 &  0    &  0    & R/-   & RFU \\ \hline
     * 15:12 & SILENT\_TIME        & h5    & h5    & R/W   & Silent time $Ts$ value for the current communication
     * sequence\\
     *       &                     &       &       &       & Defined in number of slots\\ \hline
     * 26:16 & TX\_RAM\_FRAME      &  0    &  0    & R/W   & $IO$ $RAM$ frame size for $Tx$, \\
     *       & \_SIZE              &       &       &       & included optional split byte if any: \\
     *       &                     &       &       &       & To be updated by $SW$ before/when LAUNCH\_TX\\ \hline
     * 27    & RFU                 & '0'   & '0'   & R/-   & RFU \\ \hline
     * 31:28 & TX\_RAM\_BIT        &  0    &  0    & R/W   & 0x0 : No split byte added \\
     *       & \_SIZE              &       &       &       & [0x1-0x7] : Split byte added at begining of the frame \\
     *       &                     &       &       &       & The number of split byte bits sent is 8-TX\_RAM\_BIT\_SIZE
     *\\
     * \invia_endReg
     */
    HFCTRL_PROTOCOL_CTRL = 4,

    /* Base address + 0x14 - SLOT COUNTER STATUS register
     * \invia_beginReg
     * 19:0   & CNTER\_STATUS      &   0   &  0    & R/-   & Slot counter status ($FDT$ $PDC$ to $PICC$) \\ \hline
     * 31:20  & RFU                &   0   &  0    & R/-   & RFU                                         \\
     * \invia_endReg
     */
    HFCTRL_COUNTER_STATUS = 5,

    /* Base address + 0x18 - STATUS Register
     * \invia_beginReg
     * 1:0   & COM\_INFO           & "00" & "00" & R/- & Communication state status \\
     *       &                     &      &      &     & '00': Digital controller not launched (HFCTRL\_PROTOCOL\_CTRL
     *\\
     *       &                     &      &      &     & .WAIT\_RX) \\
     *       &                     &      &      &     & '01': $Rx$ state       \\
     *       &                     &      &      &     & '10': $Tx$ state       \\
     *       &                     &      &      &     & '11': $Exec$ state     \\
     *       &                     &      &      &     & Only in Layer 4. \\  \hline
     * 2     & PLATFORM\_          & '0'  & '0'  & R/- & When '0', the Platform can access \\
     *       & HAND\_N             &      &      &     & the $IO$ $RAM$ buffer\\
     *       &                     &      &      &     & This status is set to'0' when in 1) COM\_INFO='00' (BOOT), or
     * 2) COM\_INFO='11' ($Exec$ period) during ISO Type A Layer 4 \\
     *       &                     &      &      &     & It is only updated with the presence of the $SClk$ \\
     *       &                     &      &      &     & It is automatically set/reset by $HW$ \\ \hline
     * 3     & END\_OF\_COM        & '0'  & '0'  & R/C1 & When '1', the $Rx$ period has finished (end of reception), or
     *\\
     *       &                     &      &      &     & the $Tx$ transmission has finished with the end of transaction
     * feature activated.\\
     *       &                     &      &      &     & It is only updated with the presence of the $SClk$ \\
     *       &                     &      &      &     & It is automatically set by $HW$, clearable by $SW$ by writing
     *'1' \\ \hline
     * 4     & FRAME\_LOGIC        & '0'  & '0'  & R/- & Logic error flag when ISO type A layer 4 $Rx$ \\
     *       & \_ERR               &      &      &     & When '1', an logic bit error has been detected inside the
     * frame\\
     *       &                     &      &      &     & Updated when $Rx$ to $Exec$ period transition \\ \hline
     * 5     & FRAME\_PB           & '0'  & '0'  & R/- & Parity bit error flag when ISO type A layer 4 $Rx$ \\
     *       & \_ERR               &      &      &     & When '1', a parity bit error has been detected inside the
     * frame\\
     *       &                     &      &      &     & Updated when $Rx$ to $Exec$ period transition \\ \hline
     * 6     & FRAME\_SIZE         & '0'  & '0'  & R/- & Frame size error flag : not standard \\
     *       & \_ERR               &      &      &     &  frame size (ISO type A layer 4) in $Rx$\ \\
     *       &                     &      &      &     & When '1', a frame size error has been detected\\
     *       &                     &      &      &     & Updated when $Rx$ to $Exec$ period transition \\ \hline
     * 7     & FRAME\_CRC          & '0'  & '0'  & R/- & Bad frame $CRC$ error flag : $CRC$ on \\
     *       & \_ERR               &      &      &     & two bytes, done in the whole byte of the frame (ISO type A
     * layer 4)  in $Rx$ \\
     *       &                     &      &      &     & When '1', a $CRC$ error has been detected\\
     *       &                     &      &      &     & Updated when $Rx$ to $Exec$ period transition \\ \hline
     * 8     & RAM\_OVERF          & '0'  & '0'  & R/- & $IO$ $RAM$ overflow error flag \\
     *       & \_ERR               &      &      &     & When '1', the complete 1KB $IO$ $RAM$ buffer has been filled by
     * the $Rx$ frame \\
     *       &                     &      &      &     & Updated when $Rx$ to $Exec$ period transition\\ \hline
     * 11:9  & RFU                 &  0   &  0   & R/- & RFU \\ \hline
     * 12    & CNTER\_OVERF        & '0'  & '0'  & R/C1 & Slot counter Overflow error flag \\
     *       & \_ERR               &      &      &      & When '1', the Slot counter has reached the maximum value and
     * continues counting from 0\\
     *       &                     &      &      &      & Clear only by $SW$ by writing '1'\\ \hline
     * 13    & CNTER\_WP           & '0'  & '0'  & R/C1 & Slot counter Watchpoint flag \\
     *       &                     &      &      &     & When '1', the Slot counter has reached the value of the
     * watchpoint in $Exec$ period\\
     *       &                     &      &      &     & Clear only by $SW$ by writing '1' \\ \hline
     * 15:14 & RFU                 &  0   &  0   & R/- & RFU \\ \hline
     * 26:16 & RX\_RAM\_FRAME      &  0   &  0   & R/- & $IO$ $RAM$ frame size status when $Rx$\\
     *       & \_SIZE              &      &      &     & Updated when $Rx$ to $Exec$ period transition \\ \hline
     * 27    & RFU                 & '0'  & '0'  & R/- & RFU \\ \hline
     * 31:28 & RX\_RAM\_FRAME      &  0   &  0   & R/- & $IO$ $RAM$ bit size part status when $Rx$\\
     *       & \_BIT\_SIZE         &      &      &     & Updated when $Rx$ to $Exec$ period transition \\
     * \invia_endReg
     */
    HFCTRL_STATUS = 6,

    /* Base address + 0x1C - ITENA Interrupt enable register
     * \invia_beginReg
     * 2:0   & RFU                 &  0  &  0  & R/-   & RFU                                  \\ \hline
     * 3     & END\_OF\_COM\_IT    & '0' & '0' & R/W   & '1': An interruption is raised when END\_OF\_COM set to '1' \\
     *       &                     &     &     &       & '0': The interruption for this flag is disabled \\ \hline
     * 11:4  & RFU                 &  0  &  0  & R/-   & RFU                                  \\ \hline
     * 12    & CNTER\_OVERF\_IT    & '0' & '0' & R/W   & '1': An interruption is raised when CNTER\_OVERF\_ERR set to
     *'1' \\
     *       &                     &     &     &       & '0': The interruption for this flag is disabled \\ \hline
     * 13    & CNTER\_WP\_IT       & '0' & '0' & R/W   & '1': An interruption is raised when CNTER\_WP set to '1' \\
     *       &                     &     &     &       & '0': The interruption for this flag is disabled \\ \hline
     * 31:14 & RFU                 &  0  &  0  & R/-   & RFU \\
     * \invia_endReg
     */
    HFCTRL_ITENA = 7,

    /* Base address + 0x20 - ANALOG FRONTEND CONFIG register
     * \invia_beginReg
     * 5:0   & TR\_DEM\_A          & h25 &  h25 & R/W  & Configuration for type A demodulator ($Rx$)\\ \hline
     * 15:6  & RFU                 &  0  &  0   & R/-   & RFU \\ \hline
     * 19:16 & TR\_LOAD\_MOD       & h7  &  h7  & R/W   & Configuration of the load modulation level ($Tx$)  \\  \hline
     * 23:20 & RFU                 &  0  &  0   & R/-   & RFU \\ \hline
     * 26:24 & TR\_REG             & h4  &  h4  & R/W   & Configuration of the Voltage regulator \\\hline
     * 31:23 & RFU                 &  0  &  0   & R/-   & RFU \\
     * \invia_endReg
     */
    HFCTRL_ANALOG_CFG = 8,

    /* Base address + 0x24 - DIGITAL FRONTEND CONFIG register
     * \invia_beginReg
     * 1:0   & BIT\_RATE           & "00" & "00" & R/W   & Bit rate selection for $Tx$   \\
     *       &                     &      &      &       & "00" : Bit rate $Fc$/128 \\
     *       &                     &      &      &       & "01" : Bit rate $Fc$/64  \\
     *       &                     &      &      &       & "10" : Bit rate $Fc$/32  \\
     *       &                     &      &      &       & "11" : No effect on the register \\
     *       &                     &      &      &       & This selection influences the digital encodeur subcarrier
     * when modulation $BPSK$ $NRZ$-L \\
     *       &                     &      &      &       & This selection influences the parity bit and generation
     *($Tx$) \\\hline
     * 7:2   & RFU                 &  0   &  0   & R/-   & RFU                                  \\ \hline
     *   8   & COD\_DIG\_TYPE      & '0'  & '01' & R/W   & Digital encodeur type selection ($Tx$)  \\
     *       &                     &      &      &       & "0" : for modulation $OOK$ Manchester             \\
     *       &                     &      &      &       & "1" : for modulation $BPSK$ $NRZ$-L               \\ \hline
     * 15:9  & RFU                 &  0   &  0   & R/-   & RFU                                  \\ \hline
     *   16  & GLITCH\_FILT        & '0'  & '0'  & R/W   & RX glitch filter disable \\
     *       & \_DISABLE           &      &      &       & '1': disable glitch filter \\
     *       &                     &      &      &       & '0': enable glitch filter  \\ \hline
     * 19:17 & RFU                 &  0   &  0   & R/-   & RFU                                  \\ \hline
     * 23:20 & REBOUND             &  h0  &  h0  & R/W   & Rebound filter for \texttt{sda\_a} pulse \\
     *       & \_FILTER            &      &      &       &  \\  \hline
     * 31:24 & RFU                 &  0   &  0   & R/-   & RFU                                  \\
     * \invia_endReg
     */
    HFCTRL_DIGITAL_CFG = 9,

    /* Base address + 0x28 - DIGITAL FRONTEND TRIM COUNTER0 register
     * \invia_beginReg
     * 9:0   & DEC\_TH\_0          &  h18  &  h18   & R/W   & When the demodulation is $ASK$ 100\% Modified Miller only:
     * Logic counter threshold 0 ($Rx$)   \\\hline
     * 15:10 & RFU                 &  0    &  0     & R/-   & RFU                                  \\ \hline
     * 25:16 & DEC\_TH\_1          &  h78  &  h78   & R/W   & When the demodulation is $ASK$ 100\% Modified Miller only:
     * Logic counter threshold 1 ($Rx$)   \\\hline
     * 27:26 & RFU                 &  0    &  0     & R/-   & RFU                                  \\ \hline
     * 28    & DEC\_TH\_CFG        & '0'   & '0'    & R/W   & When the demodulation is $ASK$ 100\% Modified Miller only:
     *\\
     *       &                     &       &        &       & When '1': allows to count with $Fc$ when \texttt{sda\_a}
     * is down \\ \hline
     * 31:29 & RFU                 &  0    &  0     & R/-   & RFU                                  \\
     * \invia_endReg
     */
    HFCTRL_DIGITAL_CNT0_CFG = 10,

    /* Base address + 0x2C - DIGITAL FRONTEND TRIM COUNTER1 register
     * \invia_beginReg
     * 9:0   & DEC\_TH\_2          &  hB8  &  hB8  & R/W   & When the demodulation is $ASK$ 100\% Modified Miller only:
     * Logic counter threshold 2 ($Rx$)   \\\hline
     * 15:10 & RFU                 &  0    &  0    & R/-   & RFU                                  \\ \hline
     * 25:16 & DEC\_TH\_3          &  hF8  &  hF8  & R/W   & When the demodulation is $ASK$ 100\% Modified Miller only:
     * Logic counter threshold 3 ($Rx$)   \\\hline
     * 31:26 & RFU                 &  0    &  0    & R/-   & RFU                                  \\
     * \invia_endReg
     */
    HFCTRL_DIGITAL_CNT1_CFG = 11,

    /* Base address + 0x30 - DIGITAL FRONTEND TRIM COUNTER2 register
     * \invia_beginReg
     * 9:0   & DEC\_TH\_4          &  h30F  &  h30F & R/W   & SPARE REGISTERS   \\\hline
     * 31:10 & RFU                 &  0     &  0    & R/-   & RFU                                  \\
     * \invia_endReg
     */
    HFCTRL_DIGITAL_CNT2_CFG = 12,

    /* Base address + 0x34 - FDT CONFIG register
     * \invia_beginReg
     * 7:0    & RFU                 &   0   &  0    & R/W   & RFU              \\ \hline
     * 11:8   & FDT\_LAYER3         &  h5   & h5    & R/W   & $FDT$ for Layer 3                                      \\
     *\hline
     * 15:12  & RFU                 &   0   &  0    & R/-   & RFU                                  \\ \hline
     * 23:16  & FDT0\_VALUE         &  h4F  & h4F   & R/W   & Offset following a '0' before the first slot - granularity
     * 1/$Fc$ - $FDT$ definition \\ \hline
     * 31:24  & FDT1\_VALUE         &  h4F  & h4F   & R/W   & Offset following a '1' before the first slot - granularity
     * 1/$Fc$ - $FDT$ definition \\
     * \invia_endReg
     */
    HFCTRL_FDT_TIMER_CFG = 13,

    /* Base address + 0x38 - POWER CONFIG register
     * \invia_beginReg
     * 1:0    & RF\_ON\_            &   0   &  0    & R/W   & Configuration of the threshold for
     *                                 \\
     *        & THRESHOLD           &       &       &       & RF\_ON detection, depending on the HF\_DET\_LS[2:0]
     *                \\
     *        &                     &       &       &       & when '00' : threshold is '110'
     *                                     \\
     *        &                     &       &       &       & when '01' : threshold is '100'
     *                                     \\
     *        &                     &       &       &       & when '11' or '10 : threshold is '000'
     *                              \\  \hline
     * 2      & RF\_ON\_ITENA       &  '0'  & '0'   & R/W   & When '1', enables an interrupt when
     * HFCTRL\_POWER\_STATUS.RF\_ON is set     \\ \hline
     * 3      & RF\_OFF\_ITENA      &  '0'  & '0'   & R/W   & When '1', enables an interrupt when
     * HFCTRL\_POWER\_STATUS.RF\_OFF is set    \\ \hline
     * 7:4    & RFU                 &   0   &  0    & R/-   & RFU
     *                                                                \\ \hline
     * 8      & HF\_RESET\_CFG      &  '0'  & '0'   & R/W   & When '1', a $HF$ reset will be triggered at each RF\_ON
     * event       \\ \hline
     * 9      & HF\_RESET\_ENA      &  '0'  & '0'   & R/S   & When '1', a $HF$ reset is triggered. Set only by $SW$
     *              \\ \hline
     * 10     & HF\_RESET\_         &  '0'  & '0'   & R/W   & When '1', enables an interrupt when
     *                                \\
     *        & ITENA               &       &       &       & HFCTRL\_POWER\_STATUS.
     *                                             \\
     *        &                     &       &       &       & HF\_RESET is set
     *                                                   \\ \hline
     * 31:11  & RFU                 &   0   &  0    & R/-   & RFU
     *                                                                \\
     * \invia_endReg
     */
    HFCTRL_POWER_CFG = 14,

    /* Base address + 0x3C - POWER STATUS register
     * \invia_beginReg
     * 4:0    & HF\_DET\_LS         &   h1F &  h1F  & R/-   & HF\_DET\_LS from AFE after system clock synchronization \\
     *\hline
     * 7:5    & RFU                 &   0   &  0    & R/-   & RFU                                                     \\
     *\hline
     * 8      & RF\_ON              &  '0'  & '0'   & R/C1  & When '1', a RF ON event occurred.                       \\
     *        &                     &       &       &       & Clear only by $SW$ by writing '1'                       \\
     *\hline
     * 9      & RF\_OFF             &  '0'  & '0'   & R/C1  & When '1', a RF OFF event occurred.                      \\
     *        &                     &       &       &       & Clear only by $SW$ by writing '1'                       \\
     *\hline
     * 10     & HF\_RESET           &  '0'  & '0'   & R/C1  & When '1', a HF RESET event occurred.                    \\
     *        &                     &       &       &       & Clear only by $SW$ by writing '1'                       \\
     *\hline
     * 31:11  & RFU                 &   0   &  0    & R/-   & RFU                                                     \\
     * \invia_endReg
     */
    HFCTRL_POWER_STATUS = 15
};

typedef volatile uint32_t *HFCTRL;

#define HFCTRL_IP                           ((HFCTRL)PLATFORM_HF_BANK_ADDR)
#define HFCTRL_IP_RAM                       ((HFCTRL)PLATFORM_HF_BUFFER_ADDR)

/***************************************
* HFCTRL_TEST_FC_CNT
***************************************/

/* Get the complete Fc clock test register */
static inline uint32_t _isohf_getTestFcCnt(HFCTRL isohf)
{
    return isohf[HFCTRL_TEST_FC_CNT];
}

/* Set the complete Fc clock test register */
static inline void _isohf_setTestFcCnt(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_TEST_FC_CNT] = configValue;
}

/***************************************
* HFCTRL_TEST_SDA_CNT
***************************************/

/* Get the complete SDA clock test register */
static inline uint32_t _isohf_getTestSDACnt(HFCTRL isohf)
{
    return isohf[HFCTRL_TEST_SDA_CNT];
}

/* Set the complete SDA clock test register */
static inline void _isohf_setTestSDACnt(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_TEST_SDA_CNT] = configValue;
}

/***************************************
* HFCTRL_TEST_CTRL
***************************************/

/* Get the complete test control register */
static inline uint32_t _isohf_getTestCtrl(HFCTRL isohf)
{
    return isohf[HFCTRL_TEST_CTRL];
}

/* Set the complete test control register */
static inline void _isohf_setTestCtrl(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_TEST_CTRL] = configValue;
}

/***************************************
* HFCTRL_PROTOCOL_CFG
***************************************/

/* Get the complete protocol register */
static inline uint32_t _isohf_getProtocolCfg(HFCTRL isohf)
{
    return isohf[HFCTRL_PROTOCOL_CFG];
}

/* Get the protocol type config */
static inline uint32_t _isohf_getProtocolType(HFCTRL isohf)
{
    return ((isohf[HFCTRL_PROTOCOL_CFG] & HF_P_CFG_TYPE_MASK) >> HF_P_CFG_TYPE_SHIFT);
}

/* Get the protocol ISO Type A UID config */
static inline uint32_t _isohf_getProtocolUID(HFCTRL isohf)
{
    return ((isohf[HFCTRL_PROTOCOL_CFG] & HF_P_CFG_UID_MASK) >> HF_P_CFG_UID_SHIFT);
}

/* Get the protocol L4 error ignore config */
static inline uint32_t _isohf_getL4ErrorIgnoreCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_PROTOCOL_CFG] & HF_P_CFG_L4_ERR_FRAME_MASK) >> HF_P_CFG_L4_ERR_FRAME_SHIFT);
}

/* Get the protocol ISO Type A layer 4 Watchpoint configuration */
static inline uint32_t _isohf_getProtocolWP(HFCTRL isohf)
{
    return ((isohf[HFCTRL_PROTOCOL_CFG] & HF_P_CFG_CNT_WP_MASK) >> HF_P_CFG_CNT_WP_SHIFT);
}

/* Set the complete protocol register */
static inline void _isohf_setProtocolCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_PROTOCOL_CFG] = configValue;
}

/* Set the protocol type config -- do not touch others */
static inline void _isohf_setProtocolType(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_PROTOCOL_CFG] =
        ((isohf[HFCTRL_PROTOCOL_CFG] & ~HF_P_CFG_TYPE_MASK) | (configValue << HF_P_CFG_TYPE_SHIFT));
}

/* Set the Ignore REQA config -- do not touch others */
static inline void _isohf_setProtocolIgnoreReqA(HFCTRL isohf)
{
    isohf[HFCTRL_PROTOCOL_CFG] = (isohf[HFCTRL_PROTOCOL_CFG] | HF_P_CFG_REQA_IGNORE_ENA);
}

/* Set the Layer 4 ISO config -- do not touch others */
static inline void _isohf_setProtocolTypeALayer4(HFCTRL isohf)
{
    isohf[HFCTRL_PROTOCOL_CFG] = (isohf[HFCTRL_PROTOCOL_CFG] | HF_P_CFG_TYPEA_LAYER4_ENA);
}

/* Reset the Layer 4 ISO config -- do not touch others */
static inline void _isohf_resetProtocolTypeALayer4(HFCTRL isohf)
{
    isohf[HFCTRL_PROTOCOL_CFG] = (isohf[HFCTRL_PROTOCOL_CFG] & ~HF_P_CFG_TYPEA_LAYER4_MASK);
}

/* Set the protocol ISO Type A UID config -- do not touch others */
static inline void _isohf_setProtocolUID(HFCTRL isohf, uint32_t uidValue)
{
    isohf[HFCTRL_PROTOCOL_CFG] = ((isohf[HFCTRL_PROTOCOL_CFG] & ~HF_P_CFG_UID_MASK) | (uidValue << HF_P_CFG_UID_SHIFT));
}

/* Set the protocol ISO L4 error ignore config -- do not touch others */
static inline void _isohf_setL4ErrorIgnoreCfg(HFCTRL isohf)
{
    isohf[HFCTRL_PROTOCOL_CFG] = ((isohf[HFCTRL_PROTOCOL_CFG] | HF_P_CFG_L4_ERR_FRAME_ENA));
}

/* Set the protocol ISO Type A layer 4 Watchpoint -- do not touch others */
static inline void _isohf_setProtocolWP(HFCTRL isohf, uint32_t wpValue)
{
    isohf[HFCTRL_PROTOCOL_CFG] =
        ((isohf[HFCTRL_PROTOCOL_CFG] & ~HF_P_CFG_CNT_WP_MASK) | (wpValue << HF_P_CFG_CNT_WP_SHIFT));
}

/***************************************
* HFCTRL_PROTOCOL_CTRL
***************************************/

/* Get the complete protocol control register */
static inline uint32_t _isohf_getProtocolCtrl(HFCTRL isohf)
{
    return isohf[HFCTRL_PROTOCOL_CTRL];
}

/* Set the complete protocol control register */
static inline void _isohf_setProtocolCtrl(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_PROTOCOL_CTRL] = configValue;
}

/***************************************
* HFCTRL_COUNTER_STATUS
***************************************/

/* Get the slot counter status */
static inline uint32_t _isohf_getSlotCounterStatus(HFCTRL isohf)
{
    return isohf[HFCTRL_COUNTER_STATUS];
}

/***************************************
* HFCTRL_STATUS
***************************************/

/* Get the complete status register */
static inline uint32_t _isohf_getStatus(HFCTRL isohf)
{
    return isohf[HFCTRL_STATUS];
}

/* Get the Rx-Exec-Tx current period status */
static inline uint32_t _isohf_getComStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_COM_INFO_MASK) >> HF_STATUS_COM_INFO_SHIFT);
}

/* Get the Platform hand status */
static inline uint32_t _isohf_getPlatformHandStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_PLATHAND_MASK) >> HF_STATUS_PLATHAND_SHIFT);
}

/* Get the End of Com status */
static inline uint32_t _isohf_getEndOfComStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_ENDOFCOM_MASK) >> HF_STATUS_ENDOFCOM_SHIFT);
}

/* Get the Rx error status */
static inline uint32_t _isohf_getRxErrorStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_ERR_RX_MASK) >> HF_STATUS_ERR_FRAME_LOGIC_SHIFT);
}

/* Get the Counter overflow error status */
static inline uint32_t _isohf_getCntOverErrorStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_ERR_CNT_OFLOW_MASK) >> HF_STATUS_ERR_CNT_OFLOW_SHIFT);
}

/* Get the slot counter watchpoint status */
static inline uint32_t _isohf_getWPStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_CNT_WP_MASK) >> HF_STATUS_CNT_WP_SHIFT);
}

/* Get the Rx frame size */
static inline uint32_t _isohf_getRxFrameSize(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_RX_FRAME_SIZE_MASK) >> HF_STATUS_RX_FRAME_SIZE_SHIFT);
}

/* Get the Rx splitbyte bit size */
static inline uint32_t _isohf_getRxSplitByteSize(HFCTRL isohf)
{
    return ((isohf[HFCTRL_STATUS] & HF_STATUS_RX_FRAME_BIT_NUM_MASK) >> HF_STATUS_RX_FRAME_BIT_NUM_SHIFT);
}

/* Clear End of Com status - clear by '1' */
static inline void _isohf_clearEndOfComStatus(HFCTRL isohf)
{
    isohf[HFCTRL_STATUS] = HF_STATUS_ENDOFCOM_MASK;
}

/* Clear Counter overflow status - clear by '1' */
static inline void _isohf_clearCntOverStatus(HFCTRL isohf)
{
    isohf[HFCTRL_STATUS] = HF_STATUS_ERR_CNT_OFLOW_MASK;
}

/* Clear Watchpoint status - clear by '1' */
static inline void _isohf_clearWPStatus(HFCTRL isohf)
{
    isohf[HFCTRL_STATUS] = HF_STATUS_CNT_WP_MASK;
}

/* Clear whole status - clear by '1' */
static inline void _isohf_clearStatus(HFCTRL isohf)
{
    isohf[HFCTRL_STATUS] = ((HF_STATUS_CNT_WP_MASK | HF_STATUS_ERR_CNT_OFLOW_MASK | HF_STATUS_ENDOFCOM_MASK));
}

/***************************************
* HFCTRL_ITENA
***************************************/

/* Get the complete itena register */
static inline uint32_t _isohf_getItena(HFCTRL isohf)
{
    return isohf[HFCTRL_ITENA];
}

/* Set the complete itena register */
static inline void _isohf_setItena(HFCTRL isohf, uint32_t itena_value)
{
    isohf[HFCTRL_ITENA] = itena_value;
}

/* Mask the whole IT , let the PM wakeup enable and End of Com IT */
static inline void _isohf_maskAllIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] = (isohf[HFCTRL_ITENA] & ~(HF_ITENA_CNT_WP_MASK | HF_ITENA_CNT_OFLOW_MASK));
}

/* Mask the End of com it -- do not touch others */
static inline void _isohf_maskEndOfComIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] =  (isohf[HFCTRL_ITENA] & ~HF_ITENA_ENDOFCOM_MASK);
}

/* Mask the Watchpoint Counter it -- do not touch others */
static inline void _isohf_maskWPIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] =  (isohf[HFCTRL_ITENA] & ~HF_ITENA_CNT_WP_MASK);
}

/* Mask the Overflow Counter it -- do not touch others */
static inline void _isohf_maskOverflowIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] =  (isohf[HFCTRL_ITENA] & ~HF_ITENA_CNT_OFLOW_MASK);
}

/* Enable the whole It, let the Automatic wakeup configuration and end of Com IT */
static inline void _isohf_enableAllIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] = (isohf[HFCTRL_ITENA] | (HF_ITENA_CNT_OFLOW_MASK & HF_ITENA_CNT_WP_MASK));
}

/* Enable the End of com Counter IT --  do not touch others */
static inline void _isohf_enableEndOfComIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] = (isohf[HFCTRL_ITENA] | HF_ITENA_ENDOFCOM_MASK);
}

/* Enable the  Watchpoint Counter IT --  do not touch others */
static inline void _isohf_enableWPIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] = (isohf[HFCTRL_ITENA] | HF_ITENA_CNT_WP_MASK);
}

/* Enable the  Overflow Counter IT --  do not touch others */
static inline void _isohf_enableOverflowIt(HFCTRL isohf)
{
    isohf[HFCTRL_ITENA] = (isohf[HFCTRL_ITENA] | HF_ITENA_CNT_OFLOW_MASK);
}

/***************************************
* HFCTRL_ANALOG_CFG
***************************************/

/* Get the complete analog config register */
static inline uint32_t _isohf_getAnalogCfg(HFCTRL isohf)
{
    return isohf[HFCTRL_ANALOG_CFG];
}

/* Get the demodulateur A config */
static inline uint32_t _isohf_getAnalogDemACfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_ANALOG_CFG] & HF_ANA_CFG_TR_DEM_A_MASK) >> HF_ANA_CFG_TR_DEM_A_SHIFT);
}

/* Get the load for modulation config */
static inline uint32_t _isohf_getAnalogLoadModCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_ANALOG_CFG] & HF_ANA_CFG_LOAD_MOD_MASK) >> HF_ANA_CFG_LOAD_MOD_SHIFT);
}

/* Get the voltage regulator config */
static inline uint32_t _isohf_getAnalogVregCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_ANALOG_CFG] & HF_ANA_CFG_VREG_MASK) >> HF_ANA_CFG_VREG_SHIFT);
}

/* Set the complete analog config register */
static inline void _isohf_setAnalogCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_ANALOG_CFG] = configValue;
}

/* Set the demodulateur A config -- do not touch others */
static inline void _isohf_setAnalogDemACfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_ANALOG_CFG] =
        ((isohf[HFCTRL_ANALOG_CFG] & ~HF_ANA_CFG_TR_DEM_A_MASK) | (configValue << HF_ANA_CFG_TR_DEM_A_SHIFT));
}

/* Set the load for modulation config -- do not touch others */
static inline void _isohf_setAnalogLoadModCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_ANALOG_CFG] =
        ((isohf[HFCTRL_ANALOG_CFG] & ~HF_ANA_CFG_LOAD_MOD_MASK) | (configValue << HF_ANA_CFG_LOAD_MOD_SHIFT));
}

/* Set the voltage regulator config -- do not touch others */
static inline void _isohf_setAnalogVregCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_ANALOG_CFG] =
        ((isohf[HFCTRL_ANALOG_CFG] & ~HF_ANA_CFG_VREG_MASK) | (configValue << HF_ANA_CFG_VREG_SHIFT));
}

/***************************************
* HFCTRL_DIGITAL_CFG
***************************************/

/* Get the complete digital config register */
static inline uint32_t _isohf_getDigitalCfg(HFCTRL isohf)
{
    return isohf[HFCTRL_DIGITAL_CFG];
}

/* Get the bit rate config */
static inline uint32_t _isohf_getBitRateCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_DIGITAL_CFG] & HF_DIG_CFG_BIT_RATE_MASK) >> HF_DIG_CFG_BIT_RATE_SHIFT);
}

/* Get the Coding type config */
static inline uint32_t _isohf_getCodingCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_DIGITAL_CFG] & HF_DIG_CFG_COD_TYPE_MASK) >> HF_DIG_CFG_COD_TYPE_SHIFT);
}

/* Get the Glitch filter disable config */
static inline uint32_t _isohf_getGlitchFilterCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_DIGITAL_CFG] & HF_DIG_CFG_GLITCH_FILTER_DIS_MASK) >> HF_DIG_CFG_GLITCH_FILTER_DIS_SHIFT);
}

/* Get the Rebound filter config */
static inline uint32_t _isohf_getReboundFilterCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_DIGITAL_CFG] & HF_DIG_CFG_REBOUND_FILTER_MASK) >> HF_DIG_CFG_REBOUND_FILTER_SHIFT);
}

/* Set the complete digital config register */
static inline void _isohf_setDigitalCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CFG] = configValue;
}

/* Set the bit rate config -- do not touch others */
static inline void _isohf_setBitRateCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CFG] =
        ((isohf[HFCTRL_DIGITAL_CFG] & ~HF_DIG_CFG_BIT_RATE_MASK) | (configValue << HF_DIG_CFG_BIT_RATE_SHIFT));
}

/* Set the Coding type config -- do not touch others */
static inline void _isohf_setCodingCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CFG] =
        ((isohf[HFCTRL_DIGITAL_CFG] & ~HF_DIG_CFG_COD_TYPE_MASK) | (configValue << HF_DIG_CFG_COD_TYPE_SHIFT));
}

/* Disable the Glitch filter -- do not touch others */
static inline void _isohf_disableGlitchFilter(HFCTRL isohf)
{
    isohf[HFCTRL_DIGITAL_CFG] = (isohf[HFCTRL_DIGITAL_CFG] | HF_DIG_CFG_GLITCH_FILTER_DIS);
}

/* Enable the Glitch filter -- do not touch others */
static inline void _isohf_enableGlitchFilter(HFCTRL isohf)
{
    isohf[HFCTRL_DIGITAL_CFG] =
        ((isohf[HFCTRL_DIGITAL_CFG] & ~HF_DIG_CFG_GLITCH_FILTER_DIS_MASK) | HF_DIG_CFG_GLITCH_FILTER_ENA);
}

/* Set the Rebound filter config -- do not touch others */
static inline void _isohf_setReboundFilterCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CFG] =
        ((isohf[HFCTRL_DIGITAL_CFG] & ~HF_DIG_CFG_REBOUND_FILTER_MASK) |
         (configValue << HF_DIG_CFG_REBOUND_FILTER_SHIFT));
}

/***************************************
* HFCTRL_DIGITAL_CNT0_CFG
***************************************/

/* Get the complete digital threshold 0 config register with threshold type config */
static inline uint32_t _isohf_getDigitalCnt0Cfg(HFCTRL isohf)
{
    return isohf[HFCTRL_DIGITAL_CNT0_CFG];
}

/* Get the Decoding threshold type config */
static inline uint32_t _isohf_getDecodingThCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_DIGITAL_CNT0_CFG] & HF_DIG_CFG_DEC_TH_MASK) >> HF_DIG_CFG_DEC_TH_SHIFT);
}

/* Set the complete digital threshold 0 config register  with threshold type config */
static inline void _isohf_setDigitalCnt0Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CNT0_CFG] = configValue;
}

/* Set the Decoding threshold type config -- do not touch others */
static inline void _isohf_setDecodingThCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CNT0_CFG] =
        ((isohf[HFCTRL_DIGITAL_CNT0_CFG] & ~HF_DIG_CFG_DEC_TH_MASK) | (configValue <<  HF_DIG_CFG_DEC_TH_SHIFT));
}

/***************************************
* HFCTRL_DIGITAL_CNT1_CFG
***************************************/

/* Get the complete digital threshold 1 config register */
static inline uint32_t _isohf_getDigitalCnt1Cfg(HFCTRL isohf)
{
    return isohf[HFCTRL_DIGITAL_CNT1_CFG];
}

/* Set the complete digital threshold 1 config register */
static inline void _isohf_setDigitalCnt1Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CNT1_CFG] = configValue;
}

/***************************************
* HFCTRL_DIGITAL_CNT2_CFG -- // Spares registers
***************************************/

/* Get the complete digital threshold 2 config register */
static inline uint32_t _isohf_getDigitalCnt2Cfg(HFCTRL isohf)
{
    return isohf[HFCTRL_DIGITAL_CNT2_CFG];
}

/* Set the complete digital threshold 2 config register */
static inline void _isohf_setDigitalCnt2Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_DIGITAL_CNT2_CFG] = configValue;
}

/***************************************
* Generic function for HFCTRL_DIGITAL_CNT*_CFG
***************************************/

/* CounterIndex is 0-5 - no check on this argument */
/* CntValue is the Value of the threshold */
static inline void _isohf_setDigitalCntCfg(HFCTRL isohf, uint32_t CounterIndex, uint32_t CntValue)
{
    if ((CounterIndex & 0x1) == 0)
    {
        isohf[HFCTRL_DIGITAL_CNT0_CFG +
              (CounterIndex >> 1)] = (isohf[HFCTRL_DIGITAL_CNT0_CFG + (CounterIndex >> 1)] & ~HF_DIG_CNT0_MASK) |
                                     ((CntValue) & HF_DIG_CNT0_MASK);
    }
    else
    {
        isohf[HFCTRL_DIGITAL_CNT0_CFG +
              (CounterIndex >> 1)] = (isohf[HFCTRL_DIGITAL_CNT0_CFG + (CounterIndex >> 1)] & ~HF_DIG_CNT1_MASK) |
                                     ((CntValue << HF_DIG_CNT1_SHIFT) & HF_DIG_CNT1_MASK);
    }
}

/* CounterIndex is 0-5 - no check on this argument */
static inline uint32_t _isohf_getDigitalCntCfg(HFCTRL isohf, uint32_t CounterIndex)
{
    if ((CounterIndex & 0x1) == 0)
    {
        return (isohf[HFCTRL_DIGITAL_CNT0_CFG + (CounterIndex >> 1)] & HF_DIG_CNT0_MASK);
    }
    else
    {
        return ((isohf[HFCTRL_DIGITAL_CNT0_CFG + (CounterIndex >> 1)] >> HF_DIG_CNT1_SHIFT) & HF_DIG_CNT0_MASK);
    }
}

/***************************************
* HFCTRL_FDT_TIMER_CFG
***************************************/

/* Get the complete FDT digital config register */
static inline uint32_t _isohf_getDigitalFDTTimerCfg(HFCTRL isohf)
{
    return isohf[HFCTRL_FDT_TIMER_CFG];
}

/* Get the FDT layer3 config */
static inline uint32_t _isohf_getFDTLayer3Cfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_FDT_TIMER_CFG] & HF_FDT_L3_VAL_MASK) >> HF_FDT_L3_VAL_SHIFT);
}

/* Get the Offset after 0 value */
static inline uint32_t _isohf_getOffset0Cfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_FDT_TIMER_CFG] & HF_OFFSET0_VAL_MASK) >> HF_OFFSET0_VAL_SHIFT);
}

/* Get the Offset after 1 value */
static inline uint32_t _isohf_getOffset1Cfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_FDT_TIMER_CFG] & HF_OFFSET1_VAL_MASK) >> HF_OFFSET1_VAL_SHIFT);
}

/* Set the complete digital config register */
static inline void _isohf_setDigitalFDTTimerCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_FDT_TIMER_CFG] = configValue;
}

/* Set the FDT Layer3 value config --  do not touch others */
static inline void _isohf_setFDTLayer3Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_FDT_TIMER_CFG] =
        ((isohf[HFCTRL_FDT_TIMER_CFG] & ~HF_FDT_L3_VAL_MASK) | (configValue << HF_FDT_L3_VAL_SHIFT));
}

/* Set the Offset after 0 value -- do not touch others */
static inline void _isohf_setOffset0Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_FDT_TIMER_CFG] =
        ((isohf[HFCTRL_FDT_TIMER_CFG] & ~HF_OFFSET0_VAL_MASK) | (configValue << HF_OFFSET0_VAL_SHIFT));
}

/* Set the Offset after 1 value -- do not touch others */
static inline void _isohf_setOffset1Cfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_FDT_TIMER_CFG] =
        ((isohf[HFCTRL_FDT_TIMER_CFG] & ~HF_OFFSET1_VAL_MASK) | (configValue << HF_OFFSET1_VAL_SHIFT));
}

/***************************************
* HFCTRL_POWER_CFG
***************************************/

/* Get the RF ON Threshold config */
static inline uint32_t _isohf_getRFONThresholdCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_CFG] & HF_RF_ON_THRESHOLD_MASK) >> HF_RF_ON_THRESHOLD_SHIFT);
}

/* Get the RF ON Itena config */
static inline uint32_t _isohf_getRFONItenaCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_CFG] & HF_RF_ON_ITENA_MASK) >> HF_RF_ON_ITENA_SHIFT);
}

/* Get the RF OFF Itena config */
static inline uint32_t _isohf_getRFOFFItenaCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_CFG] & HF_RF_OFF_ITENA_MASK) >> HF_RF_OFF_ITENA_SHIFT);
}

/* Get the HF reset config */
static inline uint32_t _isohf_getHFResetCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_CFG] & HF_RESET_CFG_MASK) >> HF_RESET_CFG_SHIFT);
}

/* Get the HF reset itena config */
static inline uint32_t _isohf_getHFResetItenaCfg(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_CFG] & HF_RESET_ITENA_MASK) >> HF_RESET_ITENA_SHIFT);
}

/* Set the RF ON threshold -- do not touch others */
static inline void _isohf_setRFONThresholdCfg(HFCTRL isohf, uint32_t configValue)
{
    isohf[HFCTRL_POWER_CFG] =
        ((isohf[HFCTRL_POWER_CFG] & ~HF_RF_ON_THRESHOLD_MASK) | (configValue << HF_RF_ON_THRESHOLD_SHIFT));
}

/* Enable the RF ON IT --  do not touch others */
static inline void _isohf_enableRFONIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] | HF_RF_ON_ITENA_MASK);
}

/* Enable the RF OFF IT --  do not touch others */
static inline void _isohf_enableRFOFFIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] | HF_RF_OFF_ITENA_MASK);
}

/* Enable the HF reset when RFON event --  do not touch others */
static inline void _isohf_enableHFResetCfg(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] | HF_RESET_CFG_MASK);
}

/* Disable the HF reset when RFON event --  do not touch others */
static inline void _isohf_disableHFResetCfg(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] & ~HF_RESET_CFG_MASK);
}

/* HF reset by SW */
static inline void _isohf_enableHFReset(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] | HF_RESET_ENA_MASK);
}

/* Enable the HF reset IT --  do not touch others */
static inline void _isohf_enableHFResetIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] | HF_RESET_ITENA_MASK);
}

/* Enable the whole Power It */
static inline void _isohf_enableAllPowerIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] =
        (isohf[HFCTRL_POWER_CFG] | (HF_RESET_ITENA_MASK & HF_RF_OFF_ITENA_MASK & HF_RF_ON_ITENA_MASK));
}

/* Mask the RF ON IT --  do not touch others */
static inline void _isohf_maskRFONIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] & ~HF_RF_ON_ITENA_MASK);
}

/* Mask the RF OFF IT --  do not touch others */
static inline void _isohf_maskRFOFFIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] & ~HF_RF_OFF_ITENA_MASK);
}

/* Mask the HF reset IT --  do not touch others */
static inline void _isohf_maskHFResetIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] = (isohf[HFCTRL_POWER_CFG] & ~HF_RESET_ITENA_MASK);
}

/* Mask the whole Power It */
static inline void _isohf_maskAllPowerIt(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_CFG] =
        (isohf[HFCTRL_POWER_CFG] & ~(HF_RESET_ITENA_MASK | HF_RF_OFF_ITENA_MASK | HF_RF_ON_ITENA_MASK));
}

/***************************************
* HFCTRL_POWER_STATUS
***************************************/

/* Get the HF_DET_LS status */
static inline uint32_t _isohf_getHFDetLSStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_STATUS] & HF_DET_LS_MASK) >> HF_DET_LS_SHIFT);
}

/* Get the RF ON status */
static inline uint32_t _isohf_getRFONStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_STATUS] & HF_RF_ON_STATUS_MASK) >> HF_RF_ON_STATUS_SHIFT);
}

/* Get the RF OFF status */
static inline uint32_t _isohf_getRFOFFStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_STATUS] & HF_RF_OFF_STATUS_MASK) >> HF_RF_OFF_STATUS_SHIFT);
}

/* Get the HF reset status */
static inline uint32_t _isohf_getHFResetStatus(HFCTRL isohf)
{
    return ((isohf[HFCTRL_POWER_STATUS] & HF_RESET_STATUS_MASK) >> HF_RESET_STATUS_SHIFT);
}

/*Clear RF ON status - clear by '1' */
static inline void _isohf_clearRFONStatus(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_STATUS] = HF_RF_ON_STATUS_MASK;
}

/*Clear RF OFF status - clear by '1' */
static inline void _isohf_clearRFOFFStatus(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_STATUS] = HF_RF_OFF_STATUS_MASK;
}

/*Clear HF reset status - clear by '1'*/
static inline void _isohf_clearHFResetStatus(HFCTRL isohf)
{
    isohf[HFCTRL_POWER_STATUS] = HF_RESET_STATUS_MASK;
}

/***************************************
* HF_IO_RAM_BUFFER
***************************************/

/* Get IO RAM buffer data*/
static inline uint32_t _isohf_getHFIORAMbyte(uint32_t offset)
{
    return *(uint32_t *)(HF_IO_RAM_START_ADD + offset);
}

/*set IO RAM buffer data*/
static inline void _isohf_setHFIORAMbyte(uint32_t offset, uint32_t data)
{
    ((uint32_t *)(HF_IO_RAM_START_ADD))[offset] = data;
}

#endif    /* ASSEMBLY */

#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /*__ISOHF14443_H__*/
