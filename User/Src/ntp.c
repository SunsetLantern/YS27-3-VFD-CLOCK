#include "ntp.h"
#include "esp8285.h"

static const char AT_ConnectNtpServer[] = "AT+CIPSTART=\"UDP\",\"1.cn.pool.ntp.org\",123\r\n";
static const uint8_t AT_GetNTP[] = {0xE3, 0x00, 0x06, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
									0x03, 0x14, 0xE3, 0x13, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const AT_Command_Struct ESP8285_AT_ConnectNtpServer = { AT_ConnectNtpServer, sizeof(AT_ConnectNtpServer) - 1 , AT_CONNECT, sizeof(AT_ConnectNtpServer) + sizeof(AT_CONNECT) + 30, 100 };
static const char AT_GetNtp_FindStr[] = "+IPD,48:";
static uint8_t recvBuffer[255] = {0};


static inline uint8_t NtpServerConnect(void)
{
	return ESP8285_AT_Command_Send(&ESP8285_AT_ConnectNtpServer);
}

uint8_t GetNtpInit(void)
{
	if( ESP8285_OK != ESP8285_Init() )
	{
		return NTP_STATUS_INIT_FAIL;
	}
	if( ESP8285_OK != Esp8285ConnectConfirm() )
	{
		return NTP_STATUS_INIT_FAIL;
	}
	if( ESP8285_OK != Esp8285SetCwMode() )
	{
		return NTP_STATUS_INIT_FAIL;
	}
	if( ESP8285_OK != Esp8285ApConnect() )
	{
		return NTP_STATUS_INIT_FAIL;
	}	
	if( ESP8285_OK != Esp8285CIMUX0() )
	{
		return NTP_STATUS_INIT_FAIL;
	}
	if( ESP8285_OK != NtpServerConnect() )
	{
		return NTP_STATUS_INIT_FAIL;
	}
	ESP8285_CipSend_FixLength((char*)AT_GetNTP, 48, (char*)recvBuffer, 255);
	
	static char* test = {0};
	test = strstr((char*)recvBuffer, AT_GetNtp_FindStr);// 不能这么用，因为有0
	
	return NTP_STATUS_INIT_SUCCESS;
}
