#include "SHT3XA.h"

// Constructor
SHT3XA::SHT3XA()
{
    // nothing to do for this constructor.
}

uint8_t SHT3XA::ReadTH(void)
{
    uint8_t THData[6];
    if (!ConvStart())
        return 0;
    delay(10);
    if (!thread(THData, 6))
        return 0;
    //Temp[0] = (int32_t)THData[0]*256 + (int32_t)THData[1];
    temp = ((int32_t)THData[0] * 256 + (int32_t)THData[1]) * 5 / 187 - 450;
    humidity = ((uint16_t)THData[3] * 25) / 64;
    return 1;
}

uint8_t SHT3XA::Config(void)
{
    return HeaterDisble();
}

void SHT3XA::Print(byte serialNUM)
{

    switch (serialNUM)
    {
    case 0:
        Serial.print("T:");
        Serial.print(temp);
        Serial.print("/H:");
        Serial.print(humidity);
        Serial.println("/");
        break;
    case 2:
        Serial2.print("T:");
        Serial2.print(temp);
        Serial2.print("/H:");
        Serial2.print(humidity);
        Serial2.println("/");
        break;
    case 3:
        SerialBT.print("T:");
        SerialBT.print(temp);
        SerialBT.print("/H:");
        SerialBT.print(humidity);
        SerialBT.println("/");
        break;

    default:
        break;
    }
}

uint8_t SHT3XA::HeaterEnble(void)
{
    return thwrite(SHT3XA_HEAT_ON);
}

uint8_t SHT3XA::HeaterDisble(void)
{
    return thwrite(SHT3XA_HEAT_OFF);
}

uint8_t SHT3XA::Reset(void)
{
    return thwrite(SHT3XA_RESET);
}

uint8_t SHT3XA::ConvStart(void)
{
    return thwrite(SHT3XA_CONV_START);
}

uint8_t SHT3XA::thread(byte *data, byte len)
{
    Wire.beginTransmission(SHT3XA_HADDR);
    Wire.requestFrom(SHT3XA_HADDR, (int)len);
    for (byte i = 0; i < len; i++)
        data[i] = Wire.read();
    if (Wire.endTransmission())
        return 0;
    else
        return 1;
}

uint8_t SHT3XA::thwrite(uint16_t command)
{
    Wire.beginTransmission(SHT3XA_HADDR);
    if (Wire.write(command >> 8) != 1)
        return 0;
    if (Wire.write(command & 0xff) != 1)
        return 0;
    if (Wire.endTransmission())
        return 0;
    else
        return 1;
}
