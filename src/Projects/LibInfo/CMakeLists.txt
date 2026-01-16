cmake_minimum_required (VERSION 3.14)

set(BIN_NAME "libinfo")

enable_hot_reload()

project(${BIN_NAME} VERSION 1.0.0 LANGUAGES C CXX)

add_executable(${BIN_NAME}
 "src/main.cpp"
)

target_link_libraries (${BIN_NAME} PRIVATE ${CMAKE_DL_LIBS})

setup_opt(${BIN_NAME})
clean_files(${BIN_NAME})
