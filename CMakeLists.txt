cmake_minimum_required(VERSION 3.22)

project(VirtualTrumpet VERSION 0.1.0 LANGUAGES C CXX OBJC OBJCXX)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure output directories for built products
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include dependencies (fetches JUCE)
include(cmake/Dependencies.cmake)

# ==============================================================================
# VirtualTrumpet_Core (Static Library)
# Contains DSP, Physics, Performance, and Core engine logic
# ==============================================================================
add_library(VirtualTrumpet_Core STATIC)

target_sources(VirtualTrumpet_Core PRIVATE
    Source/Core/SynthEngine.cpp
    Source/Core/SynthEngine.h
    Source/Core/Parameters.cpp
    Source/Core/Parameters.h
)

target_include_directories(VirtualTrumpet_Core 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link Core with juce_audio_processors so it can manage parameters
target_link_libraries(VirtualTrumpet_Core 
    PUBLIC 
        juce::juce_audio_processors
)

# ==============================================================================
# VirtualTrumpet JUCE Plugin Target
# ==============================================================================
juce_add_plugin(VirtualTrumpet
    COMPANY_NAME "Personal"
    PLUGIN_MANUFACTURER "Personal"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_NAME "VirtualTrumpet"
    PLUGIN_DESC "Physical Modeling Trumpet Synthesizer"
    PLUGIN_MANUFACTURER_CODE "Prsn"
    PLUGIN_CODE "VTrp"
    VST3_REPLACES_VST2 FALSE
    FORMATS AU VST3 Standalone
)

target_sources(VirtualTrumpet PRIVATE
    Source/Plugin/PluginProcessor.cpp
    Source/Plugin/PluginProcessor.h
    Source/Plugin/PluginEditor.cpp
    Source/Plugin/PluginEditor.h
)

target_link_libraries(VirtualTrumpet 
    PRIVATE
        VirtualTrumpet_Core
        juce::juce_audio_utils
)

target_compile_definitions(VirtualTrumpet
    PRIVATE
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# JUCE standard plugin definitions (e.g. JUCE_SHARED_CODE)
juce_generate_juce_header(VirtualTrumpet)

# ==============================================================================
# Tests Subdirectory
# ==============================================================================
add_subdirectory(Tests)
