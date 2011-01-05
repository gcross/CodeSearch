//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2459: * @thin column.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2460: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "constraints.hpp"
#include "constraints/ordering/column.hpp"
#include "utilities.hpp"

#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20110102182304.1596: ** Values
static set<Constraint> column_ordering_constraints = list_of(ColumnOrdering);
//@+node:gcross.20110104191728.1579: ** function checkCorrectness
template<typename Matrix> void checkCorrectness(Matrix matrix) {
    if(matrix.width() <= 1 || matrix.height() <= 0) return;
    BOOST_FOREACH(const int col, irange(0,matrix.width()-1)) {
        BOOST_FOREACH(const int row, irange(0,matrix.height())) {
            ASSERT_TRUE(matrix(col,row).val() >= matrix(col+1,row).val());
            if(matrix(col,row).val() > matrix(col+1,row).val()) break;
        }
    }
}
//@+node:gcross.20101229110857.2467: ** Tests
TEST_SUITE(Constraints) { TEST_SUITE(ColumnOrdering) {

//@+others
//@+node:gcross.20110102182304.1582: *3* for each subregion
TEST_SUITE(for_each_subregion) {

//@+others
//@+node:gcross.20101229110857.2461: *4* function forEachColumnOrdering
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
                postColumnOrderingConstraint(*space,start,end,0,number_of_operators);
                f(start,end,space);
            }
        }
}
//@+node:gcross.20110104191728.1578: *4* function forEachColumnOrderingSolution
void forEachColumnOrderingSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (const unsigned int start
                    ,const unsigned int end
                    ,const OperatorSpace& space
                    )
              > f
    ) {
        BOOST_FOREACH(unsigned int start, irange(0u,number_of_qubits)) {
            BOOST_FOREACH(unsigned int end, irange(start+2,number_of_qubits+1)) {
                auto_ptr<OperatorSpace> initial_space(new OperatorSpace(number_of_qubits,number_of_operators));
                postColumnOrderingConstraint(*initial_space,start,end,0,number_of_operators);
                for_each(generateSolutionsFor(initial_space),bind(f,start,end,_1));
            }
        }
}
//@+node:gcross.20101229110857.2474: *4* number of solutions
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
            postColumnOrderingConstraint(*space,start,end,0,number_of_operators);
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
//@+node:gcross.20101229110857.2484: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void runTest(
        const unsigned int number_of_qubits
    ,   const unsigned int number_of_operators
    ) {
        forEachColumnOrderingSolution(
             number_of_qubits
            ,number_of_operators
            ,bind(checkCorrectness<IntMatrix>
                ,bind(&IntMatrix::slice
                    ,bind(&OperatorSpace::getOMatrix,_3)
                    ,_1
                    ,_2
                    ,0u
                    ,number_of_operators
                 )
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
//@+node:gcross.20101229110857.2468: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachColumnOrdering(
             number_of_qubits
            ,number_of_operators
            ,bind(checkCodes,_3)
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
//@-others

}
//@+node:gcross.20110102182304.1588: *3* for each standard form
TEST_SUITE(for_each_standard_form) {

//@+others
//@+node:gcross.20110102182304.1592: *4* correct solutions
TEST_SUITE(correct_solutions) {

    void doCheck(
         const unsigned int number_of_qubits
        ,const unsigned int number_of_operators
        ,const StandardFormParameters& parameters
        ,const OperatorSpace& space
    ) {
        const unsigned int x_bit_diagonal_size = parameters.x_bit_diagonal_size
                         , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                         ;
        checkCorrectness<BoolMatrix>(space.getZMatrix().slice(
             0
            ,x_bit_diagonal_size
            ,0
            ,number_of_operators
        ));
        checkCorrectness<IntMatrix>(space.getOMatrix().slice(
             x_bit_diagonal_size+z_bit_diagonal_size
            ,number_of_qubits
            ,0
            ,x_bit_diagonal_size+z_bit_diagonal_size
        ));
        checkCorrectness<IntMatrix>(space.getOMatrix().slice(
             x_bit_diagonal_size
            ,x_bit_diagonal_size+z_bit_diagonal_size
            ,0
            ,x_bit_diagonal_size
        ));
    }

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardFormSolution(
             number_of_qubits
            ,number_of_operators
            ,column_ordering_constraints
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
//@+node:gcross.20110104191728.1583: *4* correct codes
TEST_SUITE(correct_codes) {

    void runTest(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
        forEachStandardForm(
             number_of_qubits
            ,number_of_operators
            ,column_ordering_constraints
            ,bind(checkCodes,_2)
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
//@-others

}
//@-others

} }
//@-others
//@-leo
