
#ifndef __MISC_H
#define	__MISC_H

#include "stdint.h"
#include "ds3231.h"

extern DateTime Ds3231Time;

void Misc_Init( void );
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif