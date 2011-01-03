//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2102: * @thin ordering.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_ORDERING_HPP
#define CONSTRAINTS_ORDERING_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2103: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2104: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2105: ** Functions
BoolVarArgs postOrderingConstraint(
      OperatorSpace& space
    , IntMatrix variables
    , BoolVarArgs initial_ties=BoolVarArgs()
);

BoolVarArgs postOrderingConstraint(
      OperatorSpace& space
    , BoolMatrix variables
    , BoolVarArgs initial_ties=BoolVarArgs()
);
//@-others

}

#endif
//@-leo
