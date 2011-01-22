//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2768: * @thin utilities.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2769: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/set_algorithm.hpp>
#include <iterator>
#include <map>
#include <utility>

#include "solution_iterator.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20110104130849.1576: ** << Usings >>
using namespace boost;
using namespace boost::adaptors;
using namespace boost::assign;
using namespace boost::range;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20110120115216.2103: ** struct Code
//@+node:gcross.20110120115216.2104: *3* operator<
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
//@+node:gcross.20110120115216.2105: *3* operator==
bool Code::operator==(const Code& c) const {
    if(number_of_stabilizers != c.number_of_stabilizers) return false;
    if(number_of_gauge_qubits != c.number_of_gauge_qubits) return false;
    if(logical_qubit_distances.size() != c.logical_qubit_distances.size()) return false;
    BOOST_FOREACH(const unsigned int i, irange((size_t)0u,logical_qubit_distances.size())) {
        if(logical_qubit_distances[i] != c.logical_qubit_distances[i]) return false;
    }
    return true;
}
//@+node:gcross.20110121100120.1580: *3* isNonTrivial/isTrivial
bool Code::isNonTrivial(const unsigned int number_of_measurement_operators) const {
    return
        (number_of_stabilizers > 0u || number_of_gauge_qubits > 0u || logical_qubit_distances.size() > 0u)
     && (number_of_stabilizers + 2*number_of_gauge_qubits == number_of_measurement_operators);
}

bool Code::isTrivial(const unsigned int number_of_measurement_operators) const {
    return !isNonTrivial(number_of_measurement_operators);
}
//@+node:gcross.20110120115216.2106: *3* toString
string Code::toString() const {
    ostringstream s;
    s << number_of_stabilizers << " " << number_of_gauge_qubits << " |";
    BOOST_FOREACH(const size_t distance, logical_qubit_distances) { s << " " << distance; }
    return s.str();
}
//@+node:gcross.20101224191604.2770: ** Functions
//@+node:gcross.20110114154616.2017: *3* channelMatrix
IntMatrix channelMatrix(Space& space, const BoolMatrix& matrix) {
    BoolVarArgs vars = matrix.get_array();
    IntVarArgs vars_as_ints(space,vars.size(),0,1);
    BOOST_FOREACH(const unsigned int i, irange(0u,(unsigned int)vars.size())) { channel(space,vars[i],vars_as_ints[i]); }
    return IntMatrix(vars_as_ints,matrix.width(),matrix.height());
}
//@+node:gcross.20101224191604.2772: *3* countSolutions
unsigned long long countSolutions(auto_ptr<OperatorSpace> initial_space, const Search::Options& options) {
    return distance(SolutionIterator(initial_space,options),SolutionIterator());
}
//@+node:gcross.20110120115216.2090: *3* gatherCodes
template<unsigned int number_of_qubits, unsigned int number_of_operators> static set<Code> gatherCodesImpl(
      auto_ptr<OperatorSpace> initial_space
    , const bool ignore_solutions_with_trivial_columns
) {
    assert(number_of_qubits == initial_space->number_of_qubits);
    assert(number_of_operators == initial_space->number_of_operators);

    set<Code> codes;

    typedef static_qec<number_of_qubits,number_of_operators> qec_t;

    if(ignore_solutions_with_trivial_columns) {
        copy(
             generateSolutionsFor(initial_space)
                | filtered(hasNoTrivialColumns)
                | transformed(computeOptimizedCodeForOperatorSpace<qec_t>)
                | transformed(summarizeCode<qec_t>)
                | filtered(bind(&Code::isNonTrivial,_1,number_of_operators))
            ,inserter(codes,codes.end())
        );
    } else {
        copy(
             generateSolutionsFor(initial_space)
                | transformed(computeOptimizedCodeForOperatorSpace<qec_t>)
                | transformed(summarizeCode<qec_t>)
                | filtered(bind(&Code::isNonTrivial,_1,number_of_operators))
            ,inserter(codes,codes.end())
        );
    }

    return codes;
}

typedef map<
     pair<unsigned int,unsigned int>
    ,function<set<Code> (auto_ptr<OperatorSpace>,bool)>
    > gatherCodesImplsType;

static const gatherCodesImplsType gatherCodesImpls = map_list_of
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

set<Code> gatherCodes(
      auto_ptr<OperatorSpace> initial_space
    , const bool ignore_solutions_with_trivial_columns
) {
    const unsigned int number_of_qubits = initial_space->number_of_qubits
                     , number_of_operators = initial_space->number_of_operators
                     ;
    gatherCodesImplsType::const_iterator gatherCodesImplPtr = gatherCodesImpls.find(make_pair(number_of_qubits,number_of_operators));
    if(gatherCodesImplPtr == gatherCodesImpls.end()) {
        throw GatherCodesNotSupported(number_of_qubits,number_of_operators);
    }
    return gatherCodesImplPtr->second(initial_space,ignore_solutions_with_trivial_columns);
}
//@+node:gcross.20110121100120.1583: *3* hasNoTrivialColumns
bool hasNoTrivialColumns(const OperatorSpace& space) {
    if(space.number_of_operators <= 0) return false;
    IntMatrix O_matrix = space.getOMatrix();
    BOOST_FOREACH(const unsigned int col, irange(0u,space.number_of_qubits)) {
        const int first_O = O_matrix(col,0).val();
        BOOST_FOREACH(const unsigned int row, irange(1u,space.number_of_operators)) {
            if(O_matrix(col,row).val() != first_O) return false;
        }
    }
    return true;
}
//@-others

}
//@-leo
