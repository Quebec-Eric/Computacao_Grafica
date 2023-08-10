cmake_minimum_required(VERSION 3.10)

project(MyQtProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(MyQtProject
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(MyQtProject PRIVATE Qt5::Widgets)
