cmake_minimum_required(VERSION 3.22)

# Professional JUCE Plugin Lab template.
# Copy this folder into products/<ProductName> then rename the project/target/product fields below.

project(ProCompressor VERSION 0.1.0)

set(JUCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../JUCE" CACHE PATH "Path to local JUCE checkout")
add_subdirectory("${JUCE_PATH}" JUCE)

set(PLUGIN_FORMATS VST3 Standalone)
if(APPLE)
    list(APPEND PLUGIN_FORMATS AU)
endif()

juce_add_plugin(ProCompressor
    COMPANY_NAME "PluginLab"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE PlLb
    PLUGIN_CODE Pcm1
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "Pro Compressor")

target_sources(ProCompressor
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

target_compile_features(ProCompressor PRIVATE cxx_std_17)

target_compile_definitions(ProCompressor
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(ProCompressor
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
