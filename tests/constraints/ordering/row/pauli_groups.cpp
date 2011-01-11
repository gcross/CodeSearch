//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2068: * @thin pauli_groups.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101231214817.2069: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <gecode/int.hh>
#include <iostream>
#include <illuminate.hpp>

#include "constraints/ordering/row/pauli_groups.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
using namespace std;

#include <typeinfo>
//@-<< Includes >>

//@+others
//@+node:gcross.20110102182304.1600: ** Values
static set<Constraint> pauli_groups_row_ordering_constraints = list_of(PauliGroupsRowOrdering);
//@+node:gcross.20101231214817.2070: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(RowOrdering) { TEST_SUITE(PauliGroups) {

//@+others
//@+node:gcross.20110102182304.1610: *3* for whole region
TEST_SUITE(for_whole_region) {

//@+others
//@+node:gcross.20101231214817.2147: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        postPauliGroupsRowOrderingConstraint(*initial_space,0u,number_of_qubits,0u,number_of_operators);
        checkCodes(initial_space);
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(1,3)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(2,3)
    DO_TEST_FOR(2,4)
    DO_TEST_FOR(3,1)
    DO_TEST_FOR(3,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,1)
    DO_TEST_FOR(4,2)
    DO_TEST_FOR(5,1)

}
//@-others

}
//@+node:gcross.20110102182304.1614: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110102182304.1615: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,pauli_groups_row_ordering_constraints
            ,bind(checkCodes,_2)
        );
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(1,3)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(2,3)
    DO_TEST_FOR(2,4)
    DO_TEST_FOR(3,1)
    DO_TEST_FOR(3,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,1)
    DO_TEST_FOR(4,2)
    DO_TEST_FOR(5,1)

}
//@-others

}
//@-others

} } }
//@-others
//@-leo
