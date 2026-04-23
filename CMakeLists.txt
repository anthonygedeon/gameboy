cmake_minimum_required(VERSION 3.24)
project(gbemu VERSION 0.0.1
  DESCRIPTION "A gameboy (color) emulator")

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB TEST_FILES "test/*.cpp")

set(CMAKE_CXX_STANDARD 20)           
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_CXX_EXTENSIONS OFF)        # Use clean C++, not compiler-specific "gnu++20"
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakePrintHelpers)
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.8
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad 
    GIT_TAG        v2.0.8
    SOURCE_SUBDIR  cmake   
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG docking
)

FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.14.0
)

FetchContent_MakeAvailable(glfw glad imgui catch)

add_library(imgui STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

glad_add_library(glad_opengl_46 STATIC API gl:core=4.6)

target_link_libraries(imgui PUBLIC glfw glad_opengl_46)

# TODO Change to PRIVATE instead of PUBLIC once more
# executables are made
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

add_executable(gb_tests ${TEST_FILES})
target_link_libraries(gb_tests PUBLIC Catch2::Catch2WithMain)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC imgui glfw OpenGL::GL)
