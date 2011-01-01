//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2068: * @thin pauli_groups.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101231214817.2069: ** << Includes >>
#include <boost/foreach.hpp>
#include <gecode/int.hh>
#include <iostream>
#include <illuminate.hpp>

#include "constraints/ordering/row/pauli_groups.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace std;

#include <typeinfo>
//@-<< Includes >>

//@+others
//@+node:gcross.20101231214817.2070: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(RowOrdering) { TEST_SUITE(PauliGroups) {

//@+others
//@+node:gcross.20101231214817.2145: *3* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        postPauliGroupsRowOrderingConstraint(*initial_space,0u,number_of_qubits,0u,number_of_operators);
        unsigned int number_of_solutions = 0;
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            const PauliGroupsRowOrderingData& data = *space.getAuxiliaryData<PauliGroupsRowOrderingData>()[0];
            IntMatrix O_matrix = space.getOMatrix(),
                      unsorted_orderings_matrix(data.unsorted_orderings,number_of_operators,number_of_qubits),
                      pauli_orderings_matrix(data.pauli_orderings,4,number_of_qubits);
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_qubits)) {
                unsigned int counts[3] = {0,0,0};
                BOOST_FOREACH(const unsigned int j, irange(0u,number_of_operators)) {
                    if(O_matrix(i,j).val() > 0) ++counts[O_matrix(i,j).val()-1];
                }
                BOOST_FOREACH(const unsigned int j, irange(0u,3u)) {
                    BOOST_FOREACH(const unsigned int k, irange(0u,3u)) {
                        #define tryRelation(_CMP_) \
                            if(counts[j] _CMP_  counts[k]) \
                                ASSERT_TRUE(pauli_orderings_matrix(j+1,i).val() _CMP_ pauli_orderings_matrix(k+1,i).val());
                        tryRelation(<);
                        tryRelation(==);
                        tryRelation(>);
                    }
                    ASSERT_TRUE(pauli_orderings_matrix(j+1,i).val() > pauli_orderings_matrix(0,i).val());
                }
                BOOST_FOREACH(const unsigned int j, irange(0u,number_of_operators)) {
                    ASSERT_EQ(pauli_orderings_matrix(O_matrix(i,j).val(),i).val(),unsorted_orderings_matrix(j,i).val());
                }
            }
            ++number_of_solutions;
        }
        ASSERT_TRUE(number_of_solutions > 0);
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
//@+node:gcross.20101231214817.2147: *3* correct codes
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

} } }
//@-others
//@-leo
