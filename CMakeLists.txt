cmake_minimum_required(VERSION 3.15)

# Project name
project(DemoTape VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and include JUCE CMake integration
set(JUCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../JUCE" CACHE PATH "Path to JUCE library")
if(NOT EXISTS "${JUCE_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE path doesn't exist or isn't a JUCE repository: ${JUCE_PATH}")
endif()

# Force iterator debug level to match Rive library
if(MSVC)
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

# Globally define the entry point macro BEFORE adding JUCE
# add_compile_definitions(JUCE_CREATE_PLUGIN_FILTER=DemoTapeAudioProcessor)

# Add JUCE as a subdirectory
add_subdirectory(${JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)

set(RIVE_CPP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../rive-runtime-main/include")
set(RIVE_RENDERER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../rive-runtime-main/renderer/include")
set(RIVE_RENDERER_SRC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../rive-runtime-main/renderer/src")

# Explicitly set the absolute path to the desired rive.lib
set(RIVE_CPP_LIB_PATH "E:/__Design/__/__VST/__Phoenix/rive-runtime-main/out/debug/rive.lib")

# Include our stub headers
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/StubHeaders)

# Define source and header files explicitly
set(PLUGIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveOpenGLRenderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveComponent.cpp"
    # Commented out files remain commented
    # "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveStubs.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveTestApp.cpp"
)

set(PLUGIN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/juce_config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginEditor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginProcessor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveComponent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveDemo.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveFactory.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveOpenGLRenderer.h"
)

# Check if RiveComponent.cpp exists and add it - No longer needed, added directly above
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveComponent.cpp")
#     list(APPEND PLUGIN_SOURCES Source/RiveComponent.cpp)
# endif()

# Use JUCE's convenience functions
juce_add_plugin(DemoTape
    COMPANY_NAME "YourCompany"
    PLUGIN_MANUFACTURER_CODE "Ydmo"
    PLUGIN_CODE "Dmtp"
    FORMATS VST3 Standalone
    PRODUCT_NAME "DemoTape"

    # Explicitly list both sources and headers
    SOURCES
        ${PLUGIN_SOURCES}
        ${PLUGIN_HEADERS} 
)

# Generate JuceHeader.h for this plugin
juce_generate_juce_header(DemoTape)

# === DIAGNOSTIC STEP 1: Verify the variable (Keep this if you added it) ===
message(STATUS "Checking RIVE_CPP_INCLUDE_DIR before use: ${RIVE_CPP_INCLUDE_DIR}")

# === REMOVE the set_target_properties block ===
# set_target_properties(DemoTape PROPERTIES
#     INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Source;${RIVE_CPP_INCLUDE_DIR}"
# )
# Note: We combine the Source and Rive include dirs here, semicolon-separated.

# === USE target_include_directories instead ===
target_include_directories(DemoTape
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${RIVE_CPP_INCLUDE_DIR}
        ${RIVE_RENDERER_INCLUDE_DIR}
        ${RIVE_RENDERER_SRC_INCLUDE_DIR}
)

# Set preprocessor definitions specific to the final plugin targets
target_compile_definitions(DemoTape
    PRIVATE
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES;WIN32_LEAN_AND_MEAN>
)

# Define OS-specific settings
if(WIN32)
    target_compile_definitions(DemoTape
        PRIVATE
            _USE_MATH_DEFINES
            WIN32_LEAN_AND_MEAN
    )
endif()

# Add required JUCE modules and Rive
target_link_libraries(DemoTape
    PRIVATE
        juce::juce_audio_utils
        juce::juce_opengl
        juce::juce_audio_plugin_client
        ${RIVE_CPP_LIB_PATH}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)

# Force Static Runtime (/MDd) for Debug builds to match Rive library
target_compile_options(DemoTape PRIVATE
    $<$<CONFIG:Debug>:/MDd>
)

# Ignore the default dynamic C++ runtime libs in Debug
target_link_options(DemoTape PRIVATE
    $<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCPRTD>
    $<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCRTD>
    $<$<CONFIG:Debug>:/NODEFAULTLIB:LIBCMTD>
    $<$<CONFIG:Debug>:/FORCE:MULTIPLE>
)

# Print some debug information
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "JUCE_PATH: ${JUCE_PATH}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# --- Organize files in Visual Studio Solution Explorer ---
source_group("Source Files" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveOpenGLRenderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveComponent.cpp"
    # Add commented-out files here if you want them grouped too
    # "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveStubs.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveTestApp.cpp"
)

source_group("Header Files" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/juce_config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginEditor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginProcessor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveComponent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveDemo.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveFactory.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/RiveOpenGLRenderer.h"
)
# --- End of source_group section --- 