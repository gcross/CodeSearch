#@+leo-ver=5-thin
#@+node:gcross.20101224191604.2597: * @thin CMakeLists.txt
add_library(CodeSearch
    constraints.cpp
    constraints/standard_form
    operator_space
    solution_iterator
    utilities
)
target_link_libraries(CodeSearch
    gecodesearch
    gecodeint
    gecodeset
    gecodekernel
    gecodesupport
    gecodeminimodel
)
#@-leo
