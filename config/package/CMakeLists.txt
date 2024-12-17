include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/JarvistoConfig.cmake.in
        JarvistoConfig.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    COMPONENT Jarvisto_DevLib
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    COMPONENT Jarvisto_DevLib
)