cmake_minimum_required(VERSION 3.5)

project(program_prac )

# 头文件目录
include_directories(include)

# 设置源文件
# set(SRC_LIST src/ReadScript.cpp)
aux_source_directory(src SRC_LIST)
list(REMOVE_ITEM SRC_LIST "src/main.cpp")

# 可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加测试
add_executable(test_read ${SRC_LIST} test/test_read_script.cpp)
add_executable(test_execute_exp ${SRC_LIST} test/test_exp_execute.cpp)

# 配置测试
enable_testing()
add_test(
    NAME test_read
    COMMAND test_read
) 

add_test(
    NAME test_exp
    COMMAND test_execute_exp
)