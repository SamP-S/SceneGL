cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# set opengl config
if (POLICY CMP0072)
  cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

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
message(STATUS "INCLUDE_DIRS: " ${INCLUDE_DIRS})

# file(GLOB_RECURSE SRC_FILES "marathon/src/**.cpp")
# message(STATUS "SRC_FILES: ${SRC_FILES}")

# game engine
add_library(marathon INTERFACE)
target_include_directories(marathon INTERFACE ${INCLUDE_DIRS})
target_link_libraries(marathon INTERFACE la)


# runtime
add_executable(sprinter "sprinter/src/sprinter.cpp")
target_include_directories(sprinter PUBLIC "sprinter/include")
target_link_libraries(sprinter PUBLIC marathon)

find_package(OpenGL REQUIRED)
target_include_directories(sprinter PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(sprinter PUBLIC ${OPENGL_LIBRARIES})
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries are ${OPENGL_LIBRARIES}")

find_package(SDL2 REQUIRED)
target_include_directories(sprinter PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(sprinter PUBLIC ${SDL2_LIBRARIES})
message(STATUS "SDL2 included at ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries are ${SDL2_LIBRARIES}")

find_package(GLEW REQUIRED)
target_include_directories(sprinter PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(sprinter PUBLIC ${GLEW_LIBRARIES})
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW libraries are ${GLEW_LIBRARIES}")



# editor
add_executable(spectator "spectator/src/main.cpp")
target_include_directories(spectator PUBLIC "spectator/include")
target_link_libraries(spectator PUBLIC tinyfiledialogs)
target_link_libraries(spectator PUBLIC imguizmo)
target_link_libraries(spectator PUBLIC marathon)

# test scripts
add_executable(ecs_test "test/ecs_test.cpp")
target_include_directories(ecs_test PUBLIC ${INCLUDE_DIRS})
target_link_libraries(ecs_test PUBLIC la)

add_executable(la_test "test/la_test.cpp")
target_link_libraries(la_test PUBLIC la)

add_executable(imguizmo_test "test/imguizmo_test.cpp")
target_link_libraries(imguizmo_test PUBLIC la)
target_link_libraries(imguizmo_test PUBLIC imguizmo)
