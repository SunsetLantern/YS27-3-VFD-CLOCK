
#ifndef __IIC_SOFT_H
#define	__IIC_SOFT_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

void SoftIIC_Init( GPIO_TypeDef *SDA_Port, uint32_t SDA_Pin, GPIO_TypeDef *SCL_Port, uint32_t SCL_Pin );
void SoftIIC_Start(void);
void SoftIIC_Stop(void);

void SoftIIC_SendByte(uint8_t Byte);
uint8_t SoftIIC_ReceiveByte(void);

void SoftIIC_SendAck(uint8_t AckBit);
uint8_t SoftIIC_ReceiveAck(void);


#endif
