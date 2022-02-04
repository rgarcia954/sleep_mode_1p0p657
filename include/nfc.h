/**
 * @file nfc.h
 * @brief NFC support
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
/* ---------------------------------------------------------------------------
* Function prototype definitions
* --------------------------------------------------------------------------*/
uint32_t _LLHW_isohf_compareIORAM2Mem_local(HFCTRL isohf,  uint8_t *pComp, uint32_t byte_size);

void _LLHW_isohf_copyMem2IORAM_local(HFCTRL isohf, uint8_t *pSource, uint8_t offset, uint32_t bytesLength);

void _LLHW_isohf_configIORAM4TypeALayer3_local(HFCTRL isohf, uint8_t *Layer3Source);

void _isohf_configTypeALayer3BootAndWait_local(HFCTRL isohf, uint8_t *Layer3Source);

uint8_t read_block(uint8_t *ptr_resp, uint8_t *ptr_data);
