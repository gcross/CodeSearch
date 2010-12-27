//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2719: * @thin test_utils.hpp
//@@language cplusplus

#ifndef TEST_UTILS_HH
#define TEST_UTILS_HH

//@+<< Includes >>
//@+node:gcross.20101224191604.2720: ** << Includes >>
#include <gecode/int.hh>
#include <illuminate.hpp>
#include <set>
#include <vector>

#include "operator_space.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2721: ** struct Code
struct Code {
    unsigned int number_of_stabilizers, number_of_gauge_qubits;
    vector<size_t> logical_qubit_distances;

    Code(const int number_of_stabilizers, const int number_of_gauge_qubits, const vector<size_t>& logical_qubit_distances)
        : number_of_stabilizers(number_of_stabilizers)
        , number_of_gauge_qubits(number_of_gauge_qubits)
        , logical_qubit_distances(logical_qubit_distances)
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
void checkCodes(auto_ptr<OperatorSpace> space);
long long encodeOperatorSpace(const OperatorSpace& space);
set<Code> gatherCodes(auto_ptr<OperatorSpace> space);
vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> space);
//@+node:gcross.20101224191604.3436: ** Macros
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
