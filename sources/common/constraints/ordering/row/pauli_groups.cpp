//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2585: * @thin pauli_groups.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2586: ** << Includes >>
#include <gecode/minimodel.hh>

#include "constraints/ordering/row/pauli_groups.hpp"
#include "column_pauli_sets.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.1483: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.2587: ** class PauliGroupsRowOrderingData
//@+node:gcross.20101229110857.2588: *3* (constructors)
PauliGroupsRowOrderingData::PauliGroupsRowOrderingData(
      OperatorSpace& space
    , ColumnPauliSetsData& pauli_column_sets_data
    , unsigned int start_column
    , unsigned int end_column
    , unsigned int start_row
    , unsigned int end_row
    )
    : number_of_columns(end_column-start_column)
    , number_of_rows(end_row-start_row)
    , pauli_orderings(space,number_of_columns*4,0,3)
    , unsorted_orderings(space,number_of_columns*number_of_rows,0,3)
    , orderings(space,number_of_columns*number_of_rows,0,3)
    , X_set_sizes(space,number_of_columns,0,number_of_rows)
    , Y_set_sizes(space,number_of_columns,0,number_of_rows)
    , Z_set_sizes(space,number_of_columns,0,number_of_rows)
{
    IntMatrix  pauli_orderings_matrix(pauli_orderings,4,number_of_columns),
               unsorted_orderings_matrix(unsorted_orderings,number_of_rows,number_of_columns),
               orderings_matrix = getOrderingsMatrix(),
               O_matrix = space.getOMatrix().slice(start_column,end_column,start_row,end_row);
    BOOST_FOREACH(const unsigned int i, irange(0u,number_of_columns)) {
        cardinality(space,pauli_column_sets_data.X_sets[i],X_set_sizes[i]);
        cardinality(space,pauli_column_sets_data.Y_sets[i],Y_set_sizes[i]);
        cardinality(space,pauli_column_sets_data.Z_sets[i],Z_set_sizes[i]);
        rel(space,
            (X_set_sizes[i] < Z_set_sizes[i] && (
                (Z_set_sizes[i] <  Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 1
                 && pauli_orderings_matrix(2,i) == 2
                 && pauli_orderings_matrix(3,i) == 3
                )
             ^  (Z_set_sizes[i] == Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 1
                 && pauli_orderings_matrix(2,i) == 2
                 && pauli_orderings_matrix(3,i) == 2
                )
             ^  (Z_set_sizes[i] >  Y_set_sizes[i] && (
                    (X_set_sizes[i] <  Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 1
                     && pauli_orderings_matrix(2,i) == 3
                     && pauli_orderings_matrix(3,i) == 2
                    )
                 ^  (X_set_sizes[i] == Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 1
                     && pauli_orderings_matrix(2,i) == 2
                     && pauli_orderings_matrix(3,i) == 1
                    )
                 ^  (X_set_sizes[i] >  Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 2
                     && pauli_orderings_matrix(2,i) == 3
                     && pauli_orderings_matrix(3,i) == 1
                    )
                ))
            ))
         ^  (X_set_sizes[i] == Z_set_sizes[i] && (
                (Z_set_sizes[i] <  Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 1
                 && pauli_orderings_matrix(2,i) == 1
                 && pauli_orderings_matrix(3,i) == 2
                )
             ^  (Z_set_sizes[i] == Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 1
                 && pauli_orderings_matrix(2,i) == 1
                 && pauli_orderings_matrix(3,i) == 1
                )
             ^  (Z_set_sizes[i] >  Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 2
                 && pauli_orderings_matrix(2,i) == 2
                 && pauli_orderings_matrix(3,i) == 1
                )
            ))
         ^  (X_set_sizes[i] > Z_set_sizes[i] && (
                (Z_set_sizes[i] <  Y_set_sizes[i] && (
                    (X_set_sizes[i] <  Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 2
                     && pauli_orderings_matrix(2,i) == 1
                     && pauli_orderings_matrix(3,i) == 3
                    )
                 ^  (X_set_sizes[i] == Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 2
                     && pauli_orderings_matrix(2,i) == 1
                     && pauli_orderings_matrix(3,i) == 2
                    )
                 ^  (X_set_sizes[i] >  Y_set_sizes[i]
                     && pauli_orderings_matrix(0,i) == 0
                     && pauli_orderings_matrix(1,i) == 3
                     && pauli_orderings_matrix(2,i) == 1
                     && pauli_orderings_matrix(3,i) == 2
                    )
                ))
             ^  (Z_set_sizes[i] == Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 2
                 && pauli_orderings_matrix(2,i) == 1
                 && pauli_orderings_matrix(3,i) == 1
                )
             ^  (Z_set_sizes[i] >  Y_set_sizes[i]
                 && pauli_orderings_matrix(0,i) == 0
                 && pauli_orderings_matrix(1,i) == 3
                 && pauli_orderings_matrix(2,i) == 2
                 && pauli_orderings_matrix(3,i) == 1
                )
            ))
        );
        BOOST_FOREACH(const unsigned int j, irange(0u,number_of_rows)) {
            element(space,pauli_orderings_matrix.row(i),O_matrix(i,j),unsorted_orderings_matrix(j,i));
        }
    }
    BOOST_FOREACH(const unsigned int i, irange(0u,number_of_rows)) {
        sorted(space,unsorted_orderings_matrix.col(i),orderings_matrix.col(i));
    }
}

PauliGroupsRowOrderingData::PauliGroupsRowOrderingData(bool share, OperatorSpace& space, PauliGroupsRowOrderingData& my)
    : number_of_columns(my.number_of_columns)
    , number_of_rows(my.number_of_rows)
{
    pauli_orderings.update(space,share,my.pauli_orderings);
    unsorted_orderings.update(space,share,my.unsorted_orderings);
    orderings.update(space,share,my.orderings);
    X_set_sizes.update(space,share,my.X_set_sizes);
    Y_set_sizes.update(space,share,my.Y_set_sizes);
    Z_set_sizes.update(space,share,my.Z_set_sizes);
}
//@+node:gcross.20101229110857.2619: *3* copy
auto_ptr<OperatorSpace::AuxiliaryData> PauliGroupsRowOrderingData::copy(const bool share, OperatorSpace& space) {
    return wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new PauliGroupsRowOrderingData(share,space,*this)));
}
//@+node:gcross.20101229110857.2615: *3* getOrderingsMatrix
IntMatrix PauliGroupsRowOrderingData::getOrderingsMatrix() {
    return IntMatrix(orderings,number_of_rows,number_of_columns);
}
//@+node:gcross.20101231214817.2266: ** Functions
//@+node:gcross.20101229110857.2617: *3* postPauliGroupsRowOrderingConstraint
BoolVarArgs postPauliGroupsRowOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_column
    , const unsigned int ending_column
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties
) {
    if(ending_column-starting_column <= 0 || ending_row-starting_row <= 0) return BoolVarArgs();

    auto_ptr<ColumnPauliSetsData> pauli_column_sets_data(new ColumnPauliSetsData(space,starting_column,ending_column,starting_row,ending_row));
    auto_ptr<PauliGroupsRowOrderingData> pauli_groups_ordering_data(new PauliGroupsRowOrderingData(space,*pauli_column_sets_data,starting_column,ending_column,starting_row,ending_row));

    BoolVarArgs final_ties = postOrderingConstraint(space,pauli_groups_ordering_data->getOrderingsMatrix());

    space.attachAuxiliaryData((auto_ptr<OperatorSpace::AuxiliaryData>)pauli_column_sets_data);
    space.attachAuxiliaryData((auto_ptr<OperatorSpace::AuxiliaryData>)pauli_groups_ordering_data);

    return final_ties;
}
//@+node:gcross.20101231214817.2248: *3* postPauliGroupsRowOrderingConstraints
StandardFormTies postPauliGroupsRowOrderingConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
    , const StandardFormTies& initial_ties
) {
    const unsigned int
          number_of_qubits = space.number_of_qubits
        , number_of_operators = space.number_of_operators
        , x_bit_diagonal_size = parameters.x_bit_diagonal_size
        , z_bit_diagonal_size = parameters.z_bit_diagonal_size
        ;

    return
        make_tuple(
         // First region ties
            postPauliGroupsRowOrderingConstraint(
                  space
                , 0
                , number_of_qubits
                , 0
                , x_bit_diagonal_size
                , initial_ties.get<1>()
            )
        ,// Second region ties
            postPauliGroupsRowOrderingConstraint(
                  space
                , 0
                , number_of_qubits
                , x_bit_diagonal_size
                , x_bit_diagonal_size+z_bit_diagonal_size
                , initial_ties.get<2>()
            )
        ,// Third region ties
            postPauliGroupsRowOrderingConstraint(
                  space
                , 0
                , x_bit_diagonal_size
                , x_bit_diagonal_size+z_bit_diagonal_size
                , number_of_operators
                , initial_ties.get<2>()
            )
        );
}
//@-others

}
//@-leo
