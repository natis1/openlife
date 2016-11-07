# I think version 2.8.10 is good enough

cmake_minimum_required (VERSION 2.8.10)
cmake_policy(VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #GCC with c++14 support (I think 4.9) required
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(FATAL_ERROR "GCC version must be at least 4.9. Sorry. I might make one that works on c++11 but not right now.")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
    #CLANG with c++14 support for auto is 3.8 methinks
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.8)
        message(FATAL_ERROR "Clang version must be at least 3.8. Sorry. I might make one that works on c++11 but not right now.")
    endif()
else()
    #If the person is doing this it's fine but they get to deal with their own problems when it breaks
    message (WARNING "You are using an unsupported compiler. GCC is recommended for this program, even on Windows. If you do have any errors, do leave a report though")
endif()

set(PROJECT_NAME openlife)
set(PROJECT_LIB  openlifelib)
set(TEST_NAME    openlifetests)

file(GLOB_RECURSE LIB_SOURCES
  "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*/*.hpp")

file (GLOB_RECURSE PROG_SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.hpp")

file (GLOB_RECURSE TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/tests/*.cpp" # Skip Compiler.hpp and Compiler.cpp, which include int main()
    "${CMAKE_SOURCE_DIR}/tests/*.hpp")

add_library(${PROJECT_LIB} SHARED "${LIB_SOURCES}")
add_executable(${PROJECT_NAME} ${PROG_SOURCES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})
add_executable(${TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()


