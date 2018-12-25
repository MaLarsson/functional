add_executable(functional_test EXCLUDE_FROM_ALL test.cpp)

target_link_libraries(functional_test PUBLIC Functional::Functional)

add_custom_target(check COMMAND ./functional_test DEPENDS functional_test)
