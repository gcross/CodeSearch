//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2748: * @thin test_utils.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2749: ** << Includes >>
#include <algorithm>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/set_algorithm.hpp>
#include <boost/range/irange.hpp>
#include <codequest.hpp>
#include <functional>
#include <illuminate.hpp>
#include <map>

#include "boost/local/function.hpp"
#include "solution_iterator.hpp"
#include "utilities.hpp"
#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace boost::assign;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101231214817.2058: ** Typedefs
typedef map<pair<unsigned int,unsigned int>,set<Code> > CodeTable;
//@+node:gcross.20101229110857.1592: ** Variables
CodeTable operator_space_code_table;
//@+node:gcross.20110114113432.1476: ** struct Code
//@+node:gcross.20110114113432.1477: *3* operator<
bool Code::operator<(const Code& c) const {
    if(number_of_stabilizers < c.number_of_stabilizers) return true;
    if(number_of_stabilizers > c.number_of_stabilizers) return false;
    if(number_of_gauge_qubits < c.number_of_gauge_qubits) return true;
    if(number_of_gauge_qubits > c.number_of_gauge_qubits) return false;
    if(logical_qubit_distances.size() < c.logical_qubit_distances.size()) return true;
    if(logical_qubit_distances.size() > c.logical_qubit_distances.size()) return false;
    BOOST_FOREACH(const unsigned int i, irange((size_t)0u,logical_qubit_distances.size())) {
        if(logical_qubit_distances[i] < c.logical_qubit_distances[i]) return true;
        if(logical_qubit_distances[i] > c.logical_qubit_distances[i]) return false;
    }
    return false;
}
//@+node:gcross.20110114113432.1478: *3* operator==
bool Code::operator==(const Code& c) const {
    if(number_of_stabilizers != c.number_of_stabilizers) return false;
    if(number_of_gauge_qubits != c.number_of_gauge_qubits) return false;
    if(logical_qubit_distances.size() != c.logical_qubit_distances.size()) return false;
    BOOST_FOREACH(const unsigned int i, irange((size_t)0u,logical_qubit_distances.size())) {
        if(logical_qubit_distances[i] != c.logical_qubit_distances[i]) return false;
    }
    return true;
}
//@+node:gcross.20110114113432.1479: *3* toString
string Code::toString() const {
    ostringstream s;
    s << number_of_stabilizers << " " << number_of_gauge_qubits << " |";
    BOOST_FOREACH(const size_t distance, logical_qubit_distances) { s << " " << distance; }
    return s.str();
}
//@+node:gcross.20101224191604.2750: ** Functions
//@+node:gcross.20101231214817.2055: *3* checkCodes
void checkCodes(auto_ptr<OperatorSpace> initial_space) {
    const set<Code>& operator_space_codes = fetchAllCodes(initial_space->number_of_qubits,initial_space->number_of_operators);
    const set<Code> constrained_space_codes = gatherCodes(initial_space);

    vector<Code> missing_codes;
    set_difference(operator_space_codes,constrained_space_codes,back_inserter(missing_codes));

    BOOST_FOREACH(const Code& code, missing_codes) {
        FAIL(string("Missing code: ") + code.toString());
    }
}
//@+node:gcross.20110110211728.1589: *3* checkCorrectXOrdering
void checkCorrectOrdering(const matrix<unsigned int>& ordering_matrix) {
    if(ordering_matrix.size1() <= 1 || ordering_matrix.size2() <= 0) return;
    BOOST_FOREACH(const size_t col, irange((size_t)0u,ordering_matrix.size1()-1u)) {
        BOOST_FOREACH(const size_t row, irange((size_t)0u,ordering_matrix.size2())) {
            ASSERT_TRUE(ordering_matrix(col,row) >= ordering_matrix(col+1,row));
            if(ordering_matrix(col,row) > ordering_matrix(col+1,row)) break;
        }
    }
}

void checkCorrectBoolMatrixOrdering(const BoolMatrix& ordering_matrix) {
    return checkCorrectOrdering(extractFromBoolMatrix(ordering_matrix));
}

void checkCorrectIntMatrixOrdering(const IntMatrix& ordering_matrix) {
    return checkCorrectOrdering(extractFromIntMatrix(ordering_matrix));
}
//@+node:gcross.20110114154616.2071: *3* checkRowOrderings
void checkRowOrderings(
      function<matrix<unsigned int>
        ( const unsigned int maximum_value
        , const matrix<unsigned int>& region
        )
      > getOrdering
    , const StandardFormParameters& parameters
    , const OperatorSpace& space
) {
    const unsigned int number_of_qubits = space.number_of_qubits
                     , number_of_operators = space.number_of_operators
                     , x_bit_diagonal_size = parameters.x_bit_diagonal_size
                     , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                     ;
    const IntMatrix O_matrix = space.getOMatrix();
    const BoolMatrix Z_matrix = space.getZMatrix();
    checkCorrectOrdering(concatenateMatricesVertically(
        list_of(getOrdering(4,extractFromIntMatrix(O_matrix.slice(
                  x_bit_diagonal_size
                , number_of_qubits
                , 0
                , x_bit_diagonal_size
               ))))
               (getOrdering(2,extractFromBoolMatrix(Z_matrix.slice(
                  z_bit_diagonal_size
                , x_bit_diagonal_size
                , 0
                , z_bit_diagonal_size
               ))))
               (getOrdering(2,extractFromBoolMatrix(Z_matrix.slice(
                  z_bit_diagonal_size
                , number_of_qubits
                , x_bit_diagonal_size
                , number_of_operators
               ))))
    ));
    checkCorrectOrdering(getOrdering(2,extractFromBoolMatrix(Z_matrix.slice(
                  x_bit_diagonal_size
                , number_of_qubits
                , z_bit_diagonal_size
                , x_bit_diagonal_size
    ))));
}
//@+node:gcross.20110114113432.1705: *3* concatenateXMatricesVertically
matrix<unsigned int> concatenateMatricesVertically(vector<matrix<unsigned int> > matrices) {
    if(matrices.size() == 0) return matrix<unsigned int>(0,0);
    const size_t width = matrices[0].size1();
    size_t height = 0u;
    BOOST_FOREACH(const matrix<unsigned int>& m, matrices) {
        assert(m.size1() == width);
        height += m.size2();
    }
    if(width == 0 || height == 0) return matrix<unsigned int>(0,0);
    matrix<unsigned int> concatenated_matrix(width,height);
    unsigned int current_row = 0;
    BOOST_FOREACH(const matrix<unsigned int>& m, matrices) {
        BOOST_FOREACH(const size_t row, irange((size_t)0u,m.size2())) {
            BOOST_FOREACH(const size_t col, irange((size_t)0u,width)) {
                concatenated_matrix(col,current_row) = m(col,row);
            }
            ++current_row;
        }
    }
    return concatenated_matrix;
}

matrix<unsigned int> concatenateIntMatricesVertically(vector<IntMatrix> matrices) {
    vector<matrix<unsigned int> > matrices_;
    copy(matrices | transformed(extractFromIntMatrix),back_inserter(matrices_));
    return concatenateMatricesVertically(matrices_);
}

matrix<unsigned int> concatenateBoolMatricesVertically(vector<BoolMatrix> matrices) {
    vector<matrix<unsigned int> > matrices_;
    copy(matrices | transformed(extractFromBoolMatrix),back_inserter(matrices_));
    return concatenateMatricesVertically(matrices_);
}
//@+node:gcross.20101224191604.4005: *3* encodeOperatorSpace
long long encodeOperatorSpace(const OperatorSpace& space) {
    unsigned long long solution = 0u;
    BOOST_FOREACH(unsigned int i, irange(0u,space.number_of_variables)) {
        solution <<= 2;
        solution += space.O[i].val();
    }
    return solution;
}
//@+node:gcross.20110114154616.2070: *3* extractFromXMatrix
matrix<unsigned int> extractFromBoolMatrix(const BoolMatrix& m) {
    matrix<unsigned int> extracted_matrix(m.width(),m.height());
    BOOST_FOREACH(const size_t col, irange((size_t)0u,extracted_matrix.size1())) {
        BOOST_FOREACH(const size_t row, irange((size_t)0u,extracted_matrix.size2())) {
            extracted_matrix(col,row) = m(col,row).val();
        }
    }
    return extracted_matrix;
}

matrix<unsigned int> extractFromIntMatrix(const IntMatrix& m) {
    matrix<unsigned int> extracted_matrix(m.width(),m.height());
    BOOST_FOREACH(const size_t col, irange((size_t)0u,extracted_matrix.size1())) {
        BOOST_FOREACH(const size_t row, irange((size_t)0u,extracted_matrix.size2())) {
            extracted_matrix(col,row) = m(col,row).val();
        }
    }
    return extracted_matrix;
}
//@+node:gcross.20101231214817.2057: *3* fetchAllCodes
const set<Code>& fetchAllCodes(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
    typedef CodeTable::const_iterator CodeTablePosition;

    const pair<unsigned int,unsigned int> id = make_pair(number_of_operators,number_of_qubits);

    CodeTablePosition operator_space_code_position = operator_space_code_table.find(id);
    if(operator_space_code_position == operator_space_code_table.end()) {
        operator_space_code_position =
            operator_space_code_table.insert(
                make_pair(
                    id,
                    gatherCodes(
                        wrapAutoPtr(
                            new OperatorSpace(
                                    number_of_qubits,
                                    number_of_operators
                            )
                        )
                    )
                )
            ).first;
    }

    return operator_space_code_position->second;
}
//@+node:gcross.20110114154616.2066: *3* forEachOMatrix
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
) {
    BOOST_LOCAL_FUNCTION(
        (void) (postConstraint)(
            (OperatorSpace&)(initial_space)
            (const unsigned int)(start_column)
            (const unsigned int)(end_column)
            (const unsigned int)(start_row)
            (const unsigned int)(end_row)
            (const bind)((postOMatrixConstraint))
        )
    ) {
        postOMatrixConstraint(initial_space,initial_space.getOMatrix().slice(start_column,end_column,start_row,end_row));
    } BOOST_LOCAL_FUNCTION_END(postConstraint)
    forEachConstrainedRegion(
         number_of_qubits
        ,number_of_operators
        ,postConstraint
        ,checkAllSolutions
    );
}
//@+node:gcross.20110114154616.2068: *3* forEachOMatrixSolution
void forEachOMatrixSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,IntMatrix region
                    )
              > postOMatrixConstraint
    , function<void (const IntMatrix& region)> checkOMatrix
) {
    BOOST_LOCAL_FUNCTION(
        (void) (checkAllSolutions)(
            (auto_ptr<OperatorSpace>)(initial_space)
            (const unsigned int)(start_column)
            (const unsigned int)(end_column)
            (const unsigned int)(start_row)
            (const unsigned int)(end_row)
            (const bind)((checkOMatrix))
        )
    ) {
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            checkOMatrix(space.getOMatrix().slice(start_column,end_column,start_row,end_row));
        }
    } BOOST_LOCAL_FUNCTION_END(checkAllSolutions)
    forEachOMatrix(
         number_of_qubits
        ,number_of_operators
        ,postOMatrixConstraint
        ,checkAllSolutions
    );
}
//@+node:gcross.20110111130332.2153: *3* forEachZMatrix
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
              > checkAllSolutions
) {
    BOOST_LOCAL_FUNCTION(
        (void) (postConstraint)(
            (OperatorSpace&)(initial_space)
            (const unsigned int)(start_column)
            (const unsigned int)(end_column)
            (const unsigned int)(start_row)
            (const unsigned int)(end_row)
            (const bind)((postZMatrixConstraint))
        )
    ) {
        postZMatrixConstraint(initial_space,initial_space.getZMatrix().slice(start_column,end_column,start_row,end_row));
    } BOOST_LOCAL_FUNCTION_END(postConstraint)
    forEachConstrainedRegion(
         number_of_qubits
        ,number_of_operators
        ,postConstraint
        ,checkAllSolutions
    );
}
//@+node:gcross.20110112003748.1545: *3* forEachZMatrixSolution
void forEachZMatrixSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , function<void (OperatorSpace& initial_space
                    ,BoolMatrix region
                    )
              > postZMatrixConstraint
    , function<void (const BoolMatrix& region)> checkZMatrix
) {
    BOOST_LOCAL_FUNCTION(
        (void) (checkAllSolutions)(
            (auto_ptr<OperatorSpace>)(initial_space)
            (const unsigned int)(start_column)
            (const unsigned int)(end_column)
            (const unsigned int)(start_row)
            (const unsigned int)(end_row)
            (const bind)((checkZMatrix))
        )
    ) {
        BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) {
            checkZMatrix(space.getZMatrix().slice(start_column,end_column,start_row,end_row));
        }
    } BOOST_LOCAL_FUNCTION_END(checkAllSolutions)
    forEachZMatrix(
         number_of_qubits
        ,number_of_operators
        ,postZMatrixConstraint
        ,checkAllSolutions
    );
}
//@+node:gcross.20110111130332.1877: *3* forEachConstrainedRegion
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
              > runTest
) {
    BOOST_FOREACH(const unsigned int start_column, irange(0u,number_of_qubits-1)) {
        BOOST_FOREACH(const unsigned int end_column, irange(start_column+1,number_of_qubits)) {
            BOOST_FOREACH(const unsigned int start_row, irange(0u,number_of_operators-1)) {
                BOOST_FOREACH(const unsigned int end_row, irange(start_row+1,number_of_operators)) {
                    auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
                    postConstraint(*space,start_column,end_column,start_row,end_row);
                    runTest(space,start_column,end_column,start_row,end_row);
                }
            }
        }
    }
}
//@+node:gcross.20110111130332.1879: *3* forEachConstrainedRegionSolution
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
    , function<void (const OperatorSpace& space
                    ,const unsigned int start_column
                    ,const unsigned int end_column
                    ,const unsigned int start_row
                    ,const unsigned int end_row
                    )
              > checkSolution
) {
    BOOST_LOCAL_FUNCTION(
        (void) (checkAllSolutions)(
            (auto_ptr<OperatorSpace>)(initial_space)
            (const unsigned int)(start_column)
            (const unsigned int)(end_column)
            (const unsigned int)(start_row)
            (const unsigned int)(end_row)
            (const bind)((checkSolution))
        )
    ) {
        for_each(
              generateSolutionsFor(initial_space)
            , bind(checkSolution
                , _1
                , start_column
                , end_column
                , start_row
                , end_row
              )
        );
    } BOOST_LOCAL_FUNCTION_END(checkAllSolutions)
    forEachConstrainedRegion(
          number_of_qubits
        , number_of_operators
        , postConstraint
        , checkAllSolutions
    );
}
//@+node:gcross.20110102182304.1594: *3* forEachStandardForm
void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,auto_ptr<OperatorSpace> space
                    )
              > checkAllSolutions
    ) {
        BOOST_FOREACH(const StandardFormParameters& parameters, generateStandardFormsFor(number_of_operators)) {
            checkAllSolutions(
                 parameters
                ,createConstrainedSpace(
                     number_of_qubits
                    ,number_of_operators
                    ,constraints
                    ,parameters
                 )
            );
        }
}
//@+node:gcross.20110104191728.1581: *3* forEachStandardFormSolution
void forEachStandardFormSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,const OperatorSpace& space
                    )
              > checkSolution
) {
    BOOST_LOCAL_FUNCTION(
        (void) (checkAllSolutions)(
            (const StandardFormParameters&)(parameters)
            (auto_ptr<OperatorSpace>)(initial_space)
            (const bind)((checkSolution))
        )
    ) {
        for_each(
             generateSolutionsFor(initial_space)
            ,bind(checkSolution,parameters,_1)
        );
    } BOOST_LOCAL_FUNCTION_END(checkAllSolutions)
    forEachStandardForm(
         number_of_qubits
        ,number_of_operators
        ,constraints
        ,checkAllSolutions
    );
}
//@+node:gcross.20101231214817.2053: *3* gatherCodes
template<unsigned int number_of_qubits, unsigned int number_of_operators> set<Code> gatherCodesImpl(auto_ptr<OperatorSpace> initial_space) {
    assert(number_of_qubits == initial_space->number_of_qubits);
    assert(number_of_operators == initial_space->number_of_operators);

    set<Code> codes;

    typedef static_qec<number_of_qubits,number_of_operators> qec_t;

    BOOST_FOREACH
    (   auto_ptr<qec_t> code
    ,   generateSolutionsFor(initial_space)
            | transformed(computeOptimizedCodeForOperatorSpace<qec_t>)
    ) {
        const unsigned int
            number_of_stabilizers = code->stabilizers.size(),
            number_of_gauge_qubits = code->gauge_qubits.size(),
            number_of_logical_qubits = code->logical_qubit_error_distances.size();
        if((number_of_stabilizers > 0u || number_of_gauge_qubits > 0u || number_of_logical_qubits > 0u)
        && (number_of_stabilizers + 2*number_of_gauge_qubits == number_of_operators)
        ) {
            codes.insert(Code(number_of_stabilizers,number_of_gauge_qubits,code->logical_qubit_error_distances));
        }
    }

    return codes;
}

typedef map<
     pair<unsigned int,unsigned int>
    ,function<set<Code> (auto_ptr<OperatorSpace>)>
    > gatherCodesImplsType;

const gatherCodesImplsType gatherCodesImpls = map_list_of
        (make_pair(1u,1u),gatherCodesImpl<1,1>)
        (make_pair(1u,2u),gatherCodesImpl<1,2>)
        (make_pair(1u,3u),gatherCodesImpl<1,3>)
        (make_pair(1u,4u),gatherCodesImpl<1,4>)
        (make_pair(1u,5u),gatherCodesImpl<1,5>)
        (make_pair(2u,1u),gatherCodesImpl<2,1>)
        (make_pair(2u,2u),gatherCodesImpl<2,2>)
        (make_pair(2u,3u),gatherCodesImpl<2,3>)
        (make_pair(2u,4u),gatherCodesImpl<2,4>)
        (make_pair(2u,5u),gatherCodesImpl<2,5>)
        (make_pair(3u,1u),gatherCodesImpl<3,1>)
        (make_pair(3u,2u),gatherCodesImpl<3,2>)
        (make_pair(3u,3u),gatherCodesImpl<3,3>)
        (make_pair(3u,4u),gatherCodesImpl<3,4>)
        (make_pair(4u,1u),gatherCodesImpl<4,1>)
        (make_pair(4u,2u),gatherCodesImpl<4,2>)
        (make_pair(4u,3u),gatherCodesImpl<4,3>)
        (make_pair(5u,1u),gatherCodesImpl<5,1>)
        (make_pair(5u,2u),gatherCodesImpl<5,2>)
        (make_pair(6u,1u),gatherCodesImpl<6,1>);

set<Code> gatherCodes(auto_ptr<OperatorSpace> initial_space) {
    const unsigned int number_of_qubits = initial_space->number_of_qubits
                     , number_of_operators = initial_space->number_of_operators
                     ;
    gatherCodesImplsType::const_iterator gatherCodesImplPtr = gatherCodesImpls.find(make_pair(number_of_qubits,number_of_operators));
    if(gatherCodesImplPtr == gatherCodesImpls.end()) {
        throw GatherCodesNotSupported(number_of_qubits,number_of_operators);
    }
    return gatherCodesImplPtr->second(initial_space);
}
//@+node:gcross.20101224191604.2754: *3* gatherSolutions
vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> initial_space) {
    vector<unsigned long long> solutions;
    copy(
        generateSolutionsFor(initial_space) | transformed(encodeOperatorSpace),
        back_inserter(solutions)
    );
    return solutions;
}
//@-others
//@-leo
