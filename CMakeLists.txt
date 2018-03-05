cmake_minimum_required(VERSION 3.0.0)
project(mars)

set (CMAKE_CXX_STANDARD 11)

# Include all header files in the "include" directory
include_directories(include)
 
# We glob all the source files using a wildcard, and store the in the SOURCES variabel
file(GLOB SOURCES "src/*.cpp")

# Build an excutable with our sources (SOURCES) and a main file
add_executable(run-tests ${SOURCES} main/tests.cpp)

