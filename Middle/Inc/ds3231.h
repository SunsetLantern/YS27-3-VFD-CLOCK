/**
	* @brief
	* @Author Lny
	* @Date
*/

#ifndef __DS3231_H
#define	__DS3231_H

/* CMSIS */
#include "stdint.h"

#define DS3231_ADDRESS					0x68
#define	DS3231_ADDRESS_Write			0xD0
#define	DS3231_ADDRESS_Read				0xD1

#define DS3231_SEC_REG					0x00
#define DS3231_MIN_REG					0x01
#define DS3231_HOUR_REG					0x02
#define DS3231_WDAY_REG					0x03
#define DS3231_MDAY_REG					0x04
#define DS3231_MONTH_REG				0x05
#define DS3231_YEAR_REG					0x06

#define DS3231_AL1SEC_REG				0x07
#define DS3231_AL1MIN_REG				0x08
#define DS3231_AL1HOUR_REG				0x09
#define DS3231_AL1WDAY_REG				0x0A

#define DS3231_AL2MIN_REG				0x0B
#define DS3231_AL2HOUR_REG				0x0C
#define DS3231_AL2WDAY_REG				0x0D

#define DS3231_CONTROL_REG				0x0E
#define DS3231_STATUS_REG				0x0F
#define DS3231_AGING_OFFSET_REG			0x0F
#define DS3231_TMP_UP_REG				0x11
#define DS3231_TMP_LOW_REG				0x12

#define EverySecond						0x01
#define EveryMinute						0x02
#define EveryHour						0x03

typedef struct DateTImeStruct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayofmonth;
	uint8_t month;
	uint16_t year;
	uint8_t dayOfWeek;  /*Su=0 Mo=1 Tu=3 We=4 Th=5 Fr=6 Sa=7 */
}DateTime;


void DS3231_Init( void );
uint8_t DS3231_setDate(uint8_t year,uint8_t mon,uint8_t day);
uint8_t DS3231_setTime(uint8_t hour , uint8_t min , uint8_t sec);
uint8_t DS3231_getdate(DateTime* ans);
uint8_t DS3231_gettime(DateTime* ans);

#endif
