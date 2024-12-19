/**
	* @file       iic_soft.c
	* @brief      Soft IIC Driver
	* @author     lny
	* @date       2024/05/28
	* @version    1.0
*/

#include "iic_soft.h"

uint8_t fac_us = 0;

void HAL_Delay_us_init(uint8_t SYSCLK)
{
     fac_us=SYSCLK; 
}
 
void HAL_Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;
    ticks=nus*fac_us; 
    told=SysTick->VAL; 
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break; 
        }
    };
}


/** @brief Config for iic_soft.c */
static struct
{
	GPIO_TypeDef *SDA_Port;
	uint32_t SDA_Pin;
	GPIO_TypeDef *SCL_Port;
	uint32_t SCL_Pin;
}SoftIIC_Config;

/**
	* @Brief Soft IIC Initialize
	* @Param[in] GPIO_TypeDef *SDA_Port
	* @Param[in] uint32_t SDA_Pin
	* @Param[in] GPIO_TypeDef *SCL_Port
	* @Param[in] uint32_t SCL_Pin
	* @Return void

*/
void SoftIIC_Init( GPIO_TypeDef *SDA_Port, uint32_t SDA_Pin, GPIO_TypeDef *SCL_Port, uint32_t SCL_Pin )
{
	/* RCC */
	if( (SDA_Port == GPIOA) || (SCL_Port == GPIOA) )
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	if( (SDA_Port == GPIOB) || (SCL_Port == GPIOB) )
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	if( (SDA_Port == GPIOC) || (SCL_Port == GPIOC) )
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	if( (SDA_Port == GPIOD) || (SCL_Port == GPIOD) )
		__HAL_RCC_GPIOA_CLK_ENABLE();	

	/* SDA Init */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pin = SDA_Pin;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( SDA_Port, &GPIO_InitStructure );
	HAL_GPIO_WritePin( SDA_Port, SDA_Pin, GPIO_PIN_SET);

	/* SCL Init */
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pin = SCL_Pin;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( SCL_Port, &GPIO_InitStructure );	
	HAL_GPIO_WritePin( SCL_Port, SCL_Pin, GPIO_PIN_SET);

	/* Static Param */
	SoftIIC_Config.SDA_Port = SDA_Port;
	SoftIIC_Config.SCL_Port = SCL_Port;
	SoftIIC_Config.SDA_Pin = SDA_Pin;
	SoftIIC_Config.SCL_Pin = SCL_Pin;	
}

void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin( SoftIIC_Config.SCL_Port, SoftIIC_Config.SCL_Pin, BitValue );
	HAL_Delay_us(10);
}


void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin( SoftIIC_Config.SDA_Port, SoftIIC_Config.SDA_Pin, BitValue );
	HAL_Delay_us(10);
}
 
//读与写不是一个寄存器
//读数据
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin( SoftIIC_Config.SDA_Port, SoftIIC_Config.SDA_Pin );
	HAL_Delay_us(10);
	return BitValue;
}
 

 
//开始
void SoftIIC_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
 
//结束
void SoftIIC_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
 
//发送单元
void SoftIIC_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
		
	}
}
 
//接收单元
uint8_t SoftIIC_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	//MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	for (i = 0; i< 8;i++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80>>i);
		}
		MyI2C_W_SCL(0);
	}
	return Byte;
	
}
 
//发送应答
void SoftIIC_SendAck(uint8_t AckBit)
{
	
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
		
}

//接收应答
uint8_t SoftIIC_ReceiveAck(void)
{
	uint8_t AckBit;
	//主机释放SDA
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
	
}