cmake_minimum_required(VERSION 3.10)
project(tinytor C)

set(CMAKE_C_STANDARD 99)

add_executable(tinytor main.c)
set_source_files_properties(  ${SRC_DIR}  PROPERTIES  COMPILE_FLAGS "-w")
target_compile_options(tinytor PRIVATE -Wno-cpp -Wno-int-conversion -w)

