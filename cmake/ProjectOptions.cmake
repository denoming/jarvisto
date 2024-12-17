include(FeatureSummary)

option(ENABLE_NETWORK_COMPONENT "Enable network component" OFF)
if(ENABLE_NETWORK_COMPONENT)
    list(APPEND VCPKG_MANIFEST_FEATURES "network")
endif()
add_feature_info(
    ENABLE_NETWORK_COMPONENT ENABLE_NETWORK_COMPONENT "Build with network component"
)

option(ENABLE_TESTS "Enable testing" OFF)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
