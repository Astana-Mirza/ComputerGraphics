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
endif()

# Set build options
set_option(USE_GLEW ON BOOL "Build project with GLEW")
set_option(USE_GLFW ON BOOL "Build project with GLFW")
set_option(BUILD_LAB1 ON BOOL "Build lab1")

if (${USE_GLEW})
    set(BUILD_SHARED_LIBS OFF)
    add_compile_definitions(GLEW_STATIC)
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

set(ALL_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
set(LINK_DIRS ${GLEW_LINK_DIRS} ${GLFW_LINK_DIRS})
set(INCLUDE_DIRS ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

if (${BUILD_LAB1})
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS SHOW_MESSAGE=1)
    add_subdirectory(lab1)
    if (${USE_GLEW})
        add_dependencies(lab1 glew_s)
    endif()
    if (${USE_GLFW})
        add_dependencies(lab1 glfw)
    endif()
endif()
