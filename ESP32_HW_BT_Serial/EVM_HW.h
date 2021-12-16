#ifndef EVM_HW_H__
#define EVM_HW_H__

#include "Arduino.h"
#include "SerialData.h"

#define LED 5
#define GPIO 17
#define PB_IN 4
#define SENSOR_EN 12
#define VCC_5V_EN 26
#define VBAT_EN 25
#define CUR_AN 35
#define VCC_5V_AN 34
#define CHRG_IN 39
#define VBAT_AN 36

class EVM_HW
{

private:
public:
    uint16_t batLevel;
    uint16_t V5Vol;
    byte chargeIn;
    bool VCC_5VO;
    uint16_t current_mA;
    bool powerOn;
    void GPIO_Init(void);
    void readData(void);
    void VCC_5V_Output(bool enable);
    void shutDownWithWakeUpForDS3231(void);
    void PC_PowerKeyPress(void);
    void printPowerInf(byte serialNUM);
    void SetLightColor(uint8_t brightness, uint8_t colorR, uint8_t colorG, uint8_t colorB);
};

#endif // RTC_HW_H
