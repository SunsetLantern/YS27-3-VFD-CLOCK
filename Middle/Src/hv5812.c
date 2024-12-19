
/* Middle */
#include "hv5812.h"
/* Core */
#include "spi.h"
/* HAL */
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_gpio.h"
/* CMSIS */
#include "stdint.h"
/* C */
#include "string.h"

static uint8_t dataTransBuffer[4] = { 0 }; 

#if 1
	#define			USE_HARD_SPI
#endif

void HV5812_Init( void )
{
#ifdef	USE_HARD_SPI
	/* Use Hardware Spi */
	MX_SPI1_Init();
	memset( dataTransBuffer, 0, sizeof( dataTransBuffer ) );
	HV5812_Write( HV5812_GRID_ALL | 0 );
	HV5812_Refresh();
	HAL_Delay(1);
	
#else
	/* Use Software Spi */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = CLK_5812_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(CLK_5812_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA_5812_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DATA_5812_GPIO_Port, &GPIO_InitStruct);
#endif
	
	HAL_GPIO_WritePin( BLANKING_5812_GPIO_Port, BLANKING_5812_Pin, GPIO_PIN_RESET );
	HAL_GPIO_WritePin( STROBE_5812_GPIO_Port, STROBE_5812_Pin, GPIO_PIN_RESET );	
}

void HV5812_Refresh( void )
{
	HAL_GPIO_WritePin( STROBE_5812_GPIO_Port, STROBE_5812_Pin,GPIO_PIN_SET );
	__nop();
	HAL_GPIO_WritePin( STROBE_5812_GPIO_Port, STROBE_5812_Pin,GPIO_PIN_RESET );	
}

void HV5812_Write( uint32_t data )
{
	
#ifdef	USE_HARD_SPI
	/* Use Hardware Spi */
	dataTransBuffer[0] = (data >> 24) & 0xff;	
	dataTransBuffer[1] = (data >> 16) & 0xff;
	dataTransBuffer[2] = (data >> 8) & 0xff;	
	dataTransBuffer[3] = (data >> 0) & 0xff;

	HAL_SPI_Transmit( &hspi1, dataTransBuffer, 4 , 100);
#else
	/* Use Software Spi */
	for( uint8_t i = 0; i < 32; i++, data >>= 1 )
	{
		if( data & 0x01 )
			HAL_GPIO_WritePin( DATA_5812_GPIO_Port, DATA_5812_Pin,GPIO_PIN_SET );	
		else
			HAL_GPIO_WritePin( DATA_5812_GPIO_Port, DATA_5812_Pin,GPIO_PIN_RESET );		

		__nop();
		__nop();
		HAL_GPIO_WritePin( CLK_5812_GPIO_Port, CLK_5812_Pin,GPIO_PIN_SET );		 
		__nop();
		__nop();
		HAL_GPIO_WritePin( CLK_5812_GPIO_Port, CLK_5812_Pin,GPIO_PIN_RESET );		
	}
#endif
}
