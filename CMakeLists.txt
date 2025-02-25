cmake_minimum_required(VERSION 3.10)
project(CR_STD_LIB C)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Directories
set(SRC_DIR src)
set(INC_DIR include)
set(BIN_DIR bin)
set(TEST_DIR tests)

# Installation directories (Change For Your System)
if(WIN32)
    set(LIB_DEST_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(INCLUDE_DEST_DIR "${CMAKE_INSTALL_PREFIX}/include")
else()
    set(LIB_DEST_DIR "/usr/local/lib")
    set(INCLUDE_DEST_DIR "/usr/local/include")
endif()

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR})

# Library and Executable Names
set(LIB_NAME cr_std)
set(EXEC main)
set(TEST_EXEC test)

# Find source files
file(GLOB SRCS "${SRC_DIR}/*.c")
file(GLOB TEST_SRCS "${TEST_DIR}/*.c")
file(GLOB HEADERS "${INC_DIR}/*.h")

# Remove main.c from library sources (if it exists)
list(FILTER SRCS EXCLUDE REGEX ".*main\\.c$")

# Create static library
add_library(${LIB_NAME} STATIC ${SRCS})
target_include_directories(${LIB_NAME} PUBLIC ${INC_DIR})

# Create main executable
add_executable(${EXEC} src/main.c)
target_link_libraries(${EXEC} ${LIB_NAME})

# Create test executable
add_executable(${TEST_EXEC} ${TEST_SRCS})
target_link_libraries(${TEST_EXEC} ${LIB_NAME})
target_compile_definitions(${TEST_EXEC} PRIVATE CR_STD_TESTING_MODE)

# Debug flags (for debug builds)
# cmake .. -DCMAKE_BUILD_TYPE=Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${EXEC} PRIVATE -g -Wall -Werror -fsanitize=address)
    target_link_options(${EXEC} PRIVATE -fsanitize=address)
endif()

# Install rules (for library and headers)
install(TARGETS ${LIB_NAME} DESTINATION ${LIB_DEST_DIR})
install(FILES ${HEADERS} DESTINATION ${INCLUDE_DEST_DIR})
