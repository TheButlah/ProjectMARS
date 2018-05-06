# Project MARS

Project MARS is an exploration of near-optimally designing electricity grids amidst a growing population of people. We model several growth patterns for people spreading across a terrain, and hope to learn how to place power stations to support the population. We factor in population density, financial constraints, power station supply constraints, and consumer preference to model the economics and social benefit behind how power stations are built in the real world.

## Installing SDL2 (tested on Windows 10)

We use SDL2 to display our graphics. For now, we do a sort of "manual" install of the development library since this has only been tested on Windows 10.

1. At the [site](https://www.libsdl.org/download-2.0.php), go to the "Development Libraries" section.
2. Download and unzip `SDL2-devel-2.0.8-mingw.tar.gz` completely.
3. In it there's a folder called `x86_64-w64-mingw32`, drag it into this project and rename it to `sdl`.
4. You should be able to build our SDL executable using the instructions below.

## Requirements

You will need a C++ compiler, such as `g++` or `clang++`, that supports C++11. You will also need CMake (`cmake`) installed.

## Build

We use CMake for our build system. As such, you will need to create a `build` directory from which to run CMake and build executables.

After cloning, create the `build` directory.
```
mkdir build
```

And create a basic configuration for running the simulator as a CLI.
```
cp config.ini.in config.ini
```

## Running the CLI

When running the command line interface, we must pass in a configuration file as an argument to the program. If you created the configuration file as described by the Build section, and you compiled the `cli` program in the `build` directory, then running the CLI should be as simple as:

```
./cli ../config.ini
```

## macOS and Linux

The steps to build on macOS and Linux are the simplest. Run the following commands:
```
cd build   # Assuming you're at the root of the project.
cmake ..   # Run CMake, and tell it to look for a CMakeLists.txt in the directory above.
make       # CMake generates Makefiles for use to make.
```

## Windows 10 + MinGW (no Git Bash)

On Windows 10 with MinGW *but no Git Bash*, CMake must be told to use the MinGW generator so it can take advantage of the MinGW toolset. The formal commands:
```
cmake .. -G "MinGW Makefiles"
```
which has been placed in `build-mingw.bat` for convenience.

```
cd build             # Assuming you're at the root of the project
..\build-mingw.bat   # Run CMake, and specify that we need MinGW-compatible makefiles
mingw32-make         # Use MinGW's make tool, which is called mingw32-make. Why? Lord knows.
```


## Windows 10 + MinGW + Git Bash

On Windows 10 with MinGW and Git Bash, CMake will complain about how `sh.exe` is on the path when running the previous commands. We need to use the MSYS generator for CMake, but specify the `make` tool manually for CMake. Sounds overcomplicated? I agree.

The command is:
```
cmake .. -DCMAKE_MAKE_PROGRAM=mingw32-make -G "MSYS Makefiles"
```
which has been placed in `build-mingw-sh.bat` for convenience

```
cd build                # Assuming you're at the root of the project
..\build-mingw-sh.bat   # This is much shorter than above :D
mingw32-make            # Use MinGW's make tool
```
