cmake_minimum_required(VERSION 3.10)

# Set project name and version number
project(ccAnimation VERSION 1.0)

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories ("${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src)

file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.hpp)
install(FILES ${INCLUDE_FILES} DESTINATION include/ccanimation)

# Enable testing
enable_testing()
add_subdirectory(test)