cmake_minimum_required(VERSION 3.0.0)
project(mars)

set (CMAKE_CXX_STANDARD 11)

# Include and link SDL libraries for Windows 10
if (WIN32)  
  include_directories(sdl/include)
  link_directories(sdl/lib)
  file(COPY sdl/bin/SDL2.dll DESTINATION .)
endif()

# Include all header files in the "include" directory
include_directories(include)
 
# We glob all the source files using a wildcard, and store the in the SOURCES variabel
file(GLOB SOURCES "src/*.cpp")

# Build an excutable with our sources (SOURCES) and a main file
add_executable(run-tests ${SOURCES} main/tests.cpp include/PopulationGen.h)
add_executable(run-sdl-test main/sdl-test.cpp include/PopulationGen.h)
if (WIN32)  
  target_link_libraries(run-sdl-test mingw32 SDL2main SDL2)
else()
  target_link_libraries(run-sdl-test SDL2main SDL2)  
endif()
