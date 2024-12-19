
#ifndef VFD_SEG_H
#define	VFD_SEG_H

/* CMSIS */
#include "stdint.h"

static const uint32_t VFD_SEG_ALL = 0x3de00;
static const uint32_t VFD_SELECT[8] = { 0x00002, 0x00004, 0x00008, 0x00010, 0x00040, 0x00080, 0x00100, 0x00200 };
static const uint32_t VFD_SEG_LINE = ( 0x1u << 18 );
static const uint32_t VFD_SEG_POINT = ( 0x1u << 16 );
static const uint32_t VFD_SEG_NUM[10] = { 0xA6C00, 0xA0000, 0x87400, 0xA5400, 0xA1800, 0x25C00, 0x27C00, 0xA0400, 0xA7C00, 0xA5C00 };

#endif
