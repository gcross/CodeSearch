//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2459: * @thin column_ordering.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2460: ** << Includes >>
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
//@+node:gcross.20101229110857.2461: ** Functions
void forEachColumnOrdering(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (const unsigned int start
                    ,const unsigned int end
                    ,auto_ptr<OperatorSpace> space
                    )
              > f
    ) {
        BOOST_FOREACH(unsigned int start, irange(0u,number_of_qubits)) {
            BOOST_FOREACH(unsigned int end, irange(start+2,number_of_qubits+1)) {
                auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
                postColumnOrderingConstraint(*space,start,end);
                f(start,end,space);
            }
        }
}
//@+node:gcross.20101229110857.2467: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(ColumnOrdering) {

//@+others
//@+node:gcross.20101229110857.2474: *3* number of solutions
TEST_SUITE(number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int number_of_columns
    ,   const unsigned int expected_number_of_solutions
    ) {
        assert(number_of_columns <= number_of_qubits);
        assert(number_of_columns >= 2);
        BOOST_FOREACH(const unsigned int start, irange(0u,number_of_qubits-number_of_columns+1)) {
            const unsigned int end = start + number_of_columns;
            auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
            postColumnOrderingConstraint(*space,start,end);
            const unsigned int number_of_solutions = countSolutions(space);
            ASSERT_EQ(expected_number_of_solutions,number_of_solutions);
        }
    }

    DO_TEST_FOR_2(2,1,2,4+6)
    DO_TEST_FOR_2(3,1,2,(4+6)*4)
    DO_TEST_FOR_2(3,1,3,4+2*6+4)
    DO_TEST_FOR_2(4,1,2,(4+6)*4*4)
    DO_TEST_FOR_2(4,1,3,(4+2*6+4)*4)
    DO_TEST_FOR_2(4,1,4,4+3*6+3*4+1)
    DO_TEST_FOR_2(2,2,2,4*(4+6)+6*16)
    DO_TEST_FOR_2(3,2,2,(4*(4+6)+6*16)*4*4)
    DO_TEST_FOR_2(3,2,3,4*(4+2*6+4)+2*6*4*(4+6)+4*4*4*4)

}
//@+node:gcross.20101229110857.2468: *3* correct codes
TEST_SUITE(correct_codes) {

    template
        < const unsigned int number_of_qubits
        , const unsigned int number_of_operators
        >
    void runTest() {
        forEachColumnOrdering(
             number_of_qubits
            ,number_of_operators
            ,bind(checkCodes<number_of_qubits,number_of_operators>,_3)
        );
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
