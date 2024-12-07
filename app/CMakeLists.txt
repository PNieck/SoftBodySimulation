add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE simulation_lib)
enable_compiler_warnings(${PROJECT_NAME})
