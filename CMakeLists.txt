cmake_minimum_required(VERSION 3.5)

project(program_prac )


# 头文件目录
include_directories(include)

# 设置源文件，并设置子目录可见
file(GLOB INTERPRETER_SOURCE "${CMAKE_SOURCE_DIR}/src/interpreter/*.cpp")
file(GLOB SERVER_SOURCE "${CMAKE_SOURCE_DIR}/src/server/*.cpp")

# 可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 项目文件
add_executable(robot1 ${INTERPRETER_SOURCE} 
                        ${SERVER_SOURCE}          
                        src/main.cpp)

# 添加测试
add_subdirectory(test)