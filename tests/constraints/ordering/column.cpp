//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2459: * @thin column.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2460: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
using namespace std;

using boost::numeric::ublas::matrix;
//@-<< Includes >>

//@+others
//@+node:gcross.20110102182304.1596: ** Values
const static set<Constraint> column_ordering_only_constraints = list_of(ColumnOrdering);
//@+node:gcross.20101229110857.2467: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(ColumnOrdering) {

//@+others
//@+node:gcross.20110102182304.1582: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20110114113432.1481: *4* correct number of solutions
TEST_SUITE(correct_number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int expected_number_of_solutions
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        postColumnOrderingConstraintOnRegion(*initial_space,initial_space->getZMatrix());
        rel(*initial_space,initial_space->X,IRT_EQ,0);
        ASSERT_EQ(expected_number_of_solutions,countSolutions(initial_space));
    }

    DO_TEST_FOR_1(1,1,1<<1)
    DO_TEST_FOR_1(1,2,1<<2)
    DO_TEST_FOR_1(1,3,1<<3)
    DO_TEST_FOR_1(1,4,1<<4)

    DO_TEST_FOR_1(2,1,3)
    DO_TEST_FOR_1(2,2,2*3+1*4)
    DO_TEST_FOR_1(2,3,2*(2*3+1*4)+1*4*4)

    DO_TEST_FOR_1(3,1,4)
    DO_TEST_FOR_1(3,2,2*4+2*2*3)
}
//@+node:gcross.20101229110857.2484: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachZMatrixSolution(
             number_of_qubits
            ,number_of_operators
            ,postColumnOrderingConstraintOnRegion
            ,checkCorrectBoolMatrixOrdering
        );
    }

    DO_USUAL_TESTS

}
//@+node:gcross.20101229110857.2468: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachZMatrix(
             number_of_qubits
            ,number_of_operators
            ,postColumnOrderingConstraintOnRegion
            ,bind(checkCodes,_1)
        );
    }

    DO_USUAL_TESTS

}
//@-others

}
//@+node:gcross.20110102182304.1588: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110102182304.1592: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_LOCAL_FUNCTION(
            (void) (checkSolution)(
                (const StandardFormParameters&)(parameters)
                (const OperatorSpace&)(space)
                (const bind)((number_of_qubits)(number_of_operators))
            )
        ) {
            const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                             , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                             ;
            const BoolMatrix X_matrix = space.getXMatrix()
                           , Z_matrix = space.getZMatrix()
                           ;
            {
                matrix<unsigned int> correct_ordering(
                     number_of_qubits-x_bit_diagonal_size
                    ,number_of_operators+x_bit_diagonal_size
                );
                BOOST_FOREACH(const unsigned int col, irange(0u,number_of_qubits-x_bit_diagonal_size)) {
                    BOOST_FOREACH(const unsigned int row, irange(0u,number_of_operators)) {
                        correct_ordering(col,row) = Z_matrix(col+x_bit_diagonal_size,row).val();
                    }
                }
                BOOST_FOREACH(const unsigned int col, irange(0u,number_of_qubits-x_bit_diagonal_size)) {
                    BOOST_FOREACH(const unsigned int row, irange(0u,x_bit_diagonal_size)) {
                        correct_ordering(col,number_of_operators+row) = X_matrix(col+x_bit_diagonal_size,row).val();
                    }
                }
                checkCorrectOrdering(correct_ordering);
            }
            {
                matrix<unsigned int> correct_ordering(
                     x_bit_diagonal_size-z_bit_diagonal_size
                    ,2*z_bit_diagonal_size
                );
                BOOST_FOREACH(const unsigned int col, irange(0u,x_bit_diagonal_size-z_bit_diagonal_size)) {
                    BOOST_FOREACH(const unsigned int row, irange(0u,z_bit_diagonal_size)) {
                        correct_ordering(col,row) = Z_matrix(col+x_bit_diagonal_size,row).val();
                    }
                    BOOST_FOREACH(const unsigned int row, irange(0u,number_of_operators-x_bit_diagonal_size)) {
                        correct_ordering(col,z_bit_diagonal_size+row) = Z_matrix(col+x_bit_diagonal_size,row+x_bit_diagonal_size).val();
                    }
                }
                checkCorrectOrdering(correct_ordering);
            }
        } BOOST_LOCAL_FUNCTION_END(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,column_ordering_only_constraints
            ,checkSolution
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110104191728.1583: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,column_ordering_only_constraints
            ,bind(checkCodes,_2)
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
