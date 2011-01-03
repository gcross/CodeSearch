//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2534: * @thin weight.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2535: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints/ordering.hpp"
#include "constraints/ordering/row/weight.hpp"
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
//@+node:gcross.20101229110857.2538: *3* postWeightRowOrderingConstraint
BoolVarArgs postWeightRowOrderingConstraint(
      OperatorSpace& space
    , const unsigned int starting_row
    , const unsigned int ending_row
    , BoolVarArgs initial_ties
) {
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
//@+node:gcross.20101231214817.2265: *3* postWeightRowOrderingConstraints
StandardFormTies postWeightRowOrderingConstraints(
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
            postWeightRowOrderingConstraint(
                  space
                , 0
                , x_bit_diagonal_size
                , initial_ties.get<1>()
            )
        ,// Second region ties
            postWeightRowOrderingConstraint(
                  space
                , x_bit_diagonal_size
                , x_bit_diagonal_size+z_bit_diagonal_size
                , initial_ties.get<2>()
            )
        ,// Third region ties
            postWeightRowOrderingConstraint(
                  space
                , x_bit_diagonal_size+z_bit_diagonal_size
                , number_of_operators
                , initial_ties.get<2>()
            )
        );
}
//@-others

}
//@-leo
