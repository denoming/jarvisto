set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
if(DEFINED VCPKG_INSTALLED_DIR)
    list(APPEND CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
endif()

# Runtime package configuration
set(CPACK_DEBIAN_JARVISTO_LIB_PACKAGE_NAME "libjarvisto")
set(CPACK_DEBIAN_JARVISTO_LIB_FILE_NAME DEB-DEFAULT)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPACK_DEBIAN_JARVISTO_LIB_DEBUGINFO_PACKAGE ON)
endif()

# Set package dependencies
string(CONCAT CPACK_DEBIAN_JARVISTO_LIB_PACKAGE_DEPENDS
    "libspdlog1.10 (>= 1.10.0+ds),"
    "libboost-program-options1.81.0 (>= 1.81.0),"
    "libboost-url1.81.0 (>= 1.81.0),"
    "libdate-tz3 (>= 3.0.1+ds),"
    "libmosquitto1 (>= 2.0.11),"
    "libmosquittopp1 (>= 2.0.11),"
    "libssl3 (>= 3.0.15),"
    "libsigc++-3.0-0 (>= 3.4.0)"
)

# Development package configuration
set(CPACK_DEBIAN_JARVISTO_DEVLIB_PACKAGE_NAME "libjarvisto-dev")
set(CPACK_DEBIAN_JARVISTO_DEVLIB_FILE_NAME DEB-DEFAULT)
