//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2748: * @thin test_utils.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2749: ** << Includes >>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/irange.hpp>
#include <codequest.hpp>
#include <functional>
#include <illuminate.hpp>

#include "solution_iterator.hpp"
#include "utilities.hpp"
#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2750: ** Functions
//@+node:gcross.20101224191604.2751: *3* checkCodes
void checkCodes(auto_ptr<OperatorSpace> initial_space) {
    const unsigned int number_of_operators = initial_space->number_of_operators
                     , number_of_qubits    = initial_space->number_of_qubits
                     ;
    typedef map<pair<unsigned int,unsigned int>,set<Code> > CodeTable;
    typedef CodeTable::const_iterator CodeTablePosition;

    static CodeTable operator_space_code_table;

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
                                    number_of_operators,
                                    number_of_qubits
                            )
                        )
                    )
                )
            ).first;
    }

    const set<Code>& operator_space_codes = operator_space_code_position->second;

    const set<Code> constrained_space_codes = gatherCodes(initial_space);

    ASSERT_EQ(operator_space_codes.size(),constrained_space_codes.size());
    ASSERT_TRUE(equal(operator_space_codes.begin(),operator_space_codes.end(),constrained_space_codes.begin()));
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
//@+node:gcross.20101224191604.2753: *3* gatherCodes
set<Code> gatherCodes(auto_ptr<OperatorSpace> initial_space) {
    set<Code> codes;

    BOOST_FOREACH
    (   auto_ptr<dynamic_qec> code
    ,   generateSolutionsFor(initial_space)
            | transformed(computeOptimizedCodeForOperatorSpace<dynamic_qec>)
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
//@+node:gcross.20101224191604.2754: *3* gatherSolutions
vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> initial_space) {
    vector<unsigned long long> solutions;
    copy(
        generateSolutionsFor(initial_space) | transformed(encodeOperatorSpace),
        back_inserter(solutions)
    );
}
//@-others
//@-leo
