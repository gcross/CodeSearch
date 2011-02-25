//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1657: * @thin standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1658: ** << Includes >>
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
//@+node:gcross.20110102182304.1595: ** Values
const static set<Constraint> standard_form_only_constraints = list_of(StandardForm);
//@+node:gcross.20101229110857.1659: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(StandardForm) {

//@+others
//@+node:gcross.20110102182304.1581: *3* number of standard forms
TEST_SUITE(number_of_standard_forms) {

    void runTest(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) {
        const unsigned int maximum_diagonal_size = min(number_of_qubits,number_of_operators);
        ASSERT_EQ(
             maximum_diagonal_size-(number_of_operators+1)/2+1
            ,distance(StandardFormIterator(number_of_qubits,number_of_operators),StandardFormIterator())
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110114090455.1475: *3* correct standard forms
TEST_SUITE(correct_standard_forms) {

    void runTest(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) {
        vector<StandardFormParameters> parameters_list;
        copy(generateStandardFormsFor(number_of_qubits,number_of_operators),back_inserter(parameters_list));
        unsigned int correct_x_bit_diagonal_size = (number_of_operators+1)/2
                   , correct_z_bit_diagonal_size = number_of_operators-correct_x_bit_diagonal_size
                   ;
        BOOST_FOREACH(const StandardFormParameters& parameters, parameters_list) {
            if(parameters.x_bit_diagonal_size != correct_x_bit_diagonal_size++
            || parameters.z_bit_diagonal_size != correct_z_bit_diagonal_size--
            ) {
                ostringstream message;
                message << "Bad standard forms:";
                BOOST_FOREACH(const StandardFormParameters& parameters, parameters_list) {
                    message << (format(" (%1%,%2%)") % parameters.x_bit_diagonal_size % parameters.z_bit_diagonal_size);
                }
                FATALLY_FAIL(message.str());
            }
        }
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20101229110857.1660: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        void BOOST_LOCAL_FUNCTION_PARAMS(
             const StandardFormParameters& parameters
            ,auto_ptr<OperatorSpace> initial_space
            ,const bind number_of_qubits
            ,const bind number_of_operators
        ) {
            const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                             , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                             , number_of_free_bits =
                                  (number_of_qubits-z_bit_diagonal_size)*number_of_operators
                                + (number_of_qubits-x_bit_diagonal_size-1)*(x_bit_diagonal_size-z_bit_diagonal_size)
                             , expected_number_of_solutions = 1 << number_of_free_bits
                             , observed_number_of_solutions = countSolutions(initial_space)
                             ;
            ASSERT_EQ(expected_number_of_solutions,observed_number_of_solutions);
        } BOOST_LOCAL_FUNCTION_NAME(checkAllSolutions)
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,standard_form_only_constraints
            ,checkAllSolutions
            );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20101229110857.2478: *3* correct solutions
TEST_SUITE(correct_solutions) {

    void checkSolution(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        , const StandardFormParameters& parameters
        , const OperatorSpace& space
    ) {
        const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                         , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                         ;
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
            BOOST_FOREACH(const unsigned int col, irange(x_bit_diagonal_size,number_of_qubits)) {
                BOOST_FOREACH(const unsigned int row,irange(0u,z_bit_diagonal_size)) {
                    ASSERT_EQ(0,X_matrix(col,row).val());
                }
            }
            BOOST_FOREACH(const unsigned int col, irange(0u,number_of_qubits)) {
                BOOST_FOREACH(const unsigned int row,irange(x_bit_diagonal_size,number_of_operators)) {
                    ASSERT_EQ(0,X_matrix(col,row).val());
                }
            }
        }
        { // Z matrix
            const BoolMatrix Z_matrix = space.getZMatrix();
            BOOST_FOREACH(const unsigned int row, irange(0u,z_bit_diagonal_size)) {
                BOOST_FOREACH(const unsigned int col, irange(0u,z_bit_diagonal_size)) {
                    if(row == col) {
                        ASSERT_EQ(1,Z_matrix(col,x_bit_diagonal_size+row).val());
                    } else {
                        ASSERT_EQ(0,Z_matrix(col,x_bit_diagonal_size+row).val());
                    }
                }
            }
            BOOST_FOREACH(const unsigned int row, irange(0u,x_bit_diagonal_size)) {
                BOOST_FOREACH(const unsigned int col, irange(0u,z_bit_diagonal_size)) {
                    ASSERT_EQ(0,Z_matrix(col,row).val());
                }
            }
            BOOST_FOREACH(const unsigned int row, irange(z_bit_diagonal_size,x_bit_diagonal_size)) {
                ASSERT_EQ(0,Z_matrix(row,row).val());
            }
        }
    }

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,standard_form_only_constraints
            ,bind(checkSolution,number_of_qubits,number_of_operators,_1,_2)
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20101229110857.1669: *3* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,standard_form_only_constraints
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@-others

} }
//@-others
//@-leo
