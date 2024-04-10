cmake_minimum_required(VERSION 3.0.2)
project(serial_port)


# 定义函数，用于递归添加头文件
function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()


if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  roslib
  rospy
  sensor_msgs
  std_msgs
  tf
  tf2_geometry_msgs
  tf2_ros
  serial
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES serial_port serial
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp roslib rospy sensor_msgs std_msgs tf tf2_geometry_msgs tf2_ros
  DEPENDS system_lib
)


# 递归搜索工程目录下webots_update包的头文件
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/serial_port)

# # 把源文件添加到名为SOURCE_CPP的地址列表中   ???为什么catkin build用不了，catkin_make可以
# file(GLOB_RECURSE SOURCE_CPP 
#   ${CMAKE_SOURCE_DIR}/serial_port/src/*.cpp
# )


include_directories(
  ${CMAKE_SOURCE_DIR}/include/serial_port
  include
  ${catkin_INCLUDE_DIRS}
)



# 生成可执行文件
add_executable(${PROJECT_NAME}_node  ${CMAKE_SOURCE_DIR}/src/serial_driver.cpp)

# 添加依赖
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# 添加链接库
target_link_libraries(${PROJECT_NAME}_node 

  ${catkin_LIBRARIES}
)