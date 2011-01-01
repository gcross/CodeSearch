//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2135: * @thin pauli_groups.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_ORDERING_ROW_PAULI_GROUPS_HPP
#define CONSTRAINTS_ORDERING_ROW_PAULI_GROUPS_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2136: ** << Includes >>
#include <gecode/minimodel.hh>

#include "column_pauli_sets.hpp"
#include "constraints/ordering.hpp"
#include "constraints/ordering/row/pauli_groups.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2137: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2138: ** class PauliGroupsRowOrderingData
class PauliGroupsRowOrderingData : public OperatorSpace::AuxiliaryData {
//@+others
//@+node:gcross.20101231214817.2139: *3* (fields)
public:

const unsigned int number_of_columns, number_of_rows;
IntVarArray pauli_orderings, unsorted_orderings, orderings;
IntVarArray X_set_sizes, Y_set_sizes, Z_set_sizes;
//@+node:gcross.20101231214817.2140: *3* (constructors)
public:

PauliGroupsRowOrderingData(
      OperatorSpace& space
    , ColumnPauliSetsData& pauli_column_sets_data
    , unsigned int start_column
    , unsigned int end_column
    , unsigned int start_row
    , unsigned int end_row
    );

private:

PauliGroupsRowOrderingData(bool share, OperatorSpace& space, PauliGroupsRowOrderingData& my);
//@+node:gcross.20101231214817.2144: *3* (methods)
private:

virtual auto_ptr<OperatorSpace::AuxiliaryData> copy(const bool share, OperatorSpace& space);

public:

IntMatrix getOrderingsMatrix();
//@-others
};
//@+node:gcross.20101231214817.2143: ** function postPauliGroupsRowOrderingConstraint
BoolVarArgs postPauliGroupsRowOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_column
    , const unsigned int ending_column
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties = BoolVarArgs()
);
//@-others

}

#endif
//@-leo
