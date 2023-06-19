set(TARGET jarvisto-test-common)

add_library(${TARGET} STATIC)
add_library(Jarvisto::Test::Common ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE src/TestMain.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(${TARGET}
    PUBLIC Jarvisto::Jarvisto
    PUBLIC GTest::gtest
           GTest::gmock
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)
