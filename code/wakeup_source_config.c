/**
 * @file wakeup_source_config.c
 * @brief Wakeup source configuration file
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
#include "wakeup_source_config.h"
#include "sensor.h"

unsigned int reset_fifo_level;
unsigned int fifo_size = FIFO_SIZE_VALUE;
unsigned int number_of_samples = NBR_SAMPLES_VALUE;

#if SENSOR_NORMAL == 1
unsigned int adc_threshold = ADC_THRESHOLD_VALUE_1NA;
unsigned int diff_mode = SENSOR_DIFF_MODE_DISABLED;
#elif SENSOR_IMPEDANCE == 1
unsigned int adc_threshold = ADC_THRESHOLD_VALUE_77P75NA;
unsigned int diff_mode = SENSOR_DIFF_MODE_ENABLED;
#endif

uint8_t RAW_ARRAY[64] = {};

/* Define delay in run mode after wakeup from NFC in seconds */
#define APP_DELAY_S                     3

/**
 * @brief      Configure selected wake up source and enable associated interrupts
 */
void Wakeup_Source_Config(void)
{
	if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_RTC_ALARM) & WAKEUP_SRC_EN_MSK)
	{
		/* Configure and enable RTC ALARM */
		RTC_ALARM_Init();
	}

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_BB) & WAKEUP_SRC_EN_MSK)
    {
        /* Configure and enable BB Timer wakeup source */
        BB_Timer_Init();
    }

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_GPIO) & WAKEUP_SRC_EN_MSK)
    {
        /* Configure and enable GPIO wakeup source */
        GPIO_Wakeup_Init();
    }

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_NFC) & WAKEUP_SRC_EN_MSK)
    {
        /* Configure and enable NFC */
        NFC_Init();
    }

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_FIFO) & WAKEUP_SRC_EN_MSK)
    {
        /* Configure and enable ADC FIFO, Wakeup when FIFO is full. */
        ADC_FIFO_Init();
    }

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_ADC) & WAKEUP_SRC_EN_MSK)
    {
        /* Configure and enable ADC threshold, Wakeup when ADC threshold limit is reached. */
        ADC_Threshold_Init();
    }

    if ((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_SENSOR_DET) & WAKEUP_SRC_EN_MSK)
    {
    	/* Configure and enable Sensor detection, Wakeup when Sensor is detected. */
    	SENSOR_DET_Init();
    }

    /* Clear all wakeup flags */
    WAKEUP_FLAGS_CLEAR();

    /* Clear NVIC WAKEUP_IRQn interrupt */
    NVIC_ClearPendingIRQ(WAKEUP_IRQn);

    /* Enable the Wakeup interrupt */
    NVIC_EnableIRQ(WAKEUP_IRQn);
}

/**
 * @brief       Configure and enable RTC ALARM event
 * @assumptions The following are pre-defined;
 *              RTC_CLK_SRC: RTC clock source
 */
void RTC_ALARM_Init(void)
{
    /* Configure and enable clock source for RTC */
    RTC_ClockSource_Init();

    /* Configure RTC with timercounter-1 as start value*/
    ACS->RTC_CTRL = RTC_DISABLE;
    ACS->RTC_CTRL = RTC_RESET;
    ACS->RTC_CFG = 0xDEADBEEF;
    ACS->RTC_CTRL = RTC_ENABLE | RTC_CLK_SRC | RTC_ALARM_ZERO;

    /* Clear sticky wakeup RTC alarm flag */
    WAKEUP_RTC_ALARM_FLAG_CLEAR();
}

/**
 * @brief Configure and enable clock source for RTC
 */
void RTC_ClockSource_Init(void)
{
    if (RTC_CLK_SRC == RTC_CLK_SRC_RC_OSC)
    {
        /* Enable RC32k without changing any other register bits */
        ACS->RCOSC_CTRL |= RC32_OSC_ENABLE;
    }
    if (RTC_CLK_SRC == RTC_CLK_SRC_GPIO0)
    {
        /* Configure GPIO0 as an input pin */
        SYS_GPIO_CONFIG(GPIO0, GPIO_MODE_INPUT);
    }
    if (RTC_CLK_SRC == RTC_CLK_SRC_GPIO1)
    {
        /* Configure GPIO1 as an input pin */
        SYS_GPIO_CONFIG(GPIO1, GPIO_MODE_INPUT);
    }
}

/**
 * @brief Re-configure start value for the RTC timer counter
 * @param [in] timer_counter Start value(number of cycles)
 *             for RTC timer counter
 */
void RTC_ALARM_Reconfig(uint32_t timer_counter)
{
	/* NVIC set enable registers */
    uint32_t nvic_set_enable[2];

    /* Mask interrupts */
	__disable_irq();

   /* Backup NVIC set enable registers so that we can restore it later */
	nvic_set_enable[0]=NVIC->ISER[0];
	nvic_set_enable[1]=NVIC->ISER[1];

   /* Set NVIC clear enable registers to clear interrupts */
    NVIC->ICER[0] = 0xFFFFFFFFU;
    NVIC->ICER[1] = 0xFFFFFFFFU;

   /* Set wanted IRQ only (GPIO3) */
   NVIC_EnableIRQ(GPIO3_IRQn);

   /* Configure GPIO8 as standby clock */
   SYS_GPIO_CONFIG(8, GPIO_2X_DRIVE | GPIO_LPF_DISABLE | GPIO_NO_PULL | NS_CANNOT_USE_GPIO | GPIO_MODE_STANDBYCLK);

   /* Configure GPIO3 interrupt line to rising edge of GPIO8(standby clock) */
   Sys_GPIO_IntConfig(3, NS_CANNOT_ACCESS_GPIO_INT | GPIO_DEBOUNCE_DISABLE | GPIO_EVENT_NONE | GPIO_SRC_GPIO_8,
		   GPIO_DEBOUNCE_SLOWCLK_DIV32, 0);
   Sys_GPIO_IntConfig(3, NS_CANNOT_ACCESS_GPIO_INT | GPIO_DEBOUNCE_DISABLE | GPIO_EVENT_RISING_EDGE | GPIO_SRC_GPIO_8,
   		   GPIO_DEBOUNCE_SLOWCLK_DIV32, 0);

   /* Wait for rising edge of RTC_CLOCK */
   __WFI();

   /* Clear the pending GPIO3 IRQ */
   NVIC_ClearPendingIRQ(GPIO3_IRQn);

   /* Configure RTC timer counter with timeout cycles */
   ACS->RTC_CFG = timer_counter - 1;

   /* Configure GPIO3 interrupt line to falling edge of GPIO8(standby clock) */
   Sys_GPIO_IntConfig(3, NS_CANNOT_ACCESS_GPIO_INT | GPIO_DEBOUNCE_DISABLE | GPIO_EVENT_FALLING_EDGE | GPIO_SRC_GPIO_8,
		   GPIO_DEBOUNCE_SLOWCLK_DIV32, 0);

   /* Wait for falling edge of RTC_CLOCK */
   __WFI();

   /* Reset RTC to load new timer counter */
   ACS->RTC_CTRL = (ACS->RTC_CTRL & ~ACS_RTC_CTRL_ALARM_CFG_Mask) | RTC_ALARM_DISABLE;
   ACS->RTC_CTRL |= RTC_RESET;
   ACS->RTC_CTRL |= RTC_FORCE_CLOCK;
   ACS->RTC_CTRL = (ACS->RTC_CTRL & ~ACS_RTC_CTRL_ALARM_CFG_Mask) | RTC_ALARM_ZERO;

   /* Clear sticky wakeup RTC alarm flag */
   WAKEUP_RTC_ALARM_FLAG_CLEAR();

   /* Set RTC preload timer counter to DEADBEEF for next wake up */
   ACS->RTC_CFG = 0xDEADBEEF;

   /* Clear the pending GPIO3_IRQ */
   NVIC_ClearPendingIRQ(GPIO3_IRQn);

   /* Disable GPIO3 interrupt line */
   Sys_GPIO_IntConfig(3, NS_CANNOT_ACCESS_GPIO_INT | GPIO_DEBOUNCE_DISABLE | GPIO_EVENT_NONE,
   		   GPIO_DEBOUNCE_SLOWCLK_DIV32, 0);

   /* Reset GPIO8 */
   SYS_GPIO_CONFIG(8, GPIO_2X_DRIVE | GPIO_LPF_DISABLE | GPIO_WEAK_PULL_UP | NS_CANNOT_USE_GPIO | GPIO_MODE_DISABLE);

   /* Restore NVIC set enable register */
   NVIC->ISER[0] = nvic_set_enable[0];
   NVIC->ISER[1] = nvic_set_enable[1];

   /* Unmask interrupt */
   __enable_irq();
}

/* Configure BB Timer */
void BB_Timer_Init()
{
    /* Enable the baseband controller clocks generation
     * Configure the internal baseband controller clock divider */
    BBIF->CTRL = BB_CLK_ENABLE | BBCLK_DIVIDER_8;

    /* Set BB timer prescaler, and set not reset bit */
    ACS->BB_TIMER_CTRL = BB_CLK_PRESCALE_1 | BB_TIMER_RESET;

    /* Reset the complete system at 0 */
    BB->RWBBCNTL = MASTER_SOFT_RST_1 | MASTER_TGSOFT_RST_1 | RADIOCNTL_SOFT_RST_1;

    /* Do not change RTC settings, if already enabled*/
    if(!((WAKEUP_SRC_FLAG_BIT_SET << WAKEUP_SRC_RTC_ALARM) & WAKEUP_SRC_EN_MSK))
    {
    	/* Disable RTC */
    	ACS->RTC_CTRL = RTC_DISABLE;

    	/* Reset RTC */
    	ACS->RTC_CTRL = RTC_RESET;

    	/* Enable RTC with XTAL32k as clk source */
    	ACS->RTC_CTRL = RTC_ENABLE | RTC_CLK_SRC_XTAL32K;
    }

    /* Set BB timer not reset bit */
    ACS->BB_TIMER_CTRL = BB_TIMER_NRESET;

    /* Wait a few low power clocks to have BB timer reset and ready */
    Sys_Delay(SystemCoreClock / 10000);

    /* Deep sleep time (number of low power clock cycles)
     * Notes:
     *   - Low power clock frequency = 32768 Hz
     *   - 0xFFFF => 2* 32768 => 2s */
    BB->DEEPSLWKUP = BB_DEEP_SLEEP_TIME;

    /* Define the time in low power clock cycles allowed for stabilization */
    BB->ENBPRESET = (TWOSC_VALUE << BB_ENBPRESET_TWOSC_Pos) | (TWRM_VALUE << BB_ENBPRESET_TWRM_Pos);

    /* Configure deep sleep control register
     * Allow to disable high frequency crystal oscillator
     * Request RW-BLE core to switch in deep sleep mode
     * Allow to disable radio */
    BB->DEEPSLCNTL = OSC_SLEEP_EN_1 | DEEP_SLEEP_ON_1 | RADIO_SLEEP_EN_1;

    /* Wait until the status bits OSC_EN and RADIO_EN of the BBIF_STATUS
     * register are reset, so indicating that the baseband low power timer is
     * in deep sleep mode and properly isolated.*/
    while ((BBIF->STATUS & OSC_ENABLED) != OSC_DISABLED);
    while ((BBIF->STATUS & RF_ENABLED) != RF_DISABLED);

    /* Delay for timer NRESET to be effective */
    while ((BBIF->STATUS & LOW_POWER_CLK) != LOW_POWER_CLK);

    NVIC_EnableIRQ(BLE_SLP_IRQn);
}

/* Configure GPIO input */
void GPIO_Wakeup_Init(void)
{
    SYS_GPIO_CONFIG(GPIO_WAKEUP_PIN, (GPIO_MODE_DISABLE | GPIO_LPF_DISABLE |
                                      GPIO_WEAK_PULL_UP  | GPIO_6X_DRIVE));
}

/* Configure and enable sensor interface and FIFO wake up source */
void Sensor_Init(void)
{

    uint32_t sensor_if_cfg = SENSOR_ENABLED |
    		                 SENSOR_RE_VSSA |
#if SENSOR_CALIB == 1
                          SENSOR_CALIB_ENABLED |
#else
                          SENSOR_CALIB_DISABLED |
#endif
#if SENSOR_NORMAL == 1
						  SENSOR_IOFFSET_40NA |
                          SENSOR_IRANGE_80NA |
#elif SENSOR_IMPEDANCE == 1
						  SENSOR_IOFFSET_40NA |
                          SENSOR_IRANGE_240NA |
#endif
                          SENSOR_AMP_ENABLED |
						  SENSOR_GUARD_ENABLED;

    uint32_t sensor_timer_ctl = SENSOR_TIMER_ENABLED |
        					    WAITSLEEP_ENABLED |
#if SENSOR_NORMAL == 1
								DLY1_WE_L_NOT_USED |
								PULSE_CNT_NOT_USED |
#elif SENSOR_IMPEDANCE == 1
								DLY1_WE_L_USED |
								PULSE_CNT_USED |
#endif
			                    DLY2_WE_L_NOT_USED |
			                    DLY1_WE_H_NOT_USED |
			                    DLY2_WE_H_NOT_USED |
								IDLE_USED;

    /* Configure and enable clock source for RTC */
    RTC_ClockSource_Init();

    /* Configure RTC */
    ACS->RTC_CTRL = RTC_DISABLE | RTC_RESET | RTC_CLK_SRC | RTC_ALARM_DISABLE;

    /* Disable the VDDIF regulator */
    ACS->VDDIF_CTRL &= ~VDDIF_ENABLE;

    /* Enable the sensor clock */
    CLK->DIV_CFG1 = SENSOR_CLK_ENABLE;

#if SENSOR_NORMAL == 1
    Sys_Sensor_ADCConfig(sensor_if_cfg, SENSOR_WEDAC_HIGH_0600, SENSOR_WEDAC_LOW_0600,
                         SENSOR_CLK_SRC);
#elif SENSOR_IMPEDANCE == 1
    Sys_Sensor_ADCConfig(sensor_if_cfg, SENSOR_WEDAC_HIGH_0616, SENSOR_WEDAC_LOW_0600,
                         SENSOR_CLK_SRC);
#endif
    /* Reset the sensor timer */
    Sys_Sensor_TimerReset();

    /* Configure sensor timer and specify which state(s) will be used */
#if SENSOR_WUT == 1
    Sys_Sensor_TimerConfig(sensor_timer_ctl, RE_DISCONNECTED_BYTE);
#else
    Sys_Sensor_TimerConfig(sensor_timer_ctl, RE_CONNECTED_BYTE);
#endif

    /* Configure length of pre integration state */
#if SENSOR_NORMAL == 1
    SENSOR->INT_CFG = PRE_COUNT_INT_VALUE_2S;
#elif SENSOR_IMPEDANCE == 1
    SENSOR->INT_CFG = PRE_COUNT_INT_VALUE_3P9MS;
#endif

    /* Configure length of delay state */
#if SENSOR_NORMAL == 1
    SENSOR_DELAY_L_CFG->DLY1_WE_L_SHORT = DLY1_WE_L_DIV_ENABLED_SHORT | DLY1_WE_L_VALUE_1S;
#elif SENSOR_IMPEDANCE == 1
    SENSOR_DELAY_L_CFG->DLY1_WE_L_SHORT = DLY1_WE_L_DIV_ENABLED_SHORT | DLY1_WE_L_VALUE_117P19MS;
#endif

    /* Configure length of idle state */
#if SENSOR_NORMAL == 1
    Sys_Sensor_IdleConfig(IDLE_TIME_VALUE_27S);
#elif SENSOR_IMPEDANCE == 1
    Sys_Sensor_IdleConfig(IDLE_TIME_VALUE_29P875S);
#endif

    /* Read position 0 of ADC data to reset the FIFO */
    SensorFIFO_Reset();

    /* Configure sample storage FIFO
     * Set ADC threshold and number of samples */
    Sys_Sensor_StorageConfig(diff_mode, SENSOR_SUMMATION_DISABLED,
                             number_of_samples, adc_threshold,
                             SENSOR_FIFO_STORE_ENABLED, fifo_size);
}

/* Configure NFC Interrupts */
void NFC_Init(void)
{
    /* Type A config tables in RAM */
    static uint8_t Layer3Source[HF_IO_RAM_INIT_ISOALAYER3];

    /* Enable IRQ interrupts and set priority mask */
    __enable_irq();
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);

    /* Configure NFC peripheral */
    SYSCTRL->NFC_CFG = NFC_EN;
    _isohf_setProtocolWP(HFCTRL_IP, 0xFF00);
    _isohf_setReboundFilterCfg(HFCTRL_IP, 0x7);
    _isohf_setDigitalCnt2Cfg(HFCTRL_IP, 0x0);

    /* Use RC clock multiplied */
    ACS->RCOSC_CTRL = RC_OSC_12MHZ | RC_OSC_P46P5;

    /* Enable the clock extractor of the analog front end.
     * Needed in order for the HF_OK signal to be updated. */
    NFC->HFCTRL_TEST_CTRL = CLK_EXTRACTOR_ENABLED;

    /* Clear sticky wake up NFC FIELD flag */
    WAKEUP_NFC_FIELD_FLAG_CLEAR();

    /* Enable the wake up on NFC field.
     * Set the level of the NFC field to wake up the SOC.
     * Enable wake up using the HF clock frequency. */
    ACS->WAKEUP_CFG = WAKEUP_DELAY_16 | WAKEUP_NFC_FIELD_ENABLE | NFC_FIELD_0 | WAKEUP_USE_HF_OK;

    /* Enable interrupts for NFC*/
    _isohf_enableEndOfComIt(HFCTRL_IP);

    /* Setup anti collision response */
    Layer3Source[0] = 0x44;             /* ATQA first byte */
    Layer3Source[1] = 0x00;             /* ATQA second byte */
    Layer3Source[2] = RAW_ARRAY[0];     /* UID0 */
    Layer3Source[3] = RAW_ARRAY[1];     /* UID1 */
    Layer3Source[4] = RAW_ARRAY[2];     /* UID2 */
    Layer3Source[5] = RAW_ARRAY[3];     /* UID3 */
    Layer3Source[6] = RAW_ARRAY[4];     /* UID4 */
    Layer3Source[7] = RAW_ARRAY[5];     /* UID5 */
    Layer3Source[8] = RAW_ARRAY[6];     /* UID6 */
    Layer3Source[9] = RAW_ARRAY[7];     /* UID7 */
    Layer3Source[10] = RAW_ARRAY[8];    /* UID8 */
    Layer3Source[11] = RAW_ARRAY[9];    /* UID9 */
    Layer3Source[12] = 0x0F;            /* SAK NOT COMP */
    Layer3Source[13] = 0x00;            /* SAK OK (This byte indicates if compatible or not with ISO 14443-4, in this
                                         * case
                                         * not compatible) */

    /* Waiting for the end of the boot if the boot triggers the HF */
    _isohf_setProtocolUID(HFCTRL_IP, 1);

    /* IO RAM configuration for Layer 3 */
    _isohf_configTypeALayer3BootAndWait_local(HFCTRL_IP, &Layer3Source[0]);
}

/* Configure ADC Threshold Interrupts */
void ADC_Threshold_Init(void)
{
    /* Configure sensor interface */
    Sensor_Init();

    /* Clear sticky wake up THRESHOLD FULL flag */
    WAKEUP_THRESHOLD_FULL_FLAG_CLEAR();
}

/* Configure ADC FIFO Interrupts */
void ADC_FIFO_Init(void)
{
    /* Configure sensor interface */
    Sensor_Init();

    /* Clear sticky wake up FIFO FULL flag */
    WAKEUP_FIFO_FULL_FLAG_CLEAR();

    /* Disable ADC threshold */
    SENSOR->PROCESSING = SENSOR_THRESHOLD_DISABLED;
	
    /* Enable wakeup on FIFO FULL */
    ACS->WAKEUP_CFG |= WAKEUP_FIFO_ENABLE;
}

void SENSOR_DET_Init(void)
{
    /* Configure the Sensor Detector */
    /* Sensor detector is disabled and held in reset */
    ACS->SENSOR_DET_CFG = SENSOR_DET_RESET | SENSOR_DET_DISABLED;

    /* Sensor detector is enabled */
    ACS->SENSOR_DET_CFG = SENSOR_DET_RESET | SENSOR_DET_ENABLED;

    /*  Wait some number of cycles for WE pin voltage to stabilize  */
    for (uint32_t i = 0; i < 100; i++)
    {
        __NOP();
    }

    /* Sensor detector is enabled and not in reset */
    ACS->SENSOR_DET_CFG = SENSOR_DET_NOT_RESET | SENSOR_DET_ENABLED;

    /* Clear sticky wake up SENSOR DETECT flag */
    WAKEUP_SENSOR_DETECT_FLAG_CLEAR();
}

/* Reset FIFO */
void SensorFIFO_Reset(void)
{
    reset_fifo_level = SENSOR->ADC_DATA[0];
}
