#include "bsp_init.h"
#include "bsp_log.h"
#include "bsp_dwt.h"
#include "bsp_usb.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "bsp_temperature.h"

// CAN和串口会在注册实例的时候自动初始化,不注册不初始化
void BSPInit()
{
    DWT_Init(168);
    BSPLogInit();

    LEDInit();
    IMUTempInit();
    BuzzerInit();
}