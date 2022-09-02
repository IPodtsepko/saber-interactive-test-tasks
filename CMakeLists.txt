cmake_minimum_required(VERSION 3.13)

# General compile and link options
set(COMPILE_OPTS -O3 -Wall -Wextra -Werror -pedantic -pedantic-errors)
set(LINK_OPTS "")

# Configure clang-tidy
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

function(setup_warnings TARGET)
    # Warnings
    target_compile_options(${TARGET} PUBLIC -Wold-style-cast)
    target_compile_options(${TARGET} PUBLIC -Wnull-dereference)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${TARGET} PUBLIC -Wduplicated-branches)
        target_compile_options(${TARGET} PUBLIC -Wduplicated-cond)
        target_compile_options(${TARGET} PUBLIC -Wsuggest-override)
        target_compile_options(${TARGET} PUBLIC -Wuseless-cast)
        target_compile_options(${TARGET} PUBLIC -Wreturn-local-addr)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    endif()
endfunction(setup_warnings)

set(PROJECT_NAME solutions)
project(${PROJECT_NAME})

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Separate executable: main
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Compile source files into a library
add_library(solutions_lib ${SRC_FILES})
target_compile_options(solutions_lib PUBLIC ${COMPILE_OPTS})
target_link_options(solutions_lib PUBLIC ${LINK_OPTS})
setup_warnings(solutions_lib)

# Main is separate
add_executable(solutions ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_compile_options(solutions PRIVATE ${COMPILE_OPTS})
target_link_options(solutions PRIVATE ${LINK_OPTS})
setup_warnings(solutions)

# linking Main against the library
target_link_libraries(solutions solutions_lib)

# testing
enable_testing()

add_subdirectory(googletest)
add_subdirectory(test)

add_test(NAME tests COMMAND runUnitTests)
