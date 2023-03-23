cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)

# set(LIBTFD_DIR "./libtinyfiledialogs")

# find_package(Git REQUIRED)
# if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
#     execute_process(COMMAND ${GIT_EXECUTABLE}
#         submodule update --init --recursive -- ${LIBTFD_DIR}
#         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
# endif()

add_subdirectory("./dependencies/tinyfiledialogs")
add_subdirectory("./dependencies/stb")
add_subdirectory("./dependencies/imgui")

add_executable(graphics "./src/main.cpp")

find_package(assimp REQUIRED)
target_include_directories(graphics PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(graphics PUBLIC ${ASSIMP_LIBRARIES})
message(STATUS "ASSIMP included at ${ASSIMP_INCLUDE_DIRS}")
message(STATUS "ASSIMP libraries are ${ASSIMP_LIBRARIES}")

target_compile_definitions(graphics PUBLIC LA_OPEN_GL)
target_include_directories(graphics PUBLIC "./include")
target_link_libraries(graphics PUBLIC tinyfiledialogs)
target_link_libraries(graphics PUBLIC stb_image)
target_link_libraries(graphics PUBLIC imgui)
target_link_libraries(graphics PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(graphics PUBLIC assimp)
