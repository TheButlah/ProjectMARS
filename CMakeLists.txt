cmake_minimum_required(VERSION 3.0.0)
project(mars)

set (CMAKE_CXX_STANDARD 11)

# Include and link SDL libraries for Windows 10

# Include all header files in the "include" directory
include_directories(include)
 
# We glob all the source files using a wildcard, and store the in the SOURCES variable
file(GLOB SOURCES "src/*.cpp")

# Build an excutable with our sources (SOURCES) and a main file
add_executable(graphics main/graphics.cpp)
if (WIN32)  
  target_link_libraries(graphics mingw32 SDL2main SDL2)
else()
  target_link_libraries(graphics SDL2main SDL2)  
endif()

# https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
if (WIN32)
  execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_MAKE_PROGRAM=mingw32-make -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
else()
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
endif()
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()


execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_question_mark ${SOURCES} main/tests.cpp include/PopulationGen.h)
target_link_libraries(test_question_mark gtest_main)
add_test(NAME mars_test COMMAND test_question_mark)
