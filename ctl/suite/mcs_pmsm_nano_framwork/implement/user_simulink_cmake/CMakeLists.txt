cmake_minimum_required(VERSION 3.10)

# 设置项目名称和语言
project(GMP_Simulink LANGUAGES CXX)

# Check Environment GMP_PRO_LOCATION
set(GMP_PRO_LOCATION $ENV{GMP_PRO_LOCATION})
if(NOT GMP_PRO_LOCATION)
    message(FATAL_ERROR "GMP_PRO_LOCATION environment variable is not set.")
endif()

include(gmp_make_tool.cmake)

# Create Target
add_executable(gmp_test.exe ${GMP_SOURCE_FILES})
