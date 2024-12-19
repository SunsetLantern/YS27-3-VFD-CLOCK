
#include "misc.h"
#include "tim.h"


DateTime Ds3231Time;


void Misc_Init( void )
{
	/* BUZZER */
	MX_TIM1_Init();
	HAL_TIM_MspPostInit(&htim1);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	/* LED */
	
	/* KEY */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == INT_3231_Pin )
	{
		DS3231_gettime( &Ds3231Time );
	}
	else
	{
		__nop();
	}
}

