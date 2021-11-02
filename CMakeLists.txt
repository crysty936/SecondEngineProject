# run cmake .. in the build folder

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(SecondEngineProject VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Plugins/glfw-3.3.4)
# add the glfw lib to the linking process so that the includes are added.
list(APPEND EXTRA_LIBS glfw)

# used to add all cpp files under source
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Engine/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Engine/*.h")

# used to create filters for all files one to one with their folder structure
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${source_files})

add_executable(SecondEngineProject ${source_files})

target_link_libraries(SecondEngineProject PUBLIC ${EXTRA_LIBS})

target_include_directories(SecondEngineProject PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
						   
include_directories(${CMAKE_CURRENT_LIST_DIR}/Engine/Source)
						   

set(CMAKE_SUPPRESS_REGENERATION true)