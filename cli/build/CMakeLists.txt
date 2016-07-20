cmake_minimum_required ( VERSION 3.5.2)
set(CMAKE_CXX_FLAGS_DEBUG -g2)
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH .)

FIND_PACKAGE(Git)

execute_process(
COMMAND git describe --long
COMMAND bash -c "awk  -F\"-\" '{print $1\".\"$2}' "
OUTPUT_VARIABLE exp_VERSION
OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX REPLACE "^v(.*)" "\\1" exp_VERSION ${exp_VERSION})

execute_process( COMMAND bash -c "echo \"${exp_VERSION}\"")

project(exp
VERSION ${exp_VERSION})

configure_file ("exp_version.h.in"  "../../exp_version.h"  )

IF(BUILD STREQUAL R)
	set(CMAKE_BUILD_TYPE RELEASE)
ELSE (IF(BUILD STREQUAL D))
	set(CMAKE_BUILD_TYPE DEBUG)
ENDIF()

include_directories(../.. . ../../mpreal)
file(GLOB SOURCES ../src/main.cpp )

add_executable (exp ${SOURCES})
target_link_libraries(exp  mpfr gmp)
install(TARGETS exp 
DESTINATION $ENV{HOME}/bin)

