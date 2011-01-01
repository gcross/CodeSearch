//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1644: * @thin standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1645: ** << Includes >>
#include <boost/foreach.hpp>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.1666: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.1646: ** Functions
//@+node:gcross.20101229110857.1647: *3* postStandardFormConstraint
void postStandardFormConstraint(OperatorSpace& space, const unsigned int x_bit_diagonal_size, const unsigned int z_bit_diagonal_size) {
    const unsigned int total_diagonal_size = x_bit_diagonal_size + z_bit_diagonal_size
                     , number_of_qubits = space.number_of_qubits
                     , number_of_operators = space.number_of_operators
                     ;
    assert(total_diagonal_size <= number_of_qubits);
    assert(total_diagonal_size <= number_of_operators);

    { // X matrix constraints
        BoolMatrix X_matrix = space.getXMatrix();
        BOOST_FOREACH(unsigned int row, irange(0u,x_bit_diagonal_size)) {
            BOOST_FOREACH(unsigned int col, irange(0u,x_bit_diagonal_size)) {
                if(row == col) {
                    rel(space,X_matrix(col,row),IRT_EQ,1);
                } else {
                    rel(space,X_matrix(col,row),IRT_EQ,0);
                }
            }
        }
        rel(space,X_matrix.slice(0,number_of_qubits,x_bit_diagonal_size,number_of_operators),IRT_EQ,0);
    }

    { // Z matrix constraints
        BoolMatrix Z_matrix = space.getZMatrix();
        BOOST_FOREACH(unsigned int row, irange(x_bit_diagonal_size,total_diagonal_size)) {
            BOOST_FOREACH(unsigned int col, irange(x_bit_diagonal_size,total_diagonal_size)) {
                if(row == col) {
                    rel(space,Z_matrix(col,row),IRT_EQ,1);
                } else {
                    rel(space,Z_matrix(col,row),IRT_EQ,0);
                }
            }
        }
        rel(space,Z_matrix.slice(x_bit_diagonal_size,number_of_qubits,total_diagonal_size,number_of_operators),IRT_EQ,0);
    }
}
//@-others

}
//@-leo
