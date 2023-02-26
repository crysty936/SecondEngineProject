# run cmake .. in the build folder

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(3DTest VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(../.. ${CMAKE_CURRENT_LIST_DIR}/EngineBuild)
list(APPEND extra_libs SecondEngineProject)

# used to add all .cpp and .h files under source
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Source/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Source/*.h")

# used to create filters for all files one to one with their folder structure
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${source_files})

add_executable(3DTest ${source_files})

target_link_libraries(3DTest PUBLIC ${extra_libs})

list(APPEND EXTRA_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/Source)	

target_include_directories(3DTest PUBLIC
                           ${EXTRA_INCLUDES}
                           )

# Stop it from creating unnecessary Project
set(CMAKE_SUPPRESS_REGENERATION true)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 3DTest)

