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
//@+node:gcross.20101231214817.2263: ** Values
const StandardFormTies no_standard_form_ties = make_tuple(BoolVarArgs(),BoolVarArgs(),BoolVarArgs());
//@+node:gcross.20110102182304.1566: ** struct StandardFormParameters
//@+node:gcross.20110102182304.1579: *3* (constructors)
StandardFormParameters::StandardFormParameters(
      const unsigned int x_bit_diagonal_size
    , const unsigned int z_bit_diagonal_size
) : x_bit_diagonal_size(x_bit_diagonal_size)
  , z_bit_diagonal_size(z_bit_diagonal_size)
{ }
//@+node:gcross.20110102182304.1567: *3* operator==
bool StandardFormParameters::operator==(const StandardFormParameters& other) const {
    return x_bit_diagonal_size == other.x_bit_diagonal_size
        && z_bit_diagonal_size == other.z_bit_diagonal_size;
}
//@+node:gcross.20101231214817.2281: ** class StandardFormIterator
//@+node:gcross.20101231214817.2282: *3* (constructors)
StandardFormIterator::StandardFormIterator(
    const unsigned int number_of_qubits
,   const unsigned int number_of_operators
,   const bool end
) : maximum_diagonal_size(min(number_of_qubits,number_of_operators))
  , parameters(end ? StandardFormParameters(maximum_diagonal_size+1,0) : StandardFormParameters(1,0))
{ }
//@+node:gcross.20101231214817.2283: *3* dereference
const StandardFormParameters& StandardFormIterator::dereference() const {
    return parameters;
}
//@+node:gcross.20101231214817.2284: *3* equal
bool StandardFormIterator::equal(const StandardFormIterator& other) const {
    return maximum_diagonal_size == other.maximum_diagonal_size
        && parameters == other.parameters;
}
//@+node:gcross.20110102182304.1569: *3* increment
void StandardFormIterator::increment() {
    unsigned int  &x_bit_diagonal_size = parameters.x_bit_diagonal_size
                , &z_bit_diagonal_size = parameters.z_bit_diagonal_size
                ;
    ++z_bit_diagonal_size;
    if(x_bit_diagonal_size + z_bit_diagonal_size > maximum_diagonal_size) {
        ++x_bit_diagonal_size;
        z_bit_diagonal_size = 0;
        if(x_bit_diagonal_size > maximum_diagonal_size) {
            x_bit_diagonal_size = maximum_diagonal_size+1;
        }
    }
}
//@+node:gcross.20110102182304.1575: ** Functions
//@+node:gcross.20110102182304.1578: *3* generateSolutionsFor
iterator_range<StandardFormIterator> generateStandardFormsFor(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
) {
    return iterator_range<StandardFormIterator>(
                  StandardFormIterator(number_of_qubits,number_of_operators,false)
                , StandardFormIterator(number_of_qubits,number_of_operators,true)
           );
}
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

void postStandardFormConstraint(OperatorSpace& space, const StandardFormParameters& parameters) {
    postStandardFormConstraint(space,parameters.x_bit_diagonal_size,parameters.z_bit_diagonal_size);
}
//@-others

}
//@-leo
