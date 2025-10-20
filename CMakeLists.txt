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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
  set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
  set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -flto")
endif()
