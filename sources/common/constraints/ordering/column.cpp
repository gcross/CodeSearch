//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1676: * @thin column.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1677: ** << Includes >>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include "constraints/ordering.hpp"
#include "constraints/ordering/column.hpp"
#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.1678: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.1679: ** Functions
//@+node:gcross.20101229110857.1680: *3* postColumnOrderingConstraint
BoolVarArgs postColumnOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_column
    , const unsigned int ending_column
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties
) {
    return postOrderingConstraint(
              space
            , space.getOMatrix().slice(starting_column,ending_column,starting_row,ending_row)
            , initial_ties
            );
}
//@+node:gcross.20101231214817.2177: *3* postColumnOrderingConstraints
StandardFormTies postColumnOrderingConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    const unsigned int
          number_of_qubits = space.number_of_qubits
        , number_of_operators = space.number_of_operators
        , x_bit_diagonal_size = parameters.x_bit_diagonal_size
        , z_bit_diagonal_size = parameters.z_bit_diagonal_size
        ;
    return
        (// We ignore these ties because they don't affect the row ordering.
            postOrderingConstraint(
                  space
                , space.getOMatrix().slice(
                     x_bit_diagonal_size+z_bit_diagonal_size
                    ,number_of_qubits
                    ,0
                    ,x_bit_diagonal_size+z_bit_diagonal_size
                )
            )
        ,make_tuple(
         // First region row-ordering ties
            postOrderingConstraint(
                  space
                , space.getZMatrix().slice(
                     0
                    ,x_bit_diagonal_size
                    ,0
                    ,number_of_operators
                  )
            )
         // Second region row-ordering ties
        ,   postOrderingConstraint(
                  space
                , space.getOMatrix().slice(
                     x_bit_diagonal_size
                    ,x_bit_diagonal_size+z_bit_diagonal_size
                    ,0
                    ,x_bit_diagonal_size
                  )
            )
         // Third region ties (non-existent at this point)
        ,   BoolVarArgs()
        )
        );
}
//@-others

}
//@-leo
