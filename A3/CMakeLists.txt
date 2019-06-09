cmake_minimum_required(VERSION 2.8)
project(a2ece650)

set(CMAKE_INSTALL_PREFIX .)


## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# create the main executable
## add additional .cpp files if needed
add_executable(ece650-a2 src/a2ece650.cpp)
add_executable(ece650-a3 src/a3ece650.cpp)
add_executable(rgen src/rgen.cpp)

file(GLOB TEST_DATA "src/*.txt")

install(TARGETS ece650-a2 ece650-a3 rgen DESTINATION run/bin)

install(FILES src/a1ece650.py
		DESTINATION run/bin
		PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

install(FILES ${TEST_DATA} DESTINATION run/bin)
