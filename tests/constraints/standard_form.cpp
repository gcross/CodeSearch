//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1657: * @thin standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1658: ** << Includes >>
#include "constraints/standard_form.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101229110857.1659: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(StandardForm) {

//@+others
//@+node:gcross.20101229110857.1660: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest
    (   const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_FOREACH(unsigned int x_bit_diagonal_size, irange(0u,min(number_of_qubits,number_of_operators))) {
            BOOST_FOREACH(unsigned int z_bit_diagonal_size, irange(0u,min(number_of_qubits,number_of_operators)-x_bit_diagonal_size)) {
                auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
                postStandardFormConstraint(*space,x_bit_diagonal_size,z_bit_diagonal_size);
                const unsigned int total_diagonal_size = x_bit_diagonal_size + z_bit_diagonal_size
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
        }
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,4)

}
//@-others

} }
//@-others
//@-leo
