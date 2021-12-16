#include "EVM_HW.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"

#define LEDS_COUNT 8
#define CHANNEL 1
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, GPIO, CHANNEL, TYPE_GRB);

void EVM_HW::GPIO_Init(void)
{
    pinMode(LED, OUTPUT);
    pinMode(GPIO, OUTPUT);
    pinMode(SENSOR_EN, OUTPUT);
    pinMode(VCC_5V_EN, OUTPUT);
    pinMode(VBAT_EN, OUTPUT);

    pinMode(PB_IN, INPUT_PULLDOWN);
    pinMode(CUR_AN, INPUT);
    pinMode(VCC_5V_AN, INPUT);
    pinMode(CHRG_IN, INPUT);
    pinMode(VBAT_AN, INPUT);

    digitalWrite(LED, LOW);
    digitalWrite(GPIO, LOW);
    digitalWrite(SENSOR_EN, LOW);
    digitalWrite(VCC_5V_EN, LOW);
    digitalWrite(VBAT_EN, LOW);
    strip.begin();
    strip.setBrightness(20);
}

void EVM_HW::readData(void)
{
    digitalWrite(VBAT_EN, HIGH); //打开电池电压检测开关(休眠时不打开省电)
    current_mA = analogRead(CUR_AN) * 9 / 10;
    V5Vol = analogRead(VCC_5V_AN) * 9 / 500;

    if (V5Vol >= 49)
        VCC_5VO = true;
    else if (V5Vol < 45)
        VCC_5VO = false;

    if (analogRead(CHRG_IN) < 120)
    {
        chargeIn = 1;
    }
    else if (analogRead(CHRG_IN) < 3000)
    {
        chargeIn = 2;
    }
    else
        chargeIn = 0;
    batLevel = analogRead(VBAT_AN) * 9 / 500;
}

void EVM_HW::VCC_5V_Output(bool enable)
{
    digitalWrite(VCC_5V_EN, enable ? HIGH : LOW);
}

void EVM_HW::printPowerInf(byte serialNUM)
{
    switch (serialNUM)
    {
    case 0:
        Serial.print("V5:");
        Serial.print(V5Vol);
        Serial.print("/VB:");
        Serial.print(batLevel);
        Serial.print("/I:");
        Serial.print(current_mA);
        Serial.println("/");
        break;
    case 2:
        Serial2.print("V5:");
        Serial2.print(V5Vol);
        Serial2.print("/VB:");
        Serial2.print(batLevel);
        Serial2.print("/I:");
        Serial2.print(current_mA);
        Serial2.println("/");
        break;
    case 3:
        SerialBT.print("V5:");
        SerialBT.print(V5Vol);
        SerialBT.print("/VB:");
        SerialBT.print(batLevel);
        SerialBT.print("/I:");
        SerialBT.print(current_mA);
        SerialBT.println("/");
        break;
    default:
        break;
    }
}

void EVM_HW::SetLightColor(uint8_t brightness, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    strip.setBrightness(brightness);
    for (byte i = 0; i < LEDS_COUNT; i++)
    {
        strip.setLedColorData(i, colorR, colorG, colorB);
    }
    strip.show();
    delay(10);
}

void EVM_HW::shutDownWithWakeUpForDS3231(void)
{
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);
    Serial.flush();
    esp_deep_sleep_start();
}
