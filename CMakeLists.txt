#@+leo-ver=5-thin
#@+node:gcross.20101224191604.2593: * @thin CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (CodeCategorize)

find_package( Boost 1.43 COMPONENTS thread )
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

if( NOT CMAKE_BUILD_TYPE )
set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

message (STATUS "Build type is " ${CMAKE_BUILD_TYPE})

include_directories ("includes")

add_subdirectory ("sources")
add_subdirectory ("tests")
#@-leo
