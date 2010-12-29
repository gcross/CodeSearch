//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2719: * @thin test_utils.hpp
//@@language cplusplus

#ifndef TEST_UTILS_HH
#define TEST_UTILS_HH

//@+<< Includes >>
//@+node:gcross.20101224191604.2720: ** << Includes >>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <gecode/int.hh>
#include <illuminate.hpp>
#include <memory>
#include <set>
#include <vector>

#include "solution_iterator.hpp"
#include "operator_space.hpp"

using namespace CodeQuest;
using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::algorithm;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2721: ** struct Code
struct Code {
    unsigned int number_of_stabilizers, number_of_gauge_qubits;
    vector<size_t> logical_qubit_distances;

    template<typename distance_vector>
    Code(const int number_of_stabilizers, const int number_of_gauge_qubits, const distance_vector& logical_qubit_distances)
        : number_of_stabilizers(number_of_stabilizers)
        , number_of_gauge_qubits(number_of_gauge_qubits)
        , logical_qubit_distances(logical_qubit_distances.begin(),logical_qubit_distances.end())
    { }

    bool operator<(const Code& c) const {
        if(number_of_stabilizers < c.number_of_stabilizers) return true;
        if(number_of_stabilizers > c.number_of_stabilizers) return false;
        if(number_of_gauge_qubits < c.number_of_gauge_qubits) return true;
        if(number_of_gauge_qubits > c.number_of_gauge_qubits) return false;
        if(logical_qubit_distances.size() < c.logical_qubit_distances.size()) return true;
        if(logical_qubit_distances.size() > c.logical_qubit_distances.size()) return false;
        for(int i = 0; i < logical_qubit_distances.size(); ++i) {
            if(logical_qubit_distances[i] < c.logical_qubit_distances[i]) return true;
            if(logical_qubit_distances[i] > c.logical_qubit_distances[i]) return false;
        }
        return false;
    }
    bool operator==(const Code& c) const {
        if(number_of_stabilizers != c.number_of_stabilizers) return false;
        if(number_of_gauge_qubits != c.number_of_gauge_qubits) return false;
        if(logical_qubit_distances.size() != c.logical_qubit_distances.size()) return false;
        for(int i = 0; i < logical_qubit_distances.size(); ++i) {
            if(logical_qubit_distances[i] != c.logical_qubit_distances[i]) return false;
        }
        return true;
    }

};
//@+node:gcross.20101224191604.2722: ** Functions
long long encodeOperatorSpace(const OperatorSpace& space);
vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> space);

//@+others
//@+node:gcross.20101229110857.1589: *3* gatherCodes
template<unsigned int number_of_qubits, unsigned int number_of_operators> set<Code> gatherCodes(auto_ptr<OperatorSpace> initial_space) {
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
//@+node:gcross.20101229110857.1670: *3* fetchAllCodes
typedef map<pair<unsigned int,unsigned int>,set<Code> > CodeTable;
extern CodeTable operator_space_code_table;

template<unsigned int number_of_qubits, unsigned int number_of_operators> const set<Code>& fetchAllCodes() {
    typedef CodeTable::const_iterator CodeTablePosition;

    const pair<unsigned int,unsigned int> id = make_pair(number_of_operators,number_of_qubits);

    CodeTablePosition operator_space_code_position = operator_space_code_table.find(id);
    if(operator_space_code_position == operator_space_code_table.end()) {
        operator_space_code_position =
            operator_space_code_table.insert(
                make_pair(
                    id,
                    gatherCodes<number_of_qubits,number_of_operators>(
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
//@+node:gcross.20101229110857.1591: *3* checkCodes
template<unsigned int number_of_qubits, unsigned int number_of_operators> void checkCodes(auto_ptr<OperatorSpace> initial_space) {
    assert(number_of_qubits == initial_space->number_of_qubits);
    assert(number_of_operators == initial_space->number_of_operators);

    const set<Code>& operator_space_codes = fetchAllCodes<number_of_qubits,number_of_operators>();
    const set<Code> constrained_space_codes = gatherCodes<number_of_qubits,number_of_operators>(initial_space);

    ASSERT_EQ(operator_space_codes.size(),constrained_space_codes.size());
    ASSERT_TRUE(equal(operator_space_codes,constrained_space_codes));
}
//@-others
//@+node:gcross.20101224191604.3436: ** Macros
//@+node:gcross.20101229110857.1594: *3* DO_TEMPLATE_TEST_FOR
#define DO_TEMPLATE_TEST_FOR(number_of_operators,number_of_qubits) \
    TEST_CASE(_##number_of_operators##_##number_of_qubits) { \
        runTest<number_of_operators,number_of_qubits>(); \
    }
//@+node:gcross.20101224191604.3437: *3* DO_TEST_FOR
#define DO_TEST_FOR(number_of_operators,number_of_qubits) \
    TEST_CASE(_##number_of_operators##_##number_of_qubits) { \
        runTest(number_of_operators,number_of_qubits); \
    }
//@+node:gcross.20101224191604.3438: *3* DO_TEST_FOR_1
#define DO_TEST_FOR_1(number_of_operators,number_of_qubits,expected_number_of_solutions) \
    TEST_CASE(_##number_of_operators##_##number_of_qubits) { \
        runTest(number_of_operators,number_of_qubits,expected_number_of_solutions); \
    }
//@-others

#endif
//@-leo
