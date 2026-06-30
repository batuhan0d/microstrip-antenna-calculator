cmake_minimum_required(VERSION 3.10)
project(antenna_cal_gui VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Collect all ImGui source files
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)



# .h files are included automatically
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_SOURCE_DIR}/libs/glfw/include 
    ${CMAKE_SOURCE_DIR}/include
)
#linker .a files
link_directories(
    ${CMAKE_SOURCE_DIR}/libs/glfw/lib-mingw-w64
)

add_executable(${PROJECT_NAME} 
    main.cpp 
    ${CMAKE_SOURCE_DIR}/src/reqtangular.cpp
    ${IMGUI_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw3
    opengl32
    gdi32
    imm32
)
# copy files to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # copy the image directory to the build directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/image 
    ${CMAKE_BINARY_DIR}/image
    #copy the fonts directory to the build directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts
    ${CMAKE_BINARY_DIR}/fonts
)
target_link_options(${PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)

