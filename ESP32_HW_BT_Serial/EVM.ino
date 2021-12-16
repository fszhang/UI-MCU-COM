#include "Arduino.h"
#include "DS3231.h"
#include "EEP24C32.h"
#include <Wire.h>
#include "EVM_HW.h"
#include "SerialData.h"
#include "Config.h"
#include "SHT3XA.h"

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */

SHT3XA thSensor;               //温湿度传感器
byte thFd;                     //温湿度传感器设备
EVM_HW evm_HW;                 //硬件相关
SerialData serialData;         //串口
Config configESP;              //配置
uint16_t keyCount = 0;         //按键计数
byte keyPress = 0;             //按键状态
unsigned long keyTimeMillis;   //按键计数时间
String strSensorFlag = "GAS:"; //气体传感器数据标志
String strESPFlag = "ESP:";    //测试板数据标志
bool sensorStart = false;      //气体传感器运行
unsigned long timeMillis;      //系统运行时间(mS)
uint16_t timeLEDOn;            //LED点亮时间
bool sendTH = false;           //
unsigned long thTimeMillis;    //按键计数时间

void setup()
{
    Serial.begin(115200);
    Wire.begin();
#ifdef HWSERIAL1_EN
    Serial1.begin(UART1_BD, SERIAL_8N1, UART_RX1, UART_TX1); //串口1
#endif
#ifdef HWSERIAL2_EN
    Serial2.begin(UART2_BD, SERIAL_8N1, UART_RX2, UART_TX2); //串口2
#endif
    serialData.HWSerialInit();          //串口配置
    serialData.strCMDLen = STR_CMD_MAX; //串口接收数据指令集数量

    evm_HW.GPIO_Init();       //接口初始化
    thFd = thSensor.Config(); //配置温湿度传感器

    delay(1000);
    timeMillis = millis();
    thTimeMillis = millis();
}

void loop()
{
    evm_HW.readData(); //读取测试板各物理量
    //充电状态/充电完成状态/未充电状态   LED灯的状态设置
    if (evm_HW.chargeIn == 1)
        timeLEDOn = 400;
    else if (evm_HW.chargeIn == 2)
        timeLEDOn = 900;
    else
        timeLEDOn = 50;
    if (millis() > (timeMillis + 1000))
        timeMillis += 1000;
    else if (millis() > (timeMillis + timeLEDOn))
        digitalWrite(LED, LOW);
    else
        digitalWrite(LED, HIGH);

    //串口接收数据处理
    serialData.rxProcess();

    String dataRec = ""; //接收数据

    if (serialData.rxReFlag) //串口0接收数据完成
    {
        serialData.getData(&dataRec);         //读取数据
        if (dataRec.indexOf(strESPFlag) >= 0) //数据中有测试板标志字符
        {
            CMDControl(dataRec, 0); //设置测试板参数，如果有数据返回，在串口0上输出
        }
    }

    if (serialData.rxReFlagS1) //串口1接收数据完成
    {
        serialData.getDataS1(&dataRec);          //读取数据
        if (dataRec.indexOf(strSensorFlag) >= 0) //数据中有气体传感器标志字符
        {
            dataRec = strSensorFlag + dataRec.substring(strSensorFlag.length());
            Serial.print(dataRec); //在串口0上打印数据
        }
    }

    if (serialData.rxReFlagS2) //串口2接收数据完成
    {
        serialData.getDataS2(&dataRec);       //读取数据
        if (dataRec.indexOf(strESPFlag) >= 0) //数据中有测试板标志字符
        {
            CMDControl(dataRec, 2); //设置测试板参数，如果有数据返回，在串口2上输出
        }
    }

    if (serialData.rxReFlagBT) //蓝牙串口接收数据完成
    {
        serialData.getDataBT(&dataRec);       //读取数据
        if (dataRec.indexOf(strESPFlag) >= 0) //数据中有测试板标志字符
        {
            CMDControl(dataRec, 3); //设置测试板参数，如果有数据返回，在蓝牙串口上输出
        }
    }

    if (!sensorStart) //气体传感器未工作
    {
        delay(100);
        digitalWrite(SENSOR_EN, HIGH); //全能传感器电源
        delay(200);

        //给气体传感器发送开始运行指令
        Serial1.write(0x08);
        delay(100);
        Serial2.write(0x08);
        delay(200);
        byte data[9] = {0x07, 0x0E, 0x05, 0x52, 0x00, 0x00, 0x00, 0x00, 0xA9};
        Serial1.write(data, 9);
        delay(500);
        Serial2.write(data, 9);
        sensorStart = true;
    }

    if (thTimeMillis < millis())
    {
        if (sendTH)
        {
            Serial.print("ESP:");
            SerialBT.print("ESP:");
            thSensor.ReadTH();
            thSensor.Print(0);
            thSensor.Print(3);
        }
        thTimeMillis = millis() + 1200; //大于2mS检测一次
    }

    //检测按键
    if (keyTimeMillis < millis())
    {
        if (digitalRead(PB_IN) && (keyPress != 3)) //按键按下，且防止二次触发
        {
            keyCount++;
            if (keyCount > 200) //按键按下计数累计超过200为长按
                keyPress = 2;
        }
        else if (!digitalRead(PB_IN))
        {
            if (keyPress == 3) //按键释放，长按计数清零
            {
                keyCount = 0;
                keyPress = 0;
            }
            else if (keyCount > 30) //短按，去抖
            {
                keyPress = 1;
                keyCount = 0;
            }
            else
            {
                keyCount = 0;
                keyPress = 0;
            }
        }
        keyTimeMillis = millis() + 2; //大于2mS检测一次
    }

    if (keyPress == 1) //短按切换5V电源开关
    {
        if (digitalRead(VCC_5V_EN))
            evm_HW.VCC_5V_Output(false);
        else
            evm_HW.VCC_5V_Output(true);
        keyPress = 0;
    }
    else if (keyPress == 2) //长按系统休眠
    {
        keyPress = 3;
        digitalWrite(SENSOR_EN, LOW);
        evm_HW.VCC_5V_Output(false);
        digitalWrite(LED, LOW);
        while (digitalRead(PB_IN))
        {
        }
        delay(2000);
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1); //允许IO4引脚唤醒
        //Go to sleep now
        Serial.println("Going to sleep now");
        esp_deep_sleep_start();
    }
}

void CMDControl(String strInData, byte serialNUM)
{
    byte command;
    String strData = "";
    command = serialData.data2CMD(strInData, configESP.strCMDTab, &strData);
    if (command != 0xff)
    {
        if (serialNUM == 0)
            Serial.print("ESP:");
        else if (serialNUM == 2)
            Serial2.print("ESP:");
        else if (serialNUM == 3)
            SerialBT.print("ESP:");
    }

    switch (command)
    {
    case ESP_V5_OUT:
        if (strData.charAt(0) == '0')
            evm_HW.VCC_5V_Output(false);
        else if (strData.charAt(0) == '1')
            evm_HW.VCC_5V_Output(true);
        break;
    case ESP_RD_INF:
        evm_HW.printPowerInf(serialNUM);
        break;
    case ESP_RD_TH:
        if (thFd)
        {
            thSensor.ReadTH();
            thSensor.Print(serialNUM);
            sendTH = true;
            thTimeMillis = millis() + 1200;
        }
        break;
    case ESP_SET_LED:
        byte rgbData[4];
        bool dataError;
        if (!evm_HW.VCC_5VO)
        {
            evm_HW.VCC_5V_Output(true);
            delay(100);
        }
        dataError = false;
        for (byte i = 0; i < 4; i++)
        {
            if (!serialData.string2HexByte(strData, &rgbData[i], i * 2))
            {
                dataError = true;
                break;
            }
        }
        if (!dataError)
            evm_HW.SetLightColor(rgbData[0], rgbData[1], rgbData[2], rgbData[3]);
        break;
    default:
        break;
    }
}