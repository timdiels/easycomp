cmake_minimum_required (VERSION 2.8)
project (easycomp)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

############################################
# find libs

# e.g. find_package(ALGLIB REQUIRED)

############################################

include_directories(BEFORE ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")
set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -std=c++11 ${SDL_CFLAGS}")

file(GLOB_RECURSE Sources easycomp/*.cpp)
add_executable(easycomp ${Sources})
target_link_libraries(easycomp 
    ${LIBS} 
)


file(GLOB_RECURSE Sources2 prototypes/expression_performance/*.cpp)
add_executable(prototype_expression_performance ${Sources2})
target_link_libraries(prototype_expression_performance
    ${LIBS} 
)
