/*
 * nfc.c
 *
 *  Created on: Oct 13, 2017
 *      Author: fg76yr
 */

#include "app.h"
#include "iso14443.h"
#include "nfc.h"

extern uint8_t RAW_ARRAY[64];
static inline uint8_t _isohf_getHFIORAMbyte_local(uint8_t offset)
{
    return ((uint8_t *)(HF_IO_RAM_START_ADD))[offset];
}

static inline void _isohf_setHFIORAMbyte_local(uint8_t offset, uint8_t data)
{
    ((uint8_t *)(HF_IO_RAM_START_ADD))[offset] = data;
}

uint32_t _LLHW_isohf_compareIORAM2Mem_local(HFCTRL isohf,  uint8_t *pComp, uint32_t byte_size)
{
    uint32_t i;

    for (i = 0; i < byte_size; i++)
    {
        if (pComp[i] != _isohf_getHFIORAMbyte_local(i))
        {
            return -1;
        }
    }
    return 0;
}

void _LLHW_isohf_copyMem2IORAM_local(HFCTRL isohf, uint8_t *pSource, uint8_t offset, uint32_t bytesLength)
{
    uint32_t i;

    for (i = 0; i < bytesLength; i++)
    {
        _isohf_setHFIORAMbyte_local(offset + i, (uint8_t)pSource[i]);
    }
}

void _LLHW_isohf_configIORAM4TypeALayer3_local(HFCTRL isohf, uint8_t *Layer3Source)
{
    _LLHW_isohf_copyMem2IORAM_local(isohf,
                                    Layer3Source,
                                    (uint8_t)(HF_IO_RAM_EMPTY_OFFSET >> 2),
                                    HF_IO_RAM_INIT_ISOALAYER3);
}

void _isohf_configTypeALayer3BootAndWait_local(HFCTRL isohf, uint8_t *Layer3Source)
{
    /* Fill the RAM for Layer 3 */
    _LLHW_isohf_configIORAM4TypeALayer3_local(isohf, Layer3Source);

    /* Wait */
    _LLHW_isohf_waitForRx(isohf, 0x0);
}

/* read memory and copy data to array used to send the response */
uint8_t read_block(uint8_t *ptr_resp, uint8_t *ptr_data)
{
    uint16_t start_index = 0;
    uint16_t end_index = 0;
    uint8_t i, j = 0;
    start_index = ptr_data[1] * 4;            /* offset from command */
    end_index = start_index + 16;           /* offset + 16bytes = end index */
    for (i = start_index; i < end_index; i++)    /* copy all data from start index to end_index */
    {
        ptr_resp[j++] = RAW_ARRAY[i];
    }
    return j;
}
