cmake_minimum_required(VERSION 3.9)

project(server)

file(GLOB filesCPP "*.cpp")
file(GLOB filesH "*.h")
add_executable(${PROJECT_NAME} ${filesCPP} ${filesH})

add_definitions(-std=c++14)

set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(Boost COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
