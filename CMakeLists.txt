cmake_minimum_required(VERSION 4.2)
project(A1_SET9)

set(CMAKE_CXX_STANDARD 20)

add_executable(A1_SET9 main.cpp
        StringGenerator.cpp
        StringGenerator.h
        StringSortTester.cpp
        QuickSort.cpp
        MergeSort.cpp
        StringMergeSort.cpp
        StringQuickSort.cpp
        MSDRadixSort1.cpp
        MSDRadixSort2.cpp)
