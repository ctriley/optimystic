cmake_minimum_required(VERSION 3.5.1)
project(optimystic VERSION "1.0.0" DESCRIPTION "A CPP GUROBI/CPLEX Interface")

configure_file(${PROJECT_SOURCE_DIR}/config.hpp.in ${PROJECT_BINARY_DIR}/config.hpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        MESSAGE("CODE COVERAGE")
        INCLUDE(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
    endif()
    SET(CMAKE_VERBOSE_MAKEFILE on)
    add_definitions(-pedantic -Wall)
endif()

MESSAGE("ENV: " "$ENV{GUROBI_HOME}")
set(OPTIMYSTIC_LIB optimystic_lib)

FIND_PACKAGE(GUROBI)
FIND_PACKAGE(CPLEX)

if(GUROBI_FOUND)
    INCLUDE_DIRECTORIES("${GUROBI_INCLUDE_DIR}")
    add_definitions(-DGUROBI)
endif()


if(CPLEX_FOUND)
    add_definitions(-lilocplex -lconcert -lcplex -lm -lpthread)
    include_directories(cplex PRIVATE ${CPLEX_INCLUDE_DIR})
    include_directories(cplex PRIVATE ${CPLEX_ILOCPLEX_INCLUDE_DIR})
    include_directories(cp PRIVATE ${CPLEX_CP_INCLUDE_DIR})
    include_directories(concert PRIVATE ${CPLEX_CONCERT_INCLUDE_DIR})
    add_definitions(-DIL_STD )
    SET(requiredlibs ${requiredlibs} cplex-library cplex-concert ilocplex cplex-cp dl)
    add_definitions(-DCPLEX)
endif()

# download google or-tools at configure time
include(cmake/DownloadProject.cmake)

add_subdirectory(src)
include_directories(src)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_subdirectory(test)
    endif()
endif()

# include so we find the config.hpp file
include_directories(${PROJECT_BINARY_DIR})