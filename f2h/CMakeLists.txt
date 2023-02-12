# CMakeList.txt : CMake project for FileToHeader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("FileToHeader")

set (CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (FileToHeader "FileToHeader.cpp")

# TODO: Add tests and install targets if needed.
