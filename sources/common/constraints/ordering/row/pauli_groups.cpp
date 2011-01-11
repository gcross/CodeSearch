//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2585: * @thin pauli_groups.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2586: ** << Includes >>
#include <gecode/minimodel.hh>

#include "constraints/ordering/row/pauli_groups.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.1483: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2266: ** Functions
//@+node:gcross.20101229110857.2617: *3* postPauliGroupsRowOrderingConstraint
BoolVarArgs postPauliGroupsRowOrderingConstraint(
      OperatorSpace& space
    , const unsigned int start_column
    , const unsigned int end_column
    , const unsigned int start_row
    , const unsigned int end_row
    , BoolVarArgs initial_ties
) {
    const unsigned int number_of_rows = end_row-start_row
                     , number_of_columns = end_column-start_column;
                     ;
    if(number_of_columns <= 0 || number_of_rows <= 0) return BoolVarArgs();

    IntVarArgs orderings(space,number_of_columns*number_of_rows,0,3);
    IntMatrix orderings_matrix(orderings,number_of_rows,number_of_columns);

    {
        //@+<< Compute X/Y/Z sets >>
        //@+node:gcross.20110110211728.1584: *4* << Compute X/Y/Z sets >>
        SetVarArgs X_sets(space,number_of_columns,IntSet::empty,IntSet(0,number_of_rows))
                 , Y_sets(space,number_of_columns,IntSet::empty,IntSet(0,number_of_rows))
                 , Z_sets(space,number_of_columns,IntSet::empty,IntSet(0,number_of_rows))
                 ;
        {
            SetVarArgs X_bit_sets(space,number_of_columns,IntSet::empty,IntSet(0,number_of_rows))
                     , Z_bit_sets(space,number_of_columns,IntSet::empty,IntSet(0,number_of_rows))
                     ;
            BoolMatrix X_matrix = space.getXMatrix(),
                       Z_matrix = space.getZMatrix(),
                       non_trivial_matrix = space.getNonTrivialMatrix();
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_columns)) {
                channel(space,X_matrix.slice(start_column+i,start_column+i+1,start_row,end_row),X_bit_sets[i]);
                channel(space,Z_matrix.slice(start_column+i,start_column+i+1,start_row,end_row),Z_bit_sets[i]);
                X_sets[i] = expr(space,X_bit_sets[i] - Z_bit_sets[i]);
                Z_sets[i] = expr(space,Z_bit_sets[i] - X_bit_sets[i]);
                Y_sets[i] = expr(space,X_bit_sets[i] & Z_bit_sets[i]);
            }
        }
        //@-<< Compute X/Y/Z sets >>
        //@+<< Populate orderings matrix >>
        //@+node:gcross.20110110211728.1585: *4* << Populate orderings matrix >>
        {
            IntVarArgs pauli_orderings(space,number_of_columns*4,0,3)
                     , unsorted_orderings(space,number_of_columns*number_of_rows,0,3)
                     , X_set_sizes(space,number_of_columns,0,number_of_rows)
                     , Y_set_sizes(space,number_of_columns,0,number_of_rows)
                     , Z_set_sizes(space,number_of_columns,0,number_of_rows)
                     ;
            IntMatrix pauli_orderings_matrix(pauli_orderings,4,number_of_columns)
                    , unsorted_orderings_matrix(unsorted_orderings,number_of_rows,number_of_columns)
                    , O_matrix = space.getOMatrix().slice(start_column,end_column,start_row,end_row)
                    ;
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_columns)) {
                cardinality(space,X_sets[i],X_set_sizes[i]);
                cardinality(space,Y_sets[i],Y_set_sizes[i]);
                cardinality(space,Z_sets[i],Z_set_sizes[i]);
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
        //@-<< Populate orderings matrix >>
    }

    return postOrderingConstraint(space,orderings_matrix);
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
