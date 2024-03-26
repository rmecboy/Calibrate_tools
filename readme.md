# ROS标定工具

## 包含了kalibr相机标定包及imu_utils标定包

## Dependencies 

- ### ROS noetic

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





todo:双目相机标定使用说明

