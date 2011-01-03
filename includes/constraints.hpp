//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2217: * @thin constraints.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2218: ** << Includes >>
#include <memory>
#include <set>

#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2219: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2220: ** enum Constraint
enum Constraint { StandardForm, ColumnOrdering, WeightRowOrdering, PauliGroupsRowOrdering };
//@+node:gcross.20101231214817.2221: ** function createConstrainedSpace
auto_ptr<OperatorSpace> createConstrainedSpace(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , set<Constraint> constraints
    , const StandardFormParameters& parameters
);
//@-others

}

#endif
//@-leo
