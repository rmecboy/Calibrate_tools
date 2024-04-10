#include "master_process.h"

static uint8_t *usb_recv_buff;
static attitude_t *imu_data;

/* 视觉通信初始化 */
void ImuCaliInit(void)
{
    USB_Init_Config_s conf = {0};
    //conf.rx_cbk = DecodeVision;
    imu_data = INS_Init();
    usb_recv_buff = USBInit(conf);
}

static imu_data_raw_t imu_data_raw;

void SnedImuCaliData(float ax, float ay, float az, float gx, float gy, float gz)
{
    imu_data_raw.head = 0x5A;
    imu_data_raw.ax = ax;
    imu_data_raw.ay = ay;
    imu_data_raw.az = az;
    imu_data_raw.gx = gx;
    imu_data_raw.gy = gy;
    imu_data_raw.gz = gz;
    imu_data_raw.tail = 0xEE;
    USBTransmit((uint8_t *)&imu_data_raw, sizeof(imu_data_raw));
}   
