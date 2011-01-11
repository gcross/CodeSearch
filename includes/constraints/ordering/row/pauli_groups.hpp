//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2135: * @thin pauli_groups.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_ORDERING_ROW_PAULI_GROUPS_HPP
#define CONSTRAINTS_ORDERING_ROW_PAULI_GROUPS_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2136: ** << Includes >>
#include <gecode/minimodel.hh>

#include "constraints/ordering.hpp"
#include "constraints/ordering/row/pauli_groups.hpp"
#include "constraints/standard_form.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2137: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2143: ** Functions
BoolVarArgs postPauliGroupsRowOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_column
    , const unsigned int ending_column
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties = BoolVarArgs()
);

StandardFormTies postPauliGroupsRowOrderingConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
    , const StandardFormTies& initial_ties = no_standard_form_ties
);
//@-others

}

#endif
//@-leo
