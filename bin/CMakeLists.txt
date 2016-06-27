cmake_minimum_required ( VERSION 3.5.2)

set(CMAKE_CXX_FLAGS_DEBUG -g2)
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH $ENV{CPRJ}/exp/bin)

FIND_PACKAGE(Git)

execute_process(
COMMAND git describe --abbrev=4 HEAD
COMMAND bash -c "awk  -F\"-\" '{print $1\".\"$2}'"
OUTPUT_VARIABLE exp_VERSION
OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX REPLACE "^v(.*)" "\\1" exp_VERSION ${exp_VERSION})

execute_process( COMMAND bash -c "echo \"${exp_VERSION}\" > ../bin/version")

project(exp
VERSION ${exp_VERSION})

configure_file ("../include/ExpConfig.h.in"  "../include/ExpConfig.h"  )

IF(BUILD STREQUAL R)
	set(CMAKE_BUILD_TYPE RELEASE)
ELSE (IF(BUILD STREQUAL D))
	set(CMAKE_BUILD_TYPE DEBUG)
ENDIF()

include_directories(../include)

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

