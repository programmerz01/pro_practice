cmake_minimum_required(VERSION 3.5)

# 添加解释器及服务器相关的源文件
file(GLOB INTERPRETER_SOURCE "${CMAKE_SOURCE_DIR}/src/interpreter/*.cpp")
file(GLOB SERVER_SOURCE "${CMAKE_SOURCE_DIR}/src/server/*.cpp")

# 添加测试以及库依赖
# 读取脚本测试
add_executable(test_read ${INTERPRETER_SOURCE} 
                        ${CMAKE_SOURCE_DIR}/test/test_read_script.cpp)
target_include_directories(test_read PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(test_read PRIVATE cxx_std_20)

# 表达式执行测试
add_executable(test_execute_exp ${INTERPRETER_SOURCE} 
                                ${CMAKE_SOURCE_DIR}/test/test_exp_execute.cpp)
target_include_directories(test_execute_exp PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(test_execute_exp PRIVATE cxx_std_20)

# 函数执行测试
add_executable(test_execute_funcs ${INTERPRETER_SOURCE} 
                                ${CMAKE_SOURCE_DIR}/test/test_functions.cpp)
target_include_directories(test_execute_funcs PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(test_execute_funcs PRIVATE cxx_std_20)

# 日志编写测试
add_executable(test_log ${CMAKE_SOURCE_DIR}/test/test_log.cpp 
                        ${SERVER_SOURCE})
target_include_directories(test_log PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(test_log PRIVATE cxx_std_20)

# 线程池及数据库池测试
add_executable(test_pool ${SERVER_SOURCE} 
                        ${CMAKE_SOURCE_DIR}/test/test_pool.cpp)
target_compile_features(test_pool PRIVATE cxx_std_20)
target_include_directories(test_pool PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_pool /usr/lib/x86_64-linux-gnu/libmysqlclient.so /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so)

# 测试http连接
# request
add_executable(test_request ${SERVER_SOURCE} 
                            ${CMAKE_SOURCE_DIR}/test/test_http_request.cpp)
target_compile_features(test_request PRIVATE cxx_std_20)
target_include_directories(test_request PUBLIC ${CMAKE_SOURCE_DIR}/include)

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

add_test(
    NAME test_func
    COMMAND test_execute_funcs
)

add_test(
    NAME test_log
    COMMAND test_log
)