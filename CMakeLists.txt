cmake_minimum_required(VERSION 3.10)
project(Fibsonisheaf C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
find_package(Threads REQUIRED)

option(DEBUG "Enable debug output (scientific notation printing)" OFF)
option(REPORT "Enable time logging to file" OFF)

# Set output directory for all binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# List of each Fibonacci implementation with its own main()
set(FIB_VARIANTS
    builtin_gmp
    iterative
    matrix_v1
    matrix_v2
    naive
    fastdbl
    fastdbl_v2
)

# Loop through and create each as an individual executable
foreach(variant IN LISTS FIB_VARIANTS)
    add_executable(${variant} ${variant}.c main.c)
    target_link_libraries(${variant} gmp Threads::Threads)

    if (DEBUG)
        target_compile_definitions(${variant} PRIVATE DEBUG)
    endif()
    if (REPORT)
        target_compile_definitions(${variant} PRIVATE REPORT)
    endif()
endforeach()
