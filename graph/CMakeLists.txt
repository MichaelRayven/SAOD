cmake_minimum_required(VERSION 3.16)
project(graph)

# Create your game executable target as usual
add_library(graph STATIC graph.c graph.h)

# Link to the actual SDL3 library.
target_link_libraries(graph SDL3_ttf::SDL3_ttf SDL3::SDL3)