cmake_minimum_required(VERSION 3.14)
project(ComputerGraphics)

include(FetchContent)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
    cmake_policy(SET CMP0077 NEW)
endif()

# Set build options
set_option(USE_GLEW ON BOOL "Build project with GLEW")
set_option(USE_FLTK ON BOOL "Build project with FLTK")
set_option(USE_GLFW OFF BOOL "Build project with GLFW")
set_option(USE_GLAD OFF BOOL "Build project with GLAD")
set_option(USE_GLM ON BOOL "Build project with glm")
set_option(USE_OLD_OPENGL OFF BOOL "Build project with old OpenGL (older than 3.1)")

set_option(BUILD_LAB1_2 ON BOOL "Build lab1 and lab2")
set_option(BUILD_LAB3 ON BOOL "Build lab3")
set_option(BUILD_LAB4 ON BOOL "Build lab4")
set_option(BUILD_LAB5 ON BOOL "Build lab5")
set_option(BUILD_LAB6 ON BOOL "Build lab6")

set(BUILD_SHARED_LIBS OFF)

if (${USE_GLEW})
    FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG bf71a83 # release-0.9.9.8
    )
    FetchContent_MakeAvailable(GLM)
    set(GLM_INCLUDE_DIRS ${FETCHCONTENT_BASE_DIR}/glm-src)
endif()
if (${USE_GLEW})
    add_compile_definitions(GLEW_STATIC)
endif()
if (${USE_FLTK})
    set(FLTK_BUILD_FLUID OFF)
    set(FLTK_BUILD_TEST OFF)
    set(OPTION_USE_SYSTEM_ZLIB OFF)
    set(OPTION_USE_SVG OFF)
    set(OPTION_USE_WAYLAND OFF)
    set(OPTION_PRINT_SUPPORT OFF)
    set(OPTION_BUILD_HTML_DOCUMENTATION OFF)
    set(OPTION_BUILD_PDF_DOCUMENTATION OFF)

    FetchContent_Declare(
        FLTK
        GIT_REPOSITORY https://github.com/fltk/fltk
        GIT_TAG 3600522ae7d2244b69552458e4d82f869a36067b # release-1.3.8
    )
    FetchContent_MakeAvailable(FLTK)
    set(FLTK_INCLUDE_DIRS ${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})
    set(FLTK_LINK_DIRS ${fltk_BINARY_DIR}/lib)
    set(FLTK_LIBRARIES fltk fltk_gl)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # For OpenGL we need to install libraries:
    # libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev
    # libxrandr-dev libxext-dev libxcursor-dev libxinerama-dev
    # libxi-dev
    # For GLEW: libglew-dev
    # For GLFW: libglfw3-dev libglfw3

    set(OPENGL_LIBRARY ${OPENGL_LIBRARY} ${CMAKE_DL_LIBS} GL GLU Xrandr Xext X11 rt)
    if(${USE_GLEW})
        set(GLEW_LIBRARIES GLEW)
    endif()
    if(${USE_GLFW})
        set(GLFW_LIBRARIES glfw3)
    endif()
elseif (WIN32)
    # Setup GLEW
    if (${USE_GLEW})
        FetchContent_Declare(
            GLEW
            URL https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip
        )
        FetchContent_GetProperties(glew)
        if(NOT glew_POPULATED)
            set(BUILD_UTILS OFF)
            FetchContent_Populate(glew)
            add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR})
            set(GLEW_INCLUDE_DIRS ${glew_SOURCE_DIR}/include)
            set(GLEW_LINK_DIRS ${CMAKE_CURRENT_BINARY_DIR}/lib) # GLEW builds to CMAKE_CURRENT_BINARY_DIR
            set(GLEW_LIBRARIES glew_s)
        endif()
    endif()

    # Setup GLFW
    if (${USE_GLFW})
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(
            GLFW
            GIT_REPOSITORY https://github.com/glfw/glfw
        )
        FetchContent_MakeAvailable(GLFW)
        set(GLFW_INCLUDE_DIRS ${FETCHCONTENT_BASE_DIR}/glfw-src/include)
        set(GLFW_LINK_DIRS ${FETCHCONTENT_BASE_DIR}/glfw-build/src)
        set(GLFW_LIBRARIES glfw)
    endif()
endif()

set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

set(ALL_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${FLTK_LIBRARIES})
set(LINK_DIRS ${GLEW_LINK_DIRS} ${GLFW_LINK_DIRS} ${FLTK_LINK_DIRS})
set(INCLUDE_DIRS ${GLM_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${FLTK_INCLUDE_DIRS})

configure_file(${COMMON_DIR}/cg_config.h.in cg_config.h)

add_subdirectory(${COMMON_DIR})
set(ALL_LIBS ${ALL_LIBS} common)

if (${BUILD_LAB1_2})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lab1-2)
endif()
if (${BUILD_LAB3})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lab3)
endif()
if (${BUILD_LAB4})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lab4)
endif()
if (${BUILD_LAB5})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lab5)
endif()
if (${BUILD_LAB6})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lab6)
endif()