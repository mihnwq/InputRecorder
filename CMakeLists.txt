cmake_minimum_required(VERSION 3.23)
project(InputMonitorUI CXX)
set(CMAKE_CXX_STANDARD 20)

# Find native OpenGL package
find_package(OpenGL REQUIRED)

# Disable GLFW tests/docs to speed up build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Define ImGui directory
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

# Compile ImGui as a static library
add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        Headers/BackendTasks.h
        Classes/UIManager.cpp
        Headers/UIManager.h
        Headers/MouseAction.h
        Headers/KeyboardAction.h
        Classes/MouseAction.cpp
        Headers/SaveFileHandler.h
        Classes/SaveFileHandler.cpp
        Headers/MouseHookHandler.h
        Classes/MouseHookHandler.cpp
        Headers/KeyboardHookHandler.h
        Classes/KeyboardHookHandler.cpp
        Classes/KeyboardAction.cpp
        Headers/Stopwatch.h
)

target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# Main executable
add_executable(InputMonitorUI main.cpp)
target_link_libraries(InputMonitorUI PRIVATE imgui OpenGL::GL opengl32)