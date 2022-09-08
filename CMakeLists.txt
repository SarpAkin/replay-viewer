cmake_minimum_required (VERSION 3.8)

project ("auto_tester_replay_viewer")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_compile_options(/Zc:__cplusplus /permissive-)

add_executable (replay_viewer "src/failed_replay.cpp" "src/main.cpp" "src/failed_replay.hpp" "src/util.hpp")

add_subdirectory(fmt)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

target_link_libraries(replay_viewer PRIVATE 
	Qt6::Widgets
	fmt::fmt
)