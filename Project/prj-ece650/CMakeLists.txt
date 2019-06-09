cmake_minimum_required(VERSION 2.8)
project(prj-ece650)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
	add_subdirectory(minisat)
else()
	message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++0x -Wall ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## add additional .cpp files if needed
add_executable(prj-ece650 prj-ece650.cpp)
add_executable(prj-measure prj-measure.cpp)

# link MiniSAT libraries
target_link_libraries(prj-ece650 pthread minisat-lib-static)
target_link_libraries(prj-measure pthread minisat-lib-static)

# create the executable for tests
add_executable(test test.cpp)

# link MiniSAT libraries
target_link_libraries(test pthread minisat-lib-static)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/graphGen
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/measure.sh
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
