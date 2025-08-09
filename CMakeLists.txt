cmake_minimum_required(VERSION 4.0.0)


# Basic project setup
project(sovren LANGUAGES CXX VERSION 0.1.0)
include(CTest)

option(BUILD_DOCS "Generate documentation" ON)

if(NOT WIN32)
    message(FATAL_ERROR "The only supported platform is Windows")
endif()


add_library(sovren-lib)

target_include_directories(sovren-lib
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    PRIVATE "include/sovren"
)

set_target_properties(sovren-lib PROPERTIES OUTPUT_NAME sovren)

target_compile_features(sovren-lib PUBLIC cxx_std_23)


target_sources(sovren-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/sovren/Camera.hpp"
        "include/sovren/CameraNode3D.hpp"
        "include/sovren/Input.hpp"
        "include/sovren/LogNode.hpp"
        "include/sovren/Mesh.hpp"
        "include/sovren/MeshNode3D.hpp"
        "include/sovren/OrthoCamera.hpp"
        "include/sovren/PerspCamera.hpp"
        "include/sovren/Renderer2D.hpp"
        "include/sovren/Renderer3D.hpp"
        "include/sovren/RenderNode.hpp"
        "include/sovren/RenderNode2D.hpp"
        "include/sovren/RenderNode3D.hpp"
        "include/sovren/SceneNode.hpp"
        "include/sovren/SceneNode2D.hpp"
        "include/sovren/SceneNode3D.hpp"
        "include/sovren/Transform2D.hpp"
        "include/sovren/Transform3D.hpp"
        "include/sovren/WindowNode.hpp"
        "include/sovren/sovren.hpp"

        "include/sovren/core/AggregateN.hpp"
        "include/sovren/core/Color.hpp"
        "include/sovren/core/ConstructibleFromN.hpp"
        "include/sovren/core/Handle.hpp"
        "include/sovren/core/Point2.hpp"
        "include/sovren/core/Size2.hpp"

        "include/sovren/math/mat_ops.hpp"
        "include/sovren/math/Mat.hpp"
        "include/sovren/math/Mat2x2.hpp"
        "include/sovren/math/Mat3x3.hpp"
        "include/sovren/math/Mat4x4.hpp"
        "include/sovren/math/util.hpp"
        "include/sovren/math/vec_ops.hpp"
        "include/sovren/math/Vec.hpp"
        "include/sovren/math/Vec2.hpp"
        "include/sovren/math/Vec3.hpp"
        "include/sovren/math/Vec4.hpp"

        "include/sovren/log/ConsoleLogger.hpp"
        "include/sovren/log/Log.hpp"
        "include/sovren/log/Logger.hpp"
        "include/sovren/log/LogKind.hpp"
        "include/sovren/log/LogMessage.hpp"

        "include/sovren/pal/EventPump.hpp"
        "include/sovren/pal/Keys.hpp"
        "include/sovren/pal/MouseButtons.hpp"
        "include/sovren/pal/Window.hpp"
        "include/sovren/pal/WindowHandler.hpp"

        "include/sovren/pal/win32/error_util.hpp"
        "include/sovren/pal/win32/string_util.hpp"
        "include/sovren/pal/win32/Win32EventPump.hpp"
        "include/sovren/pal/win32/Win32Window.hpp"
        "include/sovren/pal/win32/WndClass.hpp"

        "include/sovren/rhi/AccessModes.hpp"
        "include/sovren/rhi/BindModes.hpp"
        "include/sovren/rhi/Buffer.hpp"
        "include/sovren/rhi/BufferDesc.hpp"
        "include/sovren/rhi/Context2D.hpp"
        "include/sovren/rhi/Context3D.hpp"
        "include/sovren/rhi/Device.hpp"
        "include/sovren/rhi/Format.hpp"
        "include/sovren/rhi/Pipeline.hpp"
        "include/sovren/rhi/PipelineDesc.hpp"
        "include/sovren/rhi/ShaderCode.hpp"
        "include/sovren/rhi/ShaderLang.hpp"
        "include/sovren/rhi/ShaderStages.hpp"
        "include/sovren/rhi/SwapChain.hpp"
        "include/sovren/rhi/Texture.hpp"
        "include/sovren/rhi/VertexAttr.hpp"
        "include/sovren/rhi/Viewport.hpp"

        "include/sovren/rhi/dx11/Dx11Buffer.hpp"
        "include/sovren/rhi/dx11/Dx11Context2D.hpp"
        "include/sovren/rhi/dx11/Dx11Context3D.hpp"
        "include/sovren/rhi/dx11/Dx11Device.hpp"
        "include/sovren/rhi/dx11/Dx11Pipeline.hpp"
        "include/sovren/rhi/dx11/Dx11SwapChain.hpp"
        "include/sovren/rhi/dx11/Dx11Texture.hpp"
        "include/sovren/rhi/dx11/com_util.hpp"

    PRIVATE
        "src/pal/win32.cpp"
        "src/shaders/shaders.cpp"
)


# Generate project configuration header
if(CMAKE_BUILD_TYPE MATCHES "Deb")
    set(PROJECT_CONFIG "Debug")
elseif(CMAKE_BUILD_TYPE MATCHES "Rel")
    set(PROJECT_CONFIG "Release")
else()
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

configure_file("src/config.hpp.in" "include/config.hpp")

target_sources(sovren-lib
    PUBLIC FILE_SET HEADERS BASE_DIRS "${CMAKE_BINARY_DIR}/include" FILES
        "${CMAKE_BINARY_DIR}/include/config.hpp"
)


# Find the required DirectX components
find_library(D3D_LIB d3d11 REQUIRED)
message(STATUS "Found Direct3D library: ${D3D_LIB}")

find_library(D2D_LIB d2d1 REQUIRED)
message(STATUS "Found Direct2D library: ${D2D_LIB}")

target_link_libraries(sovren-lib PRIVATE
    "${D3D_LIB}"
    "${D2D_LIB}"
)


# Compile HLSL shaders into C++ headers
find_program(HLSL_COMPILER fxc REQUIRED)
message(STATUS "Found HLSL compiler: ${HLSL_COMPILER}")

function(compile_hlsl_shader SHADER_FILE SHADER_PROFILE)
    cmake_path(REPLACE_EXTENSION SHADER_FILE ".hpp" OUTPUT_VARIABLE SHADER_OUTPUT)
    cmake_path(GET SHADER_FILE STEM SHADER_NAME)

    set(SHADER_FILE "${CMAKE_SOURCE_DIR}/src/shaders/${SHADER_FILE}")
    set(SHADER_OUTPUT "${CMAKE_BINARY_DIR}/include/shaders/${SHADER_OUTPUT}")

    add_custom_command(
        OUTPUT "${SHADER_OUTPUT}"
        MAIN_DEPENDENCY "${SHADER_FILE}"
        COMMAND
            "${HLSL_COMPILER}"
            "${SHADER_FILE}"
            -Fh "${SHADER_OUTPUT}"
            -T "${SHADER_PROFILE}"
            -Vn "${SHADER_NAME}"
        COMMENT "Compiling HLSL shaders: ${SHADER_FILE} -> ${SHADER_OUTPUT}"
    )
    target_sources(sovren-lib PRIVATE "${SHADER_OUTPUT}")
endfunction()

compile_hlsl_shader("vs.hlsl" "vs_5_0")
compile_hlsl_shader("ps.hlsl" "ps_5_0")


# Build the main application
add_executable(sovren "src/main.cpp")

target_link_libraries(sovren PRIVATE sovren-lib)


if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()


# Generate .gitignore file to take care of the build directory
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*")
