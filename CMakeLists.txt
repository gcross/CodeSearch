#@+leo-ver=5-thin
#@+node:gcross.20101224191604.2593: * @thin CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (CodeCategorize)

SET(Boost_ADDITIONAL_VERSIONS "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0")

find_package( Boost 1.45 COMPONENTS thread program_options REQUIRED )
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

if(UNIX)
    if(CMAKE_COMPILER_IS_GNUCC)
         add_definitions(-Wall -Wno-strict-aliasing)
    endif(CMAKE_COMPILER_IS_GNUCC)
endif(UNIX)

if( NOT CMAKE_BUILD_TYPE )
set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

message (STATUS "Build type is " ${CMAKE_BUILD_TYPE})

include_directories ("includes")

add_subdirectory ("sources")
add_subdirectory ("tests")
#@-leo
