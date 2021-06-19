juce_add_binary_data(BinaryData SOURCES
    logo.png
    npi_background.jpg
    ../models/bj_model_best.json
    ../models/ts9_model_best.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
