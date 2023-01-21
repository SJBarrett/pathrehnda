cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME pathrehdna)


set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20 -lws2_32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++20 -lws2_32")


list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#option( TRACY_ENABLE "" ON)
#add_subdirectory(external/tracy)

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif ()
include(${CMAKE_BINARY_DIR}/conan.cmake)



conan_cmake_run(REQUIRES
        ms-gsl/4.0.0
        fmt/9.0.0
        spdlog/1.10.0
        magic_enum/0.8.0
        namedtype/20190324
        boost/1.81.0
        stb/cci.20210910
        BASIC_SETUP BUILD missing BUILD_TYPE Debug)

set(SOURCE_FILES
        src/main.cpp
        src/Camera.cpp
        src/ImageBuffer.cpp
        src/ImageWriter.cpp
        src/Sampler.cpp
        src/Aggregator.cpp
        src/Perlin.cpp
        src/hittable/Sphere.cpp
        src/hittable/HittableList.cpp
        src/hittable/MovingSphere.cpp
        src/hittable/Aabb.cpp
        src/hittable/BvhNode.cpp
        src/materials/LambertianMaterial.cpp
        src/materials/MetalMaterial.cpp
        src/materials/DielectricMaterial.cpp
        src/texture/SolidColour.cpp
        src/texture/CheckerTexture.cpp
        src/texture/NoiseTexture.cpp
        src/texture/ImageTexture.cpp
        )
# Enable tracy profiler
add_compile_definitions(TRACY_ENABLE) # TODO enable this only for some builds
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME}
        ${CONAN_LIBS}
        #Tracy::TracyClient
        )
include_directories(include)

add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

# Compile shaders from -> https://gist.github.com/evilactually/a0d191701cb48f157b05be7f74d79396