//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2647: * @thin operator_space.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2648: ** << Includes >>
#include <boost/lambda/lambda.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
#include <sstream>
#include <illuminate.hpp>

#include "operator_space.hpp"
#include "solution_iterator.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2654: ** Tests
TEST_SUITE(OperatorSpace) {

//@+others
//@+node:gcross.20101224191604.2773: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest
    (   const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int expected_number_of_solutions
    ) {
        ASSERT_EQ(expected_number_of_solutions,countSolutions(wrapAutoPtr(new OperatorSpace(number_of_qubits,number_of_operators))));
    }

    DO_TEST_FOR_1(1,1,4)
    DO_TEST_FOR_1(1,2,16)
    DO_TEST_FOR_1(2,1,16)
    DO_TEST_FOR_1(2,2,256)
    DO_TEST_FOR_1(3,3,1<<(2*3*3))

}
//@+node:gcross.20101224191604.2776: *3* correct order
TEST_SUITE(correct_order) {

    void runTest
    (   const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        unsigned long solution_number = 0u;
        BOOST_FOREACH(
            const OperatorSpace& space,
            generateSolutionsFor(wrapAutoPtr(new OperatorSpace(number_of_qubits,number_of_operators)))
        ) {
            unsigned long x = solution_number;
            BOOST_FOREACH(unsigned long long i,irange(0u,space.number_of_variables) | reversed) {
                ASSERT_EQ(x%2,(unsigned int)space.X[i].val());
                x >>= 1;
                ASSERT_EQ(x%2,(unsigned int)space.Z[i].val());
                x >>= 1;
            }
            ++solution_number;
        }
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)

}
//@+node:gcross.20101224191604.3435: *3* correct operators
TEST_SUITE(correct_operators) {

    void runTest
    (   const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        auto_ptr<OperatorSpace> initial_space_ptr(new OperatorSpace(number_of_qubits,number_of_operators));
        dynamic_operator_vector operators;
        initial_space_ptr->initializeOperators(operators);
        BOOST_FOREACH(
            const OperatorSpace& space,
            generateSolutionsFor(initial_space_ptr)
        ) {
            space.updateOperators(operators);
            auto_ptr<dynamic_operator_vector> operators_(space.getOperators<dynamic_operator_vector>());
            ostringstream operators_s, operators_s_;
            BOOST_FOREACH(const dynamic_quantum_operator& op, operators) {
                operators_s << op << ',';
            }
            BOOST_FOREACH(const dynamic_quantum_operator& op, *operators_) {
                operators_s_ << op << ',';
            }
            ASSERT_EQ(operators_s_.str(),operators_s.str());
            IntMatrix O_matrix = space.getOMatrix();
            BOOST_FOREACH(unsigned int i, irange(0u,number_of_operators)) {
                BOOST_FOREACH(unsigned int j, irange(0u,number_of_qubits)) {
                    ASSERT_EQ(O_matrix(j,i).val(),operators[i][j]);
                }
            }
        }
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(3,3)

}
//@+node:gcross.20101224191604.3440: *3* correct properties
TEST_SUITE(correct_properties) {

    void runTest
    (   const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        ASSERT_EQ(number_of_operators,initial_space->number_of_operators);
        ASSERT_EQ(number_of_qubits,initial_space->number_of_qubits);
        BOOST_FOREACH(
            const OperatorSpace& space,
            generateSolutionsFor(initial_space)
        ) {
            BOOST_FOREACH(unsigned int i, irange(0,space.non_trivial.size())) {
                ASSERT_EQ(space.X[i].val() || space.Z[i].val(),space.non_trivial[i].val());
            }
            BoolMatrix non_trivial_matrix = space.getNonTrivialMatrix();
            BOOST_FOREACH(unsigned int i, irange(0u,number_of_operators)) {
                unsigned int correct_weight = 0;
                BOOST_FOREACH(unsigned int j, irange(0u,number_of_qubits)) {
                    correct_weight += non_trivial_matrix(j,i).val();
                }
                ASSERT_EQ(correct_weight,(unsigned int)space.weights[i].val());
            }
        }
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(3,3)

}
//@+node:gcross.20101224191604.3442: *3* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, unsigned int number_of_operators) {
        checkCodes(wrapAutoPtr(new OperatorSpace(number_of_qubits,number_of_operators)));
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(1,2)
    DO_TEST_FOR(2,1)
    DO_TEST_FOR(2,2)

}
//@-others

}
//@-others
//@-leo
