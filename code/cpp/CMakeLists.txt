
# CMake version
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For rtags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name
project(yodaChill)

FIND_PACKAGE( Boost 1.55.0 REQUIRED COMPONENTS system filesystem )
  find_package (Eigen3 3.3 REQUIRED NO_MODULE)

#Bring the headers, such as Student.h into the project
include_directories(include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#Can manually add the sources using the set command as follows:
set(SOURCES 
	src/main.cpp
  src/inputOutput.cpp 
	)
 
#However, the file(GLOB...) allows for wildcard additions:
# file(GLOB SOURCES "src/*.cpp")
 
add_executable(runYoda ${SOURCES})


TARGET_LINK_LIBRARIES( runYoda LINK_PUBLIC ${Boost_LIBRARIES} Eigen3::Eigen)
