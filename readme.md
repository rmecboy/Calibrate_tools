# ROS标定工具

## 包含了kalibr相机标定包及imu_utils标定包

## Dependencies 

- ### ROS noetic
- ### Eigen

## 工程目录结构

```shell
kalibr_ws
    ├── src
    │   ├── CMakeLists.txt
    │   ├── code_utils
    │   ├── imu_utils
    │   └── kalibr
    └── yaml
        ├── april.yaml
        ├── check-board.yaml
        ├── imu.yaml  此文件为imu_utils生成的imu标定参数
        └── stereo_calibra1-camchain.yaml 此文件为kalibr生成的双目标定参数（可结合imu.yaml用于联合标定）
```
### 前置准备
#### 标定包编译环节
- 首先安装Catkin build
```bash
pip3 install catkin-tools catkin-tools-python
```
- 第一步，先编译code_utils包 (code_utils是imu_utils的依赖，必须先编译)
```bash
catkin build code_utils
```
- 再编译imu_utils(imu标定包)
```bash
catkin build imu_utils
```
- 最后编译kalibr（相机标定包，编译时间会比较久，耐心等待，内存够大建议开多线程，我开单线程编译用了“Runtime: 12 minutes and 33.1 seconds total.”）
```bash
catkin build kalibr
```
#### 编译自定义串口协议包
```bash
catkin build serial_port
```

#### 编译STM32 工程，烧录代码（找电控）
```bash
cd stm32-imu_cali_proj
make -j12
make download_jlink
```
### 数据采集
#### imu标定数据采集
- C板插上Micro USB线后静置，等待开发板指示灯变为蓝色，即开发板初始化成功后
执行以下命令
```bash
collect_imu_data.sh
```
采集时间最好大于两小时，但是我们采20-30分钟的数据也够用了，到时间后CTRL+C终止数据采集程序
包含imu数据的rosbag将自动存在kalibr_ws/src/serial_port/record_bags/这个路径下
#### imu标定
- imu标定
首先,执行倍速播放rosbag命令(此时roscore未启动是不会播放的)
```bash
rosbag play -r 200 imu_raw_data_2024-04-10-22-46-42.bag（bag名换成你采集的那个） // 200 倍速播放rosbag
```
- 然后打开另一个终端执行以下命令启动imu_utils标定工具
```bash
roslaunch imu_utils C-BOARD.launch 
```
- 等一段时间，imu的标定结果会自动存放在kalibr_ws/src/imu_utils/data，标定参数文件名字为c-board-bmi088_imu_param.yaml


todo:联合标定单/双目相机标定使用说明。
推荐直接看csdn的教程https://blog.csdn.net/LoveJSH/article/details/131953776

