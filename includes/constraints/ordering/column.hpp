//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2086: * @thin column.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_ORDERING_COLUMN_HPP
#define CONSTRAINTS_ORDERING_COLUMN_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2087: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2088: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2089: ** Functions
BoolVarArgs postColumnOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_column
    , const unsigned int ending_column
    , const unsigned int starting_row
    , const unsigned int ending_row
);
//@-others

}

#endif
//@-leo
