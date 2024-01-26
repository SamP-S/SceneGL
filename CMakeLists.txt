cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

find_package(Git REQUIRED)
if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()

add_subdirectory("marathon/dependencies/tinyfiledialogs")
add_subdirectory("marathon/dependencies/imguizmo")
add_subdirectory("marathon/dependencies/la")

set(INCLUDE_DIRS
    "marathon/include" 
    "marathon/external/entt/include"
    "marathon/external/tinygltf/include"
)

file(GLOB_RECURSE SRC_FILES "marathon/src/**.cpp")
message(STATUS "SRC_FILES: ${SRC_FILES}")

add_executable(graphics ${SRC_FILES})
target_include_directories(graphics PUBLIC ${INCLUDE_DIRS})

target_link_libraries(graphics PUBLIC tinyfiledialogs)
target_link_libraries(graphics PUBLIC imguizmo)
target_link_libraries(graphics PUBLIC la)
target_link_libraries(graphics PUBLIC ${CMAKE_DL_LIBS})

add_executable(ecs_test "test/ecs_test.cpp")
target_include_directories(ecs_test PUBLIC ${INCLUDE_DIRS})
target_link_libraries(ecs_test PUBLIC la)

add_executable(la_test "test/la_test.cpp")
target_link_libraries(la_test PUBLIC la)

add_executable(imguizmo_test "test/imguizmo_test.cpp")
target_link_libraries(imguizmo_test PUBLIC la)
target_link_libraries(imguizmo_test PUBLIC imguizmo)
