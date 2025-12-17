cmake_minimum_required (VERSION 3.14)

set(LIB_NAME "strategytest")

enable_hot_reload()

project(${LIB_NAME} VERSION 1.0.0 LANGUAGES C CXX)

add_library(${LIB_NAME} MODULE
 "src/strategy.cpp"
 "src/Strategy.h"
 "src/StrategyTest.cpp"
 "src/StrategyTest.h"
 "src/interface_lib.h"
)

setup_opt(${LIB_NAME})
clean_files(${LIB_NAME})
