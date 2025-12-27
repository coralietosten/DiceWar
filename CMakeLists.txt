cmake_minimum_required (VERSION 3.14)

include(cmake_modules/tools.cmake)

project("project" VERSION 1.0.0)

set(OUTPUT_DIR "${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

add_subdirectory(src/Projects/LibInfo)
add_subdirectory(src/Projects/GenMap)
add_subdirectory(src/Projects/StrategyDummy)
add_subdirectory(src/Projects/StrategyTest)
