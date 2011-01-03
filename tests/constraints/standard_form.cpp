//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1657: * @thin standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1658: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

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
//@+node:gcross.20110102182304.1595: ** Values
static set<Constraint> standard_form_constraints = list_of(StandardForm);
//@+node:gcross.20101229110857.1659: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(StandardForm) {

//@+others
//@+node:gcross.20110102182304.1581: *3* number of standard forms
TEST_SUITE(number_of_standard_forms) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int expected_number_of_standard_forms
    ) {
        unsigned int number_of_standard_forms = 0;
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,standard_form_constraints
            ,++lambda::var(number_of_standard_forms)
        );
        ASSERT_EQ(expected_number_of_standard_forms,number_of_standard_forms);
    }

    DO_TEST_FOR_1(1,1,1)
    DO_TEST_FOR_1(1,2,1)
    DO_TEST_FOR_1(1,3,1)
    DO_TEST_FOR_1(2,1,1)
    DO_TEST_FOR_1(2,2,3)
    DO_TEST_FOR_1(2,3,3)
    DO_TEST_FOR_1(3,1,1)
    DO_TEST_FOR_1(3,2,3)
    DO_TEST_FOR_1(3,3,6)
    DO_TEST_FOR_1(3,4,6)
    DO_TEST_FOR_1(4,1,1)
    DO_TEST_FOR_1(4,2,3)
    DO_TEST_FOR_1(4,3,6)
    DO_TEST_FOR_1(4,4,10)
    DO_TEST_FOR_1(5,5,15)

}
//@+node:gcross.20101229110857.1660: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void doCheck(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        , const StandardFormParameters& parameters
        , auto_ptr<OperatorSpace> space
    ) {
        const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                         , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                         , total_diagonal_size = x_bit_diagonal_size + z_bit_diagonal_size
                         , remaining_rows = number_of_operators - total_diagonal_size
                         , remaining_cols = number_of_qubits - total_diagonal_size
                         , number_of_free_bits =
                                x_bit_diagonal_size*(2*(number_of_qubits - x_bit_diagonal_size) + number_of_operators)
                            +   z_bit_diagonal_size*remaining_cols
                         , expected_number_of_solutions = 1 << number_of_free_bits
                         , observed_number_of_solutions = countSolutions(space)
                         ;
        ASSERT_EQ(expected_number_of_solutions,observed_number_of_solutions);
    }

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,standard_form_constraints
            ,bind(doCheck,number_of_qubits,number_of_operators,_1,_2)
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
//@+node:gcross.20101229110857.2478: *3* correct solutions
TEST_SUITE(correct_solutions) {

    void doCheck(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        , const StandardFormParameters& parameters
        , auto_ptr<OperatorSpace> initial_space
    ) {
        const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                         , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                         , total_diagonal_size = x_bit_diagonal_size + z_bit_diagonal_size
                         , remaining_rows = number_of_operators - total_diagonal_size
                         , remaining_cols = number_of_qubits - total_diagonal_size;
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            { // X matrix
                const BoolMatrix X_matrix = space.getXMatrix();
                BOOST_FOREACH(const unsigned int row, irange(0u,x_bit_diagonal_size)) {
                    BOOST_FOREACH(const unsigned int col, irange(0u,x_bit_diagonal_size)) {
                        if(row == col) {
                            ASSERT_EQ(1,X_matrix(col,row).val());
                        } else {
                            ASSERT_EQ(0,X_matrix(col,row).val());
                        }
                    }
                }
                BOOST_FOREACH(const unsigned int row, irange(x_bit_diagonal_size,number_of_operators)) {
                    BOOST_FOREACH(const unsigned int col, irange(0u,number_of_qubits)) {
                        ASSERT_EQ(0,X_matrix(col,row).val());
                    }
                }
            }
            { // Z matrix
                const BoolMatrix Z_matrix = space.getZMatrix();
                BOOST_FOREACH(const unsigned int row, irange(x_bit_diagonal_size,total_diagonal_size)) {
                    BOOST_FOREACH(const unsigned int col, irange(x_bit_diagonal_size,total_diagonal_size)) {
                        if(row == col) {
                            ASSERT_EQ(1,Z_matrix(col,row).val());
                        } else {
                            ASSERT_EQ(0,Z_matrix(col,row).val());
                        }
                    }
                }
                BOOST_FOREACH(const unsigned int row, irange(total_diagonal_size,number_of_operators)) {
                    BOOST_FOREACH(const unsigned int col, irange(x_bit_diagonal_size,number_of_qubits)) {
                        ASSERT_EQ(0,Z_matrix(col,row).val());
                    }
                }
            }
        }
    }

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,standard_form_constraints
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
//@+node:gcross.20101229110857.1669: *3* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        set<Code> codes;

        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,standard_form_constraints
            ,bind(
                 copy<CodeSet,insert_iterator<CodeSet> >
                ,bind(
                     gatherCodes
                    ,_2
                 )
                ,insert_iterator<CodeSet>(codes,codes.begin())
             )
        );

        const CodeSet& correct_codes = fetchAllCodes(number_of_qubits,number_of_operators);
        ASSERT_EQ(correct_codes.size(),codes.size());
        ASSERT_TRUE(equal(correct_codes,codes));
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

} }
//@-others
//@-leo
