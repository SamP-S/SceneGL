cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

find_package(Git REQUIRED)
if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()

add_subdirectory("dependencies/tinyfiledialogs")
add_subdirectory("dependencies/imgui")
add_subdirectory("dependencies/la")

set(INCLUDE_DIRS
    "include" 
    "external/entt/include"
    "external/tinygltf/include"
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
message(STATUS "SRC_FILES: ${SRC_FILES}")

add_executable(graphics ${SRC_FILES})
target_include_directories(graphics PUBLIC ${INCLUDE_DIRS})

target_link_libraries(graphics PUBLIC tinyfiledialogs)
target_link_libraries(graphics PUBLIC imgui)
target_link_libraries(graphics PUBLIC la)
target_link_libraries(graphics PUBLIC ${CMAKE_DL_LIBS})

add_executable(ecs_test "test/ecs_test.cpp")
target_include_directories(ecs_test PUBLIC ${INCLUDE_DIRS})
