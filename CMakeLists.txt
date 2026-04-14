# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Define project name and language
project(CroseProject LANGUAGES C)

# Set the C standard to C11 (optional but recommended)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create the executable 'crose' from 'main.c'
add_executable(crose main.c)
