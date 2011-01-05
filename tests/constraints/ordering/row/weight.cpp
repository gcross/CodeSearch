//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2561: * @thin weight.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2562: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "constraints/ordering/row/weight.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110102182304.1604: ** Values
static set<Constraint> weight_row_ordering_constraints = list_of(WeightRowOrdering);
//@+node:gcross.20110104191728.1586: ** function checkCorrectness
void checkCorrectness(
      const OperatorSpace& space
    , const unsigned int start
    , const unsigned int end
) {
    if(end <= start) return;
    BOOST_FOREACH(const unsigned int row, irange(start,end-1)) {
        ASSERT_TRUE(space.weights[row].val() >= space.weights[row+1].val());
    }
}
//@+node:gcross.20101229110857.2564: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(RowOrdering) { TEST_SUITE(Weight) {

//@+others
//@+node:gcross.20110102182304.1605: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20101229110857.2563: *4* function forEachRowWeightOrdering
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
                postWeightRowOrderingConstraint(*space,start,end);
                f(start,end,space);
            }
        }
}
//@+node:gcross.20110104191728.1585: *4* function forEachRowWeightOrderingSolution
void forEachRowWeightOrderingSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (const OperatorSpace& space
                    ,const unsigned int start
                    ,const unsigned int end
                    )
              > f
    ) {
        BOOST_FOREACH(unsigned int start, irange(0u,number_of_operators)) {
            BOOST_FOREACH(unsigned int end, irange(start+2,number_of_operators+1)) {
                auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
                postWeightRowOrderingConstraint(*initial_space,start,end);
                for_each(generateSolutionsFor(initial_space),bind(f,_1,start,end));
            }
        }
}
//@+node:gcross.20101229110857.2565: *4* number of solutions
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
            postWeightRowOrderingConstraint(*space,start,end);
            const unsigned int number_of_solutions = countSolutions(space);
            ASSERT_EQ(expected_number_of_solutions,number_of_solutions);
        }
    }

    DO_TEST_FOR_2(1,2,2,3*4+1)
    DO_TEST_FOR_2(1,3,2,(3*4+1)*4)
    DO_TEST_FOR_2(1,3,3,3*(3*4+1)+1)
    DO_TEST_FOR_2(2,2,2,1+(2*3)*(1+2*3)+3*3*4*4)

}
//@+node:gcross.20101229110857.2566: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachRowWeightOrderingSolution(
             number_of_qubits
            ,number_of_operators
            ,checkCorrectness
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
//@+node:gcross.20101229110857.2567: *4* correct codes
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

}
//@+node:gcross.20110102182304.1608: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110104191728.1588: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void doCheck(
         const unsigned int number_of_qubits
        ,const unsigned int number_of_operators
        ,const StandardFormParameters& parameters
        ,const OperatorSpace& space
    ) {
        const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                         , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                         ;
        checkCorrectness(
              space
            , 0
            , x_bit_diagonal_size
        );
        checkCorrectness(
              space
            , x_bit_diagonal_size
            , x_bit_diagonal_size+z_bit_diagonal_size
        );
        checkCorrectness(
              space
            , x_bit_diagonal_size+z_bit_diagonal_size
            , number_of_operators
        );
    }

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,weight_row_ordering_constraints
            ,bind(doCheck
                ,number_of_qubits
                ,number_of_operators
                ,_1
                ,_2
             )
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
//@+node:gcross.20110102182304.1609: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,weight_row_ordering_constraints
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
