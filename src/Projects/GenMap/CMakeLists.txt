cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "genmap")

enable_hot_reload()

project(${LIB_NAME} VERSION 1.0.0 LANGUAGES C CXX)

add_library(${LIB_NAME} MODULE
 #"src/StaticMapGenerator.cpp"
 "src/RandomMapGenerator.cpp"
 "src/MapLoader.cpp"
 "src/MapLoader.h"
 #"src/DefaultMap.cpp"
 #"src/DefaultMap.h"
 "src/RandomMap.cpp"
 "src/RandomMap.h"
 "src/interface_lib.h"
)

setup_opt(${LIB_NAME})
clean_files(${LIB_NAME})
