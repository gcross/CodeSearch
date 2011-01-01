//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2607: * @thin column_pauli_sets.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2608: ** << Includes >>
#include "column_pauli_sets.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.2612: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.2609: ** class ColumnPauliSetsData
//@+node:gcross.20101229110857.2610: *3* (constructors)
ColumnPauliSetsData::ColumnPauliSetsData(
      OperatorSpace& space
    , const unsigned int start_column
    , const unsigned int end_column
    , const unsigned int start_row
    , const unsigned int end_row
    )
    : number_of_cols(end_column-start_column)
    , number_of_rows(end_row-start_row)
    , X_bit_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , Z_bit_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , X_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , Y_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , Z_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , I_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
    , non_trivial_sets(space,number_of_cols,IntSet::empty,IntSet(0,number_of_rows))
{
    BoolMatrix X_matrix = space.getXMatrix(),
               Z_matrix = space.getZMatrix(),
               non_trivial_matrix = space.getNonTrivialMatrix();
    BOOST_FOREACH(const unsigned int i, irange(0u,number_of_cols)) {
        channel(space,X_matrix.slice(start_column+i,start_column+i+1,start_row,end_row),X_bit_sets[i]);
        channel(space,Z_matrix.slice(start_column+i,start_column+i+1,start_row,end_row),Z_bit_sets[i]);
        X_sets[i] = expr(space,X_bit_sets[i] - Z_bit_sets[i]);
        Z_sets[i] = expr(space,Z_bit_sets[i] - X_bit_sets[i]);
        Y_sets[i] = expr(space,X_bit_sets[i] & Z_bit_sets[i]);
        channel(space,non_trivial_matrix.slice(start_column+i,start_column+i+1,start_row,end_row),non_trivial_sets[i]);
        I_sets[i] = expr(space,IntSet(0,number_of_rows-1)-non_trivial_sets[i]);
    }
}

ColumnPauliSetsData::ColumnPauliSetsData(const bool share, OperatorSpace& space, ColumnPauliSetsData& my)
    : number_of_cols(my.number_of_cols)
    , number_of_rows(my.number_of_rows)
{
    X_bit_sets.update(space,share,my.X_bit_sets);
    Z_bit_sets.update(space,share,my.Z_bit_sets);
    X_sets.update(space,share,my.X_sets);
    Y_sets.update(space,share,my.Y_sets);
    Z_sets.update(space,share,my.Z_sets);
    I_sets.update(space,share,my.I_sets);
    non_trivial_sets.update(space,share,my.non_trivial_sets);
}
//@+node:gcross.20101229110857.2611: *3* copy
auto_ptr<OperatorSpace::AuxiliaryData> ColumnPauliSetsData::copy(const bool share, OperatorSpace& space) {
    return wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new ColumnPauliSetsData(share,space,*this)));
}
//@-others

}
//@-leo
