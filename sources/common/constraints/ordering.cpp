//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2512: * @thin ordering.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2513: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints/ordering.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.2514: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2195: ** function postOrderingConstraint
BoolVarArgs postOrderingConstraint(OperatorSpace& space, IntMatrix variables, BoolVarArgs initial_ties) {
    if(variables.width() <= 1 || variables.height() <= 0) return initial_ties;

    const unsigned int number_of_cols = variables.width()
                     , number_of_rows = variables.height()
                     ;

    BoolVarArgs ties(space,(number_of_rows+1)*(number_of_cols-1),0,1);
    BoolMatrix ties_matrix(ties,number_of_cols-1,number_of_rows+1);

    if(initial_ties.size() > 0) {
        assert(initial_ties.size() == variables.width()-1);
        BOOST_FOREACH(const unsigned int i, irange(0u,number_of_cols-1)) {
            rel(space,ties_matrix(i,0),IRT_EQ,initial_ties[i]);
        }
    } else {
        BOOST_FOREACH(const unsigned int i, irange(0u,number_of_cols-1)) {
            rel(space,ties_matrix(i,0),IRT_EQ,1);
        }
    }

    BOOST_FOREACH(const unsigned int row, irange(0u,number_of_rows)) {
        BOOST_FOREACH(const unsigned int col, irange(0u,number_of_cols-1)) {
            ties_matrix(col,row+1) = expr(space,ties_matrix(col,row) && variables(col,row) == variables(col+1,row));
            rel(space,ties_matrix(col,row) >> (variables(col,row) >= variables(col+1,row)));
        }
    }

    return ties_matrix.row(number_of_rows);
}

BoolVarArgs postOrderingConstraint(OperatorSpace& space, BoolMatrix variables, BoolVarArgs initial_ties) {
    if(variables.width() <= 1 || variables.height() <= 0) return initial_ties;

    BoolVarArgs vars = variables.get_array();
    IntVarArgs vars_as_ints(space,vars.size(),0,1);
    BOOST_FOREACH(const unsigned int i, irange(0u,(unsigned int)vars.size())) { channel(space,vars[i],vars_as_ints[i]); }
    IntMatrix variables_as_ints(vars_as_ints,variables.width(),variables.height());
    return postOrderingConstraint(space,variables_as_ints,initial_ties);
}
//@-others

}
//@-leo
