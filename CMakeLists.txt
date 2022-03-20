########################################################################################################################
cmake_minimum_required(VERSION 3.19)

########################################################################################################################
# Details
set(JAMAL_PROJECT_TARGET  Jamal)
set(JAMAL_PROJECT_NAME    "Jamal")
set(JAMAL_PROJECT_VERSION "0.1.0")

# Settings
set(CMAKE_MINIMUM_REQUIRED_VERSION 17)
set(COLORER_USE_VCPKG              OFF)

########################################################################################################################
project(${JAMAL_PROJECT_TARGET}
    VERSION   0.1.0
    LANGUAGES C CXX)

########################################################################################################################
# Dependencies
add_subdirectory(lib/juce)
add_subdirectory(lib/jaut)
add_subdirectory(lib/xerces-c)
add_subdirectory(lib/small_vector)

########################################################################################################################
juce_add_gui_app(${JAMAL_PROJECT_TARGET}
    VERSION      0.1.0
    PRODUCT_NAME ${JAMAL_PROJECT_NAME})

########################################################################################################################
# Project files
add_subdirectory(src)
add_subdirectory(res)

########################################################################################################################
target_compile_definitions(${JAMAL_PROJECT_TARGET}
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${JAMAL_PROJECT_TARGET},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${JAMAL_PROJECT_TARGET},JUCE_VERSION>")

target_link_libraries(${JAMAL_PROJECT_TARGET}
    PRIVATE
        # Juce
        juce::juce_gui_extra

        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags

        # Jaut
        jaut::jaut_gui
    
        # 3rd party
        xerces-c
        small_vector)
