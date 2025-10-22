cmake_minimum_required(VERSION 3.21)
project(graphs VERSION 1.0.0 LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Static library" FORCE)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(graphs_lib STATIC ${SOURCES})

set_target_properties(graphs_lib PROPERTIES
  OUTPUT_NAME graphs POSITION_INDEPENDENT_CODE ON)

target_include_directories(graphs_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
  $<INSTALL_INTERFACE:include>)

add_subdirectory(submodules/check)
enable_testing()
add_subdirectory(test)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "Target architecture: 64-bit")
  add_compile_definitions(BUILD_64BIT=1)
else()
  message(STATUS "Target architecture: 32-bit")
  add_compile_definitions(BUILD_32BIT=1)
endif()

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
  set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
  set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -flto")
endif()
