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

# add Glad
add_subdirectory(Plugins/glad)
list(APPEND EXTRA_LIBS glad)

#add EASTL
set(EASTL_ROOT_DIR Plugins/EASTL)
# include_directories (${EASTL_ROOT_DIR}/include)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EAAssert/include)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EABase/include/Common)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EAMain/include)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EAStdC/include)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EATest/include)
# include_directories (${EASTL_ROOT_DIR}/test/packages/EAThread/include)
# set(EASTL_LIBRARY debug ${EASTL_ROOT_DIR}/build/Debug/EASTL.lib optimized ${EASTL_ROOT_DIR}/build/Release/EASTL.lib)
# add_custom_target(NatVis SOURCES ${EASTL_ROOT_DIR}/doc/EASTL.natvis)

# list(APPEND EXTRA_LIBS ${EASTL_LIBRARY})

add_subdirectory(Plugins/EASTL)
list(APPEND EXTRA_LIBS EASTL)

# used to add all .cpp and .h files under source
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