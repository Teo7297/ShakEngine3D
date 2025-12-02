# Compile shaders
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(SHADERS_TARGET Shaders)

# Check if shaders directory exists
if(NOT EXISTS ${SHADER_DIR})
    message(WARNING "Shaders directory not found: ${SHADER_DIR}")
    return()
endif()

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

# Find glslc compiler
find_program(GLSLC_EXECUTABLE glslc HINTS $ENV{VULKAN_SDK}/Bin REQUIRED)

# Find all vertex and fragment shaders
file(GLOB SHADERS "${SHADER_DIR}/*")

if(NOT SHADERS)
    message(WARNING "No shader files found in ${SHADER_DIR}")
    return()
endif()

set(COMPILED_SHADERS)

# Compile shaders
foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(OUTPUT_FILE ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv)
    
    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${OUTPUT_FILE} || (exit 0) # Do not stop compilation on error
        DEPENDS ${SHADER}
        COMMENT "Compiling shader: ${SHADER_NAME}"
    )
    
    list(APPEND COMPILED_SHADERS ${OUTPUT_FILE})
endforeach()

# Create custom target for shaders
add_custom_target(${SHADERS_TARGET} ALL
    DEPENDS ${COMPILED_SHADERS}
)
