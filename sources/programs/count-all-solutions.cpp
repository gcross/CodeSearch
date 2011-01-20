//@+leo-ver=5-thin
//@+node:gcross.20110119165120.2306: * @thin count-all-solutions.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110119165120.2307: ** << Includes >>
#include <boost/foreach.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/irange.hpp>
#include <cstdlib>
#include <iostream>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "utilities.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::lambda;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110119165120.2308: ** main
int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Usage: count-all-solutions <# of qubits>" << endl;
        return 1;
    }
    const unsigned int number_of_qubits = atoi(argv[1]);
    unsigned long long total_number_of_solutions = 0;
    BOOST_FOREACH(const unsigned int number_of_operators, irange(1u,2*number_of_qubits)) {
        cout << number_of_operators << ": ";
        cout.flush();
        unsigned long long number_of_solutions = 0;
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,all_constraints
            ,ref(number_of_solutions) += bind(countSolutions,_2,Search::Options::def)
            );
        cout << number_of_solutions << endl;
        total_number_of_solutions += number_of_solutions;
    }
    cout << "Total: " << total_number_of_solutions << endl;
}
//@-others
//@-leo
