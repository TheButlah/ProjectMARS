cmake_minimum_required(VERSION 3.0.0)
project(mars)

set(CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -Wno-reorder")
endif()

# Include all header files in the "include" directory
include_directories(include)

# We glob all the source files using a wildcard, and store the in the SOURCES variable
file(GLOB SOURCES "src/*.cpp")

if (NOT WIN32)
  #X11
  find_package(X11 REQUIRED)
  link_libraries(${X11_LIBRARIES})
  include_directories(${X11_INCLUDE_DIR})

  #pthreads
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  link_libraries(Threads::Threads)
endif()



# Build an excutable with our sources (SOURCES) and a main file
add_executable(cli ${SOURCES} main/cli.cpp)
if (NOT WIN32)
  add_executable(clustering ${SOURCES} main/clustering.cpp)
endif()

if (APPLE)
  #target_include_directories(cli PUBLIC /opt/X11/include)
  #target_link_libraries(cli)
endif()


##############
# GoogleTest #
##############

# https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(external/GTest.CMakeLists.txt.in googletest-download/CMakeLists.txt)
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
add_executable(test_question_mark ${SOURCES} main/tests.cpp)
target_link_libraries(test_question_mark gtest_main)

if (NOT WIN32)
  # Python bindings using pybind11
  add_subdirectory(lib/pybind11)
  pybind11_add_module(project_mars ${SOURCES} "main/bindings.cpp")
endif()