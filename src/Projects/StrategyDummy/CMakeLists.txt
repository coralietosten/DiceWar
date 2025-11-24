cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "strategydummy")

enable_hot_reload()

project(${LIB_NAME} VERSION 1.0.0 LANGUAGES C CXX)

add_library(${LIB_NAME} MODULE
 "src/Strategy.cpp"
 "src/Strategy.h"
 "src/StrategyDummy.cpp"
 "src/StrategyDummy.h"
 "src/interface_lib.h"
)

setup_opt(${LIB_NAME})
clean_files(${LIB_NAME})
