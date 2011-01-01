//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2561: * @thin weight.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2562: ** << Includes >>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "constraints.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101229110857.2563: ** Functions
void forEachRowWeightOrdering(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (const unsigned int start
                    ,const unsigned int end
                    ,auto_ptr<OperatorSpace> space
                    )
              > f
    ) {
        BOOST_FOREACH(unsigned int start, irange(0u,number_of_operators)) {
            BOOST_FOREACH(unsigned int end, irange(start+2,number_of_operators+1)) {
                auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
                postRowWeightOrderingConstraint(*space,start,end);
                f(start,end,space);
            }
        }
}
//@+node:gcross.20101229110857.2564: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(RowOrdering) { TEST_SUITE(Weight) {

//@+others
//@+node:gcross.20101229110857.2565: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int number_of_rows
    ,   const unsigned int expected_number_of_solutions
    ) {
        assert(number_of_rows <= number_of_operators);
        assert(number_of_rows >= 2);
        BOOST_FOREACH(const unsigned int start, irange(0u,number_of_operators-number_of_rows+1)) {
            const unsigned int end = start + number_of_rows;
            auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
            postRowWeightOrderingConstraint(*space,start,end);
            const unsigned int number_of_solutions = countSolutions(space);
            ASSERT_EQ(expected_number_of_solutions,number_of_solutions);
        }
    }

    DO_TEST_FOR_2(1,2,2,3*4+1)
    DO_TEST_FOR_2(1,3,2,(3*4+1)*4)
    DO_TEST_FOR_2(1,3,3,3*(3*4+1)+1)
    DO_TEST_FOR_2(2,2,2,1+(2*3)*(1+2*3)+3*3*4*4)

}
//@+node:gcross.20101229110857.2566: *3* correct solutions
TEST_SUITE(correct_solutions) {

    void doCheck(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        , const unsigned int start
        , const unsigned int end
        , auto_ptr<OperatorSpace> initial_space
    ) {
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            BOOST_FOREACH(const unsigned int row, irange(start,end-1)) {
                ASSERT_TRUE(space.weights[row].val() >= space.weights[row+1].val());
            }
        }
    }

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachRowWeightOrdering(number_of_qubits,number_of_operators,bind(doCheck,number_of_qubits,number_of_operators,_1,_2,_3));
    }

    DO_TEST_FOR(1,2)
    DO_TEST_FOR(1,3)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(2,3)
    DO_TEST_FOR(2,4)
    DO_TEST_FOR(3,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,2)

}
//@+node:gcross.20101229110857.2567: *3* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachRowWeightOrdering(
             number_of_qubits
            ,number_of_operators
            ,bind(checkCodes,_3)
        );
    }

    DO_TEST_FOR(1,2)
    DO_TEST_FOR(1,3)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(2,3)
    DO_TEST_FOR(2,4)
    DO_TEST_FOR(3,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,2)

}
//@-others

} } }
//@-others
//@-leo
