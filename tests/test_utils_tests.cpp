//@+leo-ver=5-thin
//@+node:gcross.20101224191604.4011: * @thin test_utils_tests.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.4012: ** << Includes >>
#include <boost/range/adaptor/transformed.hpp>
#include <illuminate.hpp>

#include "solution_iterator.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.4013: ** Tests
TEST_SUITE(Test_Utils) {

//@+others
//@+node:gcross.20101224191604.4014: *3* longFromOperatorSpace
TEST_SUITE(longFromOperatorSpace) {

//@+others
//@+node:gcross.20101224191604.4015: *4* correctly ordered
TEST_SUITE(correctly_ordered) {

    void runTest(int number_of_operators, int number_of_qubits) {
        unsigned long long correct = 0u;
        BOOST_FOREACH
        (   unsigned long long observed
        ,   generateSolutionsFor(
                wrapAutoPtr(
                    new OperatorSpace(number_of_operators,number_of_qubits)
                )
            ) | transformed(encodeOperatorSpace)
        ) {
            ASSERT_EQ(correct,observed);
            ++correct;
        }
    }

    DO_TEST_FOR(1,1);
    DO_TEST_FOR(1,2);
    DO_TEST_FOR(1,3);
    DO_TEST_FOR(2,1);
    DO_TEST_FOR(2,2);
    DO_TEST_FOR(2,3);
    DO_TEST_FOR(3,1);
    DO_TEST_FOR(3,2);
    DO_TEST_FOR(3,3);

}
//@-others

}
//@-others

};
//@-others
//@-leo
