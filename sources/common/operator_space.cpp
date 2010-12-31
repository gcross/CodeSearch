//@+leo-ver=5-thin
//@+node:gcross.20101224191604.1867: * @thin operator_space.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.1868: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.1873: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101224191604.1869: ** class OperatorSpace
//@+node:gcross.20101224191604.1870: *3* (constructors)
OperatorSpace::OperatorSpace(const unsigned int number_of_qubits, const unsigned int number_of_operators)
    : number_of_operators(number_of_operators)
    , number_of_qubits(number_of_qubits)
    , number_of_variables(number_of_qubits*number_of_operators)
    , X(*this,number_of_variables,0,1)
    , Z(*this,number_of_variables,0,1)
    , non_trivial(*this,number_of_variables,0,1)
    , O(*this,number_of_variables,0,3)
    , weights(*this,number_of_operators,0,number_of_qubits)
{
    BOOST_FOREACH(unsigned int i, irange(0u,number_of_variables)) {
        O[i] = expr(*this,2*Z[i] + X[i]);
        non_trivial[i] = expr(*this,Z[i] || X[i]);
    }
    BoolMatrix non_trivial_matrix = getNonTrivialMatrix();
    BOOST_FOREACH(unsigned int i, irange(0u,number_of_operators)) {
        linear(*this,non_trivial_matrix.row(i),IRT_EQ,weights[i]);
    }
    branch(*this,O,INT_VAR_NONE,INT_VAL_MIN);
}

OperatorSpace::OperatorSpace(const bool share, OperatorSpace& s)
    : Space(share,s)
    , number_of_operators(s.number_of_operators)
    , number_of_qubits(s.number_of_qubits)
    , number_of_variables(s.number_of_variables)
    , attached_auxiliary_data(s.attached_auxiliary_data.size())
{
    X.update(*this,share,s.X);
    Z.update(*this,share,s.Z);
    non_trivial.update(*this,share,s.non_trivial);
    O.update(*this,share,s.O);
    weights.update(*this,share,s.weights);

    BOOST_FOREACH(AuxiliaryData& auxiliary_data, s.attached_auxiliary_data) {
        attached_auxiliary_data.push_back(auxiliary_data.copy(share,*this));
    }
}
//@+node:gcross.20101229110857.1596: *3* attachAuxiliaryData
void OperatorSpace::attachAuxiliaryData(auto_ptr<AuxiliaryData> auxiliary_data) {
    attached_auxiliary_data.push_back(auxiliary_data.get());
    auxiliary_data.release();
}
//@+node:gcross.20101224191604.1871: *3* copy
Space* OperatorSpace::copy(bool share)
{
    return new OperatorSpace(share,*this);
}
//@-others

}
//@-leo
