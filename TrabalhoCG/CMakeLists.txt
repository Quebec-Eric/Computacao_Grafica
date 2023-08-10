cmake_minimum_required(VERSION 3.10)

project(MyQtProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Automaticamente coleta todos os arquivos .cpp e .h 
file(GLOB PROJECT_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/ModificacoesQuadro/*.cpp
)

file(GLOB PROJECT_HEADERS 
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/include/ModificacoesQuadro/*.h
)

add_executable(MyQtProject ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(MyQtProject PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(MyQtProject PRIVATE Qt5::Widgets)
