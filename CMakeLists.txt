cmake_minimum_required(VERSION 3.2)
project(tagdb)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost 1.55.0 COMPONENTS system filesystem program_options)
find_package(VSQLitePP 0.3.13)
find_package(PkgConfig)
pkg_check_modules(TAGLIB taglib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall")

include_directories(${TAGLIB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${SQLITE_INCLUDE_DIRS})
link_directories(${TAGLIB_LIBRARY_DIRS})

FILE(GLOB SOURCE_FILES src/*.cpp)
add_executable(tagdb ${SOURCE_FILES})
target_link_libraries(tagdb ${TAGLIB_LIBRARIES} ${Boost_LIBRARIES} vsqlitepp)

