//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2046: * @thin column_pauli_sets.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101231214817.2047: ** << Includes >>
#include <illuminate.hpp>

#include "column_pauli_sets.hpp"
#include "solution_iterator.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace Gecode;
using namespace CodeSearch;
//@-<< Includes >>

//@+others
//@+node:gcross.20101231214817.2048: ** Tests
TEST_SUITE(ColumnPauliSetsData) {

    void runTest(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        initial_space->attachAuxiliaryData(wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new ColumnPauliSetsData(*initial_space,0u,number_of_qubits,0u,number_of_operators))));
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            BoolMatrix X_matrix = space.getXMatrix(),
                       Z_matrix = space.getZMatrix(),
                       non_trivial_matrix = space.getNonTrivialMatrix();
            IntMatrix  O_matrix = space.getOMatrix();
            const ColumnPauliSetsData& data = *space.getAuxiliaryData<ColumnPauliSetsData>()[0];
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_qubits)) {
                BOOST_FOREACH(const unsigned int j, irange(0u,number_of_operators)) {
                    if(X_matrix(i,j).val()) {
                        ASSERT_TRUE(data.X_bit_sets[i].contains(j));
                    } else {
                        ASSERT_TRUE(data.X_bit_sets[i].notContains(j));
                    }
                    if(Z_matrix(i,j).val()) {
                        ASSERT_TRUE(data.Z_bit_sets[i].contains(j));
                    } else {
                        ASSERT_TRUE(data.Z_bit_sets[i].notContains(j));
                    }
                    if(O_matrix(i,j).val() == 0) {
                        ASSERT_TRUE(data.I_sets[i].contains(j));
                        ASSERT_TRUE(data.non_trivial_sets[i].notContains(j));
                    } else {
                        ASSERT_TRUE(data.I_sets[i].notContains(j));
                        ASSERT_TRUE(data.non_trivial_sets[i].contains(j));
                    }
                    if(O_matrix(i,j).val() == 1) {
                        ASSERT_TRUE(data.X_sets[i].contains(j));
                    } else {
                        ASSERT_TRUE(data.X_sets[i].notContains(j));
                    }
                    if(O_matrix(i,j).val() == 2) {
                        ASSERT_TRUE(data.Z_sets[i].contains(j));
                    } else {
                        ASSERT_TRUE(data.Z_sets[i].notContains(j));
                    }
                    if(O_matrix(i,j).val() == 3) {
                        ASSERT_TRUE(data.Y_sets[i].contains(j));
                    } else {
                        ASSERT_TRUE(data.Y_sets[i].notContains(j));
                    }
                }
            }
        }
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
//@-leo
