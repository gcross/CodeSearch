//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2748: * @thin test_utils.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2749: ** << Includes >>
#include <algorithm>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/irange.hpp>
#include <codequest.hpp>
#include <functional>
#include <illuminate.hpp>
#include <map>

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
//@+node:gcross.20101224191604.2750: ** Functions
//@+node:gcross.20101231214817.2055: *3* checkCodes
void checkCodes(auto_ptr<OperatorSpace> initial_space) {
    const set<Code>& operator_space_codes = fetchAllCodes(initial_space->number_of_qubits,initial_space->number_of_operators);
    const set<Code> constrained_space_codes = gatherCodes(initial_space);

    ASSERT_EQ(operator_space_codes.size(),constrained_space_codes.size());
    ASSERT_TRUE(equal(operator_space_codes,constrained_space_codes));
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
//@+node:gcross.20110102182304.1594: *3* forEachStandardForm
void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,auto_ptr<OperatorSpace> space
                    )
              > f
    ) {
        BOOST_FOREACH(const StandardFormParameters& parameters, generateStandardFormsFor(number_of_qubits,number_of_operators)) {
            f   (parameters
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
              > f
    ) {
        BOOST_FOREACH(const StandardFormParameters& parameters, generateStandardFormsFor(number_of_qubits,number_of_operators)) {
            for_each(
                 generateSolutionsFor(
                    createConstrainedSpace(
                         number_of_qubits
                        ,number_of_operators
                        ,constraints
                        ,parameters
                    )
                 )
                ,bind(f
                    ,parameters
                    ,_1
                 )
            );
        }
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
        if(number_of_stabilizers > 0u || number_of_gauge_qubits > 0u || number_of_logical_qubits > 0u) {
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
