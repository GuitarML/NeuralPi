cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")
project(NeuralPi VERSION 1.3.0)

set(CMAKE_CXX_STANDARD 17)

set(RTNEURAL_XSIMD ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(modules/RTNeural)

add_subdirectory(modules)
include_directories(modules)

set(JUCE_FORMATS AU VST3 Standalone)

# Build LV2 only on Linux
if(UNIX AND NOT APPLE)
    message(STATUS "Building LV2 plugin format")
    list(APPEND JUCE_FORMATS LV2)
endif()

juce_add_plugin(NeuralPi
    COMPANY_NAME GuitarML
    PLUGIN_MANUFACTURER_CODE GtML
    PLUGIN_CODE Npi3 
    FORMATS ${JUCE_FORMATS}
    ProductName "NeuralPi"
    LV2_URI https://github.com/GuitarML/NeuralPi
    ICON_BIG resources/logo.png
    MICROPHONE_PERMISSION_ENABLED TRUE
)

# create JUCE header
juce_generate_juce_header(NeuralPi)

# add sources
add_subdirectory(Source)
include_directories(Source)
add_subdirectory(resources)

target_compile_definitions(NeuralPi
    PUBLIC
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(NeuralPi PUBLIC
    juce_plugin_modules
)

# we need these flags for notarization on MacOS
option(MACOS_RELEASE "Set build flags for MacOS Release" OFF)
if(MACOS_RELEASE)
    message(STATUS "Setting MacOS release flags...")
    set_target_properties(NeuralPi_Standalone PROPERTIES
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)
endif()
