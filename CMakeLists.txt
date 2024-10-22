cmake_minimum_required(VERSION 3.19)

# 设置项目名称和版本
project(test VERSION 1.0.0 LANGUAGES CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 20)

message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

# 默认构建类型为 Release，允许通过命令行或 GUI 配置
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# 编译器选项配置
if(MSVC)
    add_compile_options(/utf-8)
    add_definitions(-DNOMINMAX)
    # 启用媒体功能
    add_definitions(-DRTC_ENABLE_MEDIA=1)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /Zi /arch:AVX2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2 /arch:AVX2)
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g -mbmi2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -mbmi2)
    endif()
endif()


# 引入外部依赖
find_package(LibDataChannel CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

# 定义可复用的函数，用于设置目标的属性
function(setup_target target_name src_dir)
    aux_source_directory(${src_dir} DIR_SRCS)
    add_executable(${target_name} ${DIR_SRCS})
    target_include_directories(${target_name} 
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
    target_link_directories(${target_name} 
        PRIVATE
        ${FFMPEG_LIBRARY_DIRS}
    )
    target_link_libraries(${target_name} 
        PRIVATE
        LibDataChannel::LibDataChannel
    )
endfunction()

setup_target(sender ${PROJECT_SOURCE_DIR}/src/sender)
# setup_target(receiver ${PROJECT_SOURCE_DIR}/src/receiver)