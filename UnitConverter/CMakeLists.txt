cmake_minimum_required(VERSION 3.10)
project(UnitConverter VERSION 1.0.0 LANGUAGES CXX)

# ---------- 全局编译设置 ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "构建类型" FORCE)
endif()
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")

# ---------- 项目分模块管理：把 unitlib 子目录纳入构建 ----------
# 必须写在 target_link_libraries 之前，否则找不到 unitlib 目标
add_subdirectory(unitlib)

# ---------- 查找第三方库 ----------
find_package(Threads REQUIRED)
message(STATUS "找到 Threads: ${Threads_FOUND}")

# 可选依赖：找到就用，找不到就跳过
find_package(fmt QUIET)

# ---------- 生成可执行文件 ----------
add_executable(unit_converter src/main.cpp)

target_link_libraries(unit_converter PRIVATE unitlib)
target_link_libraries(unit_converter PRIVATE Threads::Threads)

if(fmt_FOUND)
    message(STATUS "找到 fmt ${fmt_VERSION}，启用格式化输出")
    target_link_libraries(unit_converter PRIVATE fmt::fmt)
    target_compile_definitions(unit_converter PRIVATE HAVE_FMT=1)
else()
    message(STATUS "未找到 fmt（可选依赖），使用标准库输出")
endif()

target_compile_definitions(unit_converter PRIVATE APP_NAME="UnitConverter")

# ---------- 安装规则 ----------
install(TARGETS unit_converter RUNTIME DESTINATION bin)

# ---------- 信息 ----------
message(STATUS "----------------------------------------")
message(STATUS "项目名     : ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "源码目录   : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "构建目录   : ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "C++ 标准   : ${CMAKE_CXX_STANDARD}")
message(STATUS "编译器     : ${CMAKE_CXX_COMPILER}")
message(STATUS "----------------------------------------")
