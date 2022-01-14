# run cmake .. in the build folder

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(FlappyTest VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(.. ${CMAKE_CURRENT_LIST_DIR}/EngineBuild)
list(APPEND extra_libs SecondEngineProject)

# used to add all .cpp and .h files under source
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Source/*.h")

# used to create filters for all files one to one with their folder structure
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${source_files})

add_executable(FlappyTest ${source_files})

target_link_libraries(FlappyTest PUBLIC ${extra_libs})



