//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2719: * @thin test_utils.hpp
//@@language cplusplus

#ifndef TEST_UTILS_HH
#define TEST_UTILS_HH

//@+<< Includes >>
//@+node:gcross.20101224191604.2720: ** << Includes >>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/irange.hpp>
#include <gecode/int.hh>
#include <illuminate.hpp>
#include <memory>
#include <ostream>
#include <set>
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
//@-<< Includes >>

//@+others
//@+node:gcross.20101224191604.2721: ** struct Code
struct Code {
    unsigned int number_of_stabilizers, number_of_gauge_qubits;
    vector<size_t> logical_qubit_distances;

    template<typename distance_vector>
    Code(const unsigned int number_of_stabilizers, const unsigned int number_of_gauge_qubits, const distance_vector& logical_qubit_distances)
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
        BOOST_FOREACH(const unsigned int i, irange((size_t)0u,logical_qubit_distances.size())) {
            if(logical_qubit_distances[i] < c.logical_qubit_distances[i]) return true;
            if(logical_qubit_distances[i] > c.logical_qubit_distances[i]) return false;
        }
        return false;
    }
    bool operator==(const Code& c) const {
        if(number_of_stabilizers != c.number_of_stabilizers) return false;
        if(number_of_gauge_qubits != c.number_of_gauge_qubits) return false;
        if(logical_qubit_distances.size() != c.logical_qubit_distances.size()) return false;
        BOOST_FOREACH(const unsigned int i, irange((size_t)0u,logical_qubit_distances.size())) {
            if(logical_qubit_distances[i] != c.logical_qubit_distances[i]) return false;
        }
        return true;
    }

};

inline ostream& operator<<(ostream& out, const Code& code) {
    out << code.number_of_stabilizers << " " << code.number_of_gauge_qubits << " | ";
    BOOST_FOREACH(int distance, code.logical_qubit_distances) { out << distance << " "; }
    return out;
}

typedef set<Code> CodeSet;
//@+node:gcross.20101231214817.2062: ** exception GatherCodesNotSupported
struct GatherCodesNotSupported : public std::exception {
    const unsigned int number_of_qubits, number_of_operators;
    const string error_message;
    GatherCodesNotSupported(
          const unsigned int number_of_qubits
        , const unsigned int number_of_operators
    ) : number_of_qubits(number_of_qubits)
      , number_of_operators(number_of_operators)
      , error_message((format("gatherCodes is not supported for %1% qubits and %2% operators") % number_of_qubits % number_of_operators).str())
    { }
    virtual ~GatherCodesNotSupported() throw() { }
    virtual const char* what() const throw() { return error_message.c_str(); }
};
//@+node:gcross.20101224191604.2722: ** Functions
void checkCodes(auto_ptr<OperatorSpace> initial_space);

long long encodeOperatorSpace(const OperatorSpace& space);

const set<Code>& fetchAllCodes(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
);

void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,auto_ptr<OperatorSpace> space
                    )
              > f

);

void forEachStandardFormSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,const OperatorSpace& space
                    )
              > f

);

set<Code> gatherCodes(auto_ptr<OperatorSpace> initial_space);

vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> space);
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
//@-others

#endif
//@-leo
