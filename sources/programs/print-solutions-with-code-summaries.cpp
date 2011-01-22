//@+leo-ver=5-thin
//@+node:gcross.20110121100120.1577: * @thin print-solutions-with-code-summaries.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110121100120.1578: ** << Includes >>
#include <boost/format.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <cstdlib>
#include <iostream>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "codequest.hpp"
#include "solution_iterator.hpp"
#include "utilities.hpp"

using namespace CodeSearch;
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace boost::lambda;
using namespace boost::range;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110121100120.1579: ** main
int main(int argc, char** argv) {
    if(argc < 3 || argc > 4) {
        cout << "Usage: print-solutions <# of qubits> <# of operators> [x bit diagonal size]" << endl;
        return 1;
    }
    const unsigned int  number_of_qubits = atoi(argv[1])
                      , number_of_operators = atoi(argv[2])
                      ;
    BOOST_LOCAL_FUNCTION(
        (void) (process)(
            (const StandardFormParameters&)(parameters)
            (const bind)((number_of_qubits)(number_of_operators))
        )
    ) {
        assert(parameters.x_bit_diagonal_size >= parameters.z_bit_diagonal_size);
        BOOST_FOREACH(
             const OperatorSpace& space
            ,generateSolutionsFor(
                createConstrainedSpace(
                     number_of_qubits
                    ,number_of_operators
                    ,all_constraints
                    ,parameters
                    )
             )
        ) {
            cout << "- Solution:" << endl;
            auto_ptr<dynamic_operator_vector> operators = space.getOperators<dynamic_operator_vector>();
            BOOST_FOREACH(dynamic_quantum_operator op, *operators) {
                cout << "    - " << op << endl;
            }
            cout << "  Code: " << summarizeCode<dynamic_qec>(computeOptimizedCodeForOperatorSpace<dynamic_qec>(space)) << endl;
            cout << endl;
        }
    } BOOST_LOCAL_FUNCTION_END(process)
    if(argc == 4) {
        const unsigned int x_bit_diagonal_size = atoi(argv[3]);
        if(x_bit_diagonal_size > min(number_of_qubits,number_of_operators)) {
            cout << (format("%1% > min(%2%,%3%)") % x_bit_diagonal_size % number_of_qubits % number_of_operators) << endl;
            return 2;
        }
        if(x_bit_diagonal_size < (number_of_operators+1)/2) {
            cout << (format("%1% < (%2%+1)/2") % x_bit_diagonal_size % ((number_of_operators+1)/2)) << endl;
            return 3;
        }
        process(StandardFormParameters(x_bit_diagonal_size,number_of_operators-x_bit_diagonal_size));
    } else {
        for_each(generateStandardFormsFor(number_of_qubits,number_of_operators),process);
    }
    return 0;
}
//@-others
//@-leo
