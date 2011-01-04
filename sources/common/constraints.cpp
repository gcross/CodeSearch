//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2240: * @thin constraints.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101231214817.2241: ** << Includes >>
#include <boost/foreach.hpp>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include "constraints.hpp"
#include "constraints/ordering/column.hpp"
#include "constraints/ordering/row/pauli_groups.hpp"
#include "constraints/ordering/row/weight.hpp"
#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2242: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2245: ** function createConstraintedSpace
auto_ptr<OperatorSpace> createConstrainedSpace(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , set<Constraint> constraints
    , const StandardFormParameters& parameters
) {
    auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
    if(constraints.erase(StandardForm) > 0) {
        postStandardFormConstraint(*space,parameters);
    }
    StandardFormTies current_ties = no_standard_form_ties;
    if(constraints.erase(ColumnOrdering) > 0) {
        current_ties = postColumnOrderingConstraints(*space,parameters);
    }
    BOOST_FOREACH(const Constraint constraint, constraints) {
        switch(constraint) {
            case WeightRowOrdering:
                current_ties = postWeightRowOrderingConstraints(*space,parameters,current_ties);
                break;
            case PauliGroupsRowOrdering:
                current_ties = postPauliGroupsRowOrderingConstraints(*space,parameters,current_ties);
                break;
            default:
                assert(!"Should never reach here.");
        }
    }
    return space;
}
//@-others

}
//@-leo
