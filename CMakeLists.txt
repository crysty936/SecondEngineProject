# run cmake .. in the build folder

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(SecondEngineProject VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Plugins/glfw-3.3.4)
# add the glfw lib to the linking process so that the includes are added.
list(APPEND extra_libs glfw)

# add Glad
add_subdirectory(Plugins/glad)
list(APPEND extra_libs glad)

add_subdirectory(Plugins/EASTL)
list(APPEND extra_libs EASTL)

# used to add all .cpp and .h files under source
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Engine/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Engine/*.h")

# Add stb_image
list(APPEND EXTRA_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/Plugins/stb_image/")

# Add GLM
list(APPEND EXTRA_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/Plugins/glm/")

# used to create filters for all files one to one with their folder structure
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${source_files})

add_executable(SecondEngineProject ${source_files})

target_link_libraries(SecondEngineProject PUBLIC ${extra_libs})

list(APPEND EXTRA_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/Engine/Source)	

target_include_directories(SecondEngineProject PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
						   

						   

# Stop it from creating unnecessary Project
set(CMAKE_SUPPRESS_REGENERATION true)



# GTest done so that it can be used with lib and .exe by creating a copy of the original project, without the main

# Build with MDd
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Do not create GMock project
option(BUILD_GMOCK OFF)

ADD_SUBDIRECTORY(Plugins/googletest-main)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(GoogleTest)

list(APPEND source_files UnitTests/test.cpp)

get_filename_component(full_path_test_cpp ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/EntryPoint/EntryPoint.cpp ABSOLUTE)

list(REMOVE_ITEM source_files "${full_path_test_cpp}")
add_executable(EngineTests ${source_files})

target_include_directories(EngineTests PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )



target_link_libraries(EngineTests PUBLIC gtest gtest_main ${extra_libs})

gtest_discover_tests(EngineTests)
