//@+leo-ver=5-thin
//@+node:gcross.20110120115216.2084: * @thin print-code-summaries.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110120115216.2085: ** << Includes >>
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
//@+node:gcross.20110120115216.2086: ** main
int main(int argc, char** argv) {
    if(argc < 3 || argc > 4) {
        cout << "Usage: print-code-summaries <# of qubits> <# of operators> [x bit diagonal size]" << endl;
        return 1;
    }
    const unsigned int  number_of_qubits = atoi(argv[1])
                      , number_of_operators = atoi(argv[2])
                      ;
    const optional<unsigned int> x_bit_diagonal_size =
        argc == 4 ? optional<unsigned int>(atoi(argv[3])) : optional<unsigned int>(none);
    if(x_bit_diagonal_size) {
        if(*x_bit_diagonal_size > min(number_of_qubits,number_of_operators)) {
            cout << (format("%1% > min(%2%,%3%)") % *x_bit_diagonal_size % number_of_qubits % number_of_operators) << endl;
            return 2;
        }
        if(*x_bit_diagonal_size < (number_of_operators+1)/2) {
            cout << (format("%1% < (%2%+1)/2") % *x_bit_diagonal_size % ((number_of_operators+1)/2)) << endl;
            return 3;
        }
    }
    BOOST_FOREACH(
         const Code& code
        ,x_bit_diagonal_size
            ? gatherCodes(createConstrainedSpace(
                 number_of_qubits
                ,number_of_operators
                ,all_constraints
                ,StandardFormParameters(*x_bit_diagonal_size,number_of_operators-*x_bit_diagonal_size)
              ))
            : gatherAllCodesForConstraints(
                 number_of_qubits
                ,number_of_operators
                ,all_constraints
              )
    ) {
        cout << code << endl;
    }
    return 0;
}
//@-others
//@-leo
