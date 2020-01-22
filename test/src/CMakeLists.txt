project(optimystic-test)

set(SOURCE_FILES modeltest.cpp)

add_executable(optimystic-test ${SOURCE_FILES})
target_link_libraries(optimystic-test ${OPTIMYSTIC_LIB})

target_link_libraries(optimystic-test gtest gtest_main)

#add_test(NAME all COMMAND state-test)
install(TARGETS optimystic-test DESTINATION bin)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    SETUP_TARGET_FOR_COVERAGE(
            optimystic_coverage  # Name for custom target.
            optimystic-test         # Name of the test driver executable that runs the tests.
            # NOTE! This should always have a ZERO as exit code
            # otherwise the coverage generation will not complete.
            optimystic_coverage            # Name of output directory.
    )
endif()
