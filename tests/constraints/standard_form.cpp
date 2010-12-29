//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1657: * @thin standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1658: ** << Includes >>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "constraints/standard_form.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101229110857.1667: ** Functions
void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (const unsigned int x_bit_diagonal_size
                    ,const unsigned int z_bit_diagonal_size
                    ,auto_ptr<OperatorSpace> space
                    )
              > f
    ) {
        BOOST_FOREACH(unsigned int x_bit_diagonal_size, irange(0u,min(number_of_qubits,number_of_operators)+1)) {
            BOOST_FOREACH(unsigned int z_bit_diagonal_size, irange(0u,min(number_of_qubits,number_of_operators)+1-x_bit_diagonal_size)) {
                auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
                postStandardFormConstraint(*space,x_bit_diagonal_size,z_bit_diagonal_size);
                f(x_bit_diagonal_size,z_bit_diagonal_size,space);
            }
        }
}
//@+node:gcross.20101229110857.1659: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(StandardForm) {

//@+others
//@+node:gcross.20101229110857.1660: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void doCheck(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        , const unsigned int x_bit_diagonal_size
        , const unsigned int z_bit_diagonal_size
        , auto_ptr<OperatorSpace> space
    ) {
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

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachStandardForm(number_of_qubits,number_of_operators,bind(doCheck,number_of_qubits,number_of_operators,_1,_2,_3));
    }

    DO_TEST_FOR(1,1)
    DO_TEST_FOR(2,2)
    DO_TEST_FOR(3,3)
    DO_TEST_FOR(4,4)

}
//@+node:gcross.20101229110857.1669: *3* correct codes
TEST_SUITE(correct_codes) {

    template
        < const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        >
    void runTest() {
        set<Code> codes;

        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,bind(
                 copy<CodeSet,insert_iterator<CodeSet> >
                ,bind(
                     gatherCodes<number_of_qubits,number_of_operators>
                    ,_3
                 )
                ,insert_iterator<CodeSet>(codes,codes.begin())
             )
        );

        const CodeSet& correct_codes = fetchAllCodes<number_of_qubits,number_of_operators>();
        ASSERT_EQ(correct_codes.size(),codes.size());
        ASSERT_TRUE(equal(correct_codes,codes));
    }

    DO_TEMPLATE_TEST_FOR(1,1)
    DO_TEMPLATE_TEST_FOR(1,2)
    DO_TEMPLATE_TEST_FOR(1,3)
    DO_TEMPLATE_TEST_FOR(2,1)
    DO_TEMPLATE_TEST_FOR(2,2)
    DO_TEMPLATE_TEST_FOR(2,3)
    DO_TEMPLATE_TEST_FOR(2,4)
    DO_TEMPLATE_TEST_FOR(3,1)
    DO_TEMPLATE_TEST_FOR(3,2)
    DO_TEMPLATE_TEST_FOR(3,3)
    DO_TEMPLATE_TEST_FOR(4,1)
    DO_TEMPLATE_TEST_FOR(4,2)
    DO_TEMPLATE_TEST_FOR(5,1)

}
//@-others

} }
//@-others
//@-leo
