#
# Main library
#
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/simulation/*.hpp")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/simulation/*.cpp")

add_library(simulation_lib ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(simulation_lib PUBLIC ../include)
target_link_libraries(simulation_lib PUBLIC glfw glad imgui glm assimp)
enable_compiler_warnings(simulation_lib)

# Copy shaders
add_custom_target(copy_shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
)
add_dependencies(simulation_lib copy_shaders)
