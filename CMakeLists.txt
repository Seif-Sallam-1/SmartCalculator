cmake_minimum_required(VERSION 4.0)
project(SmartCalculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

 list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(SmartCalculator
        main.cpp
        AST.h
        MainWindow.h
        AST.cpp
)

target_link_libraries(SmartCalculator PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)