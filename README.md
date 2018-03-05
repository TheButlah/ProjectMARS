# Project MARS

Project MARS is an exploration of near-optimally designing electricity grids amidst a growing population of people. We model several growth patterns for people spreading across a terrain, and hope to learn how to place power stations to support the population. We factor in population density, financial constraints, power station supply constraints, and consumer preference to model the economics and social benefit behind how power stations are built in the real world.

## Requirements

You will need a C++ compiler, such as `g++` or `clang++`, that supports C++11. You will also need CMake (`cmake`) installed.

## Build

We use CMake for our build system. As such, you will need to create a `build` directory from which to run CMake and build executables.

After cloning, create the `build` directory.
```
mkdir build
```

After that, to build:
```
cd build   # Assuming you're at the root of the project
cmake ..   # Run CMake in the directory above???
make       # PSYCH, CMake can't get the job done alone
```