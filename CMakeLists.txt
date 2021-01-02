cmake_minimum_required(VERSION 3.8)
project(colorx)

set(CMAKE_CXX_STANDARD 11)

#config our engine
include_directories(include)
aux_source_directory(include COLORX_LIB_CPP)

#config glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(opengl_helper/glfw)
include_directories(opengl_helper/glfw/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

#config glad
set(GLAD_SRC opengl_helper/glad/src/glad.c)
if(NOT WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(opengl_helper/glad/include)

#config glm
include_directories(opengl_helper/glm)

set(SOURCE_FILES main.cpp ${GLAD_SRC} ${COLORX_LIB_CPP})
add_executable(colorx ${SOURCE_FILES})
target_link_libraries(colorx ${LIBS})

