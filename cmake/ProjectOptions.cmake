include(FeatureSummary)

option(JARVIS_ENABLE_DLT "Enable DLT logging" OFF)
add_feature_info(
    ENABLE_DLT JARVIS_ENABLE_DLT "Build project with DLT logging"
)

option(JARVIS_ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS JARVIS_ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
