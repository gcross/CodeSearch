//@+leo-ver=5-thin
//@+node:gcross.20110114154616.2052: * @thin weight.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110114154616.2053: ** << Includes >>
#include <algorithm>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::assign;
using namespace std;

using boost::numeric::ublas::matrix;
//@-<< Includes >>

//@+others
//@+node:gcross.20110114154616.2054: ** Values
const static set<Constraint> weight_row_ordering_only_constraints = list_of(WeightRowOrdering);
//@+node:gcross.20110114154616.2055: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(WeightRowOrdering) {

//@+others
//@+node:gcross.20110114154616.2056: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20110114154616.2057: *4* correct number of solutions
TEST_SUITE(correct_number_of_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ,   const unsigned int expected_number_of_solutions
    ) {
        auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
        postWeightRowOrderingConstraintOnRegion(*initial_space,4,initial_space->getOMatrix());
        ASSERT_EQ(expected_number_of_solutions,countSolutions(initial_space));
    }

    DO_TEST_FOR_1(1,1,4)
    DO_TEST_FOR_1(1,2,1+3*4)
    DO_TEST_FOR_1(1,3,1+3*(1+3*4))

    DO_TEST_FOR_1(2,1,16)
    DO_TEST_FOR_1(2,2,3*3*4*4+6*7+1)
    DO_TEST_FOR_1(2,3,3*3*(3*3*4*4+6*7+1) + 6*(6*7+1) + 1)

    DO_TEST_FOR_1(3,1,4*4*4)
    DO_TEST_FOR_1(3,2,3*3*3*4*4*4+3*3*3*(3*3*3+3*3+1)+3*3*(3*3+1)+1)
}
//@+node:gcross.20110114154616.2058: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_LOCAL_FUNCTION(
            (void) (checkOMatrix)(
                (const IntMatrix&)(matrix)
            )
        ) {
            vector<unsigned int> weights;
            BOOST_FOREACH(
                 const IntVarArgs row
                ,irange(0u,(unsigned int)matrix.height())
                    | transformed(bind(&IntMatrix::row,matrix,_1))
            ) {
                weights.push_back(count_if(row.begin(),row.end(),bind(&IntVar::val,lambda::_1) > 0));
            }
            if(!is_sorted(weights | reversed)) {
                ostringstream message;
                message << "Bad weight order:";  for_each(weights,lambda::var(message) << " " << lambda::_1);
                FATALLY_FAIL(message.str());
            }
        } BOOST_LOCAL_FUNCTION_END(checkOMatrix)

        forEachOMatrixSolution(
             number_of_qubits
            ,number_of_operators
            ,bind(postWeightRowOrderingConstraintOnRegion,_1,4,_2,BoolVarArgs())
            ,checkOMatrix
        );
    }

    DO_USUAL_TESTS

}
//@+node:gcross.20110114154616.2059: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachOMatrix(
             number_of_qubits
            ,number_of_operators
            ,bind(postWeightRowOrderingConstraintOnRegion,_1,4,_2,BoolVarArgs())
            ,bind(checkCodes,_1,false)
        );
    }

    DO_USUAL_TESTS

}
//@-others

}
//@+node:gcross.20110114154616.2060: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110114154616.2061: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        BOOST_LOCAL_FUNCTION(
            (matrix<unsigned int>) (getOrdering)(
                (const unsigned int)(maximum_bound)
                (const matrix<unsigned int>&)(region)
            )
        ) {
            matrix<unsigned int> ordering(region.size2(),1);
            BOOST_FOREACH(const size_t row, irange((size_t)0u,region.size2())) {
                unsigned int weight = 0;
                BOOST_FOREACH(const size_t col, irange((size_t)0u,region.size1())) {
                    if(region(col,row)>0) ++weight;
                }
                ordering(row,0) = weight;
            }
            return ordering;
        } BOOST_LOCAL_FUNCTION_END(getOrdering)
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,weight_row_ordering_only_constraints
            ,bind(checkRowOrderings,getOrdering,_1,_2)
        );
    }

    DO_STANDARD_FORM_TESTS

}
//@+node:gcross.20110114154616.2062: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        checkCodesForConstraints(
             number_of_qubits
            ,number_of_operators
            ,weight_row_ordering_only_constraints
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
