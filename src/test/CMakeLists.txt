if(BUILD_TESTING)
    add_executable(tests tests.cpp)
    target_compile_features(tests PUBLIC cxx_std_20)
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

    target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

    add_test(NAME RangeStore_Tests COMMAND tests)
endif()
