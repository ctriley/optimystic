project(optimystic)

set(SOURCE_FILES
        model.cpp
        variable.cpp
        constraint.cpp)

add_library(${OPTIMYSTIC_LIB} SHARED ${SOURCE_FILES})
target_link_libraries(${OPTIMYSTIC_LIB} ${requiredlibs} ${GUROBI_LIBRARIES})