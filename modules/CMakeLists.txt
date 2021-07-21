add_subdirectory(JUCE)

include(cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

include(cmake/WarningFlags.cmake)
add_library(juce_plugin_modules STATIC)
add_subdirectory(json)
#add_subdirectory(RTNeural)
include_directories(RTNeural)

target_link_libraries(juce_plugin_modules
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_dsp
	juce::juce_osc
	nlohmann_json::nlohmann_json
	RTNeural
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        warning_flags
)

target_compile_definitions(juce_plugin_modules
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JucePlugin_Manufacturer="GuitarML"
        JucePlugin_VersionString="${CMAKE_PROJECT_VERSION}"
        JucePlugin_Name="${CMAKE_PROJECT_NAME}"
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,COMPILE_DEFINITIONS>
)

target_include_directories(juce_plugin_modules
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)
