/* Midddle */
#include "ds3231.h"
#include "main.h"
#include "iic_soft.h"
#include "typedef.h"

static uint8_t IIC_DS3231_ByteWrite(uint8_t WriteAddr , uint8_t date)
{
	SoftIIC_Start();
	SoftIIC_SendByte(DS3231_ADDRESS_Write);
	if(SoftIIC_ReceiveAck())
		return 1;
	SoftIIC_SendByte(WriteAddr);
	if(SoftIIC_ReceiveAck())
		return 2;
	SoftIIC_SendByte(date);
	if(SoftIIC_ReceiveAck())
		return 3;
	SoftIIC_Stop();
	return 0;
}

static uint8_t IIC_DS3231_ByteRead( uint8_t ReadAddr,uint8_t* Receive )
{
	uint8_t data = 0;
	
	SoftIIC_Start();													//产生起始位
	SoftIIC_SendByte(DS3231_ADDRESS_Write); 	//发送从机地址（写模式）
	if(SoftIIC_ReceiveAck())										//等待响应
		return 1;
	
	SoftIIC_SendByte(ReadAddr);							//发送寄存器地址
	if(SoftIIC_ReceiveAck())										//等待响应
		return 2;
	
	SoftIIC_Start();													//重复起始位
	SoftIIC_SendByte(DS3231_ADDRESS_Read);		//发送从机地址（读模式）
	if(SoftIIC_ReceiveAck())										//等待响应
		return 3;
	
	data = SoftIIC_ReceiveByte();							//读取数据，参数设为0 --- NACK
	*Receive = data;											//将结果赋值给接收位
	SoftIIC_Stop();
	return 0;
}


void DS3231_Init( void )
{
	/* Soft IIC Initialize */
	SoftIIC_Init(SDA_3231_GPIO_Port,SDA_3231_Pin,SCL_3231_GPIO_Port,SCL_3231_Pin);
	/* INT ENABLE */
	uint8_t temp = 0;
	IIC_DS3231_ByteRead( DS3231_CONTROL_REG, &temp );
	temp &= ~BIT2;
	IIC_DS3231_ByteWrite( DS3231_CONTROL_REG, temp );
}

uint8_t DS3231_setDate(uint8_t year,uint8_t mon,uint8_t day)
{
	uint8_t temp_H , temp_L;
	temp_L = year%10;
	temp_H = year/10;
	year = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_YEAR_REG,year)) //set year
	{
		return 1;
	}	
	temp_L = mon%10;
	temp_H = mon/10;
	mon = (temp_H << 4) + temp_L;	
	if(IIC_DS3231_ByteWrite(DS3231_MONTH_REG,mon)) //set mon
	{
		return 2;
	}
	temp_L = day%10;
	temp_H = day/10;
	day = (temp_H << 4) + temp_L;		
	if(IIC_DS3231_ByteWrite(DS3231_MDAY_REG,day)) //set day
	{
		return 3;
	}
	return 0;
}


uint8_t DS3231_setTime(uint8_t hour , uint8_t min , uint8_t sec)
{
	uint8_t temp_H , temp_L;
	temp_L = hour%10;
	temp_H = hour/10;
	hour = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_HOUR_REG,hour)) //set hour
		return 1;
	temp_L = min%10;
	temp_H = min/10;
	min = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_MIN_REG,min)) //SET min
		return 2;	
	temp_L = sec%10;
	temp_H = sec/10;
	sec = (temp_H << 4) + temp_L;	
	if(IIC_DS3231_ByteWrite(DS3231_SEC_REG,sec))		//SET sec
		return 3;
	return 0;
}

static uint8_t bcdToDec(uint8_t byte)
{
	uint8_t temp_H , temp_L;
	temp_L = byte & 0x0f;
	temp_H = (byte & 0xf0) >> 4;
	return ( temp_H * 10 )+ temp_L;
}

uint8_t DS3231_gettime(DateTime* ans)
{
	uint8_t receive = 0;
	if(IIC_DS3231_ByteRead(DS3231_HOUR_REG,&receive))
		return 1;
	ans->hour = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_MIN_REG,&receive))
		return 2;
	ans->minute = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_SEC_REG,&receive))
		return 3;
	ans->second = bcdToDec(receive);
	return 0;
}

uint8_t DS3231_getdate(DateTime* ans)
{
	uint8_t receive = 0;
	if(IIC_DS3231_ByteRead(DS3231_YEAR_REG,&receive))
		return 1;
	ans->year = bcdToDec(receive) + 2000;
	if(IIC_DS3231_ByteRead(DS3231_MONTH_REG,&receive))
		return 2;
	ans->month = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_MDAY_REG,&receive))
		return 3;
	ans->dayofmonth = bcdToDec(receive);
	return 0;
}
