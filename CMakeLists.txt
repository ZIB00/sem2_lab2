cmake_minimum_required(VERSION 3.11)

project(Sequences)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")

set(CMAKE_AUTOMOC ON) #документация
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets) #документация

add_executable(main
    src/main.cpp
)

target_include_directories(main PRIVATE
    include
    include/other
    include/sequences
    templates
    templates/other
    templates/sequences
)

add_executable(gui WIN32
    src/gui/main_gui.cpp
    src/gui/MainWindow.cpp
)

target_include_directories(gui PRIVATE
    include
    include/other
    include/sequences
    templates
    templates/other
    templates/sequences
)

target_link_libraries(gui PRIVATE Qt6::Widgets) #документация

add_subdirectory(tests/googletest-1.17.0)

add_executable(tests
    tests/DynamicArrayTests.cpp
    tests/LinkedListTests.cpp
    tests/ListSequenceTests.cpp
    tests/AdaptiveSequenceTests.cpp
    tests/SegmentedListTests.cpp
    tests/ArraySequenceTests.cpp
    tests/BitSequenceTests.cpp
    tests/ExtendedTests.cpp
)

target_include_directories(tests PRIVATE
    include
    include/other
    include/sequences
    templates
    templates/other
    templates/sequences
)

target_link_libraries(tests PRIVATE
    gtest
    gtest_main
)
