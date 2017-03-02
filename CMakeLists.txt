# I think version 2.8.10 is good enough

cmake_minimum_required (VERSION 2.8.10)
cmake_policy(VERSION 2.8.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #GCC with c++14 support (I think 4.9) required
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(FATAL_ERROR "GCC version must be at least 4.9.")
    endif()
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -g")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
    #CLANG with c++14 support for auto is 3.8 methinks
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.8)
        message(FATAL_ERROR "Clang version must be at least 3.8. Sorry. I might make one that works on c++11 but not right now.")
    endif()
else()
    #If the person is doing this it's fine but they get to deal with their own problems when it breaks
    message (WARNING "You are using an unsupported compiler. GCC is recommended for this program, even on Windows. If you do have any errors, do leave a report though")
endif()

set(PROJECT_NAME openlifecli)
set(PROJECT_LIB  openlife)
set(TEST_NAME    openlifetests)
set(PROJECT_GUI  openlifegui)


file(GLOB LIB_SOURCES
  "${CMAKE_SOURCE_DIR}/src/libopenlife/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/libopenlife/*.hpp")

file (GLOB PROG_SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/arena.cpp"
    "${CMAKE_SOURCE_DIR}/src/arena.hpp")

file (GLOB TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/unittests/*.cpp" # Skip Compiler.hpp and Compiler.cpp, which include int main()
    "${CMAKE_SOURCE_DIR}/unittests/*.hpp")

file (GLOB GUI_SOURCES
    "${CMAKE_SOURCE_DIR}/src/lifeboard/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/lifeboard/*.hpp")


add_library(${PROJECT_LIB} SHARED "${LIB_SOURCES}")
add_executable(${PROJECT_NAME} ${PROG_SOURCES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})

add_executable(${TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_NAME} ${PROJECT_LIB})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

### We can print out our own warning. This isn't really an error.
find_package(SFML 2 QUIET OPTIONAL_COMPONENTS system window graphics)
if(SFML_FOUND)
    add_executable(${PROJECT_GUI} ${GUI_SOURCES})
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_GUI} ${PROJECT_LIB})
    target_link_libraries(${PROJECT_GUI} ${SFML_LIBRARIES})
else()
    message(WARNING "SFML 2 not found. Will NOT compile the gui version")
endif()


