#ifndef __SHT3XA_H
#define __SHT3XA_H

#include "Arduino.h"
#include <Wire.h>
#include "SerialData.h"

#define SHT3XA_HADDR 0x44
//#define SHT3XA_HADDR 0x45

#define SHT3XA_CONV_START 0x240B
#define SHT3XA_HEAT_OFF 0x3066
#define SHT3XA_HEAT_ON 0x306D
#define SHT3XA_RESET 0x30A2

class SHT3XA
{
public:
    int16_t temp;
    uint8_t humidity;
    SHT3XA();
    uint8_t Reset(void);
    uint8_t ConvStart(void);
    uint8_t HeaterDisble(void);
    uint8_t HeaterEnble(void);
    uint8_t Config(void);
    uint8_t ReadTH(void);
    void Print(byte serialNUM);

private:
    uint8_t thread(byte *data, byte len);
    uint8_t thwrite(uint16_t command);
};

#endif
