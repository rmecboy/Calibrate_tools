#include "serial_driver.hpp"

namespace imu_calibrate
{
    calibraImu::calibraImu()
    {
        ros::NodeHandle nh;
        
        nh.param<std::string>("serial_port_name",  serial_port_name,  "/dev/ttyUSB0"); //固定串口号
        nh.param<int>        ("serial_baud_rate", serial_baud_rate, 115200); //和下位机通信波特率115200

        imu_pub_ = nh.advertise<sensor_msgs::Imu>("/imu/data_raw", 50);

        try
        { 
            if(!Awakenlion_Serial.isOpen())
            {
                //尝试初始化与开启串口
                Awakenlion_Serial.setPort(serial_port_name); //选择要开启的串口号
                Awakenlion_Serial.setBaudrate(serial_baud_rate); //设置波特率
                serial::Timeout _time = serial::Timeout::simpleTimeout(1000);  //超时等待
                Awakenlion_Serial.setTimeout(_time);
                Awakenlion_Serial.open();  //开启串口
                receive_thread_ = std::thread(&calibraImu::receiveData, this);
                ROS_INFO_STREAM("car_robot serial port opened");  //串口开启成功提示
            }
        }
        catch (const std::exception & ex)
        {
            ROS_ERROR_STREAM("can not open serial port"); //如果开启串口失败，打印错误信息
            throw ex;        
        }
        
    }

    void calibraImu::reopenPort()
    {
        ROS_ERROR_STREAM("Attempting to reopen port");
        try {
            if (Awakenlion_Serial.isOpen()) 
            {
                Awakenlion_Serial.close();
            }
            Awakenlion_Serial.open();
            ROS_INFO("Successfully reopened port");
        } 
        catch (const std::exception & ex) 
        {
            ROS_ERROR_STREAM("Error while reopening port: " << ex.what());
            if (ros::ok()) {
                ros::Duration(1).sleep();
                reopenPort();
            }
        }
    }


    void calibraImu::receiveData()
    {
        while (ros::ok()) {
            try{
                int len = Awakenlion_Serial.read((uint8_t *)&rx_frame,sizeof(rx_frame));  //获取长度
                if(rx_frame.head==0x5A && rx_frame.tail==0xEE && len == sizeof(rx_frame)) 
                {
                    sensor_msgs::Imu imu_msg;
                    imu_msg.header.stamp = ros::Time::now();
                    imu_msg.header.frame_id = "imu_link";
                    imu_msg.angular_velocity.x = rx_frame.imu_data_raw.gx;
                    imu_msg.angular_velocity.y = rx_frame.imu_data_raw.gy;
                    imu_msg.angular_velocity.z = rx_frame.imu_data_raw.gz;

                    imu_msg.linear_acceleration.x = rx_frame.imu_data_raw.ax;
                    imu_msg.linear_acceleration.y = rx_frame.imu_data_raw.ay;
                    imu_msg.linear_acceleration.z = rx_frame.imu_data_raw.az;
                    
                    imu_pub_.publish(imu_msg);

                    ROS_INFO("ax:%7.3f, ay:%7.3f, az:%7.3f, gx:%7.3f, gy:%7.3f, gz:%7.3f",   
                             rx_frame.imu_data_raw.ax , rx_frame.imu_data_raw.ay , rx_frame.imu_data_raw.az,
                             rx_frame.imu_data_raw.gx , rx_frame.imu_data_raw.gy , rx_frame.imu_data_raw.gz);
                }
                else
                {
                    ROS_ERROR_STREAM("rxframe error");
                    ros::Duration(1).sleep();
                    reopenPort();
                }
            }catch (const std::exception & ex) {
                ROS_ERROR_STREAM("Error while reopening port: " << ex.what());
                reopenPort();
            }
        }
    }

    calibraImu::~calibraImu()
    {
        if (receive_thread_.joinable()) {
            receive_thread_.join();
        }

        if (Awakenlion_Serial.isOpen()) 
        {
            Awakenlion_Serial.close();
        }
    }
} // namespace imu_calibrate

int main(int argc, char *argv[])
{
    // Initiate ROS
    ros::init(argc,argv,"imu_calibrate_node");

    imu_calibrate::calibraImu test;

    // ros::AsyncSpinner spinner(0); 
    // spinner.start();
    // ros::waitForShutdown();

    return 0;
}