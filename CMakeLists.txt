cmake_minimum_required(VERSION 3.11)

project(Sequences)

add_executable(main
    src/main.cpp
)

target_include_directories(main PRIVATE
    include
    templates
)

add_subdirectory(tests/googletest-1.17.0)

add_executable(tests
    tests/DynamicArrayTests.cpp
    tests/LinkedListTests.cpp
    tests/ListSequenceTests.cpp
    tests/AdaptiveSequenceTests.cpp
    tests/SegmentedListTests.cpp
    tests/ArraySequenceTests.cpp
    tests/BitSequenceTests.cpp
)

target_include_directories(tests PRIVATE
    include
    templates
)

target_link_libraries(tests PRIVATE
    gtest
    gtest_main
)