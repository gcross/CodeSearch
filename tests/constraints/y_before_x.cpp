//@+leo-ver=5-thin
//@+node:gcross.20110120115216.2128: * @thin y_before_x.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110120115216.2129: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <sstream>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "constraints/standard_form.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110120115216.2130: ** Values
const static set<Constraint> y_before_x_only_constraints = list_of(YBeforeX);
//@+node:gcross.20110120115216.2131: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(YBeforeX) {

//@+others
//@+node:gcross.20110120115216.2140: *3* function checkRegion
void checkRegion(const IntMatrix& region) {
    BOOST_FOREACH(unsigned int col, irange(0u,(unsigned int)region.width())) {
        BOOST_FOREACH(unsigned int row, irange(0u,(unsigned int)region.height())) {
            switch(region(col,row).val()) {
                case X: goto next_column;
                case Y: FATALLY_FAIL("Seen Y before X");
                default: break;
            }
        }
        next_column: ;
    }
}
//@+node:gcross.20110120115216.2132: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20110120115216.2133: *4* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int number_of_columns
    ,   const unsigned int number_of_rows
    ,   const unsigned int expected_number_of_solutions
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
            (auto_ptr<OperatorSpace> initial_space)
            (const unsigned int start_column)
            (const unsigned int end_column)
            (const unsigned int start_row)
            (const unsigned int end_row)
            (const bind number_of_qubits)
            (const bind number_of_operators)
            (const bind number_of_columns)
            (const bind number_of_rows)
            (const bind expected_number_of_solutions)
        ) {
            if(end_column-start_column != number_of_columns) return;
            if(end_row-start_row != number_of_rows) return;
            const unsigned int observed_number_of_solutions = countSolutions(initial_space);
            ASSERT_EQ(expected_number_of_solutions,observed_number_of_solutions);
        } BOOST_LOCAL_FUNCTION_NAME(checkAllSolutions)
        forEachOMatrix(
             number_of_qubits
            ,number_of_operators
            ,postYBeforeXConstraintOnRegion
            ,checkAllSolutions
            );
    }

    DO_TEST_FOR_3(1,1,1,1,3)
    DO_TEST_FOR_3(1,2,1,1,12)
    DO_TEST_FOR_3(1,2,1,2,3*3+1)
    DO_TEST_FOR_3(1,3,1,1,48)
    DO_TEST_FOR_3(1,3,1,2,(3*3+1)*4)
    DO_TEST_FOR_3(1,3,1,3,2*(2*3+4)+1*4*4)

    DO_TEST_FOR_3(2,1,1,1,3*4)
    DO_TEST_FOR_3(2,2,1,1,12*16)
    DO_TEST_FOR_3(2,2,1,2,(3*3+1)*16)
    DO_TEST_FOR_3(2,3,1,1,48*64)
    DO_TEST_FOR_3(2,3,1,2,((3*3+1)*4)*64)
    DO_TEST_FOR_3(2,3,1,3,(2*(2*3+4)+1*4*4)*64)

    DO_TEST_FOR_3(2,1,2,1,3*3)
    DO_TEST_FOR_3(2,2,2,1,12*12)
    DO_TEST_FOR_3(2,2,2,2,(3*3+1)*(3*3+1))
    DO_TEST_FOR_3(2,3,2,1,48*48)
    DO_TEST_FOR_3(2,3,2,2,((3*3+1)*4)*((3*3+1)*4))
    DO_TEST_FOR_3(2,3,2,3,(2*(2*3+4)+1*4*4)*(2*(2*3+4)+1*4*4))
}
//@+node:gcross.20110120115216.2134: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachOMatrixSolution(
             number_of_qubits
            ,number_of_operators
            ,postYBeforeXConstraintOnRegion
            ,checkRegion
            );
    }

    DO_USUAL_TESTS

}
//@+node:gcross.20110121100120.1585: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachOMatrix(
             number_of_qubits
            ,number_of_operators
            ,postYBeforeXConstraintOnRegion
            ,bind(checkCodes,_1,false)
        );
    }

    DO_USUAL_TESTS

}
//@-others

}
//@+node:gcross.20110120115216.2135: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110120115216.2136: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
            (const StandardFormParameters& parameters)
            (const OperatorSpace& space)
        ) {
            checkRegion(space.getOMatrix().slice(
                 parameters.x_bit_diagonal_size
                ,space.number_of_qubits
                ,0u
                ,parameters.x_bit_diagonal_size
            ));
        } BOOST_LOCAL_FUNCTION_NAME(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,y_before_x_only_constraints
            ,checkSolution
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110120115216.2137: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,y_before_x_only_constraints
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@-others

}
//@-others

} }
//@-others
//@-leo
