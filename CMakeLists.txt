cmake_minimum_required(VERSION 3.10)
project(Fibsonisheaf C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Optional flags
option(DEBUG "Enable debug output (scientific notation printing)" OFF)
option(REPORT "Enable time logging to file" OFF)

# Source files
set(SOURCES
    main.c    # or whatever your source file is called
    binet.c
    gmp.c
    iterative_v2.c
    iterative.c
    matmul.c
    matrix_v2.c
    matrix_v1.c
    naive.c
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link GMP
target_link_libraries(${PROJECT_NAME} gmp)

<<<<<<< HEAD
add_compile_options(-march=native -mtune=native -pipe -flto -O3 -funroll-loops)
=======
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
# Preprocessor definitions for optional flags
if (DEBUG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()

if (REPORT)
    target_compile_definitions(${PROJECT_NAME} PRIVATE REPORT)
endif()
