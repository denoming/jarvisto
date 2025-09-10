add_library(ada-url STATIC)
add_library(ada-url::ada-url ALIAS ada-url)

set_target_properties(ada-url
    PROPERTIES VERSION 3.2.7
               POSITION_INDEPENDENT_CODE ON
)

target_include_directories(ada-url
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(ada-url
    PRIVATE src/ada.cpp
)

target_compile_features(ada-url PUBLIC cxx_std_20)
