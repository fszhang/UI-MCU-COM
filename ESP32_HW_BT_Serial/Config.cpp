#include "Config.h"

Config::Config()
{

#ifdef SERIAL_DATA
    strCMDTab[0] = "ESP:";
    strCMDTab[ESP_V5_OUT] = "V5O:";
    strCMDTab[ESP_RD_INF] = "RBI:";
    strCMDTab[ESP_RD_TH] = "RTH:";
    strCMDTab[ESP_SET_LED] = "SLT:";
    strCMDTab[RTC_RD_TEMP] = "RTE:";
    strCMDTab[RTC_SET_TIME] = "STI:";
    strCMDTab[RTC_RD_TIME] = "RTI:";
    strCMDTab[RTC_SET_ALWEEK] = "SAW:";
#endif
}
