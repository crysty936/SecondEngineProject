cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0 )

# add the executable
add_executable(Tutorial tutorial.cxx)


configure_file(ConfigTutorial.h.in ConfigTutorialGenerated.h)

target_include_directories(Tutorial PUBLIC
						"${PROJECT_BINARY_DIR}"
						)
						
						