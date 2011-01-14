//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2768: * @thin utilities.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2769: ** << Includes >>
#include <iterator>

#include "solution_iterator.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20110104130849.1576: ** << Usings >>
using namespace std;
//@-<< Usings >>

//@+others
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
//@-others

}
//@-leo
