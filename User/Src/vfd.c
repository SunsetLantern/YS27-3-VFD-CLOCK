
#include "vfd.h"
/* User */
#include "vfd_seg.h"
/* Midddle */
#include "hv5812.h"
#include "ds3231.h"
#include "misc.h"
/* Core */
#include "typedef.h"
/* hal */
#include "stm32f1xx_hal.h"


static const uint32_t VFD_NUM[12] = { 
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG5|HV5812_SEG6|HV5812_SEG8|HV5812_SEG9,				// 0
	HV5812_SEG2|HV5812_SEG4,																// 1
	HV5812_SEG4|HV5812_SEG5|HV5812_SEG7|HV5812_SEG8|HV5812_SEG9,							// 2
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG5|HV5812_SEG7|HV5812_SEG9,							// 3
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG6|HV5812_SEG7,										// 4
	HV5812_SEG2|HV5812_SEG5|HV5812_SEG6|HV5812_SEG7|HV5812_SEG9,							// 5
	HV5812_SEG2|HV5812_SEG5|HV5812_SEG6|HV5812_SEG7|HV5812_SEG8|HV5812_SEG9,				// 6
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG5,													// 7
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG5|HV5812_SEG6|HV5812_SEG7|HV5812_SEG8|HV5812_SEG9,	// 8	
	HV5812_SEG2|HV5812_SEG4|HV5812_SEG5|HV5812_SEG6|HV5812_SEG7|HV5812_SEG9,				// 9
	HV5812_SEG3,																			// -
	HV5812_SEG1																				// .				
};

static const uint32_t VFD_SYMBOL[4] = {
	HV5812_SEG2, 																			// s
	HV5812_SEG4, 																			// Hz
	HV5812_SEG7, 																			// K
	HV5812_SEG8																				// m
};


DateTime a;

void Vfd_Init( void )
{
	HV5812_Init();
#if 0
	for(uint8_t i = 0; i < 12; i++)
	{
		HV5812_Write( HV5812_GRID_ALL_NUM|VFD_NUM[i] );
		HV5812_Refresh();
		HAL_Delay(50);
	}
	for(uint8_t i = 0; i < 4; i++)
	{
		HV5812_Write( HV5812_GRID_SYMBOL|VFD_SYMBOL[i] );
		HV5812_Refresh();
		HAL_Delay(50);
	}	
	HV5812_Write( HV5812_GRID_ALL );
	HV5812_Refresh();
#endif
}

#define	VFD_DELAY_US	1
void Vfd_Clock_Screen( void )
{
	HV5812_Write( HV5812_GRID1 | VFD_NUM[Ds3231Time.hour / 10] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);
	
	HV5812_Write( HV5812_GRID2 | VFD_NUM[Ds3231Time.hour % 10] | VFD_NUM[11] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
	
	HV5812_Write( HV5812_GRID3 | VFD_NUM[Ds3231Time.minute / 10] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
	
	HV5812_Write( HV5812_GRID4 | VFD_NUM[Ds3231Time.minute % 10] | VFD_NUM[11] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
	
	HV5812_Write( HV5812_GRID5 | VFD_NUM[Ds3231Time.second / 10] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
	
	HV5812_Write( HV5812_GRID6 | VFD_NUM[Ds3231Time.second % 10] | VFD_NUM[11] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
	
	HV5812_Write( HV5812_GRID_SYMBOL|VFD_SYMBOL[0] );
	HV5812_Refresh();
	HAL_Delay(VFD_DELAY_US);	
}
