#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define SERIAL_DATA
#define HWSERIAL1_EN
#define HWSERIAL2_EN
#define BTSERIAL_EN

#ifdef HWSERIAL1_EN
#define UART_TX1 14
#define UART_RX1 27
#define UART1_BD 38400
#endif

#ifdef HWSERIAL2_EN
#define UART_TX2 19
#define UART_RX2 18
#define UART2_BD 9600
#endif

#define DATA_FLAG "ESP:"
#define STR_CMD_MAX 9

#define ESP_V5_OUT 1
#define ESP_RD_INF 2
#define ESP_RD_TH 3
#define ESP_SET_LED 4
#define RTC_RD_TEMP 5
#define RTC_SET_TIME 6
#define RTC_RD_TIME 7
#define RTC_SET_ALWEEK 8

class Config
{

private:
public:
    Config();

#ifdef SERIAL_DATA
    String strCMDTab[STR_CMD_MAX];
#endif
};

#endif
