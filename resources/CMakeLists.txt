juce_add_binary_data(BinaryData SOURCES
    logo.png
    npi_background.jpg
    ../models/BluesJR_FullD.json
    ../models/TS9_FullD.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
