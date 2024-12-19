#ifndef __NTP_H
#define	__NTP_H

#include "stdint.h"


#define NTP_STATUS_INIT_FAIL		(0X00)
#define NTP_STATUS_INIT_SUCCESS		(0X01)

uint8_t GetNtpInit(void);


#endif
