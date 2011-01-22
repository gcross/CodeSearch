//@+leo-ver=5-thin
//@+node:gcross.20110119165120.1730: * @thin all.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110119165120.1731: ** << Includes >>
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
//@+node:gcross.20110119165120.1733: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(All_constraints) {

//@+others
//@+node:gcross.20110119165120.1738: *3* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,all_constraints
        );
    }

    DO_NON_TRIVIAL_STANDARD_FORM_TESTS

}
//@-others

} }
//@-others
//@-leo
