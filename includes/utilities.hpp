//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2760: * @thin utilities.hpp
//@@language cplusplus

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

//@+<< Includes >>
//@+node:gcross.20101224191604.2761: ** << Includes >>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <gecode/set.hh>
#include <memory>
#include <utility>
#include <vector>

#include "operator_space.hpp"
#include "constraints/standard_form.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.4183: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20110120115216.2110: ** exception GatherCodesNotSupported
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
//@+node:gcross.20110120115216.2108: ** struct Code
struct Code {
    unsigned int number_of_stabilizers, number_of_gauge_qubits;
    vector<size_t> logical_qubit_distances;

    template<typename distance_vector>
    Code(const unsigned int number_of_stabilizers, const unsigned int number_of_gauge_qubits, const distance_vector& logical_qubit_distances)
        : number_of_stabilizers(number_of_stabilizers)
        , number_of_gauge_qubits(number_of_gauge_qubits)
        , logical_qubit_distances(logical_qubit_distances.begin(),logical_qubit_distances.end())
    { }

    bool operator<(const Code& c) const;
    bool operator==(const Code& c) const;
    string toString() const;

};

inline ostream& operator<<(ostream& out, const Code& code) {
    out << code.toString();
    return out;
}

typedef set<Code> CodeSet;
//@+node:gcross.20101224191604.2762: ** Functions
IntMatrix channelMatrix(Space& space,const BoolMatrix& matrix);

unsigned long long countSolutions(auto_ptr<OperatorSpace> space, const Search::Options& options = Search::Options::def);

set<Code> gatherCodes(auto_ptr<OperatorSpace> initial_space);

template<typename T> auto_ptr<T> wrapAutoPtr(T* ptr) { return auto_ptr<T>(ptr); }
template<typename T> shared_ptr<T> wrapSharedPtr(T* ptr) { return shared_ptr<T>(ptr); }
//@-others

}

#endif
//@-leo
