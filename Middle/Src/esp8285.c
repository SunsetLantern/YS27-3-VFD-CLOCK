
/* Include --------------------------------------------------------------------------*/
/* Middle */
#include "esp8285.h"
/* Core */
#include "usart.h"
#include "stdio.h"

/* Define ---------------------------------------------------------------------------*/

#define				TIME_WEBSITE					"  "

#define				ESP8285_UART_HANDLER			( huart3 )
#define				ESP8285_UART_RECV_BUFFER_SIZE	( 255 )

const char AT[] = "AT\r\n";
const char AT_CWMODE_1[] = "AT+CWMODE=1\r\n";
const char AT_RST[] ="AT+RST\r\n";
const char AT_AP_CONNECT[] = "AT+CWJAP=\""SSID"\",\""PASSWORD"\"\r\n";	// 连接AP
const char AT_GOT_IP[] = "AT+CIFSR\r\n";								// 获取IP地址
const char AT_CIPMUX[] = "AT+CIPMUX=0\r\n";								// 单路连接
const char AT_CIPSEND_FIX_LENGTH_HEADER[] = "AT+CIPSEND=";				// 进入透传模式（定长）帧头

/* AT Command */
const AT_Command_Struct ESP8285_AT_CONNECT_CONFIRM = { AT, sizeof(AT) - 1 , AT_OK, sizeof(AT) + sizeof(AT_OK) + 3, 100 };
const AT_Command_Struct ESP8285_AT_STA_MODE_SET = { AT_CWMODE_1, sizeof(AT_CWMODE_1) - 1, AT_OK, sizeof(AT_CWMODE_1) + sizeof(AT_OK) + 3, 100 };
const AT_Command_Struct ESP8285_AT_MODULE_RST = { AT_RST, sizeof(AT_RST) - 1 , AT_READY, sizeof(AT_RST) + sizeof(AT_READY) + 3, 2000 };
const AT_Command_Struct ESP8285_AT_AP_CONNECT = { AT_AP_CONNECT, sizeof(AT_AP_CONNECT) - 1, AT_OK, 88, 5000 };
const AT_Command_Struct ESP8285_AT_CIPMUX = { AT_CIPMUX, sizeof(AT_CIPMUX) - 1, AT_OK, sizeof(AT_CIPMUX) + sizeof(AT_OK) + 3, 100 };

/* Static Variable ------------------------------------------------------------------*/
uint8_t Esp8285RecvBuffer[ESP8285_UART_RECV_BUFFER_SIZE] = { 0 };

/* Static Function  -----------------------------------------------------------------*/
static void EspTransmit( char* data, uint16_t length, uint16_t timeout );
static uint8_t EspTransmit_DMA( char* data, uint16_t length );
static void EspReceive( char* data, uint16_t length, uint16_t timeout );
static void EspReceive_DMA_Channel_Open( void );

static void EspTransmit( char* data, uint16_t length, uint16_t timeout )
{
	HAL_UART_Transmit( &ESP8285_UART_HANDLER, (uint8_t*)data, length, timeout );
}


static uint8_t EspTransmit_DMA( char* data, uint16_t length )
{
	if( HAL_UART_Transmit_DMA( &ESP8285_UART_HANDLER, (uint8_t*)data, length ) != HAL_OK)
		return ESP8285_ERROR_DMA_TRANSMIT_BUSY;
	
	return ESP8285_OK;
}


static void EspReceive( char* data, uint16_t length, uint16_t timeout )
{
	HAL_UART_Receive( &ESP8285_UART_HANDLER, (uint8_t*)data, length, timeout );
}

static void EspReceive_DMA_Channel_Open( void )
{
	__HAL_UART_ENABLE_IT( &ESP8285_UART_HANDLER, UART_IT_IDLE );
	HAL_UARTEx_ReceiveToIdle_DMA( &ESP8285_UART_HANDLER, Esp8285RecvBuffer, ESP8285_UART_RECV_BUFFER_SIZE );
}


/* Function  ------------------------------------------------------------------------*/
void HAL_UARTEx_RxEventCallback( UART_HandleTypeDef *huart, uint16_t Size )
{
	__nop();
}


uint8_t ESP8285_Init( void )
{
#if ESP8285_UART_HANDLER == huart3
	MX_USART3_UART_Init();
#else
	return ESP8285_FAIL;
#endif
	
	return ESP8285_OK;
}

uint8_t ESP8285_AT_Command_Send( const AT_Command_Struct* command  )
{
	EspTransmit( (char*)command->sendCommand, command->sendCommandLength, command->timeout );
	memset((char*)Esp8285RecvBuffer, 0, ESP8285_UART_RECV_BUFFER_SIZE);
	EspReceive( (char*)Esp8285RecvBuffer, command->recvCommandLength, command->timeout );
	if( 0 == strstr((char*)Esp8285RecvBuffer, (char*)command->recvCommand))
	{
		return ESP8285_FAIL;	
	}
	else
	{
		return ESP8285_OK;
	}
}

uint8_t ESP8285_CipSend_FixLength(const char* sendData, uint8_t sendLength, char* recvData, uint8_t recvLength)
{
	static char AT_CIPSEND_FIX_LENGTH_FRAME[16] = {0};				// 进入透传模式，AT命令帧
	static AT_Command_Struct ESP8285_AT_CIPSEND_FIX_LENGTH = {0};	// 进入透传模式，AT命令帧结构体
	sprintf(AT_CIPSEND_FIX_LENGTH_FRAME, "%s%d\r\n", AT_CIPSEND_FIX_LENGTH_HEADER, sendLength);
	ESP8285_AT_CIPSEND_FIX_LENGTH.sendCommand = AT_CIPSEND_FIX_LENGTH_FRAME;
	ESP8285_AT_CIPSEND_FIX_LENGTH.sendCommandLength = sizeof(AT_CIPSEND_FIX_LENGTH_FRAME);
	ESP8285_AT_CIPSEND_FIX_LENGTH.recvCommand = AT_OK;
	ESP8285_AT_CIPSEND_FIX_LENGTH.recvCommandLength = sizeof(AT_CIPSEND_FIX_LENGTH_FRAME) + sizeof(AT_OK) + 3;
	ESP8285_AT_CIPSEND_FIX_LENGTH.timeout = 100;
	if(ESP8285_OK != ESP8285_AT_Command_Send(&ESP8285_AT_CIPSEND_FIX_LENGTH))
	{
		return ESP8285_FAIL;
	}
	HAL_UART_Transmit( &ESP8285_UART_HANDLER, (uint8_t*)sendData, sendLength, 100 );
	EspReceive( recvData, recvLength, 1000 );
	__nop();
}
