//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2534: * @thin weight.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2535: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.2536: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.2537: ** Functions
//@+node:gcross.20101229110857.2538: *3* postRowWeightOrderingConstraint
BoolVarArgs postRowWeightOrderingConstraint(OperatorSpace& space, const unsigned int starting_row, const unsigned int ending_row, BoolVarArgs initial_ties) {
    if(ending_row-starting_row <= 0) return BoolVarArgs();

    return postOrderingConstraint(
         space
        ,IntMatrix(
             space.weights
            ,space.number_of_operators
            ,1
         ).slice(starting_row,ending_row,0,1)
        ,initial_ties
    );
}
//@-others

}
//@-leo
