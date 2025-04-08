cmake_minimum_required(VERSION 3.16) # CMake最小版本要求
project(opengl VERSION 1.0 LANGUAGES CXX) # 项目名称 版本 语言

set(CMAKE_AUTOUIC ON) # 自动处理ui文件
set(CMAKE_AUTOMOC ON) # 自动处理moc文件
set(CMAKE_AUTORCC ON) # 自动处理资源文件

set(CMAKE_CXX_STANDARD 17) # C++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++标准要求

find_package(Qt6 COMPONENTS REQUIRED Widgets OpenGLWidgets) # 查找Qt6的Widgets模块

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) # 可执行文件输出目录

qt_add_executable(
    ${PROJECT_NAME} # 可执行文件名称
    MANUAL_FINALIZATION # 手动完成
    main.cpp
    mainwindow.h mainwindow.cpp
    myopenglwidget.h myopenglwidget.cpp
) # 生成可执行文件

target_link_libraries( ${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
) # 链接库

# 设置目标属性
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    # 控制台 ON不生成 OFF生成
    WIN32_EXECUTABLE OFF
    # WIN32_EXECUTABLE ON
)

qt_finalize_executable(${PROJECT_NAME}) # 完成可执行文件
