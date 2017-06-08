cmake_minimum_required(VERSION 3.7)
project(SQL_Database_Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp record.cpp record.h table.cpp table.h map.h sql.cpp sql.h my_set.h array_funcs.h constants.h)
add_executable(SQL_Database_Project ${SOURCE_FILES})
