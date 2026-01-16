cmake_minimum_required(VERSION 3.14)

# Function to enable Hot Reload for MSVC compilers if supported.
function(enable_hot_reload)
  if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endfunction()

# Function to clean the library files.
function(clean_files LIB_NAME)
  add_custom_command(TARGET ${LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -f "$<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.lib"
    COMMAND ${CMAKE_COMMAND} -E rm -f "$<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.exp"
    COMMAND ${CMAKE_COMMAND} -E rm -f "$<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.ilk"
  )

  if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E rm -f "$<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.pdb"
    )
  endif()
endfunction()

# Function to setup library options.
function(setup_opt LIB_NAME)
  set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
  set_property(TARGET ${LIB_NAME} PROPERTY ENABLE_EXPORTS ON)
  set_target_properties(${LIB_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
  set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
  set_target_properties(${LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endfunction()
