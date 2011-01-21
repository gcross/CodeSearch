//@+leo-ver=5-thin
//@+node:gcross.20110120115216.2066: * @thin non_trivial_columns.cpp
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
//@+node:gcross.20110120115216.2068: ** Values
const static set<Constraint> non_trivial_columns_only_constraints = list_of(NonTrivialColumns);
//@+node:gcross.20110120115216.2138: ** function checkRegion
void checkRegion(const BoolMatrix& region) {
    BOOST_FOREACH(unsigned int col, irange(0u,(unsigned int)region.width())) {
        unsigned int number_of_bits = 0;
        BOOST_FOREACH(unsigned int row, irange(0u,(unsigned int)region.height())) {
            number_of_bits += region(col,row).val();
        }
        ASSERT_TRUE(number_of_bits > 0);
    }
}
//@+node:gcross.20110120115216.2069: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(NonTrivialColumns) {

//@+others
//@+node:gcross.20110120115216.2077: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20110120115216.2072: *4* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_LOCAL_FUNCTION(
            (void) (checkAllSolutions)(
                (auto_ptr<OperatorSpace>)(initial_space)
                (const unsigned int)(start_column)
                (const unsigned int)(end_column)
                (const unsigned int)(start_row)
                (const unsigned int)(end_row)
                (const bind)((number_of_qubits)(number_of_operators))
            )
        ) {
            const unsigned int number_of_columns = end_column-start_column
                             , number_of_rows = end_row-start_row
                             ;
            const unsigned long long
                               solutions_per_column = (1<<number_of_rows)-1
                             , observed_number_of_solutions = countSolutions(initial_space)
                             , expected_number_of_solutions =
                                    (1<<( 2*number_of_qubits*number_of_operators
                                         -number_of_rows*number_of_columns
                                        )
                                    )
                                  * power(solutions_per_column,number_of_columns)
                             ;
            ASSERT_EQ(expected_number_of_solutions,observed_number_of_solutions);
        } BOOST_LOCAL_FUNCTION_END(checkAllSolutions)
        forEachZMatrix(
             number_of_qubits
            ,number_of_operators
            ,postNonTrivialColumnsConstraintOnRegion
            ,checkAllSolutions
            );
    }

    DO_USUAL_TESTS

}
//@+node:gcross.20110120115216.2073: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachZMatrixSolution(
             number_of_qubits
            ,number_of_operators
            ,postNonTrivialColumnsConstraintOnRegion
            ,checkRegion
            );
    }

    DO_USUAL_TESTS

}
//@-others

}
//@+node:gcross.20110120115216.2078: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110120115216.2080: *4* correct solutions
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
            checkRegion(space.getZMatrix().slice(z_bit_diagonal_size,number_of_qubits,0u,number_of_operators));
            checkRegion(space.getXMatrix().slice(x_bit_diagonal_size,number_of_qubits,0u,x_bit_diagonal_size));
        } BOOST_LOCAL_FUNCTION_END(checkSolution)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,non_trivial_columns_only_constraints
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
            ,non_trivial_columns_only_constraints
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
