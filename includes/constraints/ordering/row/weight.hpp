//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2112: * @thin weight.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_ORDERING_ROW_WEIGHT_HPP
#define CONSTRAINTS_ORDERING_ROW_WEIGHT_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2113: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2114: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2115: ** Functions
BoolVarArgs postRowWeightOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties=BoolVarArgs()
);
//@-others

}

#endif
//@-leo
