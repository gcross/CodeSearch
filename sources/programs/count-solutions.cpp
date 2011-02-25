//@+leo-ver=5-thin
//@+node:gcross.20110119165120.2298: * @thin count-solutions.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110119165120.2299: ** << Includes >>
#include <cstdlib>
#include <iostream>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "utilities.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110119165120.2300: ** main
int main(int argc, char** argv) {
    if(argc != 3) {
        cout << "Usage: count-solutions <# of qubits> <# of operators>" << endl;
        return 1;
    }
    const unsigned int number_of_qubits = atoi(argv[1])
                      , number_of_operators = atoi(argv[2])
                      ;
    unsigned long long total_number_of_solutions = 0;
    void BOOST_LOCAL_FUNCTION_PARAMS(
         const StandardFormParameters& parameters
        ,auto_ptr<OperatorSpace> initial_space
        ,const bind number_of_qubits
        ,const bind number_of_operators
        ,bind &total_number_of_solutions
    ) {
        cout << parameters.x_bit_diagonal_size << "/" << parameters.z_bit_diagonal_size << ": ";
        cout.flush();
        const unsigned long long number_of_solutions = countSolutions(initial_space);
        cout << number_of_solutions << endl;
        total_number_of_solutions += number_of_solutions;
    } BOOST_LOCAL_FUNCTION_NAME(processStandardForm);
    forEachStandardForm(
         number_of_qubits
        ,number_of_operators
        ,all_constraints
        ,processStandardForm
        );
    cout << "Total: " << total_number_of_solutions << endl;
}
//@-others
//@-leo
