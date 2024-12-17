set(comp "program_options")
if (ENABLE_NETWORK_COMPONENT)
    list(APPEND comp "url")
endif()

find_package(Boost 1.81.0
    REQUIRED
    COMPONENTS ${comp}
    CONFIG)

unset(comp)