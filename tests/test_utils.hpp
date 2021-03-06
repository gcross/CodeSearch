//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2719: * @thin test_utils.hpp
//@@language cplusplus

#ifndef TEST_UTILS_HH
#define TEST_UTILS_HH

//@+<< Includes >>
//@+node:gcross.20101224191604.2720: ** << Includes >>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/irange.hpp>
#include <gecode/int.hh>
#include <illuminate.hpp>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <vector>

#include "constraints.hpp"
#include "solution_iterator.hpp"
#include "operator_space.hpp"

using namespace CodeQuest;
using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace std;

using boost::numeric::ublas::matrix;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2722: ** Functions
void checkCodes(
      auto_ptr<OperatorSpace> initial_space
    , const bool ignore_solutions_with_trivial_columns=false
);

void checkCodesForConstraints(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint> constraints
    , const bool ignore_solutions_with_trivial_columns=false
);

void checkCorrectBoolMatrixOrdering(const BoolMatrix& matrix);
void checkCorrectIntMatrixOrdering(const IntMatrix& matrix);
void checkCorrectOrdering(const matrix<unsigned int>& matrix);

void checkRowOrderings(
      function<matrix<unsigned int>
        ( const unsigned int maximum_value
        , const matrix<unsigned int>& region
        )
      > getOrdering
    , const StandardFormParameters& parameters
    , const OperatorSpace& space
);

unsigned long long choose(const unsigned int n,const unsigned int k);

matrix<unsigned int> concatenateMatricesVertically(vector<matrix<unsigned int> > matrices);
matrix<unsigned int> concatenateBoolMatricesVertically(vector<BoolMatrix> matrices);
matrix<unsigned int> concatenateIntMatricesVertically(vector<IntMatrix> matrices);

long long encodeOperatorSpace(const OperatorSpace& space);

matrix<unsigned int> extractFromBoolMatrix(const BoolMatrix& m);
matrix<unsigned int> extractFromIntMatrix(const IntMatrix& m);

const set<Code>& fetchAllCodes(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const bool ignore_solutions_with_trivial_columns=false
);

void forEachOMatrix(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,IntMatrix region
                    )
              > postOMatrixConstraint
    , function<void (auto_ptr<OperatorSpace> initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkAllSolutions
);

void forEachOMatrix(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,IntMatrix region
                    )
              > postOMatrixConstraint
    , function<void (auto_ptr<OperatorSpace> initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkAllSolutions
    , const unsigned int number_of_rows
    , const unsigned int number_of_columns
);

void forEachOMatrixSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,IntMatrix region
                    )
              > postOMatrixConstraint
    , function<void (const IntMatrix& region)> checkOMatrix
);

void forEachZMatrix(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,BoolMatrix region
                    )
              > postZMatrixConstraint
    , function<void (auto_ptr<OperatorSpace> initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkAllZMatrixSolutions
);

void forEachZMatrixSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,BoolMatrix region
                    )
              > postZMatrixConstraint
    , function<void (const BoolMatrix& region)> checkZMatrix
);

void forEachConstrainedRegion(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > postConstraint
    , function<void (auto_ptr<OperatorSpace> initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkAllSolutions
);

void forEachConstrainedRegionSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > postConstraint
    , function<void (OperatorSpace& space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkSolution
);

vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> space);

inline unsigned long long power(const unsigned long long base, unsigned int exponent) {
    unsigned long long result = 1;
    while(exponent-- > 0) result *= base;
    return result;
}

template<typename T> T square(T x) { return x*x; }
//@+node:gcross.20101224191604.3436: ** Macros
//@+node:gcross.20101229110857.1594: *3* DO_TEMPLATE_TEST_FOR
#define DO_TEMPLATE_TEST_FOR(number_of_operators,number_of_qubits) \
    TEST_CASE(_##number_of_operators##x##number_of_qubits) { \
        runTest<number_of_operators,number_of_qubits>(); \
    }
//@+node:gcross.20101224191604.3437: *3* DO_TEST_FOR
#define DO_TEST_FOR(number_of_operators,number_of_qubits) \
    TEST_CASE(_##number_of_operators##x##number_of_qubits) { \
        runTest(number_of_operators,number_of_qubits); \
    }
//@+node:gcross.20110112003748.1546: *3* DO_X_TESTS
#define DO_USUAL_TESTS \
    DO_TEST_FOR(1,1) \
    DO_TEST_FOR(1,2) \
    DO_TEST_FOR(1,3) \
    DO_TEST_FOR(2,1) \
    DO_TEST_FOR(2,2) \
    DO_TEST_FOR(2,3) \
    DO_TEST_FOR(2,4) \
    DO_TEST_FOR(3,1) \
    DO_TEST_FOR(3,2) \
    DO_TEST_FOR(3,3) \
    DO_TEST_FOR(4,1) \
    DO_TEST_FOR(4,2) \
    DO_TEST_FOR(5,1)

#define DO_STANDARD_FORM_TESTS \
    DO_TEST_FOR(1,1) \
    DO_TEST_FOR(2,1) \
    DO_TEST_FOR(2,2) \
    DO_TEST_FOR(2,3) \
    DO_TEST_FOR(3,1) \
    DO_TEST_FOR(3,2) \
    DO_TEST_FOR(3,3) \
    DO_TEST_FOR(3,4) \
    DO_TEST_FOR(4,1) \
    DO_TEST_FOR(4,2) \
    DO_TEST_FOR(4,3) \
    DO_TEST_FOR(5,1) \
    DO_TEST_FOR(5,2)

#define DO_NON_TRIVIAL_STANDARD_FORM_TESTS \
    DO_TEST_FOR(2,2) \
    DO_TEST_FOR(2,3) \
    DO_TEST_FOR(3,2) \
    DO_TEST_FOR(3,3) \
    DO_TEST_FOR(3,4) \
    DO_TEST_FOR(4,2) \
    DO_TEST_FOR(4,3) \
    DO_TEST_FOR(5,2)
//@+node:gcross.20101224191604.3438: *3* DO_TEST_FOR_1
#define DO_TEST_FOR_1(number_of_operators,number_of_qubits,a) \
    TEST_CASE(_##number_of_operators##x##number_of_qubits) { \
        runTest(number_of_operators,number_of_qubits,a); \
    }
//@+node:gcross.20101229110857.2476: *3* DO_TEST_FOR_2
#define DO_TEST_FOR_2(number_of_operators,number_of_qubits,a,b) \
    TEST_CASE(_##number_of_operators##x##number_of_qubits##_##a) { \
        runTest(number_of_operators,number_of_qubits,a,b); \
    }
//@+node:gcross.20110120115216.2142: *3* DO_TEST_FOR_3
#define DO_TEST_FOR_3(number_of_operators,number_of_qubits,a,b,c) \
    TEST_CASE(_##number_of_operators##x##number_of_qubits##_##a##_##b) { \
        runTest(number_of_operators,number_of_qubits,a,b,c); \
    }
//@-others

#endif
//@-leo
