juce_add_binary_data(BinaryData SOURCES
    logo.png
    npi_background.jpg
    ../models/BluesJr.json
    ../models/TS9.json
    ../models/HT40_Overdrive.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
