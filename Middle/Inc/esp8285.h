#ifndef __ESP_8285_H
#define	__ESP_8285_H

#include "stdint.h"
#include "string.h"

#define				SSID 							"CMCC-18601"
#define				PASSWORD						"dzhy18601"

/* Error Code */
#define			ESP8285_OK							( 0x00 )
#define			ESP8285_FAIL			( 0x01 )
#define			ESP8285_ERROR_DMA_TRANSMIT_BUSY		( 0x02 )

typedef struct{
	const char* sendCommand;
	uint8_t sendCommandLength;
	const char* recvCommand;
	uint8_t recvCommandLength;
	uint16_t timeout;
}AT_Command_Struct;

static const char AT_OK[] = "OK";										// OK
static const char AT_READY[] = "READY";									// READY
static const char AT_CONNECT[] = "CONNECT";									// CONNECT

uint8_t ESP8285_Init( void );											// 初始化
uint8_t ESP8285_AT_Command_Send( const AT_Command_Struct* command );	// 发送AT命令
uint8_t ESP8285_CipSend_FixLength(const char* sendData, uint8_t sendLength, char* recvData, uint8_t recvLength);


static inline uint8_t Esp8285ConnectConfirm(void)
{
	extern const AT_Command_Struct ESP8285_AT_CONNECT_CONFIRM;
	return ESP8285_AT_Command_Send(&ESP8285_AT_CONNECT_CONFIRM);
}

static inline uint8_t Esp8285Reset(void)
{
	extern const AT_Command_Struct ESP8285_AT_MODULE_RST;
	return ESP8285_AT_Command_Send(&ESP8285_AT_MODULE_RST);
}

static inline uint8_t Esp8285SetCwMode(void)
{
	extern const AT_Command_Struct ESP8285_AT_STA_MODE_SET;
	return ESP8285_AT_Command_Send(&ESP8285_AT_STA_MODE_SET);
}

static inline uint8_t Esp8285ApConnect(void)
{
	extern const AT_Command_Struct ESP8285_AT_AP_CONNECT;	
	return ESP8285_AT_Command_Send(&ESP8285_AT_AP_CONNECT);
}

static inline uint8_t Esp8285CIMUX0(void)
{
	extern const AT_Command_Struct ESP8285_AT_CIPMUX;	
	return ESP8285_AT_Command_Send(&ESP8285_AT_CIPMUX);
}

#endif
