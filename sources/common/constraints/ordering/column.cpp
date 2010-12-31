//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1676: * @thin column.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1677: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.1678: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.1679: ** Functions
//@+node:gcross.20101229110857.1680: *3* postColumnOrderingConstraint
BoolVarArgs postColumnOrderingConstraint(OperatorSpace& space, const unsigned int starting_column, const unsigned int ending_column, const unsigned int starting_row, const unsigned int ending_row) {
    if(ending_column-starting_column < 2 || ending_row-starting_row <= 0) return BoolVarArgs();

    return postOrderingConstraint(space,space.getOMatrix().slice(starting_column,ending_column,starting_row,ending_row));
}
//@-others

}
//@-leo
