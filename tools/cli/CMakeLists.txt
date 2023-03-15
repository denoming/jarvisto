set(TARGET jarvis-test-application)

add_executable(${TARGET} "")
add_executable(Jarvis::Test ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE src/TestApplication.cpp
)

target_link_libraries(${TARGET}
    PRIVATE
        Jarvis::Common
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)
