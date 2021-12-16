#include "SerialData.h"
#include "Config.h"

BluetoothSerial SerialBT;

SerialData::SerialData()
{
    strCMDLen = 0;
}

void SerialData::HWSerialInit()
{
#ifdef HWSERIAL1_EN
    Serial.print("Serial1En\r\n");
#endif

#ifdef HWSERIAL2_EN
    Serial.print("Serial2En\r\n");
#endif

#ifdef BTSERIAL_EN
    SerialBT.begin("ESP32_EVM");
#endif
}

void SerialData::printTime(TimeStruct timeData)
{
    Serial.print(timeData.Year, DEC);
    Serial.print('-');
    Serial.print(timeData.Month, DEC);
    Serial.print('-');
    Serial.print(timeData.Day, DEC);
    Serial.print(" W:");
    Serial.print(timeData.Week, DEC);
    Serial.print(' ');
    Serial.print(timeData.Hour, DEC);
    Serial.print(':');
    Serial.print(timeData.Minute, DEC);
    Serial.print(':');
    Serial.print(timeData.Second, DEC);
}

void SerialData::rxProcess(void)
{
    if ((Serial.available() > 0) && (!rxReFlag))
    {
        while (Serial.available() > 0)
        {
            char readChar = Serial.read();
            rxData += readChar;
            if (readChar != '\n') //'\n'is the data end flag
            {
                if (rxData.length() > 100) //Clear data for too long data but no end flag
                {
                    rxReFlag = false;
                    rxData = "";
                    break;
                }
            }
            else
            {
                rxReFlag = true; //Receive data end
                break;
            }
        } //Serial Data available
    }

#ifdef HWSERIAL1_EN
    if ((Serial1.available() > 0) && (!rxReFlagS1))
    {
        while (Serial1.available() > 0)
        {
            char readChar = Serial1.read();
            rxDataS1 += readChar;
            if (readChar != '\n') //'\n'is the data end flag
            {

                if (rxDataS1.length() > 100) //Clear data for too long data but no end flag
                {
                    rxReFlagS1 = false;
                    rxDataS1 = "";
                    break;
                }
            }
            else
            {
                rxReFlagS1 = true; //Receive data end
                break;
            }
        } //Serial Data available
    }
#endif

#ifdef HWSERIAL2_EN
    if ((Serial2.available() > 0) && (!rxReFlagS2))
    {
        while (Serial2.available() > 0)
        {
            char readChar = Serial2.read();
            rxDataS2 += readChar;
            if (readChar != '\n') //'\n'is the data end flag
            {
                if (rxDataS2.length() > 100) //Clear data for too long data but no end flag
                {
                    rxReFlagS2 = false;
                    rxDataS2 = "";
                    break;
                }
            }
            else
            {
                rxReFlagS2 = true; //Receive data end
                break;
            }
        } //Serial Data available
    }
#endif

#ifdef BTSERIAL_EN
    if ((SerialBT.available() > 0) && (!rxReFlagBT))
    {
        while (SerialBT.available() > 0)
        {
            char readChar = SerialBT.read();
            rxDataBT += readChar;
            if (readChar != '\n') //'\n'is the data end flag
            {
                if (rxDataBT.length() > 100) //Clear data for too long data but no end flag
                {
                    rxReFlagBT = false;
                    rxDataBT = "";
                    break;
                }
            }
            else
            {
                rxReFlagBT = true; //Receive data end
                break;
            }
        } //Serial Data available
    }
#endif
}

bool SerialData::getData(String *outData)
{
    if (rxReFlag)
    {
        *outData = rxData;
        rxData = "";
        rxReFlag = false;
        return true;
    }
    else
    {
        *outData = "";
        return false;
    }
}

#ifdef HWSERIAL1_EN
bool SerialData::getDataS1(String *outData)
{
    if (rxReFlagS1)
    {
        *outData = rxDataS1;
        rxDataS1 = "";
        rxReFlagS1 = false;
        return true;
    }
    else
    {
        *outData = "";
        return false;
    }
}
#endif

#ifdef HWSERIAL2_EN
bool SerialData::getDataS2(String *outData)
{
    if (rxReFlagS2)
    {
        *outData = rxDataS2;
        rxDataS2 = "";
        rxReFlagS2 = false;
        return true;
    }
    else
    {
        *outData = "";
        return false;
    }
}
#endif

#ifdef BTSERIAL_EN
bool SerialData::getDataBT(String *outData)
{
    if (rxReFlagBT)
    {
        *outData = rxDataBT;
        rxDataBT = "";
        rxReFlagBT = false;
        return true;
    }
    else
    {
        *outData = "";
        return false;
    }
}
#endif

byte SerialData::data2CMD(String strInData, String strCommandTab[], String *outData)
{
    byte CMDIndex = 0, i;
    String strflag = strCommandTab[0];
    if (strInData.indexOf(strflag) < 0)
        return 0xff;
    else
    {
        strInData = strInData.substring(strInData.indexOf(strflag) + strflag.length());
    }
    for (i = 1; i < strCMDLen; i++)
    {
        if (strInData.indexOf(strCommandTab[i]) == 0)
        {
            CMDIndex = i;
            *outData = strInData.substring(strCommandTab[i].length());
            break;
        }
    }
    if (i == strCMDLen)
    {
        CMDIndex = 255;
        *outData = "0";
    }
    return CMDIndex;
}

bool SerialData::string2Time(String InData, TimeStruct *timeData)
{
    if (InData.length() < 15)
        return false;
    timeData->Year = InData.substring(0, 4).toInt() % 3000;
    timeData->Month = InData.substring(4, 6).toInt() % 13;
    timeData->Day = InData.substring(6, 8).toInt() % 32;
    timeData->Week = InData.substring(8, 9).toInt() % 8;
    timeData->Hour = InData.substring(9, 11).toInt() % 24;
    timeData->Minute = InData.substring(11, 13).toInt() % 60;
    timeData->Second = InData.substring(13, 15).toInt() % 60;
    return true;
}

bool SerialData::string2ALTime(String inData, TimeStruct *timeData)
{
    if (inData.length() < 6)
        return false;
    timeData->Year = 2021;
    timeData->Month = 1;
    timeData->Day = 1;
    timeData->Week = 1;
    timeData->Hour = inData.substring(0, 2).toInt() % 24;
    timeData->Minute = inData.substring(2, 4).toInt() % 60;
    timeData->Second = inData.substring(4, 6).toInt() % 60;
    return true;
}

bool SerialData::string2BitByte(String InData, byte *bitData)
{
    byte i, len = InData.length() - 1, bitTemp = 0;
    char charBit;
    for (i = 0; i < len; i++)
    {
        charBit = InData.charAt(len - 1 - i);
        Serial.print(charBit);
        Serial.print("-");
        if (charBit == '1')
            bitTemp |= (0x01 << i);
        else if (charBit == '0')
            continue;
        else
            return false;
    }
    *bitData = bitTemp;
    return true;
}

bool SerialData::string2HexByte(String strData, byte *bitData, byte strIndex)
{
    byte intData;
    char charHex;
    if (strIndex >= strData.length())
        return false;

    charHex = strData.charAt(strIndex);
    if (!isHexadecimalDigit(charHex))
        return false;
    else
        intData = char2Byte(charHex);

    if ((strIndex + 1) < strData.length())
    {
        charHex = strData.charAt(strIndex + 1);
        if (isHexadecimalDigit(charHex))
        {
            intData = intData << 4;
            intData += char2Byte(charHex);
        }
    }
    *bitData = intData;
    return true;
}

byte SerialData::char2Byte(char charData)
{
    if (!isHexadecimalDigit(charData))
        return 0;
    else if ((charData >= '0') && (charData <= '9'))
        return charData - '0';
    else if ((charData >= 'a') && (charData <= 'f'))
        return charData - 'a' + 10;
    else if ((charData >= 'A') && (charData <= 'F'))
        return charData - 'A' + 10;
}