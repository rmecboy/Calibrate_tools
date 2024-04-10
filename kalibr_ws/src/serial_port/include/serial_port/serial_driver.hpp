#ifndef SERIAL_DRIVER_HPP
#define SERIAL_DRIVER_HPP

//C++
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
//ROS
#include "ros/ros.h"
#include <serial/serial.h>
#include <sensor_msgs/Imu.h>

namespace imu_calibrate
{
    #pragma pack(1)
    typedef struct imu_data_raw_t
    {
        float ax;
        float ay;
        float az;
        float gx;
        float gy;
        float gz;
    };

    typedef struct rx_frame_t
    {
        uint8_t head; //0xA5
        imu_data_raw_t imu_data_raw;
        uint8_t tail; //0xEE
    };

    typedef struct tx_frame_t
    {
        uint8_t head = 0x5A;
        imu_data_raw_t imu_data_raw;
        uint8_t tail = 0xEE;
    };

    #pragma pack()

    class calibraImu
    {    
        public:
            calibraImu();
            ~calibraImu();
            void reopenPort();
            void receiveData();

        private:
            ros::NodeHandle nh; //节点句柄
            ros::Publisher imu_pub_;

            serial::Serial Awakenlion_Serial;

            std::string serial_port_name;
            int serial_baud_rate;
            int controller_freq;
            
            rx_frame_t rx_frame;
            tx_frame_t tx_frame;

            std::thread receive_thread_;
    };
} // namespace imu_calibrate

#endif // SERIAL_DRIVER_HPP


// 5A 
// E7 41 6B 3D
// C9 8D 5C 3C 
// 16 36 1C 41 
// 00 00 00 00 
// 00 00 00 00 
// 00 00 00 00 
// EE