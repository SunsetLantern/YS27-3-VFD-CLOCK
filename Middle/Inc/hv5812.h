

#ifndef __HV5812_H
#define	__HV5812_H
/* Core */
#include "typedef.h"
/* CMSIS */
#include "stdint.h"


/* GRID Define */
#define			HV5812_GRID1			(BIT18)
#define			HV5812_GRID2			(BIT17)
#define			HV5812_GRID3			(BIT16)
#define			HV5812_GRID4			(BIT15)
#define			HV5812_GRID5			(BIT13)
#define			HV5812_GRID6			(BIT12)
#define			HV5812_GRID7			(BIT11)
#define			HV5812_GRID8			(BIT10)

#define			HV5812_GRID_ALL			(HV5812_GRID1|HV5812_GRID2|HV5812_GRID3|HV5812_GRID4|HV5812_GRID5|HV5812_GRID6|HV5812_GRID7|HV5812_GRID8)
#define			HV5812_GRID_ALL_NUM		(HV5812_GRID1|HV5812_GRID2|HV5812_GRID3|HV5812_GRID4|HV5812_GRID5|HV5812_GRID6|HV5812_GRID7)
#define			HV5812_GRID_SYMBOL		(HV5812_GRID8)

/* SEG Define */
//             SEG5
//            ______
//           |      |
//      SEG6 | SEG7 |  SEG4
//            ------ - SEG3
//           |      |
//      SEG8 |      |  SEG2
//            ------ . SEG1
//             SEG9

#define			HV5812_SEG1				(BIT3)
#define			HV5812_SEG2				(BIT2)
#define			HV5812_SEG3				(BIT1)
#define			HV5812_SEG4				(BIT0)
#define			HV5812_SEG5				(BIT9)
#define			HV5812_SEG6				(BIT8)
#define			HV5812_SEG7				(BIT7)
#define			HV5812_SEG8				(BIT6)
#define			HV5812_SEG9				(BIT5)


void HV5812_Init( void );

void HV5812_Refresh( void );

void HV5812_Write( uint32_t data );


#endif
