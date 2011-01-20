//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1644: * @thin constraints/standard_form.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1645: ** << Includes >>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
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
const StandardFormTies no_standard_form_ties = make_pair(BoolVarArgs(),BoolVarArgs());
//@+node:gcross.20110109173246.1588: ** exception BadStandardFormParameters
BadStandardFormParameters::BadStandardFormParameters(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const StandardFormParameters& parameters
) : number_of_qubits(number_of_qubits)
  , number_of_operators(number_of_operators)
  , parameters(parameters)
  , message(
      ( format("Bad standard form parameters: # qubits = %1%, # operators = %2%, x bit diagonal = %3%, z bit diagonal = %4%")
        % number_of_qubits
        % number_of_operators
        % parameters.x_bit_diagonal_size
        % parameters.z_bit_diagonal_size
      ).str()
    )
{ }

const char* BadStandardFormParameters::what() { return message.c_str(); }
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
StandardFormIterator::StandardFormIterator()
  : maximum_diagonal_size(0)
  , parameters(none)
{ }

StandardFormIterator::StandardFormIterator(const unsigned int number_of_qubits,const unsigned int number_of_operators)
  : maximum_diagonal_size(min(number_of_qubits,number_of_operators))
  , parameters(StandardFormParameters(
         (number_of_operators+1)/2
        ,number_of_operators-(number_of_operators+1)/2)
    )
{ }
//@+node:gcross.20101231214817.2283: *3* dereference
const StandardFormParameters& StandardFormIterator::dereference() const {
    return *parameters;
}
//@+node:gcross.20101231214817.2284: *3* equal
bool StandardFormIterator::equal(const StandardFormIterator& other) const {
    return parameters == other.parameters;
}
//@+node:gcross.20110102182304.1569: *3* increment
void StandardFormIterator::increment() {
    assert(parameters);
    unsigned int  &x_bit_diagonal_size = parameters->x_bit_diagonal_size
                , &z_bit_diagonal_size = parameters->z_bit_diagonal_size
                ;
    if(x_bit_diagonal_size == maximum_diagonal_size) {
        parameters = none;
    } else {
        ++x_bit_diagonal_size;
        --z_bit_diagonal_size;
    }
}
//@+node:gcross.20110102182304.1575: ** Functions
//@+node:gcross.20110102182304.1578: *3* generateStandardFormsFor
iterator_range<StandardFormIterator> generateStandardFormsFor(const unsigned int number_of_qubits, const unsigned int number_of_operators) {
    return iterator_range<StandardFormIterator>(StandardFormIterator(number_of_qubits,number_of_operators),StandardFormIterator());
}
//@+node:gcross.20101229110857.1647: *3* postStandardFormConstraint
void postStandardFormConstraint(OperatorSpace& space, const StandardFormParameters& parameters) {
    const unsigned int number_of_qubits = space.number_of_qubits
                     , number_of_operators = space.number_of_operators
                     , x_bit_diagonal_size = parameters.x_bit_diagonal_size
                     , z_bit_diagonal_size = parameters.z_bit_diagonal_size
                     ;
    if(x_bit_diagonal_size + z_bit_diagonal_size != number_of_operators
    || x_bit_diagonal_size < x_bit_diagonal_size
    ) throw BadStandardFormParameters(number_of_qubits,number_of_operators,parameters);

    BoolMatrix X_matrix = space.getXMatrix()
             , Z_matrix = space.getZMatrix()
             ;
    BOOST_FOREACH(const unsigned int col, irange(0u,x_bit_diagonal_size)) {
        BOOST_FOREACH(const unsigned int row, irange(0u,x_bit_diagonal_size)) {
            if(col == row) {
                rel(space,X_matrix(col,row),IRT_EQ,1);
                if(col >= z_bit_diagonal_size) {
                    rel(space,Z_matrix(col,row),IRT_EQ,0);
                }
            } else {
                rel(space,X_matrix(col,row),IRT_EQ,0);
            }
        }
    }
    rel(space,space.getXMatrix().slice(0,number_of_qubits,x_bit_diagonal_size,number_of_operators),IRT_EQ,0);
    BOOST_FOREACH(const unsigned int col, irange(0u,z_bit_diagonal_size)) {
        BOOST_FOREACH(const unsigned int row, irange(0u,z_bit_diagonal_size)) {
            if(col == row) {
                rel(space,Z_matrix(col,x_bit_diagonal_size+row),IRT_EQ,1);
            } else {
                rel(space,Z_matrix(col,x_bit_diagonal_size+row),IRT_EQ,0);
            }
        }
    }
    rel(space,space.getZMatrix().slice(0,z_bit_diagonal_size,0,x_bit_diagonal_size),IRT_EQ,0);
}
//@-others

}
//@-leo
