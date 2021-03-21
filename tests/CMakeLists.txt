# ctest -VV
include(CTest)

# Catch2 cmake helper functions
include(${PROJECT_SOURCE_DIR}/contrib/Catch2/contrib/Catch.cmake)

set(TARGET_NAME run_tests)

add_executable(${TARGET_NAME} runner_tests.cpp array_list_tests.cpp linked_list_tests.cpp)

target_include_directories(${TARGET_NAME} PRIVATE utility)

target_link_libraries(${TARGET_NAME} PRIVATE adt_lib)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TESTS_DEBUG_COMPILE_OPTS "-Og")  # workaround for GCC's "too many sections bullshit"
    set(TESTS_RELEASE_COMPILE_OPTS "-O2")

    message(STATUS "Applying GNU GCC compile opts on unit tests (DEBUG): ${TESTS_DEBUG_COMPILE_OPTS}")
    message(STATUS "Applying GNU GCC compile opts on unit tests (RELEASE): ${TESTS_RELEASE_COMPILE_OPTS}")

    target_compile_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${TESTS_DEBUG_COMPILE_OPTS}>")
    target_compile_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${TESTS_RELEASE_COMPILE_OPTS}>")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
endif ()

# FakeIt
target_include_directories(${TARGET_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/contrib/FakeIt/single_header/catch)

# discover tests for CTest
catch_discover_tests(${TARGET_NAME} EXTRA_ARGS -r compact)
