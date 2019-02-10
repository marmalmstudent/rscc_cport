cmake_minimum_required( VERSION 3.0 )
enable_language( C )
project( rscc_cport )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
  add_definitions( -O2 )
elseif ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions( -Og -g3 -ggdb )
endif ( )
add_definitions( -std=gnu11 -Wall -Wextra -Wpedantic )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/" )

# Lets LOAD app our headers!
file( GLOB_RECURSE HDRS ${CMAKE_SOURCE_DIR}/src/*.h )

# Lets LOAD app our sources!
file( GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.c )

configure_file( configuration/root_directory.h.in configuration/root_directory.h @ONLY@ )
include_directories( ${CMAKE_BINARY_DIR}/configuration )

# Define the executable
add_executable( ${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS} )

#######################################
# LOOK for the packages that we need! #
#######################################

find_package( SDL2 REQUIRED )
find_package( sodium REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( SDL2_IMAGE REQUIRED SDL2_image )
pkg_search_module( SDL2_TTF REQUIRED SDL2_ttf )

# Define the include DIRs
include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${sodium_INCLUDE_DIR}
  include
  )

# Define the link libraries
target_link_libraries( ${CMAKE_PROJECT_NAME}
  ${SDL2_LIBRARIES}
  ${sodium_LIBRARY_DEBUG}
  ${sodium_LIBRARY_RELEASE}
  gmp
  pthread
  )

