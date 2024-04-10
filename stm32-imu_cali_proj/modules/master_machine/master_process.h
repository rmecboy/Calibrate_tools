#ifndef MASTER_PROCESS_H
#define MASTER_PROCESS_H

#include "bsp_usb.h"
#include "ins_task.h"

#pragma pack(1)
typedef struct
{
	uint8_t head;//0x5A
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	uint8_t tail;//0xEE
}imu_data_raw_t;
#pragma pack()

void ImuCaliInit(void);
void SnedImuCaliData(float ax, float ay, float az, float gx, float gy, float gz);

#endif // !MASTER_PROCESS_H