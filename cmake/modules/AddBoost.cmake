set(comps "")
if (ENABLE_NETWORK_COMPONENT)
    list(APPEND comps "url")
endif()

find_package(Boost
    REQUIRED
    COMPONENTS ${comps}
    CONFIG)

unset(comps)
