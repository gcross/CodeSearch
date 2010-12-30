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
