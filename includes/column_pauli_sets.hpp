//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2596: * @thin column_pauli_sets.hpp
//@@language cplusplus

#ifndef COLUMN_PAULI_SETS_HPP
#define COLUMN_PAULI_SETS_HPP

//@+<< Includes >>
//@+node:gcross.20101229110857.2597: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

using namespace Gecode;

//@+others
//@+node:gcross.20101229110857.2598: ** class ColumnPauliSetsData
class ColumnPauliSetsData : public virtual OperatorSpace::AuxiliaryData {
//@+others
//@+node:gcross.20101229110857.2599: *3* (fields)
public:
    unsigned int number_of_cols, number_of_rows;
    SetVarArray X_bit_sets, Z_bit_sets, X_sets, Y_sets, Z_sets, I_sets, non_trivial_sets;
//@+node:gcross.20101229110857.2600: *3* (constructors)
public:
    ColumnPauliSetsData(
          OperatorSpace& space
        , const unsigned int start_column
        , const unsigned int end_column
        , const unsigned int start_row
        , const unsigned int end_row
        );

private:
    ColumnPauliSetsData(const bool share, OperatorSpace& space, ColumnPauliSetsData& my);
//@+node:gcross.20101229110857.2601: *3* (methods)
private:
    virtual auto_ptr<OperatorSpace::AuxiliaryData> copy(const bool share, OperatorSpace& space);
//@-others
};
//@-others

}

#endif
//@-leo
