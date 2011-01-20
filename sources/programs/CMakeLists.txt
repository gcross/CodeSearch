#@+leo-ver=5-thin
#@+node:gcross.20110119165120.2302: * @thin CMakeLists.txt
macro(makeProgram program-name)
    add_executable(${program-name} ${program-name}.cpp)
    target_link_libraries(${program-name} CodeSearch)
    set_property(TARGET ${program-name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../programs)
endmacro(makeProgram)

makeProgram(count-all-solutions)
makeProgram(count-solutions)
makeProgram(print-solutions)
#@-leo
