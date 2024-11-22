include(FeatureSummary)

option(ENABLE_TESTS "Enable testing" OFF)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
