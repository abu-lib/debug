cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 1.0.0)

add_library(abu_debug INTERFACE)

target_include_directories(abu_debug INTERFACE include)
target_compile_features(abu_debug INTERFACE cxx_std_20)
add_library(abu::debug ALIAS abu_debug)
