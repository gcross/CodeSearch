#@+leo-ver=5-thin
#@+node:gcross.20101224191604.2731: * @thin CMakeLists.txt
#@+<< Macros >>
#@+node:gcross.20101224191604.2732: ** << Macros >>
macro(makeTestProgram program-name)
    add_executable(${program-name} EXCLUDE_FROM_ALL ${${program-name}-SOURCES})
    target_link_libraries(${program-name} CodeSearch illuminate-main illuminate ${Boost_LIBRARIES})
    set_property(TARGET ${program-name} PROPERTY RUNTIME_OUTPUT_DIRECTORY .)
endmacro(makeTestProgram)
#@-<< Macros >>

#@+<< Includes >>
#@+node:gcross.20101224191604.2733: ** << Includes >>
include_directories(".")
#@-<< Includes >>

set(test-SOURCES
    constraints/all
    constraints/every_column_has
    constraints/ordering/column
    constraints/ordering/row/weight
    constraints/standard_form
    constraints/y_before_x
    operator_space
    test_utils_tests

    test_utils
)

makeTestProgram(test)
#@-leo
