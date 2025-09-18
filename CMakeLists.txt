cmake_minimum_required(VERSION 3.10)
project(LifeGame)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 直接添加 v1 目录中的源文件
file(GLOB SOURCES "v1/*.cpp")

# 创建可执行文件
add_executable(LifeGame ${SOURCES})

# 包含 v1 目录（如果头文件在那里）
target_include_directories(LifeGame PRIVATE ${CMAKE_SOURCE_DIR}/v1)
