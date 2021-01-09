cmake_minimum_required(VERSION 3.8)
project(colorx)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_HOST_APPLE)
    add_compile_options(-Wno-deprecated-declarations)
endif(CMAKE_HOST_APPLE)

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

#config assimp
option(ASSIMP_BUILD_DOCS OFF)
option(ASSIMP_BUILD_EXAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(opengl_helper/assimp)
include_directories(opengl_helper/assimp/include)
LIST(APPEND LIBS assimp ${ASSIMP_LIBRARIES})

#config our engine
include_directories(include)
include_directories(src)
aux_source_directory(src ENGINE_SRC)


set(SOURCE_FILES main.cpp ${GLAD_SRC} ${ENGINE_SRC})
add_executable(colorx ${SOURCE_FILES})
target_link_libraries(colorx ${LIBS})

