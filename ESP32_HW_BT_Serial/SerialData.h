#ifndef SERIAL_DATA_H__
#define SERIAL_DATA_H__

#include "Arduino.h"
#include "DS3231.h"
#include "Config.h"
#include "BluetoothSerial.h"

#define HWSERIAL1_EN
#define HWSERIAL2_EN
#define BTSERIAL_EN

#ifdef BTSERIAL_EN
extern BluetoothSerial SerialBT;
#endif

class SerialData
{

private:
    String rxData;

#ifdef HWSERIAL1_EN
    String rxDataS1;
#endif
#ifdef HWSERIAL2_EN
    String rxDataS2;
#endif
#ifdef BTSERIAL_EN
    String rxDataBT;
#endif

public:
    SerialData();
    void HWSerialInit();
    bool getData(String *outData);
    byte strCMDLen;
    bool rxReFlag;

#ifdef HWSERIAL1_EN
    bool rxReFlagS1;
    bool getDataS1(String *outData);
#endif

#ifdef HWSERIAL2_EN
    bool rxReFlagS2;
    bool getDataS2(String *outData);
#endif
#ifdef BTSERIAL_EN
    bool rxReFlagBT;
    bool getDataBT(String *outData);
#endif
    void rxProcess(void);
    byte data2CMD(String strInData, String strCommandTab[], String *outData);
    bool string2Time(String InData, TimeStruct *timeData);
    bool string2BitByte(String InData, byte *bitData);
    bool string2HexByte(String InData, byte *bitData, byte len);
    byte char2Byte(char charData);
    bool string2ALTime(String InData, TimeStruct *timeData);
    void printTime(TimeStruct timeData);
};

#endif // SERIAL_DATA_H
