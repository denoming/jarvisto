# Find the native libmosquitto includes and libraries
#
# Targets:
#   mosquitto::mosquitto - mosquitto library target with C interface
#   mosquitto::mosquittopp - mosquitto library target with C++ wrapper
#
# Variables:
#   mosquitto_INCLUDE_DIR  - Include dir with header files
#   mosquitto_LIBRARIES    - List of libraries when using libmosquitto.
#   mosquitto_FOUND        - True if libmosquitto found.
#   mosquitto_FIND_QUIETLY - True if libmosquitto had been already found.
#

if(mosquitto_FOUND)
    set(mosquitto_FIND_QUIETLY TRUE)
endif()

find_path(mosquitto_INCLUDE_DIR mosquitto.h)

find_library(mosquitto_LIBRARY NAMES libmosquitto mosquitto)
find_library(mosquitto_PP_LIBRARY NAMES libmosquittopp mosquittopp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    mosquitto
    DEFAULT_MSG
    mosquitto_LIBRARY mosquitto_PP_LIBRARY mosquitto_INCLUDE_DIR)

if(mosquitto_FOUND)
    set(mosquitto_LIBRARIES ${mosquitto_LIBRARY} ${mosquitto_PP_LIBRARY})

    set(mosquitto_TARGET mosquitto::mosquitto)
    if (NOT TARGET ${mosquitto_TARGET})
        add_library(${mosquitto_TARGET} UNKNOWN IMPORTED)
        set_target_properties(${mosquitto_TARGET} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES ${mosquitto_INCLUDE_DIR}
                IMPORTED_LINK_INTERFACE_LANGUAGES "C"
                IMPORTED_LOCATION "${mosquitto_LIBRARY}")
    endif()
    unset(mosquitto_TARGET)

    set(mosquittopp_TARGET mosquitto::mosquittopp)
    if (NOT TARGET ${mosquittopp_TARGET})
        add_library(${mosquittopp_TARGET} UNKNOWN IMPORTED)
        set_target_properties(${mosquittopp_TARGET} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES ${mosquitto_INCLUDE_DIR}
                IMPORTED_LINK_INTERFACE_LANGUAGES "C"
                IMPORTED_LOCATION "${mosquitto_PP_LIBRARY}")
    endif()
else()
    set(mosquitto_LIBRARIES)
endif()

mark_as_advanced(
    mosquitto_INCLUDE_DIR
    mosquitto_LIBRARY
    mosquitto_PP_LIBRARY)

