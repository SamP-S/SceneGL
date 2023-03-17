cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

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
target_compile_definitions(graphics PUBLIC LA_OPEN_GL)
target_include_directories(graphics PUBLIC "./include")
target_link_libraries(graphics PUBLIC tinyfiledialogs)
target_link_libraries(graphics PUBLIC stb_image)
target_link_libraries(graphics PUBLIC imgui)
target_link_libraries(graphics PUBLIC ${CMAKE_DL_LIBS})
