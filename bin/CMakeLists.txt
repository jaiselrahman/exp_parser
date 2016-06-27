cmake_minimum_required ( VERSION 3.5.2)
project(exp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG -g2)
set(CMAKE_CXX_FLAGS_RELEASE -o3)
set(EXECUTABLE_OUTPUT_PATH $ENV{CPRJ}/exp/bin)
set(CMAKE_BUILD_TYPE DEBUG)
include_directories (../include)
file ( GLOB SOURCES "../src/*.cpp" )

#set(MVG_SOURCE "mvg.cpp")
# Build Library
#add_library(mvg STATIC ${MVG_SOURCE})
#set_target_properties(mvg PROPERTIES 
#ARCHIVE_OUTPUT_DIRECTORY
#${CMAKE_SOURCE_DIR}/lib)

add_executable (exp ${SOURCES})

install(TARGETS exp 
DESTINATION $ENV{HOME}/bin)
