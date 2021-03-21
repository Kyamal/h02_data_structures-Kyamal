# This is the top-level CMakeLists.txt file

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(H02_data_structures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# define libraries
add_library(adt_lib STATIC
        include/element.hpp
        include/private/internal.hpp
        src/array_list.cpp include/array_list.hpp
        src/linked_list.cpp include/linked_list.hpp)

target_include_directories(adt_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# setting up compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ADT_COMPILE_OPTS "-pipe;-fpie;-Werror;-Wall;-Wextra;-Wpedantic;-Wshadow;-Wno-unused-parameter")
    set(ADT_DEBUG_COMPILE_OPTS "-g;${ADT_COMPILE_OPTS}")
    set(ADT_RELEASE_COMPILE_OPTS "-O2;${ADT_COMPILE_OPTS}")

    message(STATUS "Applying GNU compiler opts for static library (DEBUG): ${ADT_DEBUG_COMPILE_OPTS}")
    message(STATUS "Applying GNU compiler opts for static library (RELEASE): ${ADT_RELEASE_COMPILE_OPTS}")

    target_compile_options(adt_lib PUBLIC "$<$<CONFIG:DEBUG>:${ADT_DEBUG_COMPILE_OPTS}>")
    target_compile_options(adt_lib PUBLIC "$<$<CONFIG:RELEASE>:${ADT_RELEASE_COMPILE_OPTS}>")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    message("MSVC")
endif ()

# define executables
add_executable(main main.cpp)
target_link_libraries(main PRIVATE adt_lib)

# dependencies
add_subdirectory(contrib)

# tests
enable_testing()
add_subdirectory(tests)