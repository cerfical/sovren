find_package(Catch2 3.7.0 REQUIRED)

if(NOT Catch2_FOUND)
    message(WARNING "Catch2 not found: tests disabled")
    return()
endif()


add_executable(sovren-tests
    "matrix_math_test.cpp"
    "vector_math_test.cpp"
)

target_link_libraries(sovren-tests
    PRIVATE
        sovren-lib
        Catch2::Catch2WithMain
)


include(Catch)

catch_discover_tests(sovren-tests)
