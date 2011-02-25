//@+leo-ver=5-thin
//@+node:gcross.20110120115216.2066: * @thin every_column_has.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110120115216.2067: ** << Includes >>
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
//@+node:gcross.20110120115216.2069: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(EveryColumnHasConstraint) {

//@+others
//@+node:gcross.20110120115216.2138: *3* function checkRegion
void checkRegion(const int O, const IntMatrix& region) {
    BOOST_FOREACH(unsigned int col, irange(0u,(unsigned int)region.width())) {
        BOOST_FOREACH(unsigned int row, irange(0u,(unsigned int)region.height())) {
            if(region(col,row).val() == O) goto next_column;
        }
        FATALLY_FAIL((
            format("Did not see operator value %1% in column %2%.")
                % O
                % col
        ).str());
        next_column: ;
    }
}
//@+node:gcross.20110121100120.1557: *3* EveryColumnHasO
TEST_SUITE(EveryColumnHasO) {

//@+others
//@+node:gcross.20110120115216.2072: *4* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
             auto_ptr<OperatorSpace> initial_space
            ,const unsigned int start_column
            ,const unsigned int end_column
            ,const unsigned int start_row
            ,const unsigned int end_row
            ,const bind number_of_qubits
            ,const bind number_of_operators
        ) {
            const unsigned int number_of_columns = end_column-start_column
                              ,number_of_rows = end_row-start_row
                              ;
            const unsigned long long observed_number_of_solutions = countSolutions(initial_space);
            unsigned long long number_of_solutions_in_each_region_column = 0u;
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_rows)) {
                number_of_solutions_in_each_region_column += power(3u,i) * power(4u,number_of_rows-i-1u);
            }
            const unsigned long long expected_number_of_solutions =
                power(number_of_solutions_in_each_region_column,number_of_columns)
             << 2*(number_of_qubits*number_of_operators-number_of_columns*number_of_rows)
            ;
            ASSERT_EQ(expected_number_of_solutions,observed_number_of_solutions);
        } BOOST_LOCAL_FUNCTION_NAME(checkAllSolutions)
        BOOST_FOREACH(const int O, irange(0,4)) {
            forEachOMatrix(
                 number_of_qubits
                ,number_of_operators
                ,bind(postEveryColumnHasOConstraintOnRegion,O,_1,_2)
                ,checkAllSolutions
                );
        }
    }

    DO_USUAL_TESTS

}
//@+node:gcross.20110120115216.2073: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_FOREACH(const int O, irange(0,4)) {
            forEachOMatrixSolution(
                 number_of_qubits
                ,number_of_operators
                ,bind(postEveryColumnHasOConstraintOnRegion,O,_1,_2)
                ,bind(checkRegion,O,_1)
                );
        }
    }

    DO_USUAL_TESTS

}
//@-others

}
//@+node:gcross.20110120115216.2078: *3* EveryColumnHasX
TEST_SUITE(EveryColumnHasX) {

//@+others
//@+node:gcross.20110120115216.2080: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
             const StandardFormParameters& parameters
            ,const OperatorSpace& space
        ) {
            checkRegion(X,space.getOMatrix().slice(
                 parameters.x_bit_diagonal_size
                ,space.number_of_qubits
                ,0u
                ,parameters.x_bit_diagonal_size
            ));
        } BOOST_LOCAL_FUNCTION_NAME(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasX)
            ,checkSolution
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110120115216.2074: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasX)
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@-others

}
//@+node:gcross.20110121100120.1561: *3* EveryColumnHasZ
TEST_SUITE(EveryColumnHasZ) {

//@+others
//@+node:gcross.20110121100120.1562: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
             const StandardFormParameters& parameters
            ,const OperatorSpace& space
        ) {
            checkRegion(Z,space.getOMatrix().slice(
                 parameters.z_bit_diagonal_size
                ,space.number_of_qubits
                ,0u
                ,space.number_of_operators
            ));
        } BOOST_LOCAL_FUNCTION_NAME(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasZ)
            ,checkSolution
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110121100120.1563: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasZ)
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@-others

}
//@+node:gcross.20110121100120.1567: *3* EveryColumnHasXAndZ
TEST_SUITE(EveryColumnHasXAndZ) {

//@+others
//@+node:gcross.20110121100120.1568: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
             const StandardFormParameters& parameters
            ,const OperatorSpace& space
        ) {
            checkRegion(X,space.getOMatrix().slice(
                 parameters.x_bit_diagonal_size
                ,space.number_of_qubits
                ,0u
                ,parameters.x_bit_diagonal_size
            ));
            checkRegion(Z,space.getOMatrix().slice(
                 parameters.z_bit_diagonal_size
                ,space.number_of_qubits
                ,0u
                ,space.number_of_operators
            ));
        } BOOST_LOCAL_FUNCTION_NAME(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasX)(EveryColumnHasZ)
            ,checkSolution
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110121100120.1569: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,list_of(EveryColumnHasX)(EveryColumnHasZ)
            ,true
        );
    }

    DO_NON_TRIVIAL_STANDARD_FORM_TESTS

}
//@-others

}
//@-others

} }
//@-others
//@-leo
